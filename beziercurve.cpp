#include "beziercurve.h"
#include <QDebug>

CustomBezierCurve::CustomBezierCurve(PCurve<float,3>* c, float s, float e, float t, int d)
{
    //this->_dm = GMlib::GM_DERIVATION_EXPLICIT;
    set(c, s, e, t, d);

    _scaleDer = float(1) / (_e - _s); //global/local scaling factor for derivatives
                                        //shows that the ERBS curve is adjusted by the domain scaling factor
    _scaleCen = (_t - _s)/(_e - _s); //w // t should be scaled, affine global/local mapping

    //make control points
    GMlib::DMatrix<float> BHm;
    _uniEvaluator.evalBezier(BHm, d, _scaleCen, _scaleDer);
    BHm.invert();

    GMlib::DVector<GMlib::Vector<float, 3>> evalCurve = _c->evaluateParent(_t, d);
    _controlPoints = BHm * evalCurve;


    for (int i = 0; i < evalCurve.getDim(); i++)
    {
        _controlPoints[i] -= evalCurve(0);
    }
    this->translateParent(evalCurve(0));

    // //GMlib::DVector<GMlib::Vector<float,3>> tr = _c->evaluateParent(_t, _d);
    //_trans = evalCurve[0];
    //this->translateParent( _trans );
}

void CustomBezierCurve::set(PCurve<float,3>* curve, float s, float e, float t, int d)
{
    this->_c = curve;
    this->_s = s;
    this->_e = e;
    this->_t = t;
    //this->_d = d;
}

int CustomBezierCurve::getDegree() const
{
    return _controlPoints.getDim() - 1;
}

GMlib::DVector<GMlib::Vector<float, 3>> CustomBezierCurve::getControlPoints() const
{
    return _controlPoints;
}

void CustomBezierCurve::setControlPoints(const GMlib::DVector<GMlib::Vector<float, 3>> &controlPoints)
{
   _controlPoints = controlPoints;
}

bool CustomBezierCurve::isClosed() const
{
    return _c->isClosed();
}

float CustomBezierCurve::getStartP() //bez curve on [0,1]
{
    return float(0);
}

float CustomBezierCurve::getEndP()
{
    return float(1);
}

void CustomBezierCurve::eval(float t, int d, bool /*l*/)
{
    //this->_p.setDim(d+1);
    GMlib::DMatrix<float> BHm;
    //(BHm, d, t, float(1)/_scaleDer);
    _uniEvaluator.evalBezier(BHm, _controlPoints.getDim() - 1, t, _scaleDer);
    this->_p = BHm * this->_controlPoints;
    //this->_p[0] -= _trans;
}

void CustomBezierCurve::localSimulate(double dt)
{
    //global rotation
    this->rotate(GMlib::Angle(M_PI/4 * dt), GMlib::Vector<float,3>( 0.0f, 0.0f, 1.0f ));


    //auto rotvec = GMlib::Vector<float,3>(0.25f, 0.5f, 1.0f);
//    auto rotvec = GMlib::Vector<float,3>(1.0f, 1.0f, 1.0f);
//    rotate(GMlib::Angle(2), rotvec);
    //this->translate(GMlib::Vector<float,3>(0.2,0.0,0.0));
    //this->rotate(GMlib::Angle(M_PI/4 * dt), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ));
}


