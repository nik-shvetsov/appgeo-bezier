#ifndef ERBSCURVE_H
#define ERBSCURVE_H

#include <gmParametricsModule>

class CustomERBS : public GMlib::PCurve<float,3> {
    GM_SCENEOBJECT(CustomERBS)
public:

    enum LOCAL_CURVE_TYPE
    {
        SUB_CURVE,
        BEZIER_CURVE
    };

    CustomERBS(PCurve<float,3> *curve, int n);
    CustomERBS(PCurve<float, 3> *curve, int n, int d);
    ~CustomERBS();

    GMlib::DVector<float>       generateKnotVector();
    void                        createLocalCurves(PCurve<float,3>* c, int d);
    void                        toggleVisLocalCurves();

    void                        prepareLocalCurves();

    int                         findIndex(float t);
    bool                        isClosed();
    void                        localSimulate(double dt);

protected:
    float                                       _scaleB = 1.0;
    float                                       _scaleWt;
    GMlib::DVector<float>                       _kv;
    int                                         _deg = 2; //bezier degdimdr
    int                                         _n; //number of local curves
    PCurve<float,3>*                            _curve;
    GMlib::DVector<GMlib::PCurve<float,3>*>     _localCurves;

    float                                       _s;
    float                                       _e;
    LOCAL_CURVE_TYPE                            _type;

    float                       getT(float t, int index);
    GMlib::DVector<float>       makeBFunction(float t, float scale, float d);


    void                  eval(float t, int d, bool = true);
    float                 getEndP();
    float                 getStartP();

private:

    float               _timer = 0.0;
    float               _accum = 0.0; //dt [-1;1] accumulator
    float               _cossum = M_PI_2; //function cos variable
    float               _mult = 0.001; //scaling speed
    bool                _flag = true;
    float               _sum = 0.0;

  }; // END class

#endif // ERBSCURVE_H
