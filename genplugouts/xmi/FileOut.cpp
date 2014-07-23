
#include "FileOut.h"
#include "UmlItem.h"

#include <q3textstream.h>
#include <qfile.h>
//Added by qt3to4:
#include <Q3CString>
FileOut::FileOut(QFile * fp) : Q3TextStream(fp), _indent(0){
}

void FileOut::indent() {
  Q3TextStream & ts = *this;
  
  for (int n = _indent; n > 0; n -= 1)
    ts << '\t';
}

void FileOut::id(const UmlItem * x) {
  ((Q3TextStream &) *this) << " xmi.id=\"BOUML_" << ((void *) x) << '"';

}

void FileOut::idref(const UmlItem * x) {
  ((Q3TextStream &) *this) << " xmi.idref=\"BOUML_" << ((void *) x) << '"';

}

void FileOut::idref(Q3CString s, const UmlItem * x) {
  QString keys;
  {
    Q3TextStream keyst(&keys, QIODevice::WriteOnly);
    
    keyst << ((void *) x) << "_" << s;
  }

  QMap<Q3CString, int>::ConstIterator it =
    _modifiedtypes.find((const char *) keys);
  
  if (it == _modifiedtypes.end())
    it = _modifiedtypes.insert((const char *) keys, _modifiedtypes.count());
    
  ((Q3TextStream &) *this) << " xmi.idref=\"BOUML_basedontype_"
    << it.data() << '"';

}

void FileOut::ref(const UmlItem * x) {
  ((Q3TextStream &) *this) << "BOUML_" << ((void *) x); 
}

void FileOut::idref_datatype(const Q3CString & t) {
  QMap<Q3CString, int>::ConstIterator it = _datatypes.find(t);
  
  if (it == _datatypes.end())
    it = _datatypes.insert(t, _datatypes.count());
    
  ((Q3TextStream &) *this) << " xmi.idref=\"BOUML_datatype_"
    << it.data() << '"';

}

void FileOut::define_datatypes(int taggedvalue_mode) {
  QMap<Q3CString, int>::ConstIterator it;
  
  for (it = _datatypes.begin();
       it != _datatypes.end();
       ++it) {
    indent();
    (*this) << "<UML:DataType xmi.id=\"BOUML_datatype_"
            << it.data() << "\" name = \"";
    quote(it.key());
    (*this) << "\" visibility=\"private\" isRoot=\"false\" isLeaf=\"false\" isAbstract=\"false\"/>\n";
  }
  
  for (it = _modifiedtypes.begin();
       it != _modifiedtypes.end();
       ++it) {
    Q3CString k = it.key();
    int index = k.find('_');
    
    indent();
    (*this) << "<UML:DataType xmi.id=\"BOUML_basedontype_"
            << it.data() << "\" name = \"";
    quote(k.mid(index + 1));
    (*this) << "\" visibility=\"private\" isRoot=\"false\" isLeaf=\"false\" isAbstract=\"false\">\n";
    switch (taggedvalue_mode) {
    case 1:
      indent();
      (*this) << "\t<UML:ModelElement.taggedValue>\n";
      indent();
      (*this) << "\t\t<UML:TaggedValue tag=\"basedOn\" value=\"BOUML_"
	<< k.left(index) << "\"/>\n";
      indent();
      (*this) << "\t</UML:ModelElement.taggedValue>\n";
      break;
    case 2:
      indent();
      (*this) << "\t<UML:ModelElement.taggedValue>\n";
      indent();
      (*this) << "\t\t<UML:TaggedValue.tag>basedOn</UML:TaggedValue.tag>\n";
      indent();
      (*this) << "\t\t<UML:TaggedValue.value>BOUML_" << k.left(index) << "</UML:TaggedValue.value>\n";
      indent();
      (*this) << "\t</UML:ModelElement.taggedValue>\n";
      break;
    }
    indent();
    (*this) << "</UML:DataType>\n";
  }
}

void FileOut::quote(const char * s) {
 for (;;) {
   switch (*s) {
   case 0: return;
   case '<': (*this) << "&lt;"; break;
   case '>': (*this) << "&gt;"; break;
   case '"': (*this) << "&quot;"; break;
   case '&': (*this) << "&amp;"; break;
   default: (*this) << *s;
   }
   s += 1;
 }
}

