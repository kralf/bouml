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

#include <qstring.h>
//Added by qt3to4:
#include <Q3CString>

#include "util.h"
#include "UmlCom.h"

Q3CString capitalize(const Q3CString & s)
{
  return (s.isEmpty())
    ? s
    : s.left(1).upper() + s.mid(1);
}

QString capitalize(const QString & s)
{
  return (s.isEmpty())
    ? s
    : s.left(1).upper() + s.mid(1);
}

Q3CString current_indent(const char * p, const char * begin)
{
  QString indent = "";	// to not be Q3CString::null
  
  while ((p != begin) && ((p[-1] == ' ') || (p[-1] == '\t')))
    indent.prepend(*--p);
  
  return Q3CString(indent.toAscii().constData());
}

const char * name_spec(const char * spec)
{
  const char * p = strstr(spec, "${name}");
  
  if (p == 0)
    return 0;
  
  while (p != spec) {
    char c = p[-1];
    
    if (((c >= 'a') && (c <= 'z')) ||
	((c >= 'A') && (c <= 'Z')) ||
	((c >= '0') && (c <= '9')) ||
	(c == '_'))
      p -= 1;
    else
      return p;
  }
  
  return p;
}

bool need_equal(const char * p, const char * v)
{
  for (;;) {
    switch (*v++) {
    case 0:
    case '=':
    case '(':
      return FALSE;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      break;
    default:
      for (;;) {
	switch (*--p) {
	case '=':
	  return FALSE;
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	  break;
	default:
	  return TRUE;
	}
      }
    }
  }
}

//

static Q3CString TraceHeader;
static bool Verbose = FALSE;
static bool Preserve = FALSE;

void set_verbose()
{
  Verbose = TRUE;
}

bool verbose()
{
  return Verbose;
}

void set_preserve()
{
  Preserve = TRUE;
}

bool preserve()
{
  return Preserve;
}

void set_trace_header(Q3CString s)
{
  TraceHeader = s;
}

Q3CString get_trace_header()
{
  return TraceHeader;
}

void write_trace_header()
{
  if (!Verbose && !TraceHeader.isEmpty()) {
    UmlCom::trace("<hr>" + TraceHeader);
    TraceHeader = "";
  }
}

//

static int NErrors;
static int NWarnings;

void incr_error()
{
  NErrors += 1;
}

void incr_warning()
{
  NWarnings += 1;
}

int n_errors()
{
  return NErrors;
}

int n_warnings()
{
  return NWarnings;
}
