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

#ifndef UMLATTRIBUTE_H
#define UMLATTRIBUTE_H

#include "UmlBaseAttribute.h"
//Added by qt3to4:
#include <Q3CString>
#include <Q3PtrList>

class Class;

class UmlAttribute : public UmlBaseAttribute {
  public:
    UmlAttribute(void * id, const Q3CString & n)
      :  UmlBaseAttribute(id, n) {};
  
    static bool new_one(Class * container, const Q3CString & name,
			UmlTypeSpec typespec, aVisibility visibility,
			bool staticp, bool finalp, bool transientp,
			bool volatilep, const Q3CString & array,
			const Q3CString & value, Q3CString comment,
			Q3CString description, Q3CString annotation
#ifdef ROUNDTRIP
			, bool roundtrip, Q3PtrList<UmlItem> & expected_order
#endif
			);
    
    static bool manage_enum_item(Q3CString s, UmlClass * cl
#ifdef ROUNDTRIP
				 , bool roundtrip,
				 Q3PtrList<UmlItem> & expected_order
#endif
				 );
};

#endif
