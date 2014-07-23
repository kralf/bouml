#ifndef _UMLBASEACTIVITYACTIONCLASSES_H
#define _UMLBASEACTIVITYACTIONCLASSES_H


#include "UmlActivityAction.h"
#include "anItemKind.h"
#include <q3cstring.h>
#include "UmlOnSignalAction.h"
#include "UmlAccessVariableValueAction.h"

class UmlSendObjectAction;
class UmlItem;
class UmlUnmarshallAction;
class UmlSendSignalAction;
class UmlBroadcastSignalAction;
class UmlValueSpecificationAction;
class UmlOpaqueAction;
class UmlAcceptEventAction;
class UmlCallOperationAction;
class UmlOperation;
class UmlCallBehaviorAction;
class UmlClearVariableValueAction;
class UmlReadVariableValueAction;
class UmlWriteVariableValueAction;
class UmlAddVariableValueAction;
class UmlRemoveVariableValueAction;

class UmlBaseSendObjectAction : public UmlActivityAction {
  public:
    //  returns a new send object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSendObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseSendObjectAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseUnmarshallAction : public UmlActivityAction {
  public:
    //  returns a new unmarshall action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUnmarshallAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseUnmarshallAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseSendSignalAction : public UmlOnSignalAction {
  public:
    //  returns a new send signal action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSendSignalAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseSendSignalAction(void * id, const Q3CString & s) : UmlOnSignalAction(id, s) {
    }

};

class UmlBaseBroadcastSignalAction : public UmlOnSignalAction {
  public:
    //  returns a new broadcast signal action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlBroadcastSignalAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseBroadcastSignalAction(void * id, const Q3CString & s) : UmlOnSignalAction(id, s) {
    }

};

class UmlBaseValueSpecificationAction : public UmlActivityAction {
  public:
    //  returns a new value specification action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlValueSpecificationAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseValueSpecificationAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the value
    const Q3CString & value();

    // set the value
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Value(const char * v);

#ifdef WITHCPP
    // return the value in C++
    const Q3CString & cppValue();

    // set the value in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppValue(const char * v);
#endif

#ifdef WITHJAVA
    // return the value in Java
    const Q3CString & javaValue();

    // set the value in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaValue(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    Q3CString _value;

#ifdef WITHCPP
    Q3CString _cpp_value;
#endif

#ifdef WITHJAVA
    Q3CString _java_value;
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

class UmlBaseOpaqueAction : public UmlActivityAction {
  public:
    //  returns a new opaque action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlOpaqueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseOpaqueAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the behavior
    const Q3CString & behavior();

    // set the behavior
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Behavior(const char * v);

#ifdef WITHCPP
    // return the behavior in C++
    const Q3CString & cppBehavior();

    // set the behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppBehavior(const char * v);
#endif

#ifdef WITHJAVA
    // return the behavior in Java
    const Q3CString & javaBehavior();

    // set the behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaBehavior(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    Q3CString _behavior;

#ifdef WITHCPP
    Q3CString _cpp_behavior;
#endif

#ifdef WITHJAVA
    Q3CString _java_behavior;
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

class UmlBaseAcceptEventAction : public UmlActivityAction {
  public:
    //  returns a new accept event action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAcceptEventAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAcceptEventAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isUnmarshall attribute, if TRUE there are multiple output pins for attributes or the event.
    bool isUnmarshall();

    // set the isUnmarshall attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isUnmarshall(bool v);

    // return the  return the isTimeEvent attribute, if TRUE the event is a time event
    bool isTimeEvent();

    // set the isTimeEvent attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isTimeEvent(bool v);

    // return the trigger
    const Q3CString & trigger();

    // set the trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Trigger(const char * v);

#ifdef WITHCPP
    // return the trigger in C++
    const Q3CString & cppTrigger();

    // set the trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTrigger(const char * v);
#endif

#ifdef WITHJAVA
    // return the trigger in Java
    const Q3CString & javaTrigger();

    // set the trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTrigger(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    bool _unmarshall;

    bool _timeevent;

    Q3CString _trigger;

#ifdef WITHCPP
    Q3CString _cpp_trigger;
#endif

#ifdef WITHJAVA
    Q3CString _java_trigger;
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

class UmlBaseCallOperationAction : public UmlActivityAction {
  public:
    //  returns a new call operation action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCallOperationAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCallOperationAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior
    bool isSynchronous();

    // set the isSynchronous attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);

    // return the operation
    UmlOperation * operation();

    // set the operation
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Operation(UmlOperation * v);


  private:
    bool _synchronous;

    UmlOperation * _operation;


  protected:
    virtual void read_uml_();

};

class UmlBaseCallBehaviorAction : public UmlActivityAction {
  public:
    //  returns a new call behavior action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCallBehaviorAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCallBehaviorAction(void * id, const Q3CString & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior
    bool isSynchronous();

    // set the isSynchronous attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);

    // return the behavior, may be an activity or a state machine
    UmlItem * behavior();

    // set the behavior
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Behavior(UmlItem * v);


  private:
    bool _synchronous;

    UmlItem * _behavior;


  protected:
    virtual void read_uml_();

};

class UmlBaseClearVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new clear variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClearVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseClearVariableValueAction(void * id, const Q3CString & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseReadVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new read variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadVariableValueAction(void * id, const Q3CString & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseWriteVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new write variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlWriteVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseWriteVariableValueAction(void * id, const Q3CString & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseAddVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new add variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAddVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAddVariableValueAction(void * id, const Q3CString & s) : UmlAccessVariableValueAction(id, s) {
    }


  public:
    // return the  return the isReplaceAll attribute, if TRUE existing values of the variable must be removed before adding the new value
    bool isReplaceAll();

    // set the isReplaceAll attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReplaceAll(bool v);


  private:
    bool _replace_all;


  protected:
    virtual void read_uml_();

};

class UmlBaseRemoveVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new remove variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRemoveVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRemoveVariableValueAction(void * id, const Q3CString & s) : UmlAccessVariableValueAction(id, s) {
    }


  public:
    // return the  return the isRemoveDuplicates attribute, if TRUE remove duplicates of the value if non-unique
    bool isRemoveDuplicates();

    // set the isRemoveDuplicates attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isRemoveDuplicates(bool v);


  private:
    bool _remove_duplicates;


  protected:
    virtual void read_uml_();

};

#endif
