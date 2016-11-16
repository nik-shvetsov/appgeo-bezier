#include "beziercurve.h"
#include <QDebug>

CustomBezierCurve::CustomBezierCurve(const GMlib::DVector<GMlib::Vector<float, 3>> &cp, float s, float t, float e)
{
    this->_dm = GMlib::GM_DERIVATION_EXPLICIT;
    _scale = 1.0/(e-s);

    GMlib::DMatrix<float> BHm;
    createBHMatrix(BHm, cp.getDim() - 1, (t-s)/(e-s), _scale);
    BHm.invert();
    _controlPoints = BHm * cp;

    for( int i = 0; i < cp.getDim(); i++)
    {
        _controlPoints[i] -= cp(0);
    }
    this->translateParent(cp(0));
}

CustomBezierCurve::CustomBezierCurve(PCurve<float,3>* curve, float s, float t, float e, int d)
{
    this->_dm = GMlib::GM_DERIVATION_EXPLICIT;

    set(curve, s, t, e, d);

    _scale = 1.0/(_e - _s);
    GMlib::DMatrix<float> m;
    createBHMatrix(m, d, (_t - _s)/(_e - _s), _scale);
    m.invert();
    GMlib::DVector<GMlib::Vector<float, 3>> v = _c->evaluateParent(_t, _d);
    _controlPoints = m * v;


    GMlib::DVector<GMlib::Vector<float,3>> tr = _c->evaluateParent(_t, _d);
    _trans = tr[0];
    this->translateParent( _trans );
}

//CustomBezierCurve::CustomBezierCurve(const CustomBezierCurve &copy)
//{
//    _controlPoints = copy._controlPoints;
//}

void CustomBezierCurve::set(PCurve<float,3>* curve, float s, float e, float t, int d)
{
    this->_c = curve;
    this->_s = s;
    this->_e = e;
    this->_t = t;
    this->_d = d;
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
    return _closed;
}

void CustomBezierCurve::setClosed(bool closed)
{
    _closed = closed;
}

float CustomBezierCurve::getStartP() //bez curve on [0,1]
{
    return 0.0;
}

float CustomBezierCurve::getEndP()
{
    return 1.0;
}

void CustomBezierCurve::eval(float t, int d, bool /*l*/)
{
    this->_p.setDim(d+1);
    GMlib::DMatrix<float> BHm;
    createBHMatrix(BHm, d, t, _scale );

    this->_p = BHm * this->_controlPoints;

    this->_p[0] -= _trans;
}

void CustomBezierCurve::createBHMatrix(GMlib::DMatrix<float> &BHm, int d, float t, float scale)
{
    if (d < 1)
    {
        BHm.setDim(1, 1);
        BHm[0][0] = 1;
        return;
    }
    else
    {
        BHm.setDim(d + 1, d + 1);
    }

    //Initialize with zeros
    for (int i = 0; i < BHm.getDim1(); i++)
    {
        for (int j = 0; j < BHm.getDim2(); j++)
        {
            BHm[i][j] = 0;
        }
    }

    //First step, bhp polynomials up to degree d

    BHm[d-1][0] = 1 - t;
    BHm[d-1][1] = t;

    for(int i = d - 2; i >= 0; i--)
    {
        BHm[i][0] = BHm[i+1][0] * (1-t);
        for(int j = 1; j < d-i; j++)
        {
            BHm[i][j] = t * BHm[i+1][j-1] + (1-t) * BHm[i+1][j];
        }
        BHm[i][d-i] = t * BHm[i+1][d-i-1];
    }

    //Verify if values are correct for first step; rows summing to one.
    for (int i = 0; i < d; i++)
    {
        float sum = 0;
        for (int j = 0; j <= d; j++) sum += BHm[i][j];
        if (abs(sum) - 1 > 1e-5)
        {
            std::cout << "Step one: Row " << i << " mismatch: " << sum << std::endl;
        }
        //else std::cout << "Row " << i << " ... OK" << std::endl;
    }

    //Next step, compute all existing derivatives
    BHm[d][0] = -scale;
    BHm[d][1] = scale;

    for (int k = 2; k <= d; k++)
    {
        const float s = k * scale;
        for (int i = d; i > (d - k); i--)
        {
            BHm[i][k] = s * BHm[i][k-1];
            for (int j = k -1; j > 0; j--)
            {
                BHm[i][j] = s * (BHm[i][j-1] - BHm[i][j]);
            }
            BHm[i][0] *= -s ;
        }
    }

    //Check if values are correct for second step; rows summing to 0.
    for (int i = 1; i <= d; i++)
    {
        float sum = 0;
        for (int j = 0; j <= d; j++) sum += BHm[i][j];
        if (abs(sum) > 1e-5 ) std::cout << "Step two: Row " << i << " mismatch: " << sum << std::endl;
        //else std::cout << "Row " << i << " ... OK" << std::endl;
    }
}


