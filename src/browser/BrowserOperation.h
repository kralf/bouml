//Added by qt3to4:
#include <QPixmap>
#include <QDropEvent>
#include <Q3TextStream>
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

#ifndef BROWSEROPERATION_H
#define BROWSEROPERATION_H

class QPixmap;
class OperationData;
class AType;
class BrowserClass;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserOperation : public BrowserNode, public Labeled<BrowserOperation> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserOperation> all;
    static QStringList its_default_stereotypes;
  
    OperationData * def;
    BrowserNode * get_of;	// exclusive with set_of;
    BrowserNode * set_of;

  protected:
    BrowserOperation(int id);
  
    void exec_menu_choice(int rank);
  
  public:
    BrowserOperation(QString s, BrowserNode * p, OperationData * d, int id = 0);
    BrowserOperation(const BrowserOperation * model, BrowserNode * p);
    virtual ~BrowserOperation();
  
    virtual bool undelete(bool rec, QString & warning, QString & renamed);
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    void post_duplicate();
    
    BrowserNode * get_get_of() { return get_of; };
    BrowserNode * get_set_of() { return set_of; };
    void set_get_of(BrowserNode * o);
    void set_set_of(BrowserNode * o);
    void update_get_of(const QString & attr_name, const QString & cpp_decl,
		       const QString & java_decl, const QString & php_decl, 
		       const QString & python_decl, const QString & idl_decl, 
		       bool is_const, bool is_class_member, const AType & cl,
		       QString multiplicity, QString stereotype);
    void update_set_of(const QString & attr_name, const QString & cpp_decl,
		       const QString & java_decl, const QString & php_decl, 
		       const QString & python_decl, const QString & idl_decl, 
		       bool is_const, bool is_class_member, const AType & cl,
		       QString multiplicity, QString stereotype);
    QString get_of_name() const;	// rel or attribute name
    AType get_of_association() const; // rel's association or ${association}
    QString compute_name(const char * name_spec) const;

    virtual const QPixmap* pixmap (int) const;
    virtual void paintCell(QPainter *, const QColorGroup &, int, int, int);

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual bool allow_spaces() const;
    virtual bool same_name(const QString & s, UmlCode type) const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void member_cpp_def(const QString & prefix, 
				const QString & prefix_tmplop, 
				QString & s, bool templ) const;
    virtual const char * constraint() const;
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserOperation * read_ref(char * &);
    static BrowserOperation * read(char * &, char *, BrowserNode *, bool force = TRUE);
    static BrowserNode * get_it(const char * k, int id);
    static void post_load();
    
    static BrowserOperation * new_one(QString s, BrowserNode * p);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual void referenced_by(Q3PtrList<BrowserNode> &, bool ondelete = FALSE);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, BrowserClass *);
    
    static QString python_init_self(BrowserNode * cl);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    static QString drag_key(BrowserNode * p);
    virtual QString drag_key() const;
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
    static void instances(BrowserNodeList &);
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(Q3TextStream &);
    
  protected:
    virtual bool delete_internal(QString & warning);
};

#endif

