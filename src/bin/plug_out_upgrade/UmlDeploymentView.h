#ifndef UMLDEPLOYMENTVIEW_H
#define UMLDEPLOYMENTVIEW_H

#include "UmlBaseDeploymentView.h"
//Added by qt3to4:
#include <Q3CString>

// This class manages 'deployment view'
// You can modify it as you want (except the constructor)

class UmlDeploymentView : public UmlBaseDeploymentView {
  public:
    UmlDeploymentView(void * id, const Q3CString & n)
      : UmlBaseDeploymentView(id, n) {};
};

#endif
