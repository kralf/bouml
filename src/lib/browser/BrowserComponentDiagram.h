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

#ifndef BROWSER_COMPONENTDIAGRAM_H
#define BROWSER_COMPONENTDIAGRAM_H

#include "BrowserDiagram.h"
#include "Settings.h"
//Added by qt3to4:
#include <QPixmap>
#include <Q3TextStream>
#include <Q3ValueList>

class QPixmap;
class ComponentDiagramWindow;
class SimpleData;

class BrowserComponentDiagram : public BrowserDiagram {
  friend class StereotypesDialog;
  
  protected:
    static Q3PtrList<BrowserComponentDiagram> imported;
    static Q3ValueList<int> imported_ids;
    static QStringList its_default_stereotypes;
  
    SimpleData * def;
    ComponentDiagramWindow * window;
    ComponentDiagramSettings settings;
    ComponentDiagramSettings * used_settings;
    UmlColor component_color;
    UmlColor note_color;
    UmlColor fragment_color;
    UmlColor package_color;
    
    BrowserComponentDiagram(BrowserComponentDiagram * model, BrowserNode * p);
    BrowserComponentDiagram(int id);
    void make();
    void exec_menu_choice(int rank);
  
  public:
    BrowserComponentDiagram(QString s, BrowserNode * p, int id = 0);
    virtual ~BrowserComponentDiagram();
      
    virtual void delete_it();
    virtual BrowserNode * duplicate(BrowserNode * p,
				    QString name = QString::null);
  
    virtual const QPixmap* pixmap (int) const;
    virtual void draw_svg() const;
  
    virtual void menu();
    virtual void apply_shortcut(QString s);
    virtual void open(bool);
    virtual void on_close();
    virtual void read_session(char * & st);
    virtual UmlCode get_type() const;
    virtual QString get_stype() const;
    virtual int get_identifier() const;
    virtual const char * help_topic() const;
    virtual BasicData * get_data() const;
    virtual void set_name(const char * s);
    virtual UmlColor get_color(UmlCode) const;
    virtual bool get_shadow() const;
    virtual bool get_draw_all_relations() const;
    virtual void dont_draw_all_relations();
    virtual bool get_show_stereotype_properties() const;
    virtual void update_drawing_settings();
    virtual void get_componentdiagramsettings(ComponentDiagramSettings & r) const;
    virtual bool get_auto_label_position() const;
    virtual void get_componentdrawingsettings(ComponentDrawingSettings & r) const;
    virtual void package_settings(BooL & name_in_tab, ShowContextMode & show_context) const;
    virtual bool tool_cmd(ToolCom * com, const char * args);
    virtual void save(Q3TextStream &, bool ref, QString & warning);
    static BrowserComponentDiagram * read(char * &, char *, BrowserNode *);
    static BrowserComponentDiagram * read_ref(char * &, char *);
    static BrowserNode * get_it(const char * k, int id);
    
    void edit_settings();

    static BrowserComponentDiagram * add_component_diagram(BrowserNode * future_parent);
    
    static const QStringList & default_stereotypes();
    static void read_stereotypes(char * &, char * & k);
    static void save_stereotypes(Q3TextStream &);
    
    virtual void renumber(int phase);
    static void open_all();
    static void import();
    
    static void compute_referenced_by(Q3PtrList<BrowserNode> & l, BrowserNode *,
				      const char * kc, char const * kr);
};

#endif
