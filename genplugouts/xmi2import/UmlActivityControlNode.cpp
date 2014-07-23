
#include "UmlActivityControlNode.h"
#include "FileIn.h"
#include "Token.h"
#include "UmlItem.h"

#include "UmlActivityControlNodeClasses.h"
//Added by qt3to4:
#include <Q3CString>
void UmlActivityControlNode::init()
{
  declareFct("node", "uml:InitialNode", &importIt);
  declareFct("containednode", "uml:InitialNode", &importIt);
  
  declareFct("node", "uml:FlowFinalNode", &importIt);
  declareFct("containednode", "uml:FlowFinalNode", &importIt);
  
  declareFct("node", "uml:FinalNode", &importIt);
  declareFct("containednode", "uml:FinalNode", &importIt);
  
  declareFct("node", "uml:DecisionNode", &importIt);
  declareFct("containednode", "uml:DecisionNode", &importIt);
  
  declareFct("node", "uml:MergeNode", &importIt);
  declareFct("containednode", "uml:MergeNode", &importIt);
  
  declareFct("node", "uml:ForkNode", &importIt);
  declareFct("containednode", "uml:ForkNode", &importIt);
  
  declareFct("node", "uml:JoinNode", &importIt);
  declareFct("containednode", "uml:JoinNode", &importIt);
}

void UmlActivityControlNode::importIt(FileIn & in, Token & token, UmlItem * where)
{
  where = where->container(anInitialActivityNode, token, in);
    
  if (where != 0) {
    Q3CString s = token.valueOf("name");
    UmlActivityControlNode * a;
    Q3CString t = token.xmiType();
    
    t = t.mid(4, t.length() - 8);
    
    if (t == "Initial")
      a = UmlInitialActivityNode::create(where, s);
    else if (t == "FlowFinal")
      a = UmlFlowFinalActivityNode::create(where, s);
    else if (t == "Final")
      a = UmlActivityFinalActivityNode::create(where, s);
    else if (t == "Decision")
      a = UmlDecisionActivityNode::create(where, s);
    else if (t == "Merge")
      a = UmlMergeActivityNode::create(where, s);
    else if (t == "Fork")
      a = UmlForkActivityNode::create(where, s);
    else // Join
      a = UmlJoinActivityNode::create(where, s);
      
    if (a == 0)
      in.error("cannot create activity control node of type '"
	       + t + "' in '" + where->name() + "'");
    
    a->addItem(token.xmiId(), in);
    
    if (! token.closed()) {
      Q3CString k = token.what();
      const char * kstr = k;
      
      while (in.read(), !token.close(kstr))
	a->UmlItem::import(in, token);
    }
  }
}

