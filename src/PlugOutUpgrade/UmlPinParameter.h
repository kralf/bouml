#ifndef _UMLPINPARAMETER_H
#define _UMLPINPARAMETER_H


#include "UmlBasePinParameter.h"
#include <q3cstring.h>

class UmlPinParameter : public UmlBasePinParameter {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPinParameter(void * id, const Q3CString & s) : UmlBasePinParameter(id, s) {
    }

};

#endif
