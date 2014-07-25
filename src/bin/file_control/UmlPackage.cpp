
#include "UmlPackage.h"

#include <stdlib.h>
#include <qfileinfo.h>
//Added by qt3to4:
#include <Q3CString>

#include "Dialog.h"
#include "UmlCom.h"

void UmlPackage::fileControl(bool ci) {
  UmlPackage * prj = getProject();
  Q3CString prjfile = prj->supportFile();
  BooL rec;
  BooL reload;
  Q3CString cmd;
  
  if (! prj->propertyValue((ci) ? "check-in-cmd" : "check-out-cmd", cmd))
    cmd = "specify the command containing %file and %dir or %dironly";
  
  Dialog dialog(ci, cmd, rec, reload);	// the dialog execution set 'cmd' and 'rec'
  
  if (dialog.exec() == QDialog::Accepted) {
    // save the command for a future usage
    prj->set_PropertyValue((ci) ? "check-in-cmd" : "check-out-cmd", cmd);
    
    if (reload)
      saveProject();
    
    // get files list
    Q3Dict<void> files;
  
    getFiles(files, (rec) ? ~0u : 1);
    if (this == prj)
      getAuxFiles(files);
  
    // apply the command on each file
    Q3DictIterator<void> it(files);
    QFileInfo prjpath(prjfile);
    QString dir = prjpath.dirPath(TRUE);
    QString dironly = dir;
    int index;
    
    if ((dironly.length() > 3) &&
        (((const char *) dironly)[1] == ':') &&
        (((const char *) dironly)[2] == '/'))
      dironly = dironly.mid(2);
  
    while ((index = cmd.find("%dironly")) != -1)
      cmd.replace(index, 8, dironly);
    
    while ((index = cmd.find("%dir")) != -1)
      cmd.replace(index, 4, dir);
      
    while (it.current()) {
      QString s = cmd;
  
      while ((index = s.find("%file")) != -1)
        s.replace(index, 5, it.currentKey());
      
      system((const char *) s);
      ++it;
    }
    
    UmlCom::trace("Done.");
  
    if (reload)
      loadProject(prjfile);
  }
}

void UmlPackage::getFiles(Q3Dict<void> & files, unsigned rec) {
  if (rec != 0)
    UmlItem::getFiles(files, rec - 1);
}

void UmlPackage::getAuxFiles(Q3Dict<void> & files) {
  static const char * aux[] = {
    "cpp_includes", "generation_settings", "idl_includes",
    "java_imports", "stereotypes", "tools", 0
  };
  			
  const char ** p = aux;
  QFileInfo prjpath(supportFile());
  QString dir = prjpath.dirPath(TRUE) + "/";
  
  while(*p != 0) {
    QFileInfo fi(dir + *p);
    
    if (fi.exists())
      files.replace(*p, (void *) 1);
    
    p += 1;
  }
}

