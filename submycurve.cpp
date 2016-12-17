#include "submycurve.h"

SubMyCurve::SubMyCurve(PCurve<float, 3>* c, float s, float e)
{
    this->_dm = GMlib::GM_DERIVATION_EXPLICIT;

    setParams(c, s, e, (s+e)/2);

    //for changing coord system from global to local
    GMlib::DVector<GMlib::Vector<float,3>> tr = _c->evaluateParent(_t, 0);
    _trans = tr[0];
    this->translate( _trans );
}

SubMyCurve::SubMyCurve(PCurve<float, 3>* c, float s, float e, float t)
{
    this->_dm = GMlib::GM_DERIVATION_EXPLICIT;

    setParams(c, s, e, t);

    GMlib::DVector<GMlib::Vector<float,3>> tr = _c->evaluateParent(_t, 0);
    _trans = tr[0];
    this->translate( _trans );
}

SubMyCurve::~SubMyCurve() {}

void SubMyCurve::setParams(PCurve<float, 3>* c, float s, float e, float t)
{
    _c = c;
    _s = s;
    _e = e;
    _t = t;
}

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

void SubMyCurve::localSimulate(double dt)
{
    //this->rotate(GMlib::Angle(M_PI/4 * dt), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ));
}
