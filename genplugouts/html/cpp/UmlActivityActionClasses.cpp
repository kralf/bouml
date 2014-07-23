
#include "UmlActivityActionClasses.h"

#include "UmlOperation.h"
//Added by qt3to4:
#include <Q3CString>
Q3CString UmlSendObjectAction::sKind() {
  return "send object activity action";
}

void UmlSendObjectAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlUnmarshallAction::sKind() {
  return "unmarshall activity action";
}

void UmlUnmarshallAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlSendSignalAction::sKind() {
  return "send signal activity action";
}

Q3CString UmlBroadcastSignalAction::sKind() {
  return "broadcast signal activity action";
}

Q3CString UmlValueSpecificationAction::sKind() {
  return "value specification activity action";
}

void UmlValueSpecificationAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  Q3CString s = value();
  Q3CString scpp = cppValue();
  Q3CString sjava = javaValue();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Value :</p><ul>");
  
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlOpaqueAction::sKind() {
  return "opaque activity action";
}

void UmlOpaqueAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  Q3CString s = behavior();
  Q3CString scpp = cppBehavior();
  Q3CString sjava = javaBehavior();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Behavior :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlAcceptEventAction::sKind() {
  return "accept event activity action";
}

void UmlAcceptEventAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isUnmarshall()) {
    if (isTimeEvent())
      fw.write("<p>Unmarshall, event is a time event</p>");
    else
      fw.write("<p>Unmarshall</p>");
  }
  else if (isTimeEvent())
    fw.write("<p>Event is a time event</p>");
  
  Q3CString s = trigger();
  Q3CString scpp = cppTrigger();
  Q3CString sjava = javaTrigger();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Trigger :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlCallOperationAction::sKind() {
  return "call operation activity action";
}

void UmlCallOperationAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isSynchronous())
    fw.write("<p>Is synchronous</p>");
    
  if (operation() != 0){
    fw.write("<p>Operation : ");
    operation()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlCallBehaviorAction::sKind() {
  return "call behavior activity action";
}

void UmlCallBehaviorAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isSynchronous())
    fw.write("<p>Is synchronous</p>");
    
  if (behavior() != 0){
    fw.write("<p>Behavior : ");
    behavior()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlClearVariableValueAction::sKind() {
  return "clear variable value activity action";
}

Q3CString UmlReadVariableValueAction::sKind() {
  return "read variable value activity action";
}

Q3CString UmlWriteVariableValueAction::sKind() {
  return "write variable value activity action";
}

Q3CString UmlAddVariableValueAction::sKind() {
  return "add variable value activity action";
}

void UmlAddVariableValueAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isReplaceAll())
    fw.write("<p>Replace all</p>");
    
  if (variable() != 0){
    fw.write("<p>Variable : ");
    variable()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlRemoveVariableValueAction::sKind() {
  return "remove variable value activity action";
}

void UmlRemoveVariableValueAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isRemoveDuplicates())
    fw.write("<p>Remove duplicates</p>");
    
  if (variable() != 0){
    fw.write("<p>Variable : ");
    variable()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlAcceptCallAction::sKind() {
  return "accept call activity action";
}

void UmlAcceptCallAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();
  
  Q3CString s = trigger();
  Q3CString scpp = cppTrigger();
  Q3CString sjava = javaTrigger();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>Trigger :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }
    
    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }
    
    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlReplyAction::sKind() {
  return "reply activity action";
}

void UmlReplyAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  Q3CString s = replyToCall();
  Q3CString scpp = cppReplyToCall();
  Q3CString sjava = javaReplyToCall();

  if (!s.isEmpty() || !scpp.isEmpty() || !sjava.isEmpty()) {
    fw.write("<p>replyToCall :</p><ul>");
    
    if (!s.isEmpty()) {
      fw.write("<li>OCL : <pre>\n");
      writeq(s);
      fw.write("</pre></li>");
    }

    if (!scpp.isEmpty()) {
      fw.write("<li>C++ : <pre>\n");
      writeq(scpp);
      fw.write("</pre></li>");
    }

    if (!sjava.isEmpty()) {
      fw.write("<li>Java : <pre>\n");
      writeq(sjava);
      fw.write("</pre></li>");
    }
    
    fw.write("</ul>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlCreateObjectAction::sKind() {
  return "create object activity action";
}

void UmlCreateObjectAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (!classifier().isEmpty()){
    fw.write("<p>Classifier : ");
    writeq(classifier());
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlDestroyObjectAction::sKind() {
  return "destroy object activity action";
}

void UmlDestroyObjectAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isDestroyLinks()) {
    if (isDestroyOwnedObjects())
      fw.write("<p>Destroy links, destroy owned objects</p>");
    else
      fw.write("<p>Destroy links</p>");
  }
  else if (isDestroyOwnedObjects())
    fw.write("<p>Destroy owned objects</p>");

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlTestIdentityAction::sKind() {
  return "test identity activity action";
}

void UmlTestIdentityAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlRaiseExceptionAction::sKind() {
  return "raise exception activity action";
}

void UmlRaiseExceptionAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

Q3CString UmlReduceAction::sKind() {
  return "reduce activity action";
}

void UmlReduceAction::html(Q3CString pfix, unsigned int rank, unsigned int level) {
  UmlActivityAction::html();

  if (isOrdered())
    fw.write("<p>Ordered</p>");

  if (reducer() != 0){
    fw.write("<p>Reducer : ");
    reducer()->write();
    fw.write("</p>");
  }

  write_children(pfix, rank, level);

  unload(FALSE, FALSE);
}

