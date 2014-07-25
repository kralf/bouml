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

#ifndef TRANSITIONCANVAS_H
#define TRANSITIONCANVAS_H

#include "ArrowCanvas.h"
//Added by qt3to4:
#include <Q3TextStream>

class BrowserNode;
class TransitionData;
class DiagramItem;
class StereotypePropertiesCanvas;

class TransitionCanvas : public ArrowCanvas {
  Q_OBJECT
    
  protected:
    BrowserNode * br_begin;
    TransitionData * data;
    StereotypePropertiesCanvas * stereotypeproperties; // in the first segment
    DrawingLanguage drawing_language;
    Uml3States write_horizontally;
    Uml3States show_definition;
    
  public:
    TransitionCanvas(UmlCanvas * canvas, DiagramItem * b,
		     DiagramItem * e, BrowserNode * bb,
		     int id, float d_start, float d_end,
		     TransitionData * d = 0);
    virtual ~TransitionCanvas();
    
    virtual void delete_it();
    virtual void delete_available(BooL & in_model, BooL & out_model) const;
    virtual void remove(bool from_model);
    virtual void unconnect();
    
    virtual ArrowPointCanvas * brk(const QPoint &);
    virtual ArrowCanvas * join(ArrowCanvas * other, ArrowPointCanvas * ap);
    
    virtual void default_label_position() const;
    void stereotype_default_position() const;
    
    virtual void save(Q3TextStream & st, bool ref, QString & warning) const;
    
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(Q3PtrList<DiagramItem> &);
    virtual void same_drawing_settings(Q3PtrList<DiagramItem> &);
    
    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
  
    static TransitionCanvas * read(char * & st, UmlCanvas * canvas, char *);
    virtual BasicData * get_data() const;
    
    virtual void open();
    virtual void menu(const QPoint&);
    
    virtual void setVisible( bool yes);
    virtual void moveBy(double dx, double dy);
    virtual void select_associated();
    virtual void check_stereotypeproperties();
    
    static void drop(BrowserNode *, UmlCanvas *);
    
    virtual bool represents(BrowserNode *);
    
  protected:
    BrowserNode * update_begin(DiagramItem * cnend);
    void update(bool updatepos);
    void propagate_drawing_settings();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the relation is deleted
};

#endif
