#include "erbscurve.h"
#include "submycurve.h"
#include "mycurve.h"
#include "beziercurve.h"

#include <parametrics/gmerbsevaluator>
#include <parametrics/gmpbeziercurve>
//GMlib::PERBSCurve

CustomERBSCurve::CustomERBSCurve(PCurve<float, 3>* curve, int n, int d)
{
    this->_dm = GM_DERIVATION_EXPLICIT;

    _evaluator = new GMlib::ERBSEvaluator<long double>;
    //or use logistic function

    if (curve->isClosed())
    {
        setClosed(true);
    }
    if (isClosed())
    {
        n++;
    }


    auto kv = new KnotVector(curve, curve->getParStart(), curve->getParEnd(), n);
    _knotVector = kv->getKnotVector();

    //local curves
    _curves.setDim(n);
    for(int i = 1; i < n; i++)
    {
        _curves[i-1] = makeLocalCurve(curve, _knotVector[i-1], _knotVector[i+1], _knotVector[i], d); //s,e,t
        insertCurve(_curves[i-1]);
    }

    if (isClosed())
    {
        _curves[n-1] = _curves[0];
    }
    else
    {
        _curves[n-1] = makeLocalCurve(curve, _knotVector[n-1], _knotVector[n + 1], _knotVector[n], d);
        insertCurve(_curves[n-1]);
    }
}

//CustomERBSCurve::CustomERBSCurve(const CustomERBSCurve &copy)
//{

//}

bool CustomERBSCurve::isClosed() const
{
    return _isClosed;

}

void CustomERBSCurve::setClosed(bool closed)
{
    _isClosed = closed;
}

void CustomERBSCurve::insertCurve(PCurve<float, 3>* localCurve)
{
    localCurve->toggleDefaultVisualizer();
    localCurve->replot(_resolutionLocalCurves,1);
    localCurve->setColor(GMlib::GMcolor::Green);
    localCurve->setVisible(true);
    localCurve->setCollapsed(false);
    this->insert(localCurve);
}

GMlib::PCurve<float, 3>* CustomERBSCurve::makeLocalCurve(PCurve<float, 3>* curve, float start, float end, float t, int d)
{
    //return new PSubCurve<float>(curve, start, end, t);
    return new SubMyCurve(curve, start, end, t);
}

void CustomERBSCurve::eval(float t, int d, bool l)
{
    int k;
    // Find knot
    for (k = 1; k < _knotVector.getDim() - 2; ++k)
    {
        if (t < _knotVector[k+1]) break;
    }

    //First curve
    GMlib::DVector<GMlib::Vector<float,3>> c0 = _curves[k-1]->evaluateParent(mapToLocal(t, k), d); //(new t, d)

    if (std::abs(t - _knotVector[k])  < 1e-5 )
    {
        _p = c0;
        return;
    }

    //Second curve
    GMlib::DVector<GMlib::Vector<float,3>> c1 = _curves[k]->evaluateParent(mapToLocal(t, k+1), d);

    //Blend
    GMlib::DVector<float> B;
    getB(B,k,t,d);
    computeBlending(d, B, c0, c1);
}

float CustomERBSCurve::getStartP()
{
    if (_knotVector.getDim() > 0)
    {
        return _knotVector[1];
    }
    else
    {
        return 0;
    }
}

float CustomERBSCurve::getEndP()
{
    if (_knotVector.getDim() > 1)
    {
        return _knotVector[_knotVector.getDim() - 2];
    }
    else
    {
        return 0;
    }
}

float CustomERBSCurve::mapToLocal(float t, int tk)
{
    float c_start = _curves[tk-1]->getParStart();
    float c_delta = _curves[tk-1]->getParDelta();
    return c_start + (t - _knotVector[tk-1]) / (_knotVector[tk+1] - _knotVector[tk-1]) * c_delta;
}

void CustomERBSCurve::getB(GMlib::DVector<float> &B, int k, float t, int d)
{
    B.setDim(d + 1);
    _evaluator->set(_knotVector[k], _knotVector[k+1] - _knotVector[k]);
    B[0] = 1 - (*_evaluator)(t);
    switch(d)
    {
      case 3: B[3] = float(0);
      case 2: B[2] = - _evaluator->getDer2();
      case 1: B[1] = - _evaluator->getDer1();
    }
}

void CustomERBSCurve::computeBlending(int d, const GMlib::DVector<float> &B, GMlib::DVector<GMlib::Vector<float, 3> > &c0,
                                      GMlib::DVector<GMlib::Vector<float, 3> > &c1)
{
    c0 -= c1;

    GMlib::DVector<float> a(d+1);
    for( int i = 0; i < B.getDim(); i++ )
    {
        // Compute the pascal triangle numbers
        a[i] = 1;
        for( int j = i-1; j > 0; j-- )
        {
            a[j] += a[j-1];
        }
        // Compute the sample position data
        for( int j = 0; j <= i; j++ )
        {
            c1[i] += (a[j] * B(j)) * c0[i-j];
        }
    }

    this->_p = c1;
}

void CustomERBSCurve::localSimulate(double dt)
{
    //auto rotvec = GMlib::Vector<float,3>(0.25f, 0.5f, 1.0f);
    auto rotvec = GMlib::Vector<float,3>(1.0f, 1.0f, 1.0f);

    for(int i=0; i < _curves.getDim() - 1; i++)
    {
        _curves[i]->rotate(GMlib::Angle(2), rotvec);
    }
}
