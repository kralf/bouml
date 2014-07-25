
#include "UmlCom.h"
#include "UmlBaseUseCaseDiagramDefinition.h"

void UmlBaseUseCaseDiagramDefinition::read_() {
  Q3PtrDict<UmlUseCaseReference> ucrefs;
  unsigned n;
  unsigned rank;

  n = UmlCom::read_unsigned();
  _fragments.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlFragment * f = new UmlFragment();

    _fragments.insert(rank, f);
    f->read_();
  }
  _fragments.setAutoDelete(TRUE);
  UmlBaseFragment::compute_container_(_fragments);

  n = UmlCom::read_unsigned();
  _subjects.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlSubject * sb = new UmlSubject();

    _subjects.insert(rank, sb);
    sb->read_();
  }
  _subjects.setAutoDelete(TRUE);

  n = UmlCom::read_unsigned();
  _use_cases.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlUseCaseReference * uc = new UmlUseCaseReference();

    _use_cases.insert(rank, uc);
    ucrefs.insert((void *) UmlCom::read_unsigned(), uc);
    uc->read_(_fragments, _subjects);
  }
  _use_cases.setAutoDelete(TRUE);

  n = UmlCom::read_unsigned();
  _actors.resize(n);
  for (rank = 0; rank != n; rank += 1)
    _actors.insert(rank, (UmlClass *) UmlBaseItem::read_());

  n = UmlCom::read_unsigned();
  _rels.resize(n);
  for (rank = 0; rank != n; rank += 1) {
    UmlUseCaseAssociation * r = new UmlUseCaseAssociation();

    _rels.insert(rank, r);
    r->read_(ucrefs);
  }
  _rels.setAutoDelete(TRUE);
}

 UmlBaseUseCaseDiagramDefinition::~UmlBaseUseCaseDiagramDefinition() {
}

