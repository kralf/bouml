#ifndef _UMLACTIVITYACTION_H
#define _UMLACTIVITYACTION_H


#include "UmlBaseActivityAction.h"
#include <q3cstring.h>

class FileOut;

class UmlActivityAction : public UmlBaseActivityAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivityAction(void * id, const Q3CString & s) : UmlBaseActivityAction(id, s) {
    }

    //write start of definition, don't close the element list
    void write_begin(FileOut & out, Q3CString k);

    //close the element list and write end of definition without closing it
    
    void write_end(FileOut & out, bool dontclose = FALSE);

    //close the element definition
    void write_close(FileOut & out);

    void write_condition(FileOut & out, Q3CString cond, bool pre);


  public:
    virtual void solve_output_flows();

};

#endif
