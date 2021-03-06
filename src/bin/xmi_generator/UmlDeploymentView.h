#ifndef _UMLDEPLOYMENTVIEW_H
#define _UMLDEPLOYMENTVIEW_H


#include "UmlBaseDeploymentView.h"
#include <q3cstring.h>

class FileOut;

// This class manages 'deployment view'
//
// You can modify it as you want (except the constructor)
class UmlDeploymentView : public UmlBaseDeploymentView {
  public:
    UmlDeploymentView(void * id, const Q3CString & n) : UmlBaseDeploymentView(id, n) {};

    virtual bool write_if_needed(FileOut & out);

    virtual void write(FileOut & out);

};

#endif
