
#include "UmlClassView.h"

#include "UmlDeploymentView.h"
#include "UmlCom.h"
void UmlClassView::deploy() {
  // deployment needed ?
  
  const Q3PtrVector<UmlItem> ch = children();
  unsigned i;
  bool need = FALSE;
    
  for (i = 0; i != ch.size(); i += 1) {
    if (ch[i]->deployable() != 0){
      need = TRUE;
      break;
    }
  }
  
  if (! need)
    return;
  
  UmlDeploymentView * depl = associatedDeploymentView();
  
  if (depl == 0) {
    // create the associated deployment view
    UmlCom::trace("Create <i>deployment view</i> " + name());
    
    if ((depl = UmlDeploymentView::create((UmlPackage*) parent(), name())) == 0) {
      UmlCom::trace("cannot create <i>deployment view</i> named " + name());
      return;
    }
    else
      set_AssociatedDeploymentView(depl);
  }
  
  // look at non deployed classes
  
  for (i = 0; i != ch.size(); i += 1)
    ch[i]->deploy(depl);
  
}

