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
#include <q3textstream.h> 
//Added by qt3to4:
#include <Q3CString>
#include <QTextOStream>

#include "UmlArtifact.h"
#include "UmlPackage.h"
#include "UmlOperation.h"
#include "UmlClass.h"
#include "PhpSettings.h"
#include "UmlCom.h"
#include "util.h"

UmlPackage * UmlArtifact::package_of_generated_artifact;
UmlArtifact * UmlArtifact::current;

void UmlArtifact::generate() {
  if (! managed) {
    managed = TRUE;
    
    if (stereotype() == "text") {
      generate_text();
      return;
    }
    else if (stereotype() != "source")
      return;
    
    package_of_generated_artifact = package();
    current = this;
    
    const Q3CString filedef = phpSource();
    
    if (filedef.isEmpty())
      return;
    
    const Q3CString & name = this->name();
    Q3CString path = package_of_generated_artifact->file_path(name);
    
    UmlCom::message(name);
    if (verbose())
      UmlCom::trace(Q3CString("<hr><font face=helvetica>Generate code for <i> ")
		    + name + "</i> in " + path + "</i></font><br>");      
    else
      set_trace_header(Q3CString("<font face=helvetica>Generate code for <i> ")
		       + name + "</i> in " + path + "</i></font><br>"); 
    
    // get bodies if preserve
    const Q3PtrVector<UmlClass> & cls = associatedClasses();
    
    if (preserve())
      UmlOperation::read_bodies(path);
      
    // generate file
    
    unsigned n = cls.count();
    unsigned index;
    Q3CString incl;
    QByteArray file;
    // note : QTextOStream(FILE *) does not work under windows
    QTextOStream f(&file);
    const char * p = filedef;
    const char * pp = 0;
      
    for (;;) {
      if (*p == 0) {
	if (pp == 0)
	  break;
	
	// comment management done
	p = pp;
	pp = 0;
	if (*p == 0)
	  break;
      }
      
      if (*p == '@')
	manage_alias(p, f);
      else if (*p != '$')
	f << *p++;
      else if (!strncmp(p, "${comment}", 10))
	manage_comment(p, pp, PhpSettings::isGenerateJavadocStyleComment());
      else if (!strncmp(p, "${description}", 14))
	manage_description(p, pp);
      else if (!strncmp(p, "${name}", 7)) {
	p += 7;
	f << name;
      }
      else if (!strncmp(p, "${Name}", 7)) {
	p += 7;
	f << capitalize(name);
      }
      else if (!strncmp(p, "${NAME}", 7)) {
	p += 7;
	f << name.upper();
      }
      else if (!strncmp(p, "${nAME}", 7)) {
	p += 7;
	f << name.lower();
      }
      else if (!strncmp(p, "${definition}", 13)) {
	Q3CString indent = current_indent(p, filedef);
	
	for (index = 0; index != n; index += 1)
	  cls[index]->generate(f, indent);
	p += 13;
	if (*p == '\n')
	  p += 1;
      }
      else if (!strncmp(p, "${namespace}", 12)) {
	p += 12;
	
	const Q3CString & nasp = 
	  ((UmlPackage *) parent()->parent())->phpNamespace();
	
	if (!nasp.isEmpty())
	  f << "namespace " << nasp << ";\n";
      }
      else if (!strncmp(p, "${require_once}", 15)) {
	Q3CString require_onces = "";
	
	for (index = 0; index != n; index += 1)
	  cls[index]->generate_require_onces(f, require_onces);
	
	p +=15;
      }
      else
	f << *p++;
    }
      
    f << '\000';
    
    if (must_be_saved(path, file)) {
      write_trace_header();
	
      FILE * fp;
      
      if ((fp = fopen((const char *) path, "wb")) == 0) {
	write_trace_header();
	UmlCom::trace(Q3CString("<font color=\"red\"><b><i> ")
		      + name + "</i> : cannot open <i> "
		      + path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
		      + package_of_generated_artifact->name()
		      + "</i> Php directory specification</b></font><br>");
	incr_error();
      }
      else {
	fputs((const char *) file, fp);
	fclose(fp);
      }
    }
    else if (get_trace_header().isEmpty())
      UmlCom::trace(Q3CString("<font face=helvetica><i> ")
		    + path + "</i> not modified</font><br>");
  }
}

void UmlArtifact::generate_text() {
  const Q3CString srcdef = phpSource();
  
  if (srcdef.isEmpty()) {
    if (verbose())
      UmlCom::trace(Q3CString("<hr><font face=helvetica>artifact <i>")
		    + name() + "</i> has an empty Php definition</font><br>");
    return;
  }
    
  UmlPackage * pack = package();
  const Q3CString & name = UmlArtifact::name();    
  Q3CString src_path = pack->text_path(name);
  
  Q3CString s = " in <i> " + src_path + "</i>";
      
  UmlCom::message(name);
  if (verbose())
    UmlCom::trace(Q3CString("<hr><font face=helvetica>Generate code for <i> ")
		  + name + "</i>" + s + "</font><br>");
  else
    set_trace_header(Q3CString("<font face=helvetica>Generate code for <i> ")
		     + name + "</i>" + s + "</font><br>");
      
  if (must_be_saved(src_path, (const char *) srcdef)) {
    write_trace_header();
    
    FILE * fp_src;
    
    if ((fp_src = fopen((const char *) src_path, "wb")) == 0) {
      write_trace_header();
      UmlCom::trace(Q3CString("<font color=\"red\"><b><i> ")
		    + name + " : </i> cannot open <i> " 
		    + src_path + "</i>, edit the <i> generation settings</i> (tab directory) or the <i>"
		    + pack->name() + "</i> Php directory specification</b></font><br>");
      incr_error();
    }
    else {
      fputs((const char *) srcdef, fp_src);
      fclose(fp_src);
    }
  }
  else if (get_trace_header().isEmpty())
    UmlCom::trace(Q3CString("<font face=helvetica><i> ")
		  + src_path + "</i> not modified</font><br>");
}

UmlPackage * UmlArtifact::generation_package()
{
  return package_of_generated_artifact;
}

UmlArtifact * UmlArtifact::generated_one()
{
  return current;
}

bool UmlArtifact::must_be_saved(const char * path, const char * new_contains)
{
  FILE * fp = fopen(path, "rb");
  
  if (fp == 0)
    return TRUE;
  
  int c;
  
  while ((c = fgetc(fp)) != EOF) {
    if (((char) c) != *new_contains++) {
      fclose(fp);
      return TRUE;
    }
  }
  
  fclose(fp);
  
  return *new_contains != 0;
}
