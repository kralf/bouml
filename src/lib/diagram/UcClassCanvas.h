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

#ifndef UCCLASSCANVAS_H
#define UCCLASSCANVAS_H

#include "DiagramCanvas.h"
#include "Settings.h"
//Added by qt3to4:
#include <Q3TextStream>

#define ACTOR_CANVAS_SIZE 40

class TemplateCanvas;

class UcClassCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT

  protected:
    TemplateCanvas * templ;
    UmlColor itscolor;
    UmlColor used_color;
    SimpleClassDiagramSettings settings;
    SimpleClassDiagramSettings used_settings;
    ClassDrawingMode used_view_mode;
    QString full_name;

  protected:
    UcClassCanvas(UmlCanvas * canvas, int id);
    
  public:
    UcClassCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y, int id);
    virtual ~UcClassCanvas();
    
    virtual void delete_it();

    virtual void draw(QPainter & p);
    virtual void change_scale();
    
    void compute_size();
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void remove(bool from_model);
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &);
    virtual bool has_relation(BasicData *) const; 
    void draw_all_depend_gene(UcClassCanvas * end = 0);
    virtual bool move_with_its_package() const;
    virtual void moveBy(double dx, double dy);
    virtual void set_z(double z);	// only called by upper() & lower()

    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(Q3PtrList<DiagramItem> &);
    virtual void same_drawing_settings(Q3PtrList<DiagramItem> &);
    void edit_drawing_settings();
    virtual bool get_show_stereotype_properties() const;
    
    virtual void apply_shortcut(QString s);
    
    virtual void history_load(QBuffer &);
    virtual void history_hide();

    virtual void save(Q3TextStream &, bool ref, QString & warning) const;
    static UcClassCanvas * read(char * &, UmlCanvas * canvas, char *);
    virtual void post_loaded();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the class is deleted
};

#endif
