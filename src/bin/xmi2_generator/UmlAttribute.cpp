
#include "UmlAttribute.h"
#include "FileOut.h"

#include "UmlClass.h"
#include "CppSettings.h"
#include "JavaSettings.h"
//Added by qt3to4:
#include <Q3CString>
void UmlAttribute::write(FileOut & out) {
  Q3CString stp = parent()->stereotype();
  bool enum_item = (stp == "enum_pattern") ||
    ((stp == "enum") && (stereotype() != "attribute"));
  const char * k;
  const char * kk;
  
  if (enum_item) {
    k = "ownedLiteral";
    kk = " xmi:type=\"uml:EnumerationLiteral\" name=\"";
  }
  else {
    k = "ownedAttribute";
    kk = " xmi:type=\"uml:Property\" name=\"";
  }
  
  switch (_lang) {
  case Uml:
    out.indent();
    out << "<" << k << kk << name() << '"';
    break;
  case Cpp:
    if (cppDecl().isEmpty())
      return;
    
    out.indent();
    out << "<" << k << kk << true_name(name(), cppDecl()) << '"';
    break;
  default: // Java
    if (javaDecl().isEmpty())
      return;
    
    out.indent();
    out << "<" << k << kk << true_name(name(), javaDecl()) << '"';
    break;
  }
  out.id(this);
  
  if (! enum_item) {
    write_visibility(out);
    write_scope(out);
    if (isReadOnly())
      out << " isReadOnly=\"true\"";
    if (isDerived()) {
      out << " isDerived=\"true\"";
      if (isDerivedUnion())
	out << " isDerivedUnion=\"true\"";
    }
    if (isOrdered())
      out << " isOrdered=\"true\"";
    if (isUnique())
      out << " isUnique=\"true\"";
  }

  out << ">\n";
  out.indent(+1);
  
  if (! enum_item) {
    const UmlTypeSpec & t = type();
    
    switch (_lang) {
    case Uml:
      UmlItem::write_type(out, t);
      break;
    case Cpp:
      if ((t.type != 0) || !t.explicit_type.isEmpty())
	write_cpp_type(out);
      break;
    default: // java
      if ((t.type != 0) || !t.explicit_type.isEmpty())
	write_java_type(out);
    }
    
    write_multiplicity(out, multiplicity(), this);
  }
  write_default_value(out, defaultValue(), this);
  write_constraint(out);
  write_annotation(out);
  write_description_properties(out);

  out.indent(-1);
  out.indent();
  out << "</" << k << ">\n";

  unload();
}

void UmlAttribute::write_cpp_type(FileOut & out) {
  // note : doesn't manage function/operation pointer
  Q3CString s = cppDecl();
  int index;
  
  remove_comments(s);
  
  // remove keywords not linked to the type
  if ((index = s.find("${static}")) != -1)
    s.replace(index, 9, " ");
  if ((index = s.find("${mutable}")) != -1)
    s.replace(index, 10, (isCppMutable()) ? "mutable " : "");
  if ((index = s.find("${volatile}")) != -1)
    s.replace(index, 11, (isVolatile()) ? "volatile " : "");
  if ((index = s.find("${value}")) != -1)
    s.replace(index, 8, " ");
  if ((index = s.find("${h_value}")) != -1)
    s.replace(index, 10, " ");
  
  // replace keywords linked to the type
  if ((index = s.find("${const}")) != -1)
    s.replace(index, 8, isReadOnly() ? "const " : "");
  
  UmlTypeSpec t = type();
  
  if (t.type == 0)
    t.explicit_type = CppSettings::type(t.explicit_type);
  
  out.indent();
  out << "<type xmi:type=\"uml:Class\"";
  write_type(out, t, s, "${name}", "${type}");
  out << "/>\n";
}

void UmlAttribute::write_java_type(FileOut & out) {
  Q3CString s = javaDecl();
  int index;
  
  remove_comments(s);
  
  // remove keywords not linked to the type
  if ((index = s.find("${visibility}")) != -1)
    s.replace(index, 13, " ");
  if ((index = s.find("${static}")) != -1)
    s.replace(index, 9, " ");
  if ((index = s.find("${final}")) != -1)
    s.replace(index, 8, " ");
  if ((index = s.find("${transient}")) != -1)
    s.replace(index, 12, " ");
  if ((index = s.find("${volatile}")) != -1)
    s.replace(index, 11, " ");
  if ((index = s.find("${value}")) != -1)
    s.replace(index, 8, " ");
  if ((index = s.find("${@}")) != -1)
    s.replace(index, 4, " ");
  
  UmlTypeSpec t = type();
  
  if (t.type == 0)
    t.explicit_type = JavaSettings::type(t.explicit_type);
  
  out.indent();
  out << "<type xmi:type=\"uml:Class\"";
  write_type(out, t, s, "${name}", "${type}");
  out << "/>\n";
  
}

