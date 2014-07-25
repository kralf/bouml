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

#ifndef BROWSER_FLOW_H
#define BROWSER_FLOW_H

class QPixmap;
class FlowData;

#include "BrowserNode.h"
#include "Labeled.h"

class BrowserFlow : public BrowserNode, public Labeled<BrowserFlow> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserFlow> all;
    static QStringList its_default_stereotypes;
  
    FlowData * def;

    BrowserFlow(BrowserNode * p, FlowData * d, int id);
    BrowserFlow(int id);
    
    void exec_menu_choice(int rank);
    
  public:
    BrowserFlow(BrowserNode * p, BrowserNode * end);
    BrowserFlow(const BrowserFlow * model, BrowserNode * p);
    virtual ~BrowserFlow();
  
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
    virtual const char * help_topic() const;
    virtual void modified();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;
    virtual void update_stereotype(bool = FALSE);
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserFlow * read_ref(char * & st);
    static BrowserFlow * read(char * &, char *, BrowserNode *);
    static BrowserFlow * temporary(FlowData * d);
    static BrowserNode * get_it(const char * k, int id);
    
    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual void referenced_by(Q3PtrList<BrowserNode> & l, bool ondelete);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, const BrowserNode *);
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual bool api_compatible(unsigned v) const;

    static void init();
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(Q3TextStream &);
    
    static QString drag_key(BrowserNode * p);
    virtual QString drag_key() const;
    virtual QString drag_postfix() const;
    virtual void DropAfterEvent(QDropEvent * e, BrowserNode * after);
};

#endif

