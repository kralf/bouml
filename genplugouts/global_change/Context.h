#ifndef _CONTEXT_H
#define _CONTEXT_H


#include <q3cstring.h>

enum Equation {
  NoFilter,
  Filter1,
  And12,
  Or12,
  And12_And23,
  And12_Or23,
  Or12_And23,
  Or12_Or23

};
class Context {
  public:
    Context(const Q3CString & o, const Q3CString & f);

    const char * set_filters(const Q3CString & f1, const Q3CString & f2, const Q3CString & f3, bool w1, bool w2, bool w3, bool and12, bool and23);

    void set_stereotype(const Q3CString & s, bool eq, bool neq);

    void set_targets(bool art, bool cl, bool op, bool att, bool rel);

    void set_language(bool c, bool j, bool p, bool y, bool i);

    bool match_stereotype(const Q3CString & s);

    bool match(Q3CString s);

    void err();

    Q3CString replace(Q3CString s);

    bool onArtifact() const;

    bool onClass() const;

    bool onOperation() const;

    bool onAttribute() const;

    bool onRelation() const;

    bool cpp() const;

    bool java() const;

    bool php() const;

    bool python() const;

    bool idl() const;

    int n_match() const;

    int n_err() const;


  protected:
    Q3CString _filter1;

    Q3CString _filter2;

    Q3CString _filter3;

    bool _with1;

    bool _with2;

    bool _with3;

    Q3CString _stereotype;

    bool _is;

    bool _isnot;

    bool _onArtifact;

    bool _onClass;

    bool _onOperation;

    bool _onAttribute;

    bool _onRelation;

    bool _cpp;

    bool _java;

    bool _php;

    bool _python;

    bool _idl;

    Q3CString _old;

    Q3CString _future;

    Equation _equation;

    int _n_match;

    int _n_err;

    bool match(Q3CString s, const Q3CString & filter, bool with);

};

inline void Context::set_stereotype(const Q3CString & s, bool eq, bool neq) {
  _stereotype = s;
  _is = eq;
  _isnot = neq;
}

inline void Context::set_targets(bool art, bool cl, bool op, bool att, bool rel) {
_onArtifact = art;
_onClass = cl;
_onOperation = op;
_onAttribute = att;
_onRelation = rel;
}

inline void Context::set_language(bool c, bool j, bool p, bool y, bool i) {
_cpp = c;
_java = j;
_php = p;
_python = y;
_idl = i;
}

inline bool Context::onArtifact() const {
  return _onArtifact;
}

inline bool Context::onClass() const {
  return _onClass;
}

inline bool Context::onOperation() const {
  return _onOperation;
}

inline bool Context::onAttribute() const {
  return _onAttribute;
}

inline bool Context::onRelation() const {
  return _onRelation;
}

inline bool Context::cpp() const {
  return _cpp;
}

inline bool Context::java() const {
  return _java;
}

inline bool Context::php() const {
  return _php;
}

inline bool Context::python() const {
  return _python;
}

inline bool Context::idl() const {
  return _idl;
}

inline int Context::n_match() const {
  return _n_match;
}

inline int Context::n_err() const {
  return _n_err;
}

#endif
