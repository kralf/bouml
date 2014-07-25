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

#include "CppRefType.h"
#include "UmlClass.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "UmlPackage.h"
#include "UmlArtifact.h"
#include "UmlCom.h"
#include "util.h"
//Added by qt3to4:
#include <Q3CString>
//Added by qt3to4:
#include <Q3PtrList>

// not in case of a dependency => external class in h
bool CppRefType::add(const UmlTypeSpec & t, Q3PtrList<CppRefType> & l,
		     bool incl)
{
  return (t.type)
    ? add(t.type, l, incl || t.type->isCppExternal())
    : add(t.explicit_type, l, incl);
}

bool CppRefType::add(UmlClass * cl, Q3PtrList<CppRefType> & l,
		     bool incl, bool hight)
{
  CppRefType * ref;
  Q3CString t = cl->name();
  Weight w;
  
  if (cl->parent()->kind() == aClass) {
    // nested => need include parent
    do
      cl = (UmlClass *) cl->parent();
    while (cl->parent()->kind() == aClass);
    
    incl = TRUE;
    w = Strong;
  }
  else if (incl)
    w = (hight) ? Strong : Medium;
  else
    w = (hight) ? High : Low;
  
  for (ref = l.first(); ref; ref = l.next()) {
    // don't use ref->type.toString() because of synonymous
    // in several namespaces
    if ((ref->type.type != 0)
	? (ref->type.type == cl)
	: (ref->type.explicit_type == t)) {
      if (w > ref->weight)
	ref->included = incl;
      return TRUE;
    }
  }
  
  l.append(new CppRefType(cl, incl, w));
  return TRUE;
}

bool CppRefType::add(const Q3CString & t, Q3PtrList<CppRefType> & l, bool incl)
{
  if (t.isEmpty())
    return FALSE;

  static Q3Dict<char> cpp_builtin_types;
  
  if (cpp_builtin_types.count() == 0) {
    cpp_builtin_types.insert("unsigned", " ");
    cpp_builtin_types.insert("signed", " ");
    cpp_builtin_types.insert("char", " ");
    cpp_builtin_types.insert("short", " ");
    cpp_builtin_types.insert("int", " ");
    cpp_builtin_types.insert("long", " ");
    cpp_builtin_types.insert("float", " ");
    cpp_builtin_types.insert("double", " ");
    cpp_builtin_types.insert("bool", " ");
    cpp_builtin_types.insert("void", " ");
  }
  
  if (cpp_builtin_types[t] != 0)
    return TRUE;
  
  CppRefType * ref;
  Weight w = (incl) ? Medium : Low;
  
  for (ref = l.first(); ref; ref = l.next()) {
    if (ref->type.toString() == t) {
      if (w > ref->weight)
	ref->included = incl;
      return TRUE;
    }
  }
  
  l.append(new CppRefType(t, incl, w));
  return TRUE;
}

void CppRefType::remove(const Q3CString & t, Q3PtrList<CppRefType> & l)
{
  Q3PtrListIterator<CppRefType> it(l);
  
  for (; it.current(); ++it) {
    if ((*it)->type.explicit_type == t) {
      delete *it;
      l.remove(it);
      return;
    }
  }
}

void CppRefType::remove(UmlClass * cl, Q3PtrList<CppRefType> & l)
{
  Q3PtrListIterator<CppRefType> it(l);
  
  for (; it.current(); ++it) {
    if ((*it)->type.type == cl) {
      l.remove(it);
      return;
    }
  }
}

void CppRefType::force_ref(UmlClass * cl, Q3PtrList<CppRefType> & l)
{
  CppRefType * ref;
  Q3CString t = cl->name();
  
  for (ref = l.first(); ref; ref = l.next()) {
    // don't use ref->type.toString() because of synonymous
    // in several namespaces
    if ((ref->type.type != 0)
	? (ref->type.type == cl)
	: (ref->type.explicit_type == t)) {
      ref->included = FALSE;
      return;
    }
  }
}

void CppRefType::compute(Q3PtrList<CppRefType> & dependencies,
			 const Q3CString & hdef, const Q3CString & srcdef,
			 Q3CString & h_incl,  Q3CString & decl, Q3CString & src_incl,
			 UmlArtifact * who)
{
  UmlPackage * pack = who->package();
  Q3CString hdir;
  Q3CString srcdir;
  
  if (CppSettings::isRelativePath()) {
    Q3CString empty;
    
    hdir = pack->header_path(empty);
    srcdir = pack->source_path(empty);
  }
  else if (CppSettings::isRootRelativePath())
    hdir =  srcdir = UmlPackage::rootDir();
  
  // aze.cpp includes aze.h
  src_incl += "#include \"";
  if (CppSettings::includeWithPath())
    src_incl += pack->header_path(who->name(), srcdir);
  else {
    src_incl += who->name();
    src_incl += '.';
    src_incl += CppSettings::headerExtension();
  }
  src_incl += "\"\n";
  h_incl = "";	// to not be Q3CString::null
  decl = "";	// to not be Q3CString::null
  
  CppRefType * ref;
  
  for (ref = dependencies.first(); ref != 0; ref = dependencies.next()) {
    UmlClass * cl = (ref->type.type)
      ? ref->type.type
      : UmlBaseClass::get(ref->type.explicit_type, 0);
    bool included = ref->included;
    Q3CString hform;	// form in header
    Q3CString srcform;	// form in source
    
    if (cl == 0) {
      Q3CString in = CppSettings::include(ref->type.explicit_type);
      
      if (!in.isEmpty()) 
	hform = srcform = in + '\n';
      else
	// doesn't know what it is
	continue;
    }
    else if (cl->isCppExternal()) {
      hform = cl->cppDecl();
      
      int index;
      
      if ((index = hform.find('\n')) == -1)
	// wrong form
	continue;
      
      hform = hform.mid(index + 1) + '\n';
      
      for (;;) {
	if ((index = hform.find("${name}")) != -1)
	  hform.replace(index, 7, cl->name());
	else if ((index = hform.find("${Name}")) != -1)
	  hform.replace(index, 7, capitalize(cl->name()));
	else if ((index = hform.find("${NAME}")) != -1)
	  hform.replace(index, 7, cl->name().upper());
	else if ((index = hform.find("${nAME}")) != -1)
	  hform.replace(index, 7, cl->name().lower());
	else
	  break;
      }
      srcform = hform;
    }
    else {
      Q3CString st = cl->cpp_stereotype();	
      
      if ((st == "enum") || (st == "typedef"))
	included = TRUE;
      
      UmlArtifact * art = cl->associatedArtifact();
      
      if (art != 0) {
	if (art == who)
	  // don't include itself
	  continue;
	if (CppSettings::includeWithPath()) {
	  UmlPackage * p = art->package();
	  
	  hform = "#include \"" + p->header_path(art->name(), hdir) + "\"\n";
	  srcform = "#include \"" + p->header_path(art->name(), srcdir) + "\"\n";
	}
	else
	  srcform = hform = "#include \"" + art->name() + '.' +
	    CppSettings::headerExtension() + "\"\n";
      }
      else if (cl->parent()->kind() != aClass) {
	write_trace_header();
	UmlCom::trace(Q3CString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b> class<i> ") + cl->name() +
		      "</i> referenced but does not have associated <i>artifact</i></b></font><br>");
	incr_warning();
	continue;
      }
    }
    
    if (included) {
      // #include must be placed in the header file
      if ((h_incl.find(hform) == -1) && (hdef.find(hform) == -1))
	h_incl += hform;
    }
    else if ((cl != 0) &&
	     (cl->parent()->kind() != aClass)) {	// else too complicated
      // #include useless in header file, place it in the source file
      if ((src_incl.find(srcform) == -1) && (h_incl.find(hform) == -1) &&
	  (hdef.find(hform) == -1) && (srcdef.find(srcform) == -1))
	src_incl += srcform;
      
      if (!cl->isCppExternal()) {
	// header file must contains the declaration
	hform = cl->decl();
	
	if (decl.find(hform) == -1)
	  decl += hform;
	
	if ((cl->associatedArtifact() == 0) &&
	    (cl->parent()->kind() != aClass)) {
	  write_trace_header();
	  UmlCom::trace(Q3CString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b> class<i> ") + cl->name() +
			"</i> referenced but does not have associated <i>artifact</i></b></font><br>");
	  incr_warning();
	}
      }
    }
    else if (!hform.isEmpty()) {
      // have the #include form but does not know if it is a class or other,
      // generate the #include in the header file EXCEPT if the #include is
      // already in the header/source file to allow to optimize the generated
      // code
      if ((src_incl.find(srcform) == -1) && (h_incl.find(hform) == -1) &&
	  (hdef.find(hform) == -1) && (srcdef.find(srcform) == -1))
	h_incl += hform;
    }
  }
}
