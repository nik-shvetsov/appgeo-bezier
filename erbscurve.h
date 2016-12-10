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

    void                        createSubcurves(PCurve<float,3>* g);
    void                        createBezierCurves(PCurve<float,3> *g, int d);
    void                        visualizeLocalCurves();

    int                         findIndex(float t);
    bool                        isClosed();
    void                        localSimulate(double dt);

  protected:
    float                                       _scale = 1.0;
    GMlib::DVector<float>                       _kv;
    int                                         _d; //bezier dim
    int                                         _n; //number of local curves
    PCurve<float,3>*                            _curve;
    GMlib::DVector<GMlib::PCurve<float,3>*>     _localCurves;

    float                                       _s;
    float                                       _e;
    LOCAL_CURVE_TYPE                            _type;

    float                       getT(float t, int index);
    GMlib::DVector<float>       makeBFunction(float t, float d, float scale);


    void                  eval(float t, int d, bool = true);
    float                 getEndP();
    float                 getStartP();

  }; // END class

#endif // ERBSCURVE_H
