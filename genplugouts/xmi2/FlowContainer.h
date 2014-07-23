#ifndef _FLOWCONTAINER_H
#define _FLOWCONTAINER_H


#include <q3ptrlist.h>

class UmlFlow;
class FileOut;

class FlowContainer {
  public:
    virtual ~FlowContainer();

    //memorize flow starting in sub elements to produce them later
    virtual void memo_flow(UmlFlow * flow);

    //write flows outside their source element
    
    void write_flows(FileOut & out);


  protected:
    //he flows, to produce them in the upper level of the source - dest
    Q3PtrList<UmlFlow> _flows;

};

#endif
