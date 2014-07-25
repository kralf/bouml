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

#ifndef BROWSER_ACTIVITYNODE_H
#define BROWSER_ACTIVITYNODE_H

class QPixmap;
class SimpleData;
class BrowserFlow;

#include "BrowserNode.h"
#include "Labeled.h"
#include "BrowserActivityElement.h"

class BrowserActivityNode : public BrowserNode, public Labeled<BrowserActivityNode>, public BrowserActivityElement {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserActivityNode> all;
    static QStringList its_default_stereotypes;
  
    UmlCode kind;
    SimpleData * def;
    
  protected:
    BrowserActivityNode(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserActivityNode(UmlCode c, QString s, BrowserNode * p, int id = 0);
    BrowserActivityNode(const BrowserActivityNode * model, BrowserNode * p);
    virtual ~BrowserActivityNode();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    static BrowserActivityNode * add_activitynode(BrowserNode * future_parent,
						  UmlCode c, const char * name);
    static BrowserActivityNode * get_activitynode(BrowserNode * future_parent, UmlCode c);
    virtual BasicData * add_relation(UmlCode, BrowserNode * end);
    QString may_start() const;
    QString may_connect(const BrowserNode * dest) const;
    virtual QString connexion_from(bool control) const;
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserActivityNode * read(char * &, char *, BrowserNode *);
    static BrowserActivityNode * read_ref(char * & st);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual bool api_compatible(unsigned v) const;
    
    virtual void referenced_by(Q3PtrList<BrowserNode> &, bool ondelete = FALSE);
    bool target_of_flow() const;
    
    virtual bool allow_empty() const;

    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(Q3TextStream &);
    
    static QString drag_key(BrowserNode * p);
    virtual QString drag_key() const;
    virtual QString drag_postfix() const;
    virtual void DragMoveEvent(QDragMoveEvent * e);
    virtual void DropEvent(QDropEvent * e);
    virtual void DragMoveInsideEvent(QDragMoveEvent * e);
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif
