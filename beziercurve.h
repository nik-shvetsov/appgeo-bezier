#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "uevaluator.h"
#include <gmParametricsModule>

class CustomBezierCurve : public GMlib::PCurve<float,3>
{
    GM_SCENEOBJECT(CustomBezierCurve)
public:
    CustomBezierCurve(PCurve<float,3>* c, float s, float e, float t, int d);
    ~CustomBezierCurve(){}

    int                                         getDegree() const;
    GMlib::DVector<GMlib::Vector<float, 3>>     getControlPoints() const;
    void                                        setControlPoints(const GMlib::DVector<GMlib::Vector<float, 3>> &controlPoints);

    bool                                        isClosed() const;
    //void                                        setClosed(bool closed);

protected:
    GMlib::DVector<GMlib::Vector<float, 3>>     _controlPoints;
    //GMlib::DVector<GMlib::DMatrix<float>>       _t;

    PCurve<float,3>*                            _c;
    float                                       _s;
    float                                       _e;
    //float                                       _t;
    GMlib::Vector<float,3>                      _trans;

    float                                       _scaleDer = 1.0;
    float                                       _scaleCen;

    UEvaluator                                  _uniEvaluator;

    void                                        eval(float t, int d = 0, bool l = false);
    float                                       getStartP();
    float                                       getEndP();

    void                                        localSimulate(double dt);

private:
    void                                        set(PCurve<float,3>* curve, float s, float e, float t, int d);
};
#endif // BEZIERCURVE_H
