#ifndef SUBMYCURVE_H
#define SUBMYCURVE_H

#include "mycurve.h"
#include <gmParametricsModule>

//class SubMyCurve : public MyCurve
class SubMyCurve : public GMlib::PCurve<float,3>
{
    GM_SCENEOBJECT(SubMyCurve)
public:
    SubMyCurve( PCurve<float, 3>* c, float s, float e);
    SubMyCurve( PCurve<float, 3>* c, float s, float e, float t);
//    SubMyCurve( const SubMyCurve& copy );
    virtual ~SubMyCurve();

    // virtual functions from PSurf
    bool                isClosed() const;

protected:
    PCurve<float, 3>*              _c;
    float                          _s;
    float                          _t;
    float                          _e;
    GMlib::Vector<float,3>     _trans;

    // virtual functions from PCurve
    void                    eval( float t, int d, bool l = false );
    float                   getEndP();
    float                   getStartP();

private:
    // Local help functions
    void                    set(PCurve<float, 3>* c, float s, float e, float t);
}; // END class

#endif // SUBMYCURVE_H
