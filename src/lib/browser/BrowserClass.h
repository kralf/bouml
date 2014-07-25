//Added by qt3to4:
#include <Q3TextStream>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <Q3ValueList>
#include <QPixmap>
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

#ifndef BROWSER_CLASSES_H
#define BROWSER_CLASSES_H

class QPixmap;
class ClassData;
class RelationData;
class OperationData;
class BrowserOperation;
class BrowserAttribute;
class BrowserRelation;
class BrowserExtraMember;
class StereotypesDialog;
class BrowserComponent;
class BrowserArtifact;
class BrowserClassDiagram;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserClass : public BrowserNode, public Labeled<BrowserClass> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserClass> all;
    
    static QStringList its_default_stereotypes;
    static QStringList relations_default_stereotypes[UmlRelations];

    ClassData * def;
    BrowserClassDiagram * associated_diagram;
    BrowserArtifact * associated_artifact;	// generate its def
    Q3ValueList<BrowserComponent *> associated_components;	// realize or provide it
  
  protected:
    BrowserClass(int id);
    void exec_menu_choice(int index, Q3PtrList<BrowserOperation> & l);
  
  public:
    BrowserClass(QString s, BrowserNode * p, ClassData * d, int id = 0);
    BrowserClass(const BrowserClass * model, BrowserNode * p);
    virtual ~BrowserClass();
  
    BrowserNode * add_attribute(BrowserAttribute * attr = 0, bool enum_item = FALSE);
    BrowserNode * add_relation(BrowserRelation * rel);
    BrowserNode * add_operation(BrowserOperation * oper = 0);
    BrowserNode * add_inherited_operation(BrowserOperation * model);
    BrowserNode * add_extra_member(BrowserExtraMember * em = 0);
    Q3PtrList<BrowserOperation> inherited_operations(unsigned limit) const;
    QString may_start(UmlCode l) const;
    QString may_connect(UmlCode l, BrowserClass * other);
    virtual BasicData * add_relation(UmlCode, BrowserNode *);
    virtual Q3PtrList<BrowserNode> parents() const;
    void get_all_parents(Q3PtrList<BrowserClass> &) const;
    virtual QString check_inherit(const BrowserNode * parent) const;
    bool have_abstract_operation();
    void get_opers(Q3ValueList<const OperationData *> & opers,
		   QStringList & list) const;
    void get_attrs(BrowserNodeList &) const;
    void get_rels(BrowserClass *, Q3PtrList<RelationData> &, int * rev = 0) const;
    void get_rels(BrowserClass * target, Q3PtrList<BrowserRelation> & l) const;
    void get_tree(BrowserNodeList &);
    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserClassDiagram *, bool on_read = FALSE);
    BrowserArtifact * get_associated_artifact() const;
    void set_associated_artifact(BrowserArtifact *, bool on_read = FALSE);
    const Q3ValueList<BrowserComponent *> & get_associated_components() const;
    void add_associated_component(BrowserComponent *);
    void remove_associated_component(BrowserComponent *);

    virtual void delete_it();
    virtual bool undelete(bool rec, QString & warning, QString & renamed);
    virtual bool may_contains_them(const Q3PtrList<BrowserNode> &,
				   BooL & duplicable) const;
    virtual void move(BrowserNode *, BrowserNode * after);
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    QString contextual_name(ShowContextMode) const;
    virtual void set_name(const char * s);
    virtual void member_cpp_def(const QString &, const QString &, QString & s, bool) const;
    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool force_edit);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual void modified();
    virtual void on_delete();
    virtual BasicData * get_data() const;
    virtual bool allow_spaces() const;
    virtual const QStringList & default_stereotypes(UmlCode, const BrowserNode *) const; // non class rel
    virtual const char * constraint() const;
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserClass * read_ref(char * &, const char * k = 0);
    static BrowserClass * read(char * &, char *, BrowserNode *, bool force = TRUE);
    static BrowserNode * read_any_ref(char * &, char *);
    static BrowserNode * get_it(const char * k, int id);
    static void post_load();
    
    static void plug_out_conversion();
    bool get_bodies_read();
    void set_bodies_read(bool y);
    bool get_bodies_modified();
    void set_bodies_modified(bool y);
    QString bodies_file() const;

    virtual bool tool_cmd(ToolCom * com, const char * args);
    static bool tool_global_cmd(ToolCom * com, const char * args);
    
    static BrowserNodeList & instances(BrowserNodeList &, const char * st = 0, bool non_nested = FALSE);
    static BrowserClass * get_class(BrowserNode * future_parent,
				    const char * stereotype = 0);
    static BrowserClass * add_class(bool stereotypep,
				    BrowserNode * future_parent,
				    QString name = QString::null);
    static BrowserClass * find(const char * s);

    static void init();
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    virtual void support_file(Q3Dict<char> & files, bool add) const;
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * &);
    static void save_stereotypes(Q3TextStream &);
    
    virtual void referenced_by(Q3PtrList<BrowserNode> &, bool ondelete = FALSE);
    
    static bool new_java_enums(QString new_st);

    virtual const QPixmap* pixmap (int) const;
    virtual void update_stereotype(bool rec = FALSE);
    virtual void iconChanged();
    virtual void paintCell(QPainter * p, const QColorGroup & cg, int column,
			   int width, int alignment);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
    static BrowserClass * temporary();
};

#endif
