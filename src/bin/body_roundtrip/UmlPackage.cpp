// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#include <qdir.h>
//Added by qt3to4:
#include <Q3CString>

#include "UmlPackage.h"
#include "UmlCom.h"
#include "CppSettings.h"
#include "JavaSettings.h"
#include "PhpSettings.h"
#include "PythonSettings.h"
#include "util.h"

UmlPackage::UmlPackage(void * id, const Q3CString & n)
    : UmlBasePackage(id, n) {
  dir.read = FALSE;
}

static bool RootDirRead;
static Q3CString RootDir;

Q3CString UmlPackage::rootDir(aLanguage who)
{
  if (! RootDirRead) {
    RootDirRead = TRUE;
    
    switch (who){
    case cppLanguage:
      RootDir = CppSettings::rootDir();
      break;
    case javaLanguage:
      RootDir = JavaSettings::rootDir();
      break;
    case phpLanguage:
      RootDir = PhpSettings::rootDir();
      break;
    default:
      RootDir = PythonSettings::rootDir();
    }
    
    if (!RootDir.isEmpty() && // empty -> error
	QDir::isRelativePath(RootDir)) {
      QFileInfo f(getProject()->supportFile());
      QDir d(f.dirPath());
      
      RootDir = d.filePath(RootDir);
    }
  }

  return RootDir;
}

Q3CString UmlPackage::source_path(const Q3CString & f) {
  if (!dir.read) {
    dir.src = cppSrcDir();
    dir.h = cppHDir();
    
    QDir d_root(rootDir(cppLanguage));
    
    if (dir.src.isEmpty())
      dir.src = RootDir;
    else if (QDir::isRelativePath(dir.src))
      dir.src = d_root.filePath(dir.src);

    if (dir.h.isEmpty())
      dir.h = RootDir;
    else if (QDir::isRelativePath(dir.h))
      dir.h = d_root.filePath(dir.h);
   
    if (dir.src.isEmpty()) {
      UmlCom::trace(Q3CString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
		    "or edit the package (tab 'C++')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::source_path");
    }
        
    dir.read = TRUE;
  }
  
  QDir d(dir.src);
  
  return Q3CString(d.filePath(f).toAscii().constData()) + Q3CString(".") + CppSettings::sourceExtension();
}

Q3CString UmlPackage::header_path(const Q3CString & f) {
  if (!dir.read) {
    source_path(f);
   
    if (dir.h.isEmpty()) {
      UmlCom::trace(Q3CString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
			    "or edit the package (tab 'C++')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::source_path");
    }
    
    if (QDir::isRelativePath(dir.h)) {
      UmlCom::trace(Q3CString("<font color=\"red\"><b><i>")
		    + name() + "</i>'s header path <i>(" + dir.h
		    + "</i>) is not absolute, edit the <i> generation settings</i> "
		    "(tab 'directory'), or edit the package (tab 'C++')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::source_path");
    }
  }
  
  QDir d(dir.h);
  
  return Q3CString(d.filePath(f).toAscii().constData()) + Q3CString(".") + CppSettings::headerExtension();
}

void UmlPackage::roundtrip_cpp() {
  Q3PtrVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->roundtrip_cpp();
}

Q3CString UmlPackage::java_path(const Q3CString & f) {
  if (!dir.read) {
    dir.src = javaDir();
    
    QDir d_root(rootDir(javaLanguage));
    
    if (dir.src.isEmpty())
      dir.src = RootDir;
    else if (QDir::isRelativePath(dir.src))
      dir.src = d_root.filePath(dir.src);

    if (dir.src.isEmpty()) {
      UmlCom::trace(Q3CString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
		    "or edit the package (tab 'Java')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::java_path");
    }
        
    dir.read = TRUE;
  }
  
  QDir d(dir.src);
  
  return Q3CString(d.filePath(f).toAscii().constData()) + Q3CString(".") + JavaSettings::sourceExtension();
}

Q3CString UmlPackage::php_path(const Q3CString & f) {
  if (!dir.read) {
    dir.src = phpDir();
    
    QDir d_root(rootDir(phpLanguage));
    
    if (dir.src.isEmpty())
      dir.src = RootDir;
    else if (QDir::isRelativePath(dir.src))
      dir.src = d_root.filePath(dir.src);

    if (dir.src.isEmpty()) {
      UmlCom::trace(Q3CString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
		    "or edit the package (tab 'php')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::php_path");
    }
        
    dir.read = TRUE;
  }
  
  QDir d(dir.src);
  
  return Q3CString(d.filePath(f).toAscii().constData()) + Q3CString(".") + PhpSettings::sourceExtension();
}

Q3CString UmlPackage::python_path(const Q3CString & f) {
  if (!dir.read) {
    dir.src = pythonDir();
    
    QDir d_root(rootDir(pythonLanguage));
    
    if (dir.src.isEmpty())
      dir.src = RootDir;
    else if (QDir::isRelativePath(dir.src))
      dir.src = d_root.filePath(dir.src);

    if (dir.src.isEmpty()) {
      UmlCom::trace(Q3CString("<font color=\"red\"><b><b> The generation directory "
			    "must be specified for the package<i> ") + name()
		    + "</i>, edit the <i> generation settings</i> (tab 'directory') "
		    "or edit the package (tab 'python')</b></font><br>");
      UmlCom::bye(n_errors() + 1);
      UmlCom::fatal_error("UmlPackage::python_path");
    }
        
    dir.read = TRUE;
  }
  
  QDir d(dir.src);
  
  return Q3CString(d.filePath(f).toAscii().constData()) + Q3CString(".") + PythonSettings::sourceExtension();
}

void UmlPackage::roundtrip_java() {
  Q3PtrVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->roundtrip_java();
}

void UmlPackage::roundtrip_php() {
  Q3PtrVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->roundtrip_php();
}

void UmlPackage::roundtrip_python() {
  Q3PtrVector<UmlItem> ch = UmlItem::children();
  
  for (unsigned index = 0; index != ch.size(); index += 1)
    ch[index]->roundtrip_python();
}

UmlPackage * UmlPackage::package() {
  return this;
}
