
#include "UmlMessage.h"
#include "FileOut.h"
#include "UmlItem.h"

#include <q3asciidict.h>

#include "UmlClassInstanceReference.h"

void UmlMessage::write_connectors(FileOut & out, UmlItem * diagram, const Q3PtrVector<UmlMessage> & msgs)
{
  unsigned sup = msgs.size();
  unsigned index;
  Q3AsciiDict<char> connectors;
  
  connectors.setAutoDelete(TRUE);
  
  for (index = 0; index != sup; index += 1) {
    UmlMessage * msg = msgs.at(index);
    
    if ((msg->from() != 0) || (msg->to() != 0))
      // lost or found
      continue;
    
    const char * k = msg->from()->connector(msg->to());
    
    if (connectors.find(k) == 0) {
      char * kk = new char[strlen(k) + 1];
      
      strcpy(kk, k);
      connectors.insert(kk, kk);
      
      out.indent();
      out << "<ownedConnector xmi:type=\"uml:Connector\"";
      out.id_prefix(diagram, k);
      out << ">\n";
      
      static int endrank = 0;
      
      out.indent();
      out << "\t<end xmi:type=\"uml:ConnectorEnd\"";
      out.id_prefix(diagram, "END", ++endrank);
      out.ref(diagram, "role", msg->from()->attribute());
      out << "/>\n";
      
      out.indent();
      out << "\t<end xmi:type=\"uml:ConnectorEnd\"";
      out.id_prefix(diagram, "END", ++endrank);
      out.ref(diagram, "role", msg->to()->attribute());
      out << "/>\n";
      
      out.indent();
      out << "</ownedConnector>\n";
    }
  }
}

int UmlMessage::next_rank;

