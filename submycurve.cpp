#include "submycurve.h"

  SubMyCurve::SubMyCurve(PCurve<float, 3>* c, float s, float e)
  {
      this->_dm = GM_DERIVATION_EXPLICIT;

      set(c, s, e, (s+e)/2);

      //for changing coord system to local
      DVector<Vector<float,3> > tr = _c->evaluateParent(_t, 0);
      _trans = tr[0];
      this->translate( _trans );
  }

  SubMyCurve::SubMyCurve(PCurve<float, 3>* c, float s, float e, float t)
  {
      this->_dm = GM_DERIVATION_EXPLICIT;

      set(c, s, e, t);

      //for changing coord system to local
      DVector<Vector<float,3> > tr = _c->evaluateParent(_t, 0);
      _trans = tr[0];
      this->translate( _trans );
  }

//  SubMyCurve::SubMyCurve( const SubMyCurve& copy ) : MyCurve( copy )
//  {
//    set(copy._c, copy._s, copy._e, copy._t);
//    // move origin to interpolation point
//    _trans = copy._trans;
//  }

  SubMyCurve::~SubMyCurve() {}

  void SubMyCurve::set(PCurve<float, 3>* c, float s, float e, float t)
  {
    _c = c;
    _s = s;
    _t = t;
    _e = e;
  }

  // Virtual functions

  void SubMyCurve::eval( float t, int d, bool /*l*/ )
  {
    this->_p     = _c->evaluateParent(t , d);
    this->_p[0] -= _trans;
  }

  float SubMyCurve::getStartP()
  {
      return _s;
  }

  float SubMyCurve::getEndP()
  {
      return _e;
  }

  bool SubMyCurve::isClosed() const
  {
      return false;
  }


