#ifndef UMLOPERATION_H
#define UMLOPERATION_H

#include "UmlBaseOperation.h"
//Added by qt3to4:
#include <Q3CString>

// This class manages 'operations', notes that the class 'UmlClassItem'
// is a mother class of the class's children.
// You can modify it as you want (except the constructor)

class UmlOperation : public UmlBaseOperation {
  public:
    UmlOperation(void * id, const Q3CString & n)
      : UmlBaseOperation(id, n) {};
  
    void remove_cpp_throw();
    void add_param(int rank, aDirection dir,
		   const char * name, UmlTypeSpec type);
    void set_cpp(const char * return_form_or_inherit,
		 const char * params, Q3CString body,
		 bool inlinep, const char * if_def, const char * end_if);
    void set_java(const char * return_form, const char * params,
		  Q3CString body, bool inlinep);
    
    virtual void rename_jdk5();
    
    static UmlOperation * java2Php(UmlClass * php, UmlClass * java,
				   const char * name,
				   const char * newname = 0);
    
    static UmlOperation * java2Python(UmlClass * python, UmlClass * java,
				   const char * name,
				   const char * newname = 0);
    
    static UmlOperation * cpp2Python(UmlClass * python, UmlClass * cpp,
				  const char * name,
				  const char * newname = 0);
};

#endif
