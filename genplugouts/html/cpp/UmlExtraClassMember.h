#ifndef _UMLEXTRACLASSMEMBER_H
#define _UMLEXTRACLASSMEMBER_H


#include "UmlBaseExtraClassMember.h"
#include <q3cstring.h>

// This class allows to manage extra class member, It allows to insert
// C++ pre-processor directive (even they may be placed in the other member
// definition/declaration), etc ...
//
// You can modify it as you want (except the constructor)

class UmlExtraClassMember : public UmlBaseExtraClassMember {
  public:
    UmlExtraClassMember(void * id, const Q3CString & n) : UmlBaseExtraClassMember(id, n) {};

    //returns a string indicating the king of the element
    virtual Q3CString sKind();

    //entry to produce the html code receiving chapter number
    //path, rank in the mother and level in the browser tree
    virtual void html(Q3CString pfix, unsigned int rank, unsigned int level);

};

#endif
