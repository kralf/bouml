#ifndef _UMLUSECASE_H
#define _UMLUSECASE_H


#include "UmlBaseUseCase.h"
#include <q3cstring.h>

class FileOut;

// This class manages 'use case'
//
// You can modify it as you want (except the constructor)
class UmlUseCase : public UmlBaseUseCase {
  public:
    UmlUseCase(void * id, const Q3CString & n) : UmlBaseUseCase(id, n) {};

    virtual void write(FileOut & out);

};

#endif
