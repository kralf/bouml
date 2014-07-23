#ifndef _UMLBASEONSIGNALACTION_H
#define _UMLBASEONSIGNALACTION_H


#include "UmlActivityAction.h"
#include <q3cstring.h>

class UmlBaseOnSignalAction : public UmlActivityAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseOnSignalAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the signal
    const Q3CString & signal();

    // set the signal
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Signal(const char * v);

#ifdef WITHCPP
    // return the signal in C++
    const Q3CString & cppSignal();

    // set the signal in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppSignal(const char * v);
#endif

#ifdef WITHJAVA
    // return the signal in Java
    const Q3CString & javaSignal();

    // set the signal in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaSignal(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    Q3CString _signal;

#ifdef WITHCPP
    Q3CString _cpp_signal;
#endif

#ifdef WITHJAVA
    Q3CString _java_signal;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

#endif
