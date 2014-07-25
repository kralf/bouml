
#include "UmlCom.h"
#include "UmlBaseParameterSet.h"
#include "UmlActivityParameter.h"
#include "UmlActivityAction.h"
#include "UmlActivityPin.h"

UmlActivityParameter * UmlBaseParameterSet::create(UmlActivityAction * parent, const char * s)
{
    return (UmlActivityParameter *) parent->create_(aParameterSet, s);
}

anItemKind UmlBaseParameterSet::kind() {
    return aParameterSet;
}

const Q3PtrVector<UmlActivityPin> & UmlBaseParameterSet::pins() {
    read_if_needed_();
    return _pins;
}

bool UmlBaseParameterSet::set_Pins(const Q3PtrVector<UmlActivityPin> & v) {
    UmlCom::send_cmd(_identifier, replaceParameterCmd, (const Q3PtrVector<UmlItem> &) v);
    if (UmlCom::read_bool()) {
      if (_defined) _pins = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseParameterSet::unload(bool rec, bool del) {
    _pins.resize(0);
    UmlBaseItem::unload(rec, del);
}

void UmlBaseParameterSet::read_uml_() {
    UmlBaseItem::read_uml_();
    UmlCom::read_item_list((Q3PtrVector<UmlItem> &) _pins);
}

