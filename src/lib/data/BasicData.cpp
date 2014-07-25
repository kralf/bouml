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





#include "BasicData.h"
#include "BrowserNode.h"
#include "myio.h"
#include "ToolCom.h"
#include "mu.h"
//Added by qt3to4:
#include <Q3CString>
#include <Q3TextStream>
//Added by qt3to4:
#include <Q3PtrList>

BasicParent BasicParent::the;

void BasicParent::insertChild(QObject *){
  // does nothing !
}

void BasicParent::removeChild(QObject *){
  // does nothing !
}

//

Q3PtrList<BasicData> BasicData::removed;

BasicData::BasicData(const BasicData * model) 
    : QObject(&BasicParent::the), browser_node(0) {
   stereotype = model->stereotype;
}

BasicData::~BasicData() {
}

// does not really delete it for undelete management
void BasicData::delete_it() {
  if (!deletedp()) {
    set_deletedp(TRUE);
    emit deleted();
    removed.append(this);
  }
}

void BasicData::undelete(QString &, QString &) {
  if (deletedp()) {
    set_deletedp(FALSE);
    emit changed();
    removed.removeRef(this);
  }
}

void BasicData::resignal_deleted()
{
  BasicData * d;
  
  for (d = removed.last(); d != 0; d = removed.prev())
    d->redelete_it();
}

void BasicData::on_delete() {
  browser_node->on_delete();
}
  
void BasicData::modified() {
  emit changed();
}

//

const char * BasicData::get_short_stereotype() const {
  const char * s = stereotype;
  const char * p = strchr(s, ':');
  
  return (p != 0) ? p+1 : s;
}

bool BasicData::set_stereotype(const QString & s) {
  bool result = ((const char *) stereotype != s);
  
  stereotype = s;
  if (browser_node != 0)
    browser_node->update_stereotype();
  
  return result;
}

bool BasicData::set_stereotype(const Q3CString & s) {
  bool result = ((const char *) stereotype != s);
  
  stereotype = s;
  if (browser_node != 0)
    browser_node->update_stereotype();
  
  return result;
}

bool BasicData::set_stereotype(const char * s) {
  bool result = ((const char *) stereotype != s);
  
  stereotype = s;
  if (browser_node != 0)
    browser_node->update_stereotype();
  
  return result;
}

void BasicData::set_browser_node(BrowserNode * bn) {
  browser_node = bn;
  bn->update_stereotype();
}

QString BasicData::definition(bool, bool with_kind) const {
  return (with_kind)
    ? "[" + browser_node->get_stype() + "] " + browser_node->get_name()
    : QString(browser_node->get_name());
}

bool BasicData::decldefbody_contain(const QString &, bool,
				    BrowserNode *) {
  return FALSE;
}

void BasicData::save(Q3TextStream & st, QString &) const {
  if (! stereotype.isEmpty()) {
    nl_indent(st);
    st << "stereotype ";
    save_string(stereotype, st);
  }
}

void BasicData::read(char * & st, char * & k) {
  if (!strcmp(k, "stereotype")) {
    set_stereotype(read_string(st));
    k = read_keyword(st);
  }
  else
    set_stereotype(QString::null);
}

//

void BasicData::send_uml_def(ToolCom * com, BrowserNode * bn,
			     const QString & comment) {
  if (browser_node != 0) {
    BrowserNode * d = browser_node->get_associated();
    
    if (d != browser_node) {
      // may have an associated diagram/view
      if ((d != 0) && !d->deletedp())
	d->write_id(com);
      else
	com->write_id(0);
    }
  }
  
  com->write_string(stereotype);
  ((HaveKeyValueData *) bn)->send_def(com);
  com->write_string(comment);
  
  int api = com->api_format();
  
  if (api > 11) {
    com->write_bool(bn->markedp());
    if (api > 30)
      com->write_unsigned(bn->get_identifier());
  }
}

void BasicData::send_cpp_def(ToolCom *) {
}

void BasicData::send_java_def(ToolCom *) {
}

void BasicData::send_php_def(ToolCom *) {
}

void BasicData::send_python_def(ToolCom *) {
}

void BasicData::send_idl_def(ToolCom *) {
}

bool BasicData::tool_cmd(ToolCom * com, const char * args,
			 BrowserNode * bn, const QString & comment) {
  switch ((unsigned char) args[-1]) {
  case setStereotypeCmd:
    if (!bn->is_writable() && !root_permission())
      com->write_ack(FALSE);
    else {
      set_stereotype(args);
      browser_node->package_modified();
      modified();
      com->write_ack(TRUE);
    }
    break;
  case getDefCmd:
    send_uml_def(com, bn, comment);
    send_cpp_def(com);
    send_java_def(com);
    if (com->api_format() >= 34) {
      send_php_def(com);
      if (com->api_format() >= 39)
	send_python_def(com);
    }
    send_idl_def(com);
    break;
  case getUmlDefCmd:
    send_uml_def(com, bn, comment);
    break;
  case getCppDefCmd:
    send_uml_def(com, bn, comment);
    send_cpp_def(com);
    break;
  case getJavaDefCmd:
    send_uml_def(com, bn, comment);
    send_java_def(com);
    break;
  case getPhpDefCmd:
    send_uml_def(com, bn, comment);
    send_php_def(com);
    break;
  case getPythonDefCmd:
    send_uml_def(com, bn, comment);
    send_python_def(com);
    break;
  case getIdlDefCmd:
    send_uml_def(com, bn, comment);
    send_idl_def(com);
    break;
  default:
    return FALSE;
  }
  
  return TRUE;
}    
