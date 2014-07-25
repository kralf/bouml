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

#ifndef ARTIFACTCANVAS_H
#define ARTIFACTCANVAS_H





#include <qobject.h>
//Added by qt3to4:
#include <Q3TextStream>

#include "DiagramCanvas.h"
#include "Settings.h"

#define ARTIFACT_CANVAS_MIN_SIZE 40

class ArtifactCanvas : public QObject, public DiagramCanvas {
  Q_OBJECT
    
  protected:
    UmlColor itscolor;
    UmlColor used_color;
    
  protected:
    ArtifactCanvas(UmlCanvas * canvas, int id);
  
    void update_relations();
    void update_relations(ArtifactCanvas * other);
  
  public:
    ArtifactCanvas(BrowserNode * bn, UmlCanvas * canvas, int x, int y);
    virtual ~ArtifactCanvas();
    
    virtual void delete_it();
    
    void compute_size();
    void draw_all_relations();
    virtual void unassociate(DiagramItem * other);
    
    virtual void draw(QPainter & p);
    virtual void change_scale();
    
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
    virtual void post_connexion(UmlCode, DiagramItem *);
    virtual bool move_with_its_package() const;
    
    virtual void history_load(QBuffer &);
    virtual void history_hide();
    
    virtual bool has_drawing_settings() const;
    virtual void edit_drawing_settings(Q3PtrList<DiagramItem> &);
    virtual void same_drawing_settings(Q3PtrList<DiagramItem> &);
    
    virtual void apply_shortcut(QString s);
    void edit_drawing_settings();
  
    virtual void save(Q3TextStream  & st, bool ref, QString & warning) const;
    static ArtifactCanvas * read(char * &, UmlCanvas *, char *);
    virtual void post_loaded();
    
  private slots:
    void modified();	// canvas must be updated
    void deleted();	// the class is deleted
};

#endif
