#ifndef ERBSCURVE_H
#define ERBSCURVE_H

#include <parametrics/gmpcurve>
#include <parametrics/gmpsubcurve>
#include <parametrics/evaluators/gmerbsevaluator.h>
#include "submycurve.h"
#include "knotvector.h"


class CustomERBSCurve : public PCurve<float,3>
{
    GM_SCENEOBJECT(CustomERBSCurve)
    public:

        CustomERBSCurve( PCurve<float, 3>* curve, int n, int d = 2);
        //CustomERBSCurve( const CustomERBSCurve& copy );

        bool isClosed() const;
        void setClosed(bool closed);

        void insertCurve( PCurve<float,3>* localCurve );
        GMlib::PCurve<float,3>* makeLocalCurve(PCurve<float, 3>* curve, float start, float end, float t, int d = 2);

    protected:
        int _resolutionLocalCurves = 50;
        bool _isClosed;

        GMlib::BasisEvaluator<long double>* _evaluator;

        void eval(float t, int d = 0, bool l = false);
        float getStartP();
        float getEndP();

        float mapToLocal(float t, int tk);

        void getB( GMlib::DVector<float>& B, int k,float t, int d);

        void computeBlending(int d, const GMlib::DVector<float>& B, GMlib::DVector<GMlib::Vector<float,3>>& c0,
                       GMlib::DVector<GMlib::Vector<float,3>>& c1);

        void localSimulate(double dt);

        GMlib::DVector<float> _knotVector;
        GMlib::DVector<PCurve<float,3>*> _curves;

        float _totalTime = 0; // for sim
};

#endif // ERBSCURVE_H
