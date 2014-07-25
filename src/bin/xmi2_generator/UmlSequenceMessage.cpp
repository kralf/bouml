
#include "UmlSequenceMessage.h"
#include "FileOut.h"
#include "UmlItem.h"

#include <q3ptrdict.h>
#include <q3ptrstack.h>
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "UmlFragmentCompartment.h"
#include "UmlClassInstanceReference.h"

UmlSequenceMessage::UmlSequenceMessage() : reverse(0), used(FALSE) {
}

void UmlSequenceMessage::write(FileOut & out, UmlItem * diagram, const Q3PtrVector< UmlSequenceMessage > & msgs)
{
  set_reverses(msgs);
  
  Q3PtrList<UmlSequenceMessage> l;
  
  msgs.toList(&l);
  
  UmlSequenceMessage * m;
  
  while ((m = l.getFirst()) != 0) {
    if (m->fragment() != 0)
      m->fragment()->write(out, diagram, l);
    else
      m->write_fragment(out, diagram, l);
  }
}

void UmlSequenceMessage::write_fragment(FileOut & out, UmlItem * diagram, Q3PtrList< UmlSequenceMessage > & msgs) {
  msgs.removeRef(this);
  
#define MSG  "MSG", itsrank
#define SEND "MSGOCCSPECSEND", itsrank
#define REF_SEND  "MSGOCCSPECSEND", reverse->itsrank
#define REC  "MSGOCCSPECREC", itsrank
#define BEH  "BEHEXECSPEC", itsrank
#define REF_BEH  "BEHEXECSPEC", reverse->itsrank
#define EXEC "EXECOCCSPEC", itsrank
#define REF_EXEC "EXECOCCSPEC", reverse->itsrank
#define DEL "DELOCCSPEC", itsrank
    
  UmlPackage * prj = UmlPackage::getProject();
  
  switch (kind()) {
  case aSynchronousCall:
  case anAsynchronousCall:
    used = TRUE;
    
    if (from() != 0) {
      out.indent();
      out << "<fragment xmi:type=\"uml:MessageOccurrenceSpecification\"";
      out.id_prefix(diagram, SEND);
      out.ref(diagram, "covered", from()->lifeline());
      out.ref(prj, "event", 
	      (operation() != 0) ? operation()->event(FALSE)
				 : UmlOperation::event("SEND", form()));
      out.ref(diagram, "message", MSG);
      out << "/>\n";
    }
    
    if (to() != 0) {
      out.indent();
      out << "<fragment xmi:type=\"uml:MessageOccurrenceSpecification\"";
      out.id_prefix(diagram, REC);
      out.ref(diagram, "covered", to()->lifeline());
      out.ref(prj, "event", 
	      (operation() != 0) ? operation()->event(TRUE)
				 : UmlOperation::event("REC", form()));
      out.ref(diagram, "message", MSG);
      out << "/>\n";
      
      out.indent();
      out << "<fragment xmi:type=\"uml:BehaviorExecutionSpecification\"";
      out.id_prefix(diagram, BEH);
      out.ref(diagram, "covered", to()->lifeline());
      out.ref(diagram, "start", REC);
      if (reverse != 0) {
	if (reverse->kind() == anExplicitReturn)
	  out.ref(diagram, "finish", REF_SEND);
	else
	  out.ref(diagram, "finish", REF_EXEC);
      }
      out << "/>\n";
    }
    break;
  case anExplicitReturn:
    used = TRUE;
    
    out.indent();
    out << "<fragment xmi:type=\"uml:MessageOccurrenceSpecification\"";
    out.id_prefix(diagram, SEND);
    out.ref(diagram, "covered", from()->lifeline());
    out.ref(prj, "event", UmlOperation::event("SEND", form()));
    out.ref(diagram, "message", MSG);
    out << "/>\n";
    
    out.indent();
    out << "<fragment xmi:type=\"uml:MessageOccurrenceSpecification\"";
    out.id_prefix(diagram, REC);
    out.ref(diagram, "covered", to()->lifeline());
    out.ref(prj, "event", UmlOperation::event("REC", form()));
    out.ref(diagram, "message", MSG);
    out << "/>\n";
    break;
  case aDestruction:
    out.indent();
    out << "<fragment xmi:type=\"uml:OccurrenceSpecification\"";
    out.id_prefix(diagram, DEL);
    out.ref(diagram, "covered", from()->lifeline());
    out.ref(prj, "event", UmlOperation::event("DEL", ""));
    out << "/>\n";
    break;
  case anInteractionUse:
    break;
  default:
    if (reverse != 0) {
      out.indent();
      out << "<fragment xmi:type=\"uml:ExecutionOccurrenceSpecification\"";
      out.id_prefix(diagram, EXEC);
      out.ref(diagram, "covered", from()->lifeline());
      out.ref(prj, "event", UmlOperation::event("EXEC", form()));
      out.ref(diagram, "execution", REF_BEH);
      out << "/>\n";
    }
    break;
  }

#undef MSG
#undef SEND
#undef REF_SEND
#undef REC
#undef BEH
#undef REF_BEH
#undef EXEC
#undef REF_EXEC
#undef DEL
}

void UmlSequenceMessage::write_them(FileOut & out, UmlItem * diagram, const Q3PtrVector< UmlSequenceMessage > & msgs)
{
  int n = msgs.size();
  
  for (int i = 0; i != n; i += 1) {
    UmlSequenceMessage * msg = msgs[i];
    
    if (msg->used) {
#define MSG  "MSG", msg->itsrank
#define SEND "MSGOCCSPECSEND", msg->itsrank
#define REC  "MSGOCCSPECREC", msg->itsrank
    
      switch (msg->kind()) {
      case aSynchronousCall:
      case anAsynchronousCall:
	out.indent();
	out << "<message xmi:type=\"uml:Message\"";
	out.id_prefix(diagram, MSG);
	out << " name=\"";
	out.quote((msg->operation() != 0) ? msg->operation()->name()
					  : msg->form());
	out << ((msg->kind() == anAsynchronousCall)
		? "\" messageSort=\"asynchCall\""
		: "\" messageSort=\"synchCall\"");
	
	if (msg->from() == 0) {
	  out << " messageKind=\"found\"";
	  out.ref(diagram, "receiveEvent", REC);
	}
	else if (msg->to() == 0) {
	  out << " messageKind=\"lost\"";
	  out.ref(diagram, "sendEvent", SEND);
	}
	else {
	  out << " messageKind=\"complete\"";
	  out.ref(diagram, "sendEvent", SEND);
	  out.ref(diagram, "receiveEvent", REC);
	  out.ref(diagram, "connector", msg->from()->connector(msg->to()));
	}
	
	if (!msg->stereotype().isEmpty() && UmlItem::gen_extension()) {
	  out << ">\n";
	  out.indent();
	  out << "\t<xmi:Extension extender=\"Bouml\"><stereotype name=\"";
	  out.quote(msg->stereotype());
	  out << "\"/></xmi:Extension>\n";
	  out.indent();
	  out << "</message>\n";
	}
	else
	  out << "/>\n";
	break;
      case anExplicitReturn:
	out.indent();
	out << "<message xmi:type=\"uml:Message\"";
	out.id_prefix(diagram, MSG);
	out << " name=\"";
	out.quote(msg->form());
	out << "\" messageSort=\"reply\"";
	out.ref(diagram, "sendEvent", SEND);
	out.ref(diagram, "receiveEvent", REC);
	out.ref(diagram, "connector", msg->from()->connector(msg->to()));
	if (!msg->stereotype().isEmpty() && UmlItem::gen_extension()) {
	  out << ">\n";
	  out.indent();
	  out << "\t<xmi:Extension extender=\"Bouml\"><stereotype name=\"";
	  out.quote(msg->stereotype());
	  out << "\"/></xmi:Extension>\n";
	  out.indent();
	  out << "</message>\n";
	}
	else
	  out << "/>\n";
	break;
      default:
	break;
      }
      
#undef MSG
#undef SEND
#undef REC
    }
  }
}

void UmlSequenceMessage::set_reverses(const Q3PtrVector<UmlSequenceMessage> & msgs)
{
  Q3PtrDict<Q3PtrStack<UmlSequenceMessage> > sent;
  int n = msgs.size();
  
  for (int i = 0; i != n; i += 1) {
    UmlSequenceMessage * m = msgs[i];
    Q3PtrStack<UmlSequenceMessage> * stack = 
      (m->from() != 0) ? sent[m->from()] : 0;
    
    switch (m->kind()) {
    case aDestruction:
    case anInteractionUse:
      break;
    case aSynchronousCall:
    case anAsynchronousCall:
      if ((m->from() != 0) && (m->to() != 0)) {
	stack = sent[m->to()];
	if (stack == 0) {
	  stack = new Q3PtrStack<UmlSequenceMessage>();
	  sent.insert(m->to(), stack);
	}
	stack->push(m);
      }
      
      if (m->fragment() != 0)
	m->fragment()->fragment()->cover(m);
      break;
    default: //  return
      stack = sent[m->from()];
      
      if ((stack != 0) &&
	  !stack->isEmpty() &&
	  ((m->kind() != anExplicitReturn) ||
	   (m->to() == stack->top()->from())))
	(m->reverse = stack->pop())->reverse = m;
    }
  }
  
  sent.setAutoDelete(TRUE);
}

