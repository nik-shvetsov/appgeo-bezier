#include "erbscurve.h"
#include "submycurve.h"
#include "beziercurve.h"
#include "knotvector.h"
#include <QDebug>

CustomERBS::CustomERBS(PCurve<float,3> *c, int n)
{
    this->_type = LOCAL_CURVE_TYPE::SUB_CURVE;
    this->_curve = c;

    this->_n = n;
    this->_s = c->getParStart();
    this->_e = c->getParEnd();

    _kv = generateKnotVector();
    if (_curve->isClosed()) _n++;
    createSubcurves(_curve);
}

CustomERBS::CustomERBS(PCurve<float,3>* c, int n, int d)
{
    this->_type = LOCAL_CURVE_TYPE::BEZIER_CURVE;
    this->_curve = c;

    this->_n = n;
    this->_s = c->getParStart();
    this->_e = c->getParEnd();
    //this->_d = d;

    _kv = generateKnotVector();
    if (_curve->isClosed()) _n++;
    createBezierCurves(_curve, d);
}

CustomERBS::~CustomERBS() {}

float CustomERBS::getT(float t, int index)
{
    switch(_type)
    {
    case SUB_CURVE:
        return t;
    case BEZIER_CURVE:
        float w = (t - _kv[index - 1])/(_kv[index + 1] - _kv[index - 1]); //omega for bezier
        return w;
    }
    //return t; //need to return something for compiler
}

void CustomERBS::createSubcurves(PCurve<float,3> *c)
{
    _localCurves.setDim(_n);

    for(int i = 1; i < _n; i++)
    {
        _localCurves[i-1] = new SubMyCurve(c, _kv[i-1], _kv[i+1], _kv[i]);
    }

    if (_curve->isClosed())
    {
        _localCurves[_n-1] = _localCurves[0];
    }
    else
    {
        _localCurves[_n-1] = new SubMyCurve(c, _kv[_n-1], _kv[_n+1], _kv[_n]);
    }

    visualizeLocalCurves();
}

void CustomERBS::createBezierCurves(PCurve<float,3>* c, int d)
{
    _localCurves.setDim(_n);

    for(int i = 1; i < _n; i++)
    {
        _localCurves[i-1] = new CustomBezierCurve(c, _kv[i-1], _kv[i+1], _kv[i], d);
    }

    if (_curve->isClosed())
    {
        _localCurves[_n-1] = _localCurves[0];
    }
    else
    {
        _localCurves[_n-1] = new CustomBezierCurve(c, _kv[_n-1], _kv[_n+1], _kv[_n], d);
    }

    visualizeLocalCurves();

}

void CustomERBS::visualizeLocalCurves()
{
    for (int i = 0; i < _localCurves.getDim() - 1; i++)
    {
        _localCurves[i]->toggleDefaultVisualizer();
        _localCurves[i]->replot(50,2);
        _localCurves[i]->setColor(GMlib::GMcolor::GreenYellow);
        _localCurves[i]->setVisible(false);
        this->insert(_localCurves[i]);
    }
}

GMlib::DVector<float> CustomERBS::generateKnotVector()
{
    auto kv = new KnotVector(_curve, _n);
    return kv->getKnotVector();
}


void CustomERBS::eval(float t, int d, bool)
{
    //number of derivatives to compute
    this->_p.setDim(d+1);

    //c(t) = sum ci(t)*Bi(t)

    //qDebug() << "t" << t;
    int index = findIndex(t);
    //qDebug() << "index of t" << index;

    float w = (t -_kv[index])/(_kv[index+1] - _kv[index]);

    _scale = 1.0 / (_kv[index+1] - _kv[index]);
    GMlib::DVector<float> BFunction = makeBFunction(w, d, _scale);

    GMlib::DVector<GMlib::Vector<float,3>> c1 = _localCurves[index-1]->evaluateParent(getT(t, index), d);
    GMlib::DVector<GMlib::Vector<float,3>> c2 = _localCurves[index]->evaluateParent(getT(t, index+1), d);

    auto gt = c2 - c1;

    this->_p[0] = c1[0]+ BFunction[0]*(gt[0]);

    if (d > 0)
        this->_p[1] = c1[1] + BFunction[0]*gt[1] + BFunction[1]*gt[0];
    if (d > 1)
        this->_p[2] = c1[2] + BFunction[0]*gt[2] + 2*BFunction[1]*gt[1] + BFunction[2]*gt[0];
//    if (d > 2)
//        this->_p[3] = c1[3];
//    if (d > 3)
//        this->_p[4] = c1[4];
}


float CustomERBS::getStartP()
{
    return _kv[1];
}

float CustomERBS::getEndP()
{
    return _kv[_kv.getDim()-2];
}

bool CustomERBS::isClosed()
{
    return _curve->isClosed();
}

GMlib::DVector<float> CustomERBS::makeBFunction(float t, float d, float scale)
{
    GMlib::DVector<float> B;
    B.setDim(d+1);
//    B[0] = 3*pow(t,2) - 2*pow(t,3);
//    if (d>0)
//        B[1] = (6*t - 6*pow(t,2))*scale;
//    if (d>1)
//        B[2] = (6 - 12*t)*scale*scale;
//    if (d>2)
//        B[3] = -12*scale*scale*scale;

    B[0] = pow(sin(M_PI_2 * t),2);
    if (d>0)
        B[1] = (0.5 * M_PI * sin(M_PI * t))*scale;
    if (d>1)
        B[2] = (0.5 * M_PI * M_PI * cos(M_PI * t))*scale*scale;
    if (d>2)
        B[3] = (-0.5 * M_PI * M_PI * M_PI * sin(M_PI * t) )*scale*scale*scale;

    return B;
}

int CustomERBS::findIndex(float t)
{
    int index = 1;

    for (int i = 1; i < _kv.getDim() - 2; ++i)
    {
        if (t >= _kv[i] && t < _kv[i+1])
        {
            index = i;
            return index;
        }
    }

    return _kv.getDim() - 3; // if (t == _kv[i+1])

}

void CustomERBS::localSimulate(double dt)
{
    auto rotvec = GMlib::Vector<float,3>(1.0f, 1.0f, 1.0f);
    for(int i=0; i < _localCurves.getDim() - 1; i++)
    {
        _localCurves[i]->rotate(GMlib::Angle(2), rotvec);
    }

}
