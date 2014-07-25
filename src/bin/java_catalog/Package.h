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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <q3dict.h>
#include <qstringlist.h> 
#include <qstack.h>
//Added by qt3to4:
#include <QPixmap>
#include <Q3CString>
#include <Q3ValueList>

#include "Class.h"
#include "UmlClass.h"

#include "BrowserNode.h"
#include "ClassContainer.h"

class QPixmap;
class QDir;
class QRegExp;

class UmlPackage;
class BrowserView;
class UmlClass;
class UmlItem;
class Class;

class QApplication;

class Package : public BrowserNode, public ClassContainer {
  public:
#ifndef REVERSE
    Package(BrowserView * parent, UmlPackage * u);
#endif
    Package(Package * parent, const char * p, const char * n);
#ifdef ROUNDTRIP
    Package(Package * parent, UmlPackage * pk);
    
    virtual Class * upload_define(UmlClass *);
    virtual Class * localy_defined(QString name) const;
    void own(UmlArtifact *);
    void reverse(UmlArtifact *);
    void reverse_file(Q3CString f, UmlArtifact * art = 0);
#else
    void reverse_file(Q3CString f);    
#endif
  
    virtual bool isa_package() const;

    const Q3CString & get_path() { return path; }
    UmlPackage * get_uml(bool mandatory = TRUE);
#ifdef REVERSE
    void send_dir(int n);
    const QStringList get_imports() { return imports; }
    bool is_java_lang_added() { return java_lang_added; }
    const QStringList get_static_imports() { return static_imports; }
#endif
    
    void new_class(Class *);
    
    virtual void compute_type(Q3CString type, UmlTypeSpec & typespec,
			      const Q3ValueList<FormalParameterList> & tmplts,
			      Class ** need_object = 0);
    virtual Class * define(const Q3CString & name, char st);
    virtual void declare(const Q3CString &, Class *);
    void force_class(Q3CString name, UmlTypeSpec & typespec, Class ** need_object);
    
#ifndef REVERSE
    virtual QString get_path() const;
    
    virtual void menu();
    virtual void refer(const QString & name);
    
    const QPixmap * pixmap(int) const;
    
    virtual void backup(QDataStream  & dts) const;
    void backup_children(QDataStream  & ts) const;
#endif
    void restore_children(QDataStream & dts);
    static void restore(QDataStream  & dt, Package *);

    static void init(UmlPackage *, QApplication *);
    static bool scanning() { return scan; };
    static void set_step(int s, int n);
#ifdef ROUNDTRIP
    void scan_dir(int & n);
    void accept_roundtrip_root();
#else
    static Package * scan_dir(int & n);
#endif
    static Package * get_root() { return root; };
    
    static void push_context();
    static void pop_context();
    
  private:
    UmlPackage * uml;
    Q3CString package;		// java package may be with '.'
    Q3CString path;
    Q3Dict<Class> Undefined;
#ifdef ROUNDTRIP
    Q3Dict<UmlArtifact> roundtriped;
#endif
  
    static bool scan;
    static Package * root;
    static Package * unknown;
    static QRegExp * DirFilter;
    static QRegExp * FileFilter;
    static QString Ext;
    static Q3Dict<Class> classes;
    static Q3Dict<Class> java_classes;
    static Q3Dict<UmlClass> user_classes;
    static Q3Dict<Package> known_packages;
    static Q3Dict<UmlPackage> user_packages;
    static QStringList unknown_packages;
    static QStringList imports;
    static bool java_lang_added;
    static QStringList static_imports;
    static QStack<QStringList> stack;
    static Q3ValueList<FormalParameterList> Formals;
    static QApplication * app;
    
    static int file_number(QDir & dir, bool rec);
    
    static void manage_import();
    void reverse_toplevel_form(Q3CString s);
    void reverse_directory(QDir & dir, bool rec);
    
    void set_package(Q3CString s);
    Package * find(Q3CString s, bool nohack);
    static Package * package_unknown();
    
    static void update_package_list(Q3CString name);
    static void update_class_list(Q3CString pack, UmlItem * container);
    
    Class * declare_if_needed(Q3CString name, char st);
    Class * new_class(const Q3CString & name, char st);

#ifdef ROUNDTRIP
    int count_file_number();
    void scan_dir();
    void send_dir();
#endif
};

#endif
