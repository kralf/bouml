
#include "UmlFormalParameter.h"
#include "File.h"
//Added by qt3to4:
#include <Q3CString>

void UmlFormalParameter::import(File & f, bool scanning) {
  f.read("object");
  f.read("Parameter");
    
  if (f.read(_name) != STRING)
    f.syntaxError(_name, "formal's name");
    
  Q3CString s;
  
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
      if (f.read(_type) != STRING)
	f.syntaxError(_type, "formal's type");
    }
    else if (s == "initv") {
      if (scanning)
	f.skipNextForm();
      else {
	// !!!!!!!!!!!!
	if (f.read(_default_value.explicit_type) != STRING)
	  f.syntaxError(_default_value.explicit_type, "formal's default value");
      }
    }
    else
      f.skipNextForm();
  }
}

