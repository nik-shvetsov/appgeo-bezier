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
    MyCurve( float scalefactor = float(5) );
//    MyCurve( const MyCurve& copy );

    virtual ~MyCurve();

    float           getScaleFactor();
    bool            isClosed() const;
    void            setScaleFactor( float scalefactor = float(5) );


protected:
    float               _scf;

    void                eval(float t, int d, bool l);
    float               getEndP();
    float               getStartP();

}; // END class


#endif // MYCURVE_H
