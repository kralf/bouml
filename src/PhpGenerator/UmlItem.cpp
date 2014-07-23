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

#include <q3textstream.h> 
//Added by qt3to4:
#include <Q3CString>
#include <QTextOStream>

#include "UmlItem.h"

UmlItem::~UmlItem() {
}

void UmlItem::manage_comment(const char *& p, const char *& pp,
			     bool javadoc) {
  static QString the_comment;
  
  p += 10;
  
  if ((pp != 0) || // comment contains ${comment} !
      description().isEmpty())
    return;
  
  const char * comment = description();
  
  if (javadoc) {
    the_comment = "/**\n * ";
    
    do {
      the_comment += *comment;
      if ((*comment++ == '\n') && *comment)
	the_comment += " * ";
    } while (*comment);
    
    if (*p != '\n')
      the_comment += (comment[-1] != '\n') ? "\n */\n" : " */\n";
    else
      the_comment += (comment[-1] != '\n') ? "\n */" : " */";
  }
  else {
    the_comment = "//";
    
    do {
      the_comment += *comment;
      if ((*comment++ == '\n') && *comment)
	the_comment += "//";
    } while (*comment);
    
    switch (*p) {
    case 0:
    case '\n':
      break;
    default:
      the_comment += '\n';
    }
  }
    
  pp = p;
  p = the_comment;
}

void UmlItem::manage_description(const char *& p, const char *& pp) {
  static QString the_comment;
  
  p += 14;
  
  if ((pp != 0) || // comment contains ${description} !
      description().isEmpty())
    return;
  
  the_comment = description();

  switch (*p) {
  case 0:
  case '\n':
    break;
  default:
    the_comment += '\n';
  }
    
  pp = p;
  p = the_comment;
}

void UmlItem::manage_alias(const char *& p, QTextOStream & ts) {
  // p starts by '@'
  const char * pclosed;
  
  if ((p[1] == '{') && ((pclosed = strchr(p + 2, '}')) != 0)) {
    Q3CString key(p + 2, pclosed - p - 1);
    Q3CString value;
    UmlItem * node = this;

    do {
      if (node->propertyValue(key, value))
	break;
      node = node->parent();
    } while (node != 0);
    
    if (node != 0)
      // find, insert the value
      ts << value;
    else
      // not find, insert the key
      ts << "@{" << key << '}';

    // bypass the key
    p += strlen(key) + 3;
  }
  else
    // bypass '$'
    ts << *p++;
}

void UmlItem::generate() {
  // does nothing
}

UmlPackage * UmlItem::package() {
  return parent()->package();
}
