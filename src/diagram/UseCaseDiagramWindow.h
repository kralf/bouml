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

#ifndef USECASEDIAGRAMWINDOW_H
#define USECASEDISGRAMWINDOW_H

#include "DiagramWindow.h"

class QToolButton;
class QSpinBox;

class UseCaseDiagramView;
class BrowserUseCaseDiagram;

class UseCaseDiagramWindow : public DiagramWindow {
  Q_OBJECT
    
  protected:
    UseCaseDiagramView * view;
    QToolButton * addClass;
    QToolButton * addUseCase;
    QToolButton * addPackage;
    QToolButton * addFragment;
    QToolButton * addSubject;
    QToolButton * association;
    QToolButton * directionalassociation;
    QToolButton * dependency;
    QToolButton * inherit;
    QToolButton * note;
    QToolButton * anchor;
    QToolButton * text;
    QToolButton * image;
  
  public:
    UseCaseDiagramWindow(const QString & s, BrowserUseCaseDiagram * b, int id = -1);
virtual ~UseCaseDiagramWindow();

    virtual DiagramView * get_view() const;
    
    virtual void hit_button(UmlCode, QToolButton *);
    
  protected slots:
    void hit_class();
    void hit_usecase();
    void hit_package();
    void hit_fragment();
    void hit_subject();
    void hit_association();
    void hit_directionalassociation();
    void hit_dependency();
    void hit_inherit();
    void hit_note();
    void hit_anchor();
    void hit_text();
    void hit_image();
};

#endif
