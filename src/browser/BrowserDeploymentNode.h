//Added by qt3to4:
#include <QPixmap>
#include <QDropEvent>
#include <Q3TextStream>
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

#ifndef BROWSER_DEPLOYMENTNODE_H
#define BROWSER_DEPLOYMENTNODE_H

class QPixmap;

#include "BrowserNode.h"
#include "Labeled.h"

class SimpleData;
class BrowserDeploymentDiagram;

class BrowserDeploymentNode : public BrowserNode, public Labeled<BrowserDeploymentNode> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserDeploymentNode> all;
    static QStringList its_default_stereotypes;
    
    SimpleData * def;
    BrowserDeploymentDiagram * associated_diagram;
    
  protected:
    void exec_menu_choice(int rank);
  
  public:
    BrowserDeploymentNode(QString s, BrowserNode * p, int id = 0);
    BrowserDeploymentNode(const BrowserDeploymentNode * model, BrowserNode * p);
    BrowserDeploymentNode(int id);
    virtual ~BrowserDeploymentNode();
    
    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserDeploymentDiagram *, bool on_read = FALSE);
    
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
    virtual BasicData * get_data() const;
    virtual void on_delete();
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserDeploymentNode * read_ref(char * &);
    static BrowserDeploymentNode * read(char * &, char *, BrowserNode *);
    static BrowserNode * get_it(const char * k, int id);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    static BrowserNodeList & instances(BrowserNodeList &, const char * st = 0);
    static BrowserDeploymentNode * get_deploymentnode(BrowserNode * future_parent);
    static BrowserDeploymentNode * add_deploymentnode(BrowserNode * future_parent);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;    
    virtual void referenced_by(Q3PtrList<BrowserNode> & l, bool ondelete);
    
    virtual const QPixmap* pixmap (int) const;
    virtual void iconChanged();
    
    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(Q3TextStream &);
    
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif

