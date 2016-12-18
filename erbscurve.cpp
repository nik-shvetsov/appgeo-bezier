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
    if (_curve->isClosed()) _n++; //numofLocalCurves

    //createSubcurves(_curve);
    createLocalCurves(_curve, _deg);
}

CustomERBS::CustomERBS(PCurve<float,3>* c, int n, int deg)
{
    this->_type = LOCAL_CURVE_TYPE::BEZIER_CURVE;
    this->_curve = c;

    this->_n = n;
    this->_s = c->getParStart();
    this->_e = c->getParEnd();
    this->_deg = deg;

    _kv = generateKnotVector();
    if (_curve->isClosed()) _n++; //numofLocalCurves

    //createBezierCurves(_curve, _deg);
    createLocalCurves(_curve, _deg);
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
        return w;                                                         //affine global/local mapping
    }
    //return t; //need to return something for compiler
}

void CustomERBS::createLocalCurves(PCurve<float,3>* c, int d)
{
    _localCurves.setDim(_n);

    for(int i = 1; i < _n; i++)
    {
        switch (_type)
        {
        case SUB_CURVE:
            _localCurves[i-1] = new SubMyCurve(c, _kv[i-1], _kv[i+1], _kv[i]);
            break;
        case BEZIER_CURVE:
            _localCurves[i-1] = new CustomBezierCurve(c, _kv[i-1], _kv[i+1], _kv[i], d);
            break;
        default:
            break;
        }
    }

    if (_curve->isClosed())
    {
        _localCurves[_n-1] = _localCurves[0];
    }
    else
    {
        switch (_type)
        {
        case SUB_CURVE:
            _localCurves[_n-1] = new SubMyCurve(c, _kv[_n-1], _kv[_n+1], _kv[_n]);
            break;
        case BEZIER_CURVE:
            _localCurves[_n-1] = new CustomBezierCurve(c, _kv[_n-1], _kv[_n+1], _kv[_n], d);
            break;
        default:
            break;
        }
    }

    visualizeLocalCurves(); //and prepare _localCurves
}

void CustomERBS::visualizeLocalCurves()
{
    for (int i = 0; i < _localCurves.getDim() - 1; i++)
    {
        _localCurves[i]->toggleDefaultVisualizer();
        _localCurves[i]->replot(50);
        _localCurves[i]->setColor(GMlib::GMcolor::GreenYellow);
        _localCurves[i]->setVisible(false); //true
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

    //c(t) = sum(ci(t)*Bi(t))
    //qDebug() << "t" << t;
    int index = findIndex(t);
    //qDebug() << "index of t" << index;

    _scaleWt = (t -_kv[index])/(_kv[index+1] - _kv[index]); //t scaling
    _scaleB = 1.0 / (_kv[index+1] - _kv[index]); //b scaling
                                                //shows that the ERBS curve is adjusted by the domain scaling factor
    GMlib::DVector<float> BFunction = makeBFunction(_scaleWt,_scaleB, d);

    GMlib::DVector<GMlib::Vector<float,3>> c1 = _localCurves[index-1]->evaluateParent(getT(t, index), d);
    GMlib::DVector<GMlib::Vector<float,3>> c2 = _localCurves[index]->evaluateParent(getT(t, index+1), d);

    auto gt = c2 - c1;

    this->_p[0] = c1[0]+ BFunction[0]*(gt[0]);

    if (d > 0)
        this->_p[1] = c1[1] + BFunction[0]*gt[1] + BFunction[1]*gt[0];

    if (d > 1)
        this->_p[2] = c1[2] + BFunction[0]*gt[2] + 2*BFunction[1]*gt[1] + BFunction[2]*gt[0];

    if (d > 2)
        this->_p[3] = c1[3] + BFunction[0]*gt[3] + 3*BFunction[1]*gt[2] + 3*BFunction[2]*gt[1] + BFunction[3]*gt[0];

    if (d > 3)
        this->_p[4] = c1[4] + BFunction[0]*gt[4] + 4*BFunction[1]*gt[3] + 6*BFunction[2]*gt[2] + 4*BFunction[3]*gt[1] + BFunction[4]*gt[0];
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

GMlib::DVector<float> CustomERBS::makeBFunction(float t, float scale, float d)
{
    GMlib::DVector<float> B;
    B.setDim(d + 1);
//test function
//    B[0] = 3 * pow(t,2) - 2 * pow(t,3);
//    if (d>0)
//        B[1] = (6 * t - 6 * pow(t,2)) * scale;
//    if (d>1)
//        B[2] = (6 - 12 * t) * scale * scale;
//    if (d>2)
//        B[3] = -12 * scale * scale * scale;

    B[0] = pow(sin(M_PI_2 * t), 2);
    if (d>0)
        B[1] = (0.5 * /*pow(M_PI,1) * */ sin(M_PI * t));
    if (d>1)
        B[2] = (0.5 * /*pow(M_PI,2) * */ cos(M_PI * t));
    if (d>2)
        B[3] = (-0.5 * /*pow(M_PI,3)* */ sin(M_PI * t));
    if (d>3)
        B[4] = (-0.5 * /*pow(M_PI,4)* */ cos(M_PI * t));

    for (int i = 1; i <= d; i++)
    {
        B[i] *= pow(M_PI,i); //optimizing

        B[i] *= pow(scale, i); //derivatives scaling
    }

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
    //global translation and rotation
    _accum += (_flag ? dt : -dt);
    if(_accum >= 1.0 || _accum <= -1.0)
    {
        _flag = !_flag;
    }

    for(int i = 0; i < _localCurves.getDim()-1; i++)
    {
        float trVec = 0.1 * std::sin(_accum);
        float rotAngle = std::sin(_accum * 0.1);
        _localCurves[i]->translateGlobal(GMlib::Vector<float,3> (0.0, 0.0, trVec));
        _localCurves[i]->rotate(GMlib::Angle(rotAngle), GMlib::Vector<float,3>(0.0, 0.0, 1.0));
    }

//    switch (_type)
//    {
//    case SUB_CURVE:
//        break;
//    case BEZIER_CURVE:
//        break;
//    }

    //sin translation of even curves
    _sum += dt*10;
    for(int i=0; i < _localCurves.getDim() - 1; i++)
    {
        if (i%2 == 0)
        {
            _localCurves[i]->translate(GMlib::Vector<float,3>(0.0,0.0,std::sin(_sum)*0.05));
        }
    }

    //scaling
    _cossum += dt;
    float sinfunc = std::cos(_cossum);
    //std::cout << _sum << std::endl;
    if (sinfunc >= 0) _mult = 0.01;
    else _mult = -0.01;

    for(int i = 0; i < _localCurves.getDim()-1; i++)
    {
        auto pos = _localCurves[i]->getPos();
        _localCurves[i]->translateGlobal( _mult * pos);
    }
}
