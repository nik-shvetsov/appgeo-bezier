#ifndef MYCURVE_H
#define MYCURVE_H

#include <gmParametricsModule>

class MyCurve : public GMlib::PCurve<float,3>
{
    GM_SCENEOBJECT(MyCurve)
public:
    MyCurve(float scalefactor = float(5));
    ~MyCurve();

    float           getScaleFactor();
    bool            isClosed() const;
    void            setScaleFactor( float scalefactor = float(5) );


protected:
    float               _scf;

    void                eval(float t, int d, bool l);
    float               getEndP();
    float               getStartP();

    //void                localSimulate(double dt);

};

#endif // MYCURVE_H
