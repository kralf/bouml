
#include "UmlActualParameter.h"
#include "File.h"
#include "UmlTypeSpec.h"

#include "UmlClass.h"
//Added by qt3to4:
#include <Q3CString>
void UmlActualParameter::import(File & f, UmlTypeSpec & t)
{
  f.read("object");
  f.read("Parameter");
    
  Q3CString s;
    
  if (f.read(s) != STRING)
    f.syntaxError(s, "actual's name");
    
  for (;;) {
    switch (f.read(s)) {
    case ')':
      return;
    case ATOM:
      break;
    default:
      f.syntaxError(s);
    }
    
    if (s == "type") {
      if (f.read(t.explicit_type) != STRING)
	f.syntaxError(t.explicit_type, "actual's value");
    }
    else if (s == "quidu") {
      if (f.read(s) != STRING)
	f.syntaxError("wrong quidu");
      
      UmlClass * cl = (UmlClass *) UmlItem::findItem(s, aClass);
      
      if (cl != 0) {
	t.explicit_type = 0;
	t.type = cl;
      }
    }
    else
      f.skipNextForm();
  }
}

