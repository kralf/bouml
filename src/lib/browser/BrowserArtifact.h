//Added by qt3to4:
#include <QPixmap>
#include <QDropEvent>
#include <Q3TextStream>
#include <Q3ValueList>
#include <QDragMoveEvent>
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

#ifndef BROWSER_ARTIFACT_H
#define BROWSER_ARTIFACT_H

class QPixmap;

#include "BrowserNode.h"
#include "Labeled.h"

class ArtifactData;
class BrowserClass;
class BrowserDeploymentDiagram;

class BrowserArtifact : public BrowserNode, public Labeled<BrowserArtifact> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserArtifact> all;
    static QStringList its_default_stereotypes;
    static QStringList relation_default_stereotypes;
    
    BooL cpp_h_edited;
    BooL cpp_src_edited;
    BooL java_edited;
    BooL php_edited;
    BooL python_edited;
    BooL idl_edited;
    ArtifactData * def;
    BrowserDeploymentDiagram * associated_diagram;
    Q3ValueList<BrowserClass *> associated_classes;
    
  protected:
    void exec_menu_choice(int rank,
			  QString cpp_h_path, QString cpp_src_path,
			  QString java_path, QString php_path, 
			  QString python_path, QString idl_path);
  
  public:
    BrowserArtifact(QString s, BrowserNode * p, int id = 0);
    BrowserArtifact(const BrowserArtifact * model, BrowserNode * p);
    BrowserArtifact(int id);
    
    virtual ~BrowserArtifact();
    
    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserDeploymentDiagram *, bool on_read = FALSE);
    
    const Q3ValueList<BrowserClass *> & get_associated_classes() const;
    void set_associated_classes(const Q3ValueList<BrowserClass *> & l,
				bool on_read = FALSE);
    bool add_associated_class(BrowserClass *, bool on_read = FALSE);
    bool remove_associated_class(BrowserClass *, bool on_read = FALSE);
    
    virtual void delete_it();
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool force_edit);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual const QStringList & default_stereotypes(UmlCode, const BrowserNode *) const; // non class rel
    virtual void on_delete();
    virtual QString check_inherit(const BrowserNode * parent) const;
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserArtifact * read_ref(char * &, const char *);
    static BrowserArtifact * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    static BrowserNodeList & instances(BrowserNodeList &, const char * st = 0);
    static BrowserArtifact * get_artifact(BrowserNode * future_parent,
					  bool existing = FALSE);
    static BrowserArtifact * add_artifact(BrowserNode * future_parent);
    static BrowserArtifact * find(const char * s);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual const QPixmap* pixmap (int) const;
    virtual void iconChanged();
    
    virtual void referenced_by(Q3PtrList<BrowserNode> &, bool ondelete = FALSE);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, BrowserArtifact *);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, BrowserClass *);
    
    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(Q3TextStream &);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
    
  protected:
    void associate_class(BrowserClass * c, bool on_read);
    void get_paths(QString & cpp_h_path, QString & cpp_src_path,
		   QString & java_path, QString & php_path, 
		   QString & python_path, QString & idl_path) const;
    QString get_path(QString path, QString root,
		     const char * ext) const;
};

#endif

