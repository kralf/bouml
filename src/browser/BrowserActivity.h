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

#ifndef BROWSER_ACTIVITY_H
#define BROWSER_ACTIVITY_H

#include "BrowserNode.h"
#include "Labeled.h"
//Added by qt3to4:
#include <QPixmap>
#include <QDropEvent>
#include <Q3TextStream>
#include <Q3ValueList>
#include <QDragMoveEvent>

class QPixmap;
class ActivityData;
class BrowserActivityDiagram;
class BrowserParameter;
class BrowserOperation;

class BrowserActivity : public BrowserNode, public Labeled<BrowserActivity> {
  friend class StereotypesDialog;
  
  protected:
    static IdDict<BrowserActivity> all;
    static QStringList its_default_stereotypes;
  
    ActivityData * def;
    BrowserActivityDiagram * associated_diagram;
    
  protected:
    BrowserActivity(int id);
  
    void exec_menu_choice(int rank);
    
  public:
    BrowserActivity(QString s, BrowserNode * p, int id = 0);
    BrowserActivity(const BrowserActivity * model, BrowserNode * p);
    virtual ~BrowserActivity();
  
    virtual bool may_contains_them(const Q3PtrList<BrowserNode> & l,
				   BooL & duplicable) const;
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
    static BrowserNodeList & instances(BrowserNodeList &, bool sort);
    static BrowserActivity * add_activity(BrowserNode * future_parent);
    static BrowserActivity * add_activity(BrowserNode * future_parent, const char * name);
    static BrowserActivity * get_activity(BrowserNode * parent);
    Q3ValueList<BrowserParameter *> get_params() const;
    void add_activity_diagram();
    BrowserNode * add_parameter(BrowserParameter * p);
    QString may_connect(const BrowserNode * dest) const;
    
    virtual const QPixmap* pixmap (int) const;

    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual void modified();
    virtual void on_delete();
    virtual BasicData * get_data() const;
    virtual QString full_name(bool rev = FALSE, bool itself = TRUE) const;

    virtual BrowserNode * get_associated() const;
    void set_associated_diagram(BrowserActivityDiagram *, bool on_read = FALSE);
    
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserActivity * read(char * &, char *, BrowserNode *);
    static BrowserActivity * read_ref(char * & st);
    static BrowserNode * read_any_ref(char * &, char *);
    static BrowserNode * get_it(const char * k, int id);

    static void clear(bool old);
    static void update_idmax_for_root();
    virtual void renumber(int phase);
    virtual void prepare_update_lib() const;
    
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual bool api_compatible(unsigned v) const;
    
    virtual void referenced_by(Q3PtrList<BrowserNode> &, bool ondelete = FALSE);
    static void compute_referenced_by(Q3PtrList<BrowserNode> &, BrowserOperation *);
    
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
