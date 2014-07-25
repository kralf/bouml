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

#ifndef BROWSER_TRANSITION_H
#define BROWSER_TRANSITION_H

class QPixmap;
class TransitionData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserTransition : public BrowserNode, public Labeled<BrowserTransition> {
  protected:
    static IdDict<BrowserTransition> all;
  
    TransitionData * def;
    
    BrowserTransition(BrowserNode * p, TransitionData * d, int id);
    BrowserTransition(int id);
    
    void exec_menu_choice(int rank);

  public:
    BrowserTransition(BrowserNode * p, BrowserNode * end);
    BrowserTransition(const BrowserTransition * model, BrowserNode * p);
    virtual ~BrowserTransition();
  
    virtual bool undelete(bool rec, QString & warning, QString & renamed);
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
        
    virtual const QPixmap* pixmap (int) const;

    virtual bool allow_empty() const;
    virtual bool same_name(const QString & s, UmlCode type) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual bool in_edition() const;
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void update_stereotype(bool = FALSE);
    QString str(bool horiz, DrawingLanguage lg) const;
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserTransition * read_ref(char * & st);
    static BrowserTransition * read(char * &, char *, BrowserNode *);
    static BrowserTransition * temporary(TransitionData * d);
    static BrowserNode * get_it(const char * k, int id);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual void referenced_by(Q3PtrList<BrowserNode> & l, bool ondelete);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, BrowserNode *);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);

    static QString drag_key(BrowserNode * p);
    virtual QString drag_key() const;
    virtual QString drag_postfix() const;
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif

