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

#ifndef BROWSER_SMACTION_H
#define BROWSER_SMACTION_H

class QPixmap;
class StateActionData;
class BrowserTransition;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserStateAction : public BrowserNode, public Labeled<BrowserStateAction> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserStateAction> all;
    static QStringList its_default_stereotypes;
  
    StateActionData * def;
    
  protected:
    BrowserStateAction(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserStateAction(BrowserNode * p, StateActionData * d, int id = 0);
    BrowserStateAction(const BrowserStateAction * model, BrowserNode * p);
    virtual ~BrowserStateAction();
  
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    static BrowserStateAction * add_stateaction(BrowserNode * future_parent);
    virtual BasicData * add_relation(UmlCode, BrowserNode * end);
    BrowserTransition * add_transition(BrowserNode * end);
    
    virtual const QPixmap* pixmap (int) const;
    virtual void iconChanged();

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
    static BrowserStateAction * read(char * &, char *, BrowserNode *);
    static BrowserStateAction * read_ref(char * & st);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    
    virtual void referenced_by(Q3PtrList<BrowserNode> &, bool ondelete = FALSE);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, BrowserStateAction *);
    
    bool allow_empty() const;

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

