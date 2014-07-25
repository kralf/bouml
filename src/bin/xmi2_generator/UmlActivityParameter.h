#ifndef _UMLACTIVITYPARAMETER_H
#define _UMLACTIVITYPARAMETER_H


#include "UmlBaseActivityParameter.h"
#include <q3cstring.h>

class FileOut;

class UmlActivityParameter : public UmlBaseActivityParameter {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityParameter(void * id, const Q3CString & s) : UmlBaseActivityParameter(id, s) {
    }

    //write the definition if it is not empty for the target language
    virtual void write(FileOut & out);


  private:
    void write_effect(FileOut & out);

};

#endif
