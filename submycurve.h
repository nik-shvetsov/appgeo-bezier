#ifndef SUBMYCURVE_H
#define SUBMYCURVE_H

#include "mycurve.h"

//class SubMyCurve : public MyCurve
class SubMyCurve : public GMlib::PCurve<float,3>
{
    GM_SCENEOBJECT(SubMyCurve)
public:
    SubMyCurve(GMlib::PCurve<float, 3>* c, float s, float e);
    SubMyCurve(GMlib::PCurve<float, 3>* c, float s, float e, float t);
    virtual ~SubMyCurve();

    //virtual functions from PSurf
    bool                    isClosed() const;

protected:
    GMlib::PCurve<float, 3>*        _c;
    float                           _s;
    //float                           _t;
    float                           _e;
    GMlib::Vector<float,3>          _trans;

    //virtual functions from PCurve
    void                    eval( float t, int d, bool l = false );
    float                   getEndP();
    float                   getStartP();

    void                    localSimulate(double dt);

private:
    //local help functions
    void                    setParams(GMlib::PCurve<float, 3>* c, float s, float e, float t);

    float                   _sum;
    bool                    _flag = true;
};

#endif // SUBMYCURVE_H
