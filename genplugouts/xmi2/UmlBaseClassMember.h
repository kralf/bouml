#ifndef _UMLBASECLASSMEMBER_H
#define _UMLBASECLASSMEMBER_H


#include "UmlClassItem.h"
#include "aVisibility.h"
#include <q3cstring.h>
#include "UmlBaseClassItem.h"

// This class manages 'true' class's items : attributes, relation,
// operation and sub-classes
class UmlBaseClassMember : public UmlClassItem {
  public:
    // Indicate if the member is a 'class member' (static)
    // Always false in case of a class
    bool isClassMember();

    // Set if the member is a 'class member' (static), not significant for a class
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_isClassMember(bool y);

    // Indicate if the member is 'volatile'
    // Always false in case of a class
    bool isVolatile();

    // Set if the member is 'volatile', not significant for a class
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_isVolatile(bool y);

    // The member's visibility
    aVisibility visibility();

    // Set the member visibility,  DefaultVisibility is not legal
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_Visibility(aVisibility v);

#ifdef WITHCPP
    // Special case for C++, thank to the 'friend class X' it may be
    // usefull to have a C++ visibility != other visibility
    // In case the C++ visibility must follow the other, return
    // DefaultVisibility
    aVisibility cppVisibility();

    // Set the visibility for C++, DefaultVisibility is legal
    //
    // On error return FALSE in C++, produce a RuntimeException in Java,
    // does not check that the class is (already) a typedef
    bool set_CppVisibility(aVisibility v);
#endif

#ifdef WITHJAVA
    // returns the Java annotations
    Q3CString javaAnnotations();

    // set the Java annotations
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaAnnotations(const char * v);
#endif

    // return the constraint
    const Q3CString & constraint();

    // set the constraint
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Constraint(const char * v);


  private:
    bool _class_member;

    bool _volatile;

    aVisibility _visibility : 8;

#ifdef WITHCPP
    aVisibility _cpp_visibility : 8;
#endif

#ifdef WITHJAVA
    Q3CString _java_annotation;
#endif

    Q3CString _constraint;


  protected:
    UmlBaseClassMember(void * id, const Q3CString & n) : UmlClassItem(id, n) {};

    //internal, do NOT use it
    
    void read_uml_();

#ifdef WITHCPP
    //internal, do NOT use it
    
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

  friend class UmlBaseClass;
  friend class UmlBaseRelation;
  friend class UmlBaseAttribute;
  friend class UmlBaseOperation;
};

#endif
