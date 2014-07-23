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

#ifndef _CLASS_CONTAINER_H
#define _CLASS_CONTAINER_H

#include <q3dict.h>
//Added by qt3to4:
#include <Q3CString>
#include "UmlClass.h"

class Class;
class UmlTypeSpec;

class ClassContainer {
  public:
    ClassContainer() {};
    virtual ~ClassContainer();
    
    virtual void compute_type(Q3CString type, UmlTypeSpec & typespec,
			      Class ** need_object = 0) = 0;
    virtual Class * define(const Q3CString & name, char st) = 0;
    
    bool read_type(UmlTypeSpec & typespec, Class ** cl);
    bool bypass_type(Q3CString s);

  protected:
    Q3Dict<Class> Defined;
};

#endif
