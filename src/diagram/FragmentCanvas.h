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

#ifndef FRAGMENTCANVAS_H
#define FRAGMENTCANVAS_H

#include "DiagramCanvas.h"
//Added by qt3to4:
#include <Q3TextStream>

#define FRAGMENT_CANVAS_MIN_SIZE 30

class FragmentSeparatorCanvas;
class ToolCom;

class FragmentCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    UmlColor itscolor;
    UmlColor used_color;
    QString name;
    int min_width;
    int min_height;
    Q3PtrList<FragmentSeparatorCanvas> separators;
    BrowserNode * refer;
    QString form;
  
    void check_size();
    
  public:
    FragmentCanvas(UmlCanvas * canvas, int x, int y, int id);
    virtual ~FragmentCanvas();
    
    virtual void delete_it();
    
    void remove_it(FragmentSeparatorCanvas *);

    virtual void draw(QPainter & p);
    
    virtual UmlCode type() const;
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual bool alignable() const;
    virtual bool copyable() const;
    virtual void open();
    virtual void menu(const QPoint&);
    virtual QString may_start(UmlCode &) const;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const;
    virtual aCorner on_resize_point(const QPoint &);
    virtual void resize(aCorner c, int dx, int dy, QPoint &);
    virtual void resize(const QSize & sz, bool w, bool h);
    virtual void change_scale();
    virtual void prepare_for_move(bool on_resize);
    virtual void moveBy(double dx, double dy);
    virtual void set_z(double z);	// only called by upper() & lower()
    
    virtual void save(Q3TextStream  & st, bool ref, QString & warning) const;
    static FragmentCanvas * read(char * &, UmlCanvas *, char *);
    virtual void history_save(QBuffer &) const;
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(Q3PtrList<DiagramItem> &);
    virtual void same_drawing_settings(Q3PtrList<DiagramItem> &);
    void edit_drawing_settings();
    
    virtual void apply_shortcut(QString s);
    
    QString arguments() { return form; }
    
    static void send(ToolCom * com, Q3CanvasItemList & all,
		     Q3PtrList<FragmentCanvas> & fragments,
		     Q3PtrList<FragmentCanvas> & refs);
  
  private slots:
    void modified();
};

#endif
