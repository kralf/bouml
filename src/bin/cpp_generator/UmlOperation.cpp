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

#include <stdio.h>
#include <stdlib.h>
#include <q3textstream.h> 
#include <qfile.h>
#include <qfileinfo.h>
//Added by qt3to4:
#include <Q3CString>
#include <Q3ValueList>
#include <QTextOStream>
//Added by qt3to4:
#include <Q3PtrList>

#include "UmlOperation.h"
#include "UmlSettings.h"
#include "CppSettings.h"
#include "UmlCom.h"
#include "CppRefType.h"
#include "UmlClass.h"
#include "UmlRelation.h"
#include "util.h"

// to manage preserved bodies, the key is the id under bouml
Q3IntDict<char> UmlOperation::bodies(127);
const char * BodyPrefix = "// Bouml preserved body begin ";
const char * BodyPostfix = "// Bouml preserved body end ";
const int BodyPrefixLength = 30;
const int BodyPostfixLength = 28;

// Between template < and > I suppose that a type is not included
// because I cannot know how the type is used and I do not want to
// produce circular #include
void UmlOperation::compute_dependency(Q3PtrList<CppRefType> & dependencies,
				      const Q3CString & cl_stereotype,
				      bool all_in_h) {
  if ((cl_stereotype == "enum") || (cl_stereotype == "typedef"))
    return;
  
  bool templ = !((UmlClass *) parent())->formals().isEmpty();
  Q3CString decl = cppDecl();
  int index;
    
  if (decl.isEmpty()) {
    decl = cppDef();
    
    if (decl.isEmpty())
      return;
    
    // probably a template operation
    // use definition rather than declaration
    remove_comments(decl);
    remove_preprocessor(decl);
    remove_arrays(decl);
    replace_alias(decl);
    
    if ((index = decl.find("${class}")) != -1)
      decl.remove((unsigned) index, 8);
    if ((index = decl.find("::", index)) != -1)
      decl.remove((unsigned) index, 2);
    if ((index = decl.find("${staticnl}")) != -1)
      decl.remove((unsigned) index, 11);
    if ((index = decl.find("${inline}")) != -1)
      decl.remove((unsigned) index, 9);
    if (((index = decl.find('{')) != -1) &&
	(index != 0) &&
	(((const char *) decl)[index-1] != '$'))
      decl.truncate(index);
  }
  else {
    remove_comments(decl);
    remove_preprocessor(decl);
    remove_arrays(decl);
    replace_alias(decl);
  }
  
  if ((index = decl.find("${friend}")) != -1)
    decl.remove((unsigned) index, 9);
  if ((index = decl.find("${static}")) != -1)
    decl.remove((unsigned) index, 9);
  if ((index = decl.find("${inline}")) != -1)
    decl.remove((unsigned) index, 9);
  if ((index = decl.find("${virtual}")) != -1)
    decl.remove((unsigned) index, 10);
  if ((index = decl.find("${const}")) != -1)
    decl.remove((unsigned) index, 8);
  if ((index = decl.find("${volatile}")) != -1)
    decl.remove((unsigned) index, 11);
  if ((index = decl.find("${abstract}")) != -1)
    decl.remove((unsigned) index, 11);
  if ((index = decl.find("${name}")) != -1)
    decl.remove((unsigned) index, 7);
  if ((index = decl.find("${stereotype}")) != -1)
    decl.replace((unsigned) index, 13,
		 CppSettings::relationAttributeStereotype(stereotype()));
  if ((index = decl.find("${association}")) != -1)
    // dependency computed for the relation
    decl.remove((unsigned) index, 14);

  int index2;
  
  if (((index = decl.find("${(}")) == -1) ||
      ((index2 = decl.find("${)}", index)) == -1))
    return;
  
  decl.replace(index2, 4, ")");
  decl.replace(index, 4, "(");
  
  int template_level = 0;
  const char * p = decl;
  const char * dontsubstituteuntil = 0;
  const Q3ValueList<UmlParameter> & params = this->params();
  UmlTypeSpec ts;
  
  for (;;) {
    char c;
    bool dontsearchend = FALSE;
    
    // search word beginning
    while ((c = *p) != 0) {
      if ((c == '_') ||
	  ((c >= 'a') && (c <= 'z')) ||
	  ((c >= 'A') && (c <= 'Z')))
	break;
      else if (dontsubstituteuntil != 0) {
	if (p >= dontsubstituteuntil)
	  dontsubstituteuntil = 0;
	else
	  p += 1;
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	ts = returnType();
	if (ts.type != 0) {
	  dontsearchend = TRUE;
	  break;
	}
	else {
	  decl = ts.explicit_type + p;
	  p = decl;
	}
      }
      else if (sscanf(p, "${t%u}", (unsigned *) &index) == 1) {
	p = strchr(p, '}') + 1;
	if (((unsigned) index) < params.count()) {
	  // else error signaled later
	  ts = params[index].type;
	  if (ts.type != 0) {
	    dontsearchend = TRUE;
	    break;
	  }
	  else {
	    decl = ts.explicit_type + p;
	    p = decl;
	  }
	}
      }
      else if ((sscanf(p, "${p%u}", &index) == 1) ||
	       (sscanf(p, "${v%u}", &index) == 1))
	p = strchr(p, '}') + 1;
      else if (!strncmp(p, "${throw}", 8)) {
	p += 8;
	const Q3ValueList<UmlTypeSpec> exc = exceptions();
	Q3ValueList<UmlTypeSpec>::ConstIterator it;
	
	for (it = exc.begin(); it != exc.end(); ++it)
	  UmlClassMember::compute_dependency(dependencies, "${type}", *it, TRUE);
      }
      else {
	switch (c) {
	case '{':
	  // body
	  return;
	case '<':
	  template_level += 1;
	  break;
	case '>':
	  template_level -= 1;
	}
	p += 1;
      }
    }
    
    if (c == 0)
      return;
    
    if (!dontsearchend) {
      // search word end
      const char * p2 = p;
      
      ts.type = 0;
      ts.explicit_type = p2;
      p += 1;
      
      while ((c = *p) != 0) {
	if ((c == '_') ||
	    (c == ':') ||
	    ((c >= 'a') && (c <= 'z')) ||
	    ((c >= 'A') && (c <= 'Z')) ||
	    ((c >= '0') && (c <= '9')))
	  p += 1;
	else {
	  ts.explicit_type.truncate(p - p2);
	  break;
	}
      }
    
//#warning NAMESPACE

      if (dontsubstituteuntil == 0) {
	Q3CString subst = CppSettings::type(ts.explicit_type);
	
	if (subst != ts.explicit_type) {
	  decl = subst + ' ' + p;
	  p = decl;
	  dontsubstituteuntil = p + subst.length();
	  continue;
	}
      }
    }
    
    // check manually added keyword
    if ((ts.explicit_type == "const") ||
	(ts.explicit_type == "static"))
      continue;
    
    // search for a * or & or < after the typename
    bool incl = (template_level == 0);
    
    if (incl &&
	(!CppSettings::isInlineOperationForceIncludesInHeader() ||
	 (!templ && !isCppInline()))) {
      while ((c = *p) != 0) {
	if ((c == '*') || (c == '&')) {
	  incl = FALSE;
	  p += 1;
	  break;
	}
	if ((c == '$') ||	// probably ${p}
	    (c == '<') ||
	    (c == '>') ||
	    (c == '(') ||
	    (c == ')') ||
	    (c == ',') ||
	    (c == '_') ||
	    ((c >= 'a') && (c <= 'z')) ||
	    ((c >= 'A') && (c <= 'Z'))) {
	  break;
	}
	p += 1;
      }
    }
    
    CppRefType::add(ts, dependencies, incl || all_in_h);
  }
}

static bool generate_type(const Q3ValueList<UmlParameter> & params,
			  unsigned rank, QTextOStream & f_h)
{
  if (rank >= params.count())
    return FALSE;
  
  UmlClass::write(f_h, params[rank].type);
  
  return TRUE;
}

static bool generate_var(const Q3ValueList<UmlParameter> & params, 
			 unsigned rank, QTextOStream & f_h)
{
  if (rank >= params.count())
    return FALSE;
  
  f_h << params[rank].name;
  return TRUE;
}

static bool generate_init(const Q3ValueList<UmlParameter> & params, 
			  unsigned rank, QTextOStream & f_h)
{
  if (rank >= params.count())
    return FALSE;
  
  if (! params[rank].default_value.isEmpty())
    f_h << " = " << params[rank].default_value;
  
  return TRUE;
}

static void param_error(const Q3CString & parent, const Q3CString & name,
			unsigned rank, const char * where)
{
  write_trace_header();
  UmlCom::trace(Q3CString("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>while compiling <i>")
		+ parent + "::" + name + "</i> " + where
		+ ", parameter rank " + Q3CString().setNum(rank)
		+ " does not exist</font></b><br>");
  incr_error();
}

Q3CString UmlOperation::compute_name() {
  Q3CString get_set_spec = cppNameSpec();
  
  if (! get_set_spec.isEmpty()) {
    UmlClassMember * it;
    
    if ((it = getOf()) == 0)
      it = setOf();
    
    int index;
    Q3CString s = (it->kind() == aRelation)
      ? ((UmlRelation *) it)->roleName()
      : it->name();
    
    if ((index = get_set_spec.find("${name}")) != -1)
      get_set_spec.replace(index, 7, s);
    else if ((index = get_set_spec.find("${Name}")) != -1)
      get_set_spec.replace(index, 7, capitalize(s));
    else if ((index = s.find("${NAME}")) != -1)
      get_set_spec.replace(index, 7, s.upper());
    else if ((index = s.find("${nAME}")) != -1)
      get_set_spec.replace(index, 7, s.lower());
    
    return get_set_spec;
  }
  else
    return name();
}

void UmlOperation::generate_decl(aVisibility & current_visibility, QTextOStream & f_h,
				 const Q3CString & cl_stereotype, Q3CString indent,
				 BooL & first, bool) {
  if (!cppDecl().isEmpty()) {
    if (cl_stereotype == "enum") {
      write_trace_header();
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>an <i>enum</i> cannot have operation</b></font><br>");
      incr_warning();
      return;
    }
    if (cl_stereotype == "typedef") {
      write_trace_header();
      UmlCom::trace("&nbsp;&nbsp;&nbsp;&nbsp;<font color=\"red\"><b>a <i>typedef</i> cannot have operation</b></font><br>");
      incr_warning();
      return;
    }
    
    generate_visibility(current_visibility, f_h, first, indent);
    first = FALSE;
    
    const char * p = cppDecl();
    const char * pp = 0;
    const Q3ValueList<UmlParameter> & params = this->params();
    unsigned rank;
    
    while ((*p == ' ') || (*p == '\t'))
      indent += *p++;
  
    if (*p != '#')
      f_h << indent;
    
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
	if (*p != '#')
	  f_h << indent;
      }

      if (*p == '\n') {
	f_h << *p++;
	if (*p && (*p != '#'))
	  f_h << indent;
      }
      else if (*p == '@')
	manage_alias(p, f_h);
      else if (*p != '$')
	f_h << *p++;
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp, CppSettings::isGenerateJavadocStyleComment());
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${static}", 9)) {
	p += 9;
	if (isClassMember())
	  f_h << "static ";
      }
      else if (!strncmp(p, "${inline}", 9)) {
	p += 9;
	if (isCppInline())
	  f_h << "inline ";
      }
      else if (!strncmp(p, "${friend}", 9)) {
	p += 9;
	if (isCppFriend())
	  f_h << "friend ";
      }
      else if (!strncmp(p, "${virtual}", 10)) {
	p += 10;
	if (isCppVirtual())
	  f_h << "virtual ";
      }
      else if (!strncmp(p, "${type}", 7)) {
	p += 7;
	UmlClass::write(f_h, returnType());
      }
      else if (!strncmp(p, "${stereotype}", 13)) {
	p += 13;
	// get/set relation with multiplicity > 1
	f_h << CppSettings::relationAttributeStereotype(stereotype());
      }
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f_h << compute_name();
      }
      else if (!strncmp(p, "${class}", 8)) {
	// to be placed in the description
	p += 8;
	f_h << parent()->name();
      }
      else if (!strncmp(p, "${(}", 4)) {
	p += 4;
	f_h << '(';
      }
      else if (!strncmp(p, "${)}", 4)) {
	p += 4;
	f_h << ')';
      }
      else if (!strncmp(p, "${const}", 8)) {
	p += 8;
	if (isCppConst())
	  f_h << " const";
      }
      else if (!strncmp(p, "${volatile}", 11)) {
	p += 11;
	if (isVolatile())
	  f_h << " volatile";
      }
      else if (!strncmp(p, "${throw}", 8)) {
	p += 8;
	generate_throw(f_h);
      }
      else if (!strncmp(p, "${abstract}", 11)) {
	p += 11;
	if (isAbstract())
	  f_h << " = 0";
      }
      else if (sscanf(p, "${t%u}", &rank) == 1) {
	if (!generate_type(params, rank, f_h))
	  param_error(parent()->name(), name(), rank, "declaration");
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${p%u}", &rank) == 1) {
	if (!generate_var(params, rank, f_h))
	  param_error(parent()->name(), name(), rank, "declaration");
	p = strchr(p, '}') + 1;
      }
      else if (sscanf(p, "${v%u}", &rank) == 1) {
	if (!generate_init(params, rank, f_h))
	  param_error(parent()->name(), name(), rank, "declaration");
	p = strchr(p, '}') + 1;
      }
      else if (!strncmp(p, "${association}", 14)) {
	p += 14;
	
	UmlClassMember * m;
	
	if (((m = getOf()) != 0) && (m->kind() == aRelation))
	  UmlClass::write(f_h, ((UmlRelation *) m)->association());
	else if (((m = setOf()) != 0) && (m->kind() == aRelation))
	  UmlClass::write(f_h, ((UmlRelation *) m)->association());
      }
      else
	// strange
	f_h << *p++;
    }
    
    f_h << '\n';
  }
}

void UmlOperation::generate_throw(QTextOStream & f) {
  const Q3ValueList<UmlTypeSpec> exc = exceptions();
	
  if (!exc.isEmpty()) {
    Q3ValueList<UmlTypeSpec>::ConstIterator it;
    const char * sep = " throw(";
    
    for (it = exc.begin(); it != exc.end(); ++it) {
      f << sep;
      UmlClass::write(f, *it);
      sep = ", ";
    }
    f << ')';
  }
  else if (CppSettings::operationForceThrow())
    f << " throw()";
}

// p point to {space/tab}*${body}
// indent is the one of the operation
const char * UmlOperation::generate_body(QTextOStream & fs,
					 Q3CString indent,
					 const char * p) {
  const char * body = 0;
  Q3CString modeler_body;
  bool add_nl = FALSE;
  bool no_indent;
  char s_id[9];
  
  if (preserve() && !isBodyGenerationForced()) {
    unsigned id = get_id();
    
    sprintf(s_id, "%08X", id);
    body = bodies.find((long) id);
  }
  
  if (body == 0) {
    no_indent = !cppContextualBodyIndent();
    modeler_body = cppBody();	// to not free the string
    body = modeler_body;
  }
  else
    // body from file, respect its indent
    no_indent = TRUE;
  
  // get keyword indent
  while (*p != '$')
    indent += *p++;

  if (preserve() && !isBodyGenerationForced())
    fs << indent << BodyPrefix << s_id << '\n';
    
  if ((body != 0) && (*body != 0)) {
    // output body
    if (indent.isEmpty() || no_indent) {
      fs << body;
      body += strlen(body);
    }
    else {
      if (*body != '#')
	fs << indent;
      
      while (*body) {
	switch (*body) {
	case '\\':
	  fs << '\\';
	  if (*++body == '\r')
	    fs << *body++;
	  if (*body == '\n')
	    fs << *body++;
	  break;
	case '\n':
	  fs << '\n';
	  if ((*++body != 0) && (*body != '#'))
	    fs << indent;
	  break;
	default:
	  fs << *body++;
	}
      }
    }
    
    add_nl = body[-1] != '\n';
  }
  
  if (preserve() && !isBodyGenerationForced()) {
    if (add_nl)
      fs << '\n';
    
    fs << indent << BodyPostfix << s_id << '\n';
  }
    
  return p + 7;
}

bool UmlOperation::is_template_operation() {
  if (!cppDecl().isEmpty())
    return FALSE;
  
  Q3CString def = cppDef();
  int index1 = def.find("${class}");
  
  if (index1 == -1)
    return FALSE;
  
  int index2 = def.find("${name}", index1 + 8);
  
  if (index2 == -1)
    return FALSE;
  
  def = def.mid(index1 + 8, index2 - index1 - 8);
  
  if ((index1 = def.find('<')) == -1)
    return FALSE;
  
  return (def.find('>') > index1);
}

void UmlOperation::generate_def(QTextOStream & fs, Q3CString indent, bool h,
				Q3CString templates, Q3CString cl_names,
				Q3CString templates_tmplop, 
				Q3CString cl_names_tmplop) {
  if (!cppDef().isEmpty() && !isAbstract()) {
    UmlClass * cl = (UmlClass *) parent();
    
    if ((!templates.isEmpty() || isCppInline() || (cl->name().find('<') != -1))
	? h : !h) {
      const char * p = cppDef();
      const char * pp = 0;
      const Q3ValueList<UmlParameter> & params = this->params();
      unsigned rank;
      const char * body_indent = strstr(p, "${body}");
      bool template_oper = is_template_operation();
      
      if (body_indent != 0) {
	while ((body_indent != p) &&
	       ((body_indent[-1] == ' ') || (body_indent[-1] == '\t')))
	  body_indent -= 1;
      }
      
      // manage old style indent
      while ((*p == ' ') || (*p == '\t'))
	indent += *p++;
      
      bool re_template = !templates.isEmpty() &&
	insert_template(p, fs, indent, 
			(template_oper) ? templates_tmplop : templates);
      
      if (*p != '#')
	fs << indent;
      
      for (;;) {
	if (*p == 0) {
	  if (pp == 0)
	    break;
	  
	  // comment management done
	  p = pp;
	  pp = 0;

	  if (re_template)
	    fs << ((template_oper) ? templates_tmplop : templates);

	  if (*p == 0)
	    break;

	  if (*p != '#')
	    fs << indent;
	}

	if (*p == '\n') {
	  fs << *p++;
	  if (p == body_indent)
	    p = generate_body(fs, indent, p);
	  else if (*p && (*p != '#'))
	    fs << indent;
	}
	else if (*p == '@')
	  manage_alias(p, fs);
	else if (*p != '$') {
	  if (p == body_indent)
	    p = generate_body(fs, indent, p);
	  else
	    fs << *p++;
	}
	else if (!strncmp(p, "${comment}", 10)) {
	  if (!manage_comment(p, pp, CppSettings::isGenerateJavadocStyleComment())
	      && re_template)
	    fs << ((template_oper) ? templates_tmplop : templates);
	}
	else if (!strncmp(p, "${description}", 14)) {
	  if (!manage_description(p, pp) && re_template)
	    fs << ((template_oper) ? templates_tmplop : templates);
	}
	else if (!strncmp(p, "${inline}", 9)) {
	  p += 9;
	  if (isCppInline())
	    fs << "inline ";
	}
	else if (!strncmp(p, "${type}", 7)) {
	  p += 7;
	  UmlClass::write(fs, returnType());
	  //fs << CppSettings::Type(ReturnType().Type());
	}
	else if (!strncmp(p, "${stereotype}", 13)) {
	  p += 13;
	  // get/set relation with multiplicity > 1
	  fs << CppSettings::relationAttributeStereotype(stereotype());
	}
	else if (!strncmp(p, "${class}", 8)) {
	  p += 8;
	  if (isCppFriend() && !strncmp(p, "::", 2))
	    p += 2;
	  else
	    fs << ((template_oper) ? cl_names_tmplop : cl_names);
	}
	else if (!strncmp(p, "${name}", 7)) {
	  p += 7;
	  fs << compute_name();
	}
	else if (!strncmp(p, "${(}", 4)) {
	  p += 4;
	  fs << '(';
	}
	else if (!strncmp(p, "${)}", 4)) {
	  p += 4;
	  fs << ')';
	}
	else if (!strncmp(p, "${const}", 8)) {
	  p += 8;
	  if (isCppConst())
	    fs << " const";
	}
	else if (!strncmp(p, "${volatile}", 11)) {
	  p += 11;
	  if (isVolatile())
	    fs << " volatile";
	}
	else if (!strncmp(p, "${throw}", 8)) {
	  p += 8;
	  generate_throw(fs);
	}
	else if (!strncmp(p, "${staticnl}", 11)) {
	  p += 11;
	  if (isClassMember()) {
	    fs << '\n';
	    if (*p && (*p != '#'))
	      fs << indent;
	  }
	  else
	    fs << ' ';
	}
	else if (sscanf(p, "${t%u}", &rank) == 1) {
	  if (!generate_type(params, rank, fs))
	    param_error(cl->name(), name(), rank, "definition");
	  p = strchr(p, '}') + 1;
	}
	else if (sscanf(p, "${p%u}", &rank) == 1) {
	  if (!generate_var(params, rank, fs))
	    param_error(cl->name(), name(), rank, "definition");	
	  p = strchr(p, '}') + 1;
	}
	else if (!strncmp(p, "${body}", 7) &&
		 (pp == 0))	// not in comment
	  p = generate_body(fs, indent, p);
	else if (!strncmp(p, "${association}", 14)) {
	  p += 14;
	  
	  UmlClassMember * m;
	  
	  if (((m = getOf()) != 0) && (m->kind() == aRelation))
	    UmlClass::write(fs, ((UmlRelation *) m)->association());
	  else if (((m = setOf()) != 0) && (m->kind() == aRelation))
	    UmlClass::write(fs, ((UmlRelation *) m)->association());
	}
	else
	  fs << *p++;
      }
  
      fs << '\n';
    }
  }
}

//

static char * read_file(const char * filename)
{
  QFile fp(filename);
  
  if (fp.open(QIODevice::ReadOnly)) {
    QFileInfo fi(fp);
    int size = fi.size();
    char * s = new char[size + 1];
    
    if (fp.readBlock(s, size) == -1) {
      delete [] s;
      return 0;
    }
    else {
      s[size] = 0;
      
      return s;
    }
  }
  else
    return 0;
}

static void read_bodies(const char * path, Q3IntDict<char> & bodies)
{
  char * s = read_file(path);
  
  if (s != 0) {
    char * p1 = s;
    char * p2;
    
    while ((p2 = strstr(p1, BodyPrefix)) != 0) {
      p2 += BodyPrefixLength;
      
      char * body;
      long id = strtol(p2, &body, 16);
      
      if (body != (p2 + 8)) {
	UmlCom::trace(Q3CString("<font color =\"red\"> Error in ") + path +
		      " : invalid preserve body identifier</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 1");
      }
      
      if (bodies.find(id) != 0) {
	UmlCom::trace(Q3CString("<font  color =\"red\"> Error in ") + path + 
	  " : preserve body identifier used twice</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 2");
      }
      
      if (*body == '\r')
	body += 1;
      if (*body == '\n')
	body += 1;
      else {
	UmlCom::trace(Q3CString("<font  color =\"red\"> Error in ") + path + 
		      " : invalid preserve body block, end of line expected</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 3");
      }
      
      if (((p1 = strstr(body, BodyPostfix)) == 0) ||
	  (strncmp(p1 + BodyPostfixLength, p2, 8) != 0)) {
	UmlCom::trace(Q3CString("<font  color =\"red\"> Error in ") + path + 
		      " : invalid preserve body block, wrong balanced</font><br>");
	UmlCom::bye(n_errors() + 1);
	UmlCom::fatal_error("read_bodies 4");
      }

      p2 = p1;
      while ((p2 != body) && (p2[-1] != '\n'))
	p2 -= 1;
      *p2 = 0;
      
      int len = p2 - body + 1;
      char * b = new char[len];
      
      memcpy(b, body, len);
      bodies.insert(id, b);
      
      p1 += BodyPostfixLength + 8;
    }
    
    delete [] s;
  }
}

void UmlOperation::read_bodies(const char * h_path, const char * src_path)
{
  bodies.setAutoDelete(TRUE);
  bodies.clear();
  ::read_bodies(h_path, bodies);
  ::read_bodies(src_path, bodies);  
}
