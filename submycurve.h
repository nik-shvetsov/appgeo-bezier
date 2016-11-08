#ifndef SUBMYCURVE_H
#define SUBMYCURVE_H

#include "mycurve.h"

using namespace GMlib;

class SubMyCurve : public MyCurve
{
public:
    SubMyCurve( MyCurve* c, float s, float e);
    SubMyCurve( const SubMyCurve& copy );
    virtual ~SubMyCurve();

    // virtual functions from SceneObject
    std::string         getIdentity() const;
    // virtual functions from PSurf
    bool                isClosed() const;

protected:
    MyCurve*                       _c;
    float                          _s;
    float                          _t;
    float                          _e;
    GMlib::Vector<float,3>     _trans;

    // virtual functions from PSurf
    void                    eval( float t, int d, bool l = false );
    float                   getEndP();
    float                   getStartP();

private:
    // Local help functions
    void                    set(MyCurve* c, float s, float e, float t);
}; // END class

#endif // SUBMYCURVE_H
