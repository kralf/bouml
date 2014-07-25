
#include <stdio.h>
#include <qfile.h>
//Added by qt3to4:
#include <Q3CString>

#include "UmlCom.h"
#include "Dialog.h"
#include "UmlNcRelation.h"
#include "UmlOperation.h"

#include "UmlPackage.h"
#include "FileOut.h"
#include "UmlItem.h"
#include "UmlUseCaseDiagram.h"

void UmlPackage::xmi(int argc, char ** argv) {
  if (parent() != 0) {
    UmlItem::xmi(argc, argv);
    return;
  }

  // the project

  bool noarg = (argc == 0);
  Q3CString path;
  Q3CString encoding;
  Q3CString nomodel;
  Q3CString genviews;
  Q3CString uml20;
  Q3CString pkprefix;
  Q3CString visprefix;
  Q3CString primitivetype;
  Q3CString genextension;
  Q3CString geneclipse;
  Q3CString commentexporter;
  Q3CString linefeed;
  
  bool comment_exporter = FALSE;  
  bool no_model = FALSE;

  if (noarg) {
    propertyValue("gxmi path", path);
    propertyValue("gxmi no model", nomodel);
    propertyValue("gxmi gen views", genviews);
    propertyValue("gxmi encoding", encoding);
    propertyValue("gxmi uml2.0", uml20);
    propertyValue("gxmi pk prefix", pkprefix);
    propertyValue("gxmi vis prefix", visprefix);
    propertyValue("gxmi primitive type", primitivetype);
    propertyValue("gxmi gen extensions", genextension);
    propertyValue("gxmi gen eclipse", geneclipse);
    propertyValue("gxmi comment exporter", commentexporter);
    propertyValue("gxmi linefeed", linefeed);
    
    Dialog dialog(path, encoding, nomodel, genviews, uml20, pkprefix, visprefix,
		  primitivetype, genextension, geneclipse, commentexporter, linefeed, _lang);
    
    if (dialog.exec() != QDialog::Accepted)
      return;
  }
  else if (argc >= 3) {	// at least language path and encoding
    if (!strcmp(argv[0], "-uml"))
      _lang = Uml;
    else if (!strcmp(argv[0], "-c++"))
      _lang = Cpp;
    else if (!strcmp(argv[0], "-java"))
      _lang = Java;
    else {
      UmlCom::trace("language is missing");
      return;
    }
    path = argv[1];
    encoding = argv[2];
    argc -= 3;
    argv += 3;
    
    _uml_20 = FALSE;
    _gen_views = FALSE;
    _pk_prefix = FALSE;
    _vis_prefix = FALSE;
    _primitive_type = FALSE;
    _gen_extension = FALSE;
    _gen_eclipse = FALSE;
    _linefeed = FALSE;
    
    while (argc != 0) {
      if (! strcmp(argv[0], "-nomodel"))
	no_model = TRUE;
      else if (!strcmp(argv[0], "-view"))
	_gen_views = TRUE;
      else if (! strcmp(argv[0], "-uml2.0"))
	_uml_20 = TRUE;
      else if (! strcmp(argv[0], "-pk"))
	_pk_prefix = TRUE;
      else if (! strcmp(argv[0], "-vis"))
	_vis_prefix = TRUE;
      else if (! strcmp(argv[0], "-primitypeType"))
	_primitive_type = TRUE;
      else if (! strcmp(argv[0], "-extension"))
	_gen_extension = TRUE;
      else if (! strcmp(argv[0], "-eclipse"))
	_gen_eclipse = TRUE;
      else if (! strcmp(argv[0], "-commentexporter"))
	comment_exporter = TRUE;
      else if (! strcmp(argv[0], "-lf"))
	_linefeed = TRUE;
      else {
	UmlCom::trace("wrong parameters");
	return;
      }
      argc -= 1;
      argv += 1;
    }
  }
  else {
    UmlCom::trace("wrong number of parameters");
    return;
  }
  

  if (! path.isEmpty()) {
    // note : QTextStream(FILE *) bugged under windows
    QFile fp(path);
    
    if (! fp.open(QIODevice::WriteOnly))
      UmlCom::trace("cannot open " + path);
    else {
      if (noarg) {
	set_PropertyValue("gxmi path", path);
	set_PropertyValue("gxmi no model", nomodel);
	set_PropertyValue("gxmi gen views", genviews);
	set_PropertyValue("gxmi encoding", encoding);
	set_PropertyValue("gxmi uml2.0", uml20);
	set_PropertyValue("gxmi pk prefix", pkprefix);
	set_PropertyValue("gxmi vis prefix", visprefix);
	set_PropertyValue("gxmi primitive type", primitivetype);
	set_PropertyValue("gxmi gen extensions", genextension);
	set_PropertyValue("gxmi gen eclipse", geneclipse);
	set_PropertyValue("gxmi comment exporter", commentexporter);
	set_PropertyValue("gxmi linefeed", linefeed);
	no_model = (nomodel == "yes");
	_gen_views = (genviews == "yes");
	_uml_20 = (uml20 == "yes");
	_pk_prefix = (pkprefix == "yes");
	_vis_prefix = (visprefix == "yes");
	_primitive_type = (primitivetype == "yes");
	_gen_extension = (genextension == "yes");
	_gen_eclipse = (geneclipse == "yes");
	comment_exporter = (commentexporter == "yes");
	_linefeed = (linefeed == "yes");
      }
	
      {
	bool utf8;
	
	if (encoding.left(3).lower() == "utf") {
	  if (encoding.right(1) != "8") {
	    encoding = "UTF-8";
	    UmlCom::trace("warning : replace encoding by UTF-8");
	  }
	  utf8 = TRUE;
	}
	else
	  utf8 = FALSE;
	
	FileOut out(&fp, _linefeed, utf8);
	Q3PtrList<UmlPackage> profiles;
	UmlPackage * prof;
	
	search_profiles(profiles);
	
	out << "\
<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>\n\
<xmi:XMI xmi:version=\"2." << ((_uml_20) ? "0" : "1")
  << "\" xmlns:uml=\"http://schema.omg.org/spec/UML/2." << ((_uml_20) ? "0" : "1")
  << "\" xmlns:xmi=\"http://schema.omg.org/spec/XMI/2." << ((_uml_20) ? "0" : "1")
  << "\"";
	
	for (prof = profiles.first(); prof != 0; prof = profiles.next()) {
	  out << " xmlns:";
	  out.quote((const char*)prof->name());//[jasa] ambiguous call
	  out << "=\"http:///schemas/";
	  out.quote((const char*)prof->name());//[jasa] ambiguous call
	  out << "/1\"";
	}
	
	out << ">\n\t";
	if (comment_exporter)
	  out << "<!-- ";
	out << "<xmi:Documentation exporter=\"Bouml\" exporterVersion=\"1.14.1\"/>";
	if (comment_exporter)
	  out << " -->\n";
	else
	  out << '\n';
	
	if (no_model)
	  out.indent(+1);
	else {
	  out << "\t<uml:Model xmi:type=\"uml:Model\" xmi:id=\"themodel\" name=\""
	    << name() << "\">\n";
	  out.indent(+2);
	}
	
	for (prof = profiles.first(); prof != 0; prof = profiles.next()) {
	  out.indent();
	  out << "<profileApplication xmi:type=\"uml:ProfileApplication\"";
	  out.id_prefix(prof, "PRFA_");
	  out << ">\n";
	  out.indent();
	  out << "<appliedProfile";
	  out.idref(prof);
	  out << "/>\n";
	  out.indent();
	  out << "</profileApplication>\n";
	}
  
	search_class_assoc();
	write(out);
		
	out.define_datatypes(_uml_20, _primitive_type, _gen_extension);
	UmlOperation::write_events(out);
	
	if (! no_model)
	  out << "\t</uml:Model>\n";
	
	write_stereotyped(out);

	out << "</xmi:XMI>\n";
      }
      
      UmlCom::trace("xmi generation done in <i>" + path + "</i><br>");
      
      fp.flush();
      fp.close();
    }
  }
}

void UmlPackage::write(FileOut & out) {
  const char * k = (_uml_20) ? "ownedMember" : "packagedElement";
  bool is_profile = (stereotype() == "profile");
  
  out.indent(); 
  out << '<' << k << " xmi:type=\"uml:"
    << ((is_profile) ? "Profile\"" : "Package\""); 
  out.id(this); 
  out << " name =\"";
  out.quote((const char*)name());//[jasa] ambiguous call
  out << '"'; 
  
  Q3CString mcr;
  Q3CString mmr;
  
  if (is_profile) {
    propertyValue("metaclassreference", mcr);
    propertyValue("metamodelReference", mmr);
    
    if (!mcr.isEmpty())
      out.ref(this, "metaclassReference", "MCR_");
    else if (!mmr.isEmpty())
      out.ref(this, "metamodelReference", "MMR_");
  }
  out << ">\n"; 
  out.indent(+1); 
  write_description_properties(out);
  
  if (!mcr.isEmpty()) {
    out.indent(); 
    out << "<elementImport";
    out.id_prefix(this, "MCR_");
    
    int index = mcr.findRev('#');
    
    if (index != -1)
      out << " alias=\"" << mcr.mid(index + 1) << "\">\n";
    else
      out << ">\n";
    
    out.indent(); 
    out << "\t<importedElement xmi:type=\"uml:Class\" href=\"" << mcr << "#1\"/>\n";
    out.indent(); 
    out << "</elementImport>\n";
  }

  if (!mmr.isEmpty()) {
    out.indent(); 
    out << "<packageImport xmi:type=\"uml:PackageImport\"";
    out.id_prefix(this, "MMR_");
    out << ">\n";
    out.indent(); 
    out << "\t<importedPackage xmi:type=\"uml:Model\" href=\"" << mmr << "#1\"/>\n";
    out.indent(); 
    out << "</packageImport>\n";
  }

  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  if (is_profile) {
    for (unsigned i = 0; i != n; i += 1) {
      UmlItem * elt = ch[i];
      UmlNcRelation * ncr = dynamic_cast<UmlNcRelation *>(elt);
      
      if ((ncr == 0) ||
	  (ncr->relationKind() != aDependency) ||
	  (ncr->stereotype() != "import") ||
	  (ncr->target()->kind() != aPackage) ||
	  (ncr->target()->stereotype() != "profile"))
	elt->write(out);
      else {
	out.indent(); 
	out << "<packageImport xmi:type=\"uml:PackageImport\"";
	out.id_prefix(this, "PI_");
	out << ">\n";
	out.indent(); 
	out << "\t<importedPackage xmi:type=\"uml:Profile\"";
	out.idref(ncr->target());
	out << "/>\n";
	out.indent(); 
	out << "</packageImport>\n";
      }
    }
  }
  else {
    for (unsigned i = 0; i != n; i += 1)
      ch[i]->write(out);
  }

  while (! _relations.isEmpty())
    _relations.take(0)->write(out, FALSE);
    
  while (! _assocs.isEmpty())
    _assocs.take(0)->write_it(out);

  out.indent(-1); 
  out.indent(); 
  out << "</" << k << ">\n";
}

void UmlPackage::search_profiles(Q3PtrList<UmlPackage> & l) {
  if (stereotype() == "profile")
    l.append(this);
    
  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    if (ch[i]->kind() == aPackage)
      ((UmlPackage *) ch[i])->search_profiles(l);
}

void UmlPackage::search_class_assoc() {
  const Q3PtrVector<UmlItem> ch = children();
  unsigned n = ch.size();
  
  for (unsigned i = 0; i != n; i += 1)
    ch[i]->search_class_assoc();
}

void UmlPackage::memo_relation(UmlItem * r) {
  _relations.append(r);
}

void UmlPackage::memo_ac_uc_assoc(UmlUseCaseDiagram * d) {
  _assocs.append(d);
}

