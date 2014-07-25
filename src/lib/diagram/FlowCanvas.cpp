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





#include <qcursor.h>
//Added by qt3to4:
#include <Q3TextStream>
#include <math.h>
#include <q3popupmenu.h>

#include "FlowCanvas.h"
#include "ArrowPointCanvas.h"
#include "UmlCanvas.h"
#include "LabelCanvas.h"
#include "InfoCanvas.h"
#include "BrowserFlow.h"
#include "BrowserActivityDiagram.h"
#include "FlowData.h"
#include "StereotypePropertiesCanvas.h"
#include "UmlGlobal.h"
#include "myio.h"
#include "ToolCom.h"
#include "Tool.h"
#include "MenuTitle.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "DialogUtil.h"
#include "strutil.h"
#include "translate.h"

FlowCanvas::FlowCanvas(UmlCanvas * canvas, DiagramItem * b,
		       DiagramItem * e, BrowserNode * bb, int id,
		       float d_start, float d_end, FlowData * d)
      : ArrowCanvas(canvas, b, e, UmlFlow, id, TRUE, d_start, d_end),
        br_begin(bb), data(d), selection(0),
  	transformation(0), stereotypeproperties(0),
  	write_horizontally(UmlDefaultState) {
  if ((e->type() != UmlArrowPoint) && (bb == 0)) {
    // end of line construction
    update_begin(e);
  }
  else if (d != 0) {
    if (e->type() != UmlArrowPoint)
      check_stereotypeproperties();
    connect(d, SIGNAL(changed()), this, SLOT(modified()));
    connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  }
  
  connect(DrawingSettings::instance(), SIGNAL(changed()),
	  this, SLOT(modified()));
  
  // manages the case start == end
  if (b == e)
    cut_self();
  
  if (d != 0) {
    update(TRUE);
    check_sel_trans();
  }
}

FlowCanvas::~FlowCanvas() {
}

void FlowCanvas::delete_it() {
  ArrowCanvas::delete_it();	// call unconnect

  if (selection != 0)
    selection->delete_it();
  if (transformation != 0)
    transformation->delete_it();
  if (stereotypeproperties != 0)
    ((UmlCanvas *) canvas())->del(stereotypeproperties);
}

void FlowCanvas::unconnect() {
  disconnect(data, 0, this, 0);
  disconnect(DrawingSettings::instance(), SIGNAL(changed()),
	     this, SLOT(modified()));
  
  ArrowCanvas::unconnect();
}

void FlowCanvas::deleted() {
  delete_it();
  canvas()->update();
  package_modified();
}

void FlowCanvas::delete_available(BooL & in_model, BooL & out_model) const {
  out_model |= TRUE;
  in_model |= data->get_start()->is_writable();
}

void FlowCanvas::remove(bool from_model) {
  if (! from_model) {
    if (the_canvas()->must_draw_all_relations()) {
      const FlowCanvas * a = this;
  
      while (a->begin->type() == UmlArrowPoint) {
	a = (FlowCanvas *) ((ArrowPointCanvas *) a->begin)->get_other(a);
	if (a == 0)
	  break;
      }

      if (a && !a->begin->isSelected() && !a->begin->get_bn()->deletedp()) {
	a = this;
  
	while (a->end->type() == UmlArrowPoint) {
	  a = (FlowCanvas *) ((ArrowPointCanvas *) a->end)->get_other(a);
	  if (a == 0)
	    break;
	}
  
	if (a && !a->end->isSelected() && !a->end->get_bn()->deletedp()) {
	  msg_warning("Bouml", TR("<i>Draw all relations</i> forced to <i>no</i>"));
	  the_canvas()->dont_draw_all_relations();
	}
      }
    }

    delete_it();
  }
  else
    data->delete_it();	// will remove canvas
}


// Updates br_begin going back, Returns the start browser node

BrowserNode * FlowCanvas::update_begin(DiagramItem * cnend) {
  // static to be updated in all the cases
  static FlowData * d = 0;
  
  if (begin->type() == UmlArrowPoint) {
    FlowCanvas * other =
      ((FlowCanvas *) ((ArrowPointCanvas *) begin)->get_other(this));
    
    br_begin = other->update_begin(cnend);	// update d
  }
  else {
    // the relation start here
    br_begin = begin->get_bn();
    d = (FlowData *) begin->add_relation(UmlFlow, cnend);
  }
  
  connect(d, SIGNAL(changed()), this, SLOT(modified()));
  connect(d, SIGNAL(deleted()), this, SLOT(deleted()));
  data = d;
  
  return br_begin;
}

void FlowCanvas::propagate_drawing_settings() {
  FlowCanvas * t;
  
  t = this;
  while (t->begin->type() == UmlArrowPoint) {
    t = ((FlowCanvas *) ((ArrowPointCanvas *) t->begin)->get_other(t));
    t->settings = settings;
    t->used_settings = used_settings;
    t->write_horizontally = write_horizontally;
  }
  
  t = this;
  while (t->end->type() == UmlArrowPoint) {
    t = ((FlowCanvas *) ((ArrowPointCanvas *) t->end)->get_other(t));
    t->settings = settings;
    t->used_settings = used_settings;
    t->write_horizontally = write_horizontally;
  }
}

void FlowCanvas::open() {
  if (data &&
      !data->get_start()->in_edition())	// for dble click on diagram
    data->edit();
}

void FlowCanvas::menu(const QPoint &) {
  if ((data != 0) && !data->get_start()->in_edition()) {
    FlowCanvas * plabel;
    FlowCanvas * pstereotype;
    
    {
      ArrowCanvas * aplabel;
      ArrowCanvas * apstereotype;
      
      search_supports(aplabel, apstereotype);
      plabel = (FlowCanvas *) aplabel;
      pstereotype = (FlowCanvas *) apstereotype;
    }
    
    Q3PopupMenu m(0);
    Q3PopupMenu geo(0);
    Q3PopupMenu toolm(0);
    
    m.insertItem(new MenuTitle(data->definition(FALSE, TRUE), m.font()),
		 -1);
    m.insertSeparator();
    m.insertItem(TR("Edit"), 0);
    m.insertSeparator();

    m.insertItem(TR("Select in browser"), 2);
    if (plabel || pstereotype) {
      m.insertSeparator();
      m.insertItem(TR("Edit drawing settings"), 1);
      m.insertItem(TR("Select labels"), 3);
      m.insertItem(TR("Labels default position"), 4);
      if (plabel && (label == 0))
	m.insertItem(TR("Attach flow label to this segment"), 5);
      if (pstereotype && (stereotype == 0))
	m.insertItem(TR("Attach stereotype to this segment"), 6);
    }
  
    if (get_start() != get_end()) {
      m.insertSeparator();
      init_geometry_menu(geo, 10);
      m.insertItem(TR("Geometry (Ctrl+l)"), &geo);
    }
    
    m.insertSeparator();
    m.insertItem(TR("Remove from diagram"),7);
    if (data->get_start()->is_writable())
      m.insertItem(TR("Delete from model"), 8);
    
    m.insertSeparator();
    if (Tool::menu_insert(&toolm, itstype, 20))
      m.insertItem(TR("Tool"), &toolm);
    
    int rank = m.exec(QCursor::pos());
    
    switch (rank) {
    case 0:
      data->edit();
      return;
    case 1:
      edit_drawing_settings();
      return;
    case 2:
      data->get_start()->select_in_browser();
      return;
    case 3:
      the_canvas()->unselect_all();
      if (plabel)
	the_canvas()->select(plabel->label);
      if (pstereotype)
	the_canvas()->select(pstereotype->stereotype);
      return;
    case 4:
      if (plabel)
	plabel->default_label_position();
      if (pstereotype)
	pstereotype->stereotype_default_position();
      return;
    case 5:
      label = plabel->label;
      plabel->label = 0;
      default_label_position();
      return;
    case 6:
      stereotype = pstereotype->stereotype;
      pstereotype->stereotype = 0;
      if (label != 0)
	default_label_position();
      stereotype_default_position();
      return;
    case 7:
      // not removed from the browser : just hide it
      remove(FALSE);
      break;
    case 8:
      data->delete_it();	// will delete the canvas
      break;
    default:
      if (rank >= 20) {
	ToolCom::run(Tool::command(rank - 20), data->get_start());
	return;
      }
      else if (rank >= 10) {
	rank -= 10;
	if (rank == RecenterBegin)
	  set_decenter(-1.0, decenter_end);
	else if (rank == RecenterEnd)
	  set_decenter(decenter_begin, -1.0);
	else if (rank != (int) geometry)
	  set_geometry((LineGeometry) rank, TRUE);
	else
	  return;
      }
      else
	return;
    }
    
    package_modified();
  }
}

void FlowCanvas::apply_shortcut(QString s) {
  if (s == "Select in browser") {
    data->get_start()->select_in_browser();
    return;
  }
  else if (s == "Edit drawing settings") {
    ArrowCanvas * aplabel;
    ArrowCanvas * apstereotype;
    
    search_supports(aplabel, apstereotype);
    if ((aplabel != 0) || (apstereotype != 0))
      edit_drawing_settings();
    return;
  }
  else {
    data->get_start()->apply_shortcut(s);
    return;
  }

  modified();
  package_modified();
}

void FlowCanvas::edit_drawing_settings() {
  for (;;) {
    StateSpecVector st(1);
    
    st[0].set(TR("write horizontally"), &write_horizontally);
    settings.complete(st, TRUE);
    
    SettingsDialog dialog(&st, 0, FALSE);
    
    dialog.setCaption(TR("Flow Drawing Settings dialog"));
    dialog.raise();
    
    if (dialog.exec() == QDialog::Accepted) {
      propagate_drawing_settings();
      modified();
    }
    if (!dialog.redo())
      break;
  }
}

bool FlowCanvas::has_drawing_settings() const {
  ArrowCanvas * aplabel;
  ArrowCanvas * apstereotype;
  
  search_supports(aplabel, apstereotype);
  return ((aplabel != 0) || (apstereotype != 0));
}

void FlowCanvas::edit_drawing_settings(Q3PtrList<DiagramItem> & l) {
  for (;;) {
    StateSpecVector st(1);
    Uml3States write_horizontally;
    ActivityDrawingSettings settings;
    
    st[0].set(TR("write horizontally"), &write_horizontally);
    settings.complete(st, TRUE);
    
    SettingsDialog dialog(&st, 0, FALSE, TRUE);
    
    dialog.setCaption(TR("Flow Drawing Settings dialog"));
    dialog.raise();
    
    if (dialog.exec() == QDialog::Accepted) {
      Q3PtrListIterator<DiagramItem> it(l);
      
      for (; it.current(); ++it) {
	if (!st[0].name.isEmpty())
	  ((FlowCanvas *) it.current())->write_horizontally =
	    write_horizontally;
	((FlowCanvas *) it.current())->settings.set(st, 1);
	((FlowCanvas *) it.current())->propagate_drawing_settings();
	((FlowCanvas *) it.current())->modified();
      }
    }
    if (!dialog.redo())
      break;
  }
}

void FlowCanvas::same_drawing_settings(Q3PtrList<DiagramItem> & l) {
  Q3PtrListIterator<DiagramItem> it(l);
  
  FlowCanvas * x = (FlowCanvas *) it.current();
  
  while (++it, it.current() != 0) {
    FlowCanvas * o =  (FlowCanvas *) it.current();
				 
    o->write_horizontally = x->write_horizontally;
    o->settings = x->settings;
    o->propagate_drawing_settings();
    o->modified();
  }
}

ArrowPointCanvas * FlowCanvas::brk(const QPoint & p) {
  ArrowPointCanvas * ap =
    new ArrowPointCanvas(the_canvas(), p.x(), p.y());
  
  ap->setZ(z() + 1);
  
  FlowCanvas * other =
    // do not give data to not call update()
    new FlowCanvas(the_canvas(), ap, end, br_begin, 0,
		   decenter_begin, decenter_end);
  
  other->data = data;
  other->settings = settings;
  other->used_settings = used_settings;
  other->write_horizontally = write_horizontally;
  connect(data, SIGNAL(changed()), other, SLOT(modified()));
  connect(data, SIGNAL(deleted()), other, SLOT(deleted()));

  ap->add_line(this);
  end->remove_line(this, TRUE);
  end = ap;
  
  if ((p - beginp).manhattanLength() < (p - endp).manhattanLength()) {
    if (stereotype != 0) {
      other->stereotype = stereotype;
      stereotype = 0;
    }
  }
  
  ap->show();
  other->show();
  
  modified();		// to set/unset name, role_b and multiplicity_b
  other->modified();	// to set/unset role_a and multiplicity_a
  
  return ap;
}


ArrowCanvas * FlowCanvas::join(ArrowCanvas * other, ArrowPointCanvas * ap) {
  // has already check is join is possible (self relation must have two points)
  ArrowCanvas * result = ArrowCanvas::join(other, ap);

  ((FlowCanvas *) result)->modified();
  
  return result;
}

void FlowCanvas::modified() {
  if (visible()) {
    hide();
    update(TRUE);
    show();
    check_sel_trans();
    if (begin->type() != UmlArrowPoint)
      check_stereotypeproperties();
    canvas()->update();
    package_modified();
  }
}

void FlowCanvas::update(bool updatepos) {
  if (data) {    
    QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
    FlowCanvas * plabel;
    FlowCanvas * pstereotype;
    
    {
      ArrowCanvas * aplabel;
      ArrowCanvas * apstereotype;
      
      search_supports(aplabel, apstereotype);
      plabel = (FlowCanvas *) aplabel;
      pstereotype = (FlowCanvas *) apstereotype;
    }
    
    // manages relation's stereotype
    
    QString s = data->get_short_stereotype();
    bool update_label = FALSE;
    
    if (s.isEmpty()) {
      // relation does not have stereotype
      if (pstereotype != 0) {
	// removes it
	the_canvas()->del(pstereotype->stereotype);
	pstereotype->stereotype = 0;
	pstereotype = 0;
      }
    }
    else {
      if (s == "interrupt")
	s = LabelCanvas::Zigzag;
      else if (s != LabelCanvas::Zigzag)
	s = QString("<<") + toUnicode(s) + ">>";
    
      if ((pstereotype == 0) && (begin->type() != UmlArrowPoint)) {
	// adds relation's stereotype
	stereotype = new LabelCanvas(s, the_canvas(), 0, 0);
	update_label = (label != 0);
	stereotype_default_position();
      }
      else if ((pstereotype != 0) && 
	       (pstereotype->stereotype->get_name() != s)) {
	// update name
	pstereotype->stereotype->set_name(s);
	if (auto_pos)
	  pstereotype->stereotype_default_position();
      }
    }
    
    // manages relation's name, guard and weight
  
    bool horiz = (write_horizontally == UmlDefaultState)
      ? ((BrowserActivityDiagram *) the_canvas()->browser_diagram())
	->get_write_label_horizontally()
      : (write_horizontally == UmlYes);

    used_settings = settings;
    ((BrowserActivityDiagram *) the_canvas()->browser_diagram())
      ->get_activitydrawingsettings(used_settings);
      
    s = data->str(horiz, used_settings.drawing_language);
    
    QString n = data->get_start()->get_name();
    
    if (! s.isEmpty()) {
      if (n != "<flow>")
	s = n + ((horiz) ? ' ' : '\n') + toUnicode(s);
      else
	s = toUnicode(s);
    }
    else if (n != "<flow>")
      s = n;
    
    if (s.isEmpty()) {
      if (plabel != 0) {
	// removes it
	the_canvas()->del(plabel->label);
	plabel->label = 0;
      }
    }
    else if ((plabel == 0) && (begin->type() != UmlArrowPoint)) {
      // adds relation's name
      label = new LabelCanvas(s, the_canvas(), 0, 0);
      default_label_position();
    }
    else if ((plabel != 0) && (plabel->label->get_name() != s)) {
      // update it
      plabel->label->set_name(s);
      if (auto_pos || update_label)
	plabel->default_label_position();
    }
    else if (update_label)
      plabel->default_label_position();
  }
  
  if (updatepos)
    update_pos();
}

void FlowCanvas::check_sel_trans() {
  // update must be called before
  if (used_settings.show_infonote == UmlYes) {
    // selection and transformation are placed on the first segment
    if (begin->type() != UmlArrowPoint) {
      int margin = (int) (the_canvas()->zoom() * 15);
      QString s;
      
      s = data->selection(used_settings.drawing_language);
      
      if (s.isEmpty()) {
	if (selection != 0) {
	  selection->delete_it();
	  selection = 0;
	}
      }
      else {
	s = "<<selection>>\n" + s;
	
	if (selection == 0) {
	  selection = new InfoCanvas(the_canvas(), this, s);
	  selection->upper();
	  
	  int cdy = beginp.y() - selection->height() - margin;
	  
	  selection->move(beginp.x(), (cdy < 0) ? 0 : cdy);
	  selection->show();
	  (new ArrowCanvas(the_canvas(), this, selection,
			   UmlAnchor, 0, FALSE, -1.0, -1.0))
	    ->show();
	}
	else
	  selection->set(s);
      }
      
      s = data->transformation(used_settings.drawing_language);
      
      if (s.isEmpty()) {
	if (transformation != 0) {
	  transformation->delete_it();
	  transformation = 0;
	}
      }
      else {
	s = "<<transformation>>\n" + s;
	
	if (transformation == 0) {
	  transformation = new InfoCanvas(the_canvas(), this, s);
	  transformation->upper();
	  
	  int cdy = beginp.y() + margin;
	  
	  transformation->move(beginp.x(), (cdy < 0) ? 0 : cdy);
	  transformation->show();
	  (new ArrowCanvas(the_canvas(), this, transformation,
			   UmlAnchor, 0, FALSE, -1.0, -1.0))
	    ->show();
	}
	else
	  transformation->set(s);
      }
    }
  }
  else {
    if (selection != 0) {
      selection->delete_it();
      selection = 0;
    }
    if (transformation != 0) {
      transformation->delete_it();
      transformation = 0;
    }
  }
}

void FlowCanvas::default_label_position() const {
  QPoint c = center();
  QFontMetrics fm(the_canvas()->get_font(UmlNormalFont));
  QString s = label->get_name();
  QSize sz = fm.size(0, s);
  
  label->move(c.x() - sz.width()/2,
	      c.y() - ((stereotype != 0) ? sz.height() : sz.height()/2));
  package_modified();
}

void FlowCanvas::stereotype_default_position() const {
  default_stereotype_position();
  package_modified();
}

BasicData * FlowCanvas::get_data() const {
  return data;
}

void FlowCanvas::drop(BrowserNode * bn, UmlCanvas * canvas)
{
  FlowData * def = (FlowData *) bn->get_data();
  BrowserNode * from = def->get_start_node();
  BrowserNode * to = def->get_end_node();
  DiagramItem * difrom = 0;
  DiagramItem * dito = 0;
  Q3CanvasItemList all = canvas->allItems();
  Q3CanvasItemList::Iterator cit;

  // the two extremities are drawn ?
  for (cit = all.begin(); cit != all.end(); ++cit) {
    if ((*cit)->visible()) {
      DiagramItem * di = QCanvasItemToDiagramItem(*cit);
    
      if (di != 0) {
	if (di->get_bn() == from) {
	  difrom = di;
	  if (dito != 0)
	    break;
	}
	if (di->get_bn() == to) {
	  dito = di;
	  if (difrom != 0)
	    break;
	}
      }
    }
  }
  
  if ((difrom != 0) && (dito != 0)) {
    if (difrom->has_relation(UmlFlow, def))
      msg_information("Bouml", TR("flow already drawn"));
    else {
      FlowCanvas * tr = 
	new FlowCanvas(canvas, difrom, dito, from, 0, -1.0, -1.0, def);
      
      tr->show();
      tr->package_modified();
    }
  }
}

//

void FlowCanvas::setVisible(bool yes) {
  ArrowCanvas::setVisible(yes);

  if (stereotypeproperties)
    stereotypeproperties->setVisible(yes);
}

void FlowCanvas::moveBy(double dx, double dy) {
  ArrowCanvas::moveBy(dx, dy);

  if ((stereotypeproperties != 0) && !stereotypeproperties->selected())
    stereotypeproperties->moveBy(dx, dy);
}

void FlowCanvas::select_associated() {
  if (!selected()) {
    if ((stereotypeproperties != 0) && !stereotypeproperties->selected())
      the_canvas()->select(stereotypeproperties);
    ArrowCanvas::select_associated();
  }
}

void FlowCanvas::check_stereotypeproperties() {
  // the note is memorized by the first segment
  if (begin->type() == UmlArrowPoint)
    ((FlowCanvas *) ((ArrowPointCanvas *) begin)->get_other(this))
      ->check_stereotypeproperties();
  else {
    QString s = data->get_start()->stereotypes_properties();
    
    if (!s.isEmpty() &&
	(used_settings.show_stereotype_properties == UmlYes))
      StereotypePropertiesCanvas::needed(the_canvas(), this, s,
					 stereotypeproperties, center());
    else if (stereotypeproperties != 0) {
      stereotypeproperties->delete_it();
      stereotypeproperties = 0;
    }
  }
}

bool FlowCanvas::represents(BrowserNode * bn) {
  return (data == bn->get_data());
}

//

void FlowCanvas::save(Q3TextStream & st, bool ref, QString & warning) const {
  if (ref)
    st << "flowcanvas_ref " << get_ident();
  else if (begin->type() != UmlArrowPoint) {
    // relation canvas start
    nl_indent(st);
    st << "flowcanvas " << get_ident() << ' ';
    data->get_start()->save(st, TRUE, warning);	// data doesn't have id
    indent(+1);
    if (geometry != NoGeometry) {
      nl_indent(st);
      st << "geometry " << stringify(geometry);
      if (!fixed_geometry)
	st << " unfixed";
    }
    if (decenter_begin >= 0) {
      // float output/input bugged
      nl_indent(st);
      st << "decenter_begin " << ((int) (decenter_begin * 1000));
    }
    if (decenter_end >= 0) {
      // float output/input bugged
      nl_indent(st);
      st << "decenter_end " << ((int) (decenter_end * 1000));
    }
    nl_indent(st);
    (const FlowCanvas *) ArrowCanvas::save_lines(st, TRUE, TRUE, warning);
    settings.save(st);
    st << " write_horizontally " << stringify(write_horizontally);

    if (selection != 0) {
      nl_indent(st);
      st << "selection ";
      selection->save(st, FALSE, warning);
    }
    
    if (transformation != 0) {
      nl_indent(st);
      st << "transformation ";
      transformation->save(st, FALSE, warning);
    }
    
    if (stereotypeproperties != 0)
      stereotypeproperties->save(st, FALSE, warning);
    
    indent(-1);
    nl_indent(st);
    st << "end";
  }
}

FlowCanvas * FlowCanvas::read(char * & st, UmlCanvas * canvas, char * k)
{
  if (!strcmp(k, "flowcanvas_ref"))
    return ((FlowCanvas *) dict_get(read_id(st), "flowcanvas", canvas));
  else if (!strcmp(k, "flowcanvas")) {    
    int id = read_id(st);
    BrowserFlow * br = BrowserFlow::read_ref(st);
    FlowData * tr = (FlowData *) br->get_data();
    LineGeometry geo;
    bool fixed = FALSE;
    
    k = read_keyword(st);
    if (! strcmp(k, "geometry")) {
      geo = line_geometry(read_keyword(st));
      k = read_keyword(st);
      if (! strcmp(k, "unfixed"))
	k = read_keyword(st);
      else {
	fixed = TRUE;
      }
    }
    else
      geo = NoGeometry;
    
    float dbegin;
    float dend;

    if (! strcmp(k, "decenter_begin")) {
      dbegin = read_double(st) / 1000;
      k = read_keyword(st);
    }
    else
      dbegin = -1;

    if (! strcmp(k, "decenter_end")) {
      dend = read_double(st) / 1000;
      k = read_keyword(st);
    }
    else
      dend = -1;

    unread_keyword(k, st);
    
    read_keyword(st, "from");
    read_keyword(st, "ref");
    
    DiagramItem * bi = dict_get(read_id(st), "canvas", canvas);
    BrowserNode * b = tr->get_start_node();
    QString s = br->get_name();
    FlowCanvas * first = 0;
    FlowCanvas * result;
    LabelCanvas * label;
    LabelCanvas * stereotype;
    double z;
              
    for (;;) {
      read_keyword(st, "z");
      z = read_double(st);
      k = read_keyword(st);
      
      if ((label = LabelCanvas::read(st, canvas, k)) != 0) {
	// the flow name can't be empty
	label->setZ(z);
	k = read_keyword(st);
      }
      
      if (!strcmp(k, "stereotype")) {
	(void) read_string(st);
	read_keyword(st, "xyz");
	
	int x = (int) read_double(st);
	int y = (int) read_double(st);
	
	if ((tr == 0) || (*(tr->get_stereotype()) == 0)) {
	  stereotype = 0;
	  read_double(st);	// z
	}
	else {
	  QString s = tr->get_short_stereotype();
	 
	  if (s == "interrupt")
	    s = LabelCanvas::Zigzag;
	  else
	    s = QString("<<") + toUnicode(s) + ">>";

	  stereotype = new LabelCanvas(s, canvas, x, y);
	  stereotype->setZ(read_double(st));
	}
	k = read_keyword(st);
      }
      else
	stereotype = 0;

      if (strcmp(k, "to"))
	wrong_keyword(k, "to");

      DiagramItem * di;
      
      if (strcmp(k = read_keyword(st), "ref")) {
	di = ArrowPointCanvas::read(st, canvas, k);
	
	if (di == 0)
	  unknown_keyword(k);
      }
      else
	di = dict_get(read_id(st), "activity node", canvas);

      // do not give data to not call update()
      result = new FlowCanvas(canvas, bi, di, b, id, dbegin, dend);
      result->geometry = geo;
      result->fixed_geometry = fixed;
      result->set_z(z);
      result->data = tr;
      connect(tr, SIGNAL(changed()), result, SLOT(modified()));
      connect(tr, SIGNAL(deleted()), result, SLOT(deleted()));

      if (first == 0) {
	first = result;
	if (read_file_format() == 30)
	  // to remove redondant relation made by release 2.22
	  RelsToCheck.append(result);
      }
      if (label != 0)
	result->label = label;
      if (stereotype != 0)
	(result->stereotype = stereotype)->show();
      result->show();
      
      if (di->type() != UmlArrowPoint)
	break;
      
      bi = di;
      
      read_keyword(st, "line");
      id = read_id(st);
    }
    
    k = read_keyword(st);
    result->settings.read(st, k);	// updates k    
    if (strcmp(k, "write_horizontally") != 0)
      wrong_keyword(k, "write_horizontally");
    result->write_horizontally = state(read_keyword(st));
    k = read_keyword(st);
    result->propagate_drawing_settings();
    
    // to add label, stereotype ... if needed    
    first->update(FALSE);
    if (first != result)
      result->update(FALSE);
    
    if (! strcmp(k, "selection")) {
      k = read_keyword(st);
      first->selection = InfoCanvas::read(st, canvas, k, first);
      k = read_keyword(st);
    }
    if (! strcmp(k, "transformation")) {
      k = read_keyword(st);
      first->transformation = InfoCanvas::read(st, canvas, k, first);
      k = read_keyword(st);
    }

    first->stereotypeproperties = 
      StereotypePropertiesCanvas::read(st,  canvas, k, first);
    
    if (first->stereotypeproperties != 0)
      k = read_keyword(st);
    
    first->check_sel_trans();
    first->check_stereotypeproperties();

    // manage case where the relation is deleted but present in the browser
    if (result->data->get_start()->deletedp())
      RelsToDel.append(result);
    else
      result->update_geometry();
    
    if (strcmp(k, "end"))
      wrong_keyword(k, "end");
    
    return result;
  }
  else
    return 0;
}

void FlowCanvas::history_hide() {
  Q3CanvasItem::setVisible(FALSE);
  unconnect();
}

void FlowCanvas::history_load(QBuffer & b) {
  ArrowCanvas::history_load(b);
  connect(data, SIGNAL(changed()), this, SLOT(modified()));
  connect(data, SIGNAL(deleted()), this, SLOT(deleted()));
  connect(DrawingSettings::instance(), SIGNAL(changed()),
	  this, SLOT(modified()));
}
