#ifndef MYCURVE_H
#define MYCURVE_H

#include <parametrics/gmpcurve>
#include <gmSceneModule>
#include <gmParametricsModule>

using namespace GMlib;

class MyCurve : public PCurve<float,3>
{
    GM_SCENEOBJECT(MyCurve)
    public:
    MyCurve( float radius = float(20) );
    MyCurve( const MyCurve& copy );

    virtual ~MyCurve();

    float           getRadius();
    bool            isClosed() const;
    void            setRadius( float radius = float(20) );


protected:
    float               _r;

    void                eval(float t, int d, bool l);
    float               getEndP();
    float               getStartP();

}; // END class


#endif // MYCURVE_H
