#ifndef _UMLACTIVITY_H
#define _UMLACTIVITY_H


#include "UmlBaseActivity.h"
#include <q3cstring.h>
#include "anItemKind.h"

class UmlItem;
class Token;
class FileIn;

class UmlActivity : public UmlBaseActivity {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlActivity(void * id, const Q3CString & s) : UmlBaseActivity(id, s) {
       NumberOf += 1; 
    }

    //returns the first container for a 'kind', going up in the browser tree
    virtual UmlItem * container(anItemKind kind, Token & token, FileIn & in);

    //  call at end of import : try to solve reference to operation
    
    virtual void solve(Q3CString idref);

    static void init();

    //import the activity starting by 'tk' inside 'where'
    static void importIt(FileIn & in, Token & token, UmlItem * where);


  private:
    void readCondition(FileIn & in, Token & token);

    void readParameter(FileIn & in, Token & token);

    void readParameterNode(FileIn & in, Token & token);


  public:
    static int numberOf() { return NumberOf; };


  protected:
    static int NumberOf;

};

#endif
