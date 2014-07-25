#ifndef _UMLPINPARAMETER_H
#define _UMLPINPARAMETER_H


#include "UmlBasePinParameter.h"
#include <q3cstring.h>

class FileOut;

class UmlPinParameter : public UmlBasePinParameter {
  public:
    virtual void solve_output_flows();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPinParameter(void * id, const Q3CString & s) : UmlBasePinParameter(id, s) {
    }

    void write_dir(FileOut & out);

    void write_flags(FileOut & out);

};

#endif
