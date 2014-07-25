#ifndef _UMLFORMALPARAMETER_H
#define _UMLFORMALPARAMETER_H


#include "UmlBaseFormalParameter.h"
#include <q3cstring.h>

// This class manages 'formal parameters' of a template class. For instance 'T' in 
//
//	template class Cl<class T> ...
//
// You can modify it as you want (except the constructor)
class UmlFormalParameter : public UmlBaseFormalParameter {
  public:
    UmlFormalParameter() {};

    UmlFormalParameter(Q3CString name, Q3CString value);

};

inline UmlFormalParameter::UmlFormalParameter(Q3CString name, Q3CString value) {
  _name = name;
  _type = "typename";
  _default_value.explicit_type = value;
}

#endif
