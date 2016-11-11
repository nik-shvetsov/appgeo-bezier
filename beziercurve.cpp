#include "beziercurve.h"
#include <QDebug>

CustomBezierCurve::CustomBezierCurve(const GMlib::DVector<GMlib::Vector<float, 3>> &controlPoints, float s, float t, float e)
{
    GMlib::DMatrix<float> bernsitenHPoly;
    createprepMatrix(bernsitenHPoly, controlPoints.getDim() - 1, (t-s)/(e-s), 1.0/(e-s));
    bernsitenHPoly.invert();
    _controlPoints = bernsitenHPoly * controlPoints;

    for( int i = 0; i < controlPoints.getDim(); i++)
    {
        _controlPoints[i] -= controlPoints(0);
        //qDebug() << _controlPoints[i][0] << _controlPoints[i][1] << _controlPoints[i][2];
    }
    translateParent( controlPoints(0));
}

CustomBezierCurve::CustomBezierCurve(const CustomBezierCurve &copy)
{
    _controlPoints = copy._controlPoints;
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

void CustomBezierCurve::eval(float t, int d, bool l)
{
    GMlib::DMatrix<float> bernsitenHPoly;
    createprepMatrix(bernsitenHPoly, getDegree(), t, _scale );

    this->_p = bernsitenHPoly * _controlPoints;
}

float CustomBezierCurve::getStartP()
{
    return 0;
}

float CustomBezierCurve::getEndP()
{
    return 1;
}

void CustomBezierCurve::createprepMatrix(GMlib::DMatrix<float> &m, int d, float t, float delta)
{
    if (d < 1)
    {
        d = 1; //Restrict intput to setDim
    }

    m.setDim(d + 1, d + 1);

    for (int i = 0; i < m.getDim1(); i++)
    {
        for (int j = 0; j < m.getDim2(); j++)
        {
            m[i][j] = 0;
        }
    }

    if (d < 1)
    {
        m[0][0] = 1;
        return;
    }

    //First step, bhp polynomials

    m[d-1][0] = 1 - t;
    m[d-1][1] = t;

    for(int i = d - 2; i >= 0; i--)
    {
        m[i][0] = m[i+1][0]*(1-t);
        for(int j = 1; j < d-i; j++)
        {
            m[i][j] = t * m[i+1][j-1] + (1-t) * m[i+1][j];
        }
        m[i][d-i] = t * m[i+1][d-i-1];
    }

    //Verify if values are correct for first step; rows summing to one.
    for (int i = 0; i < d; i++)
    {
        float sum = 0;
        for (int j = 0; j <= d; j++) sum += m[i][j];
        if (abs(sum) - 1 > 1e-5)
        {
            std::cout << "Step one: Row " << i << " mismatch: " << sum << std::endl;
        }
        //else std::cout << "Row " << i << " ... OK" << std::endl;
    }

    //Next step, derivatives
    m[d][0] = -delta;
    m[d][1] = delta;

    for (int k = 2; k <= d; k++)
    {
        float s = k * delta;
        for (int i = d; i > (d - k); i--)
        {
            m[i][k] = s * m[i][k-1];
            for (int j = k -1; j > 0; j--)
                m[i][j] = s * (m[i][j-1] - m[i][j]);
            m[i][0] *= -s ;
        }
    }

    //Verify if values are correct for second step; rows summing to 0.
    for (int i = 1; i <= d; i++)
    {
        float sum = 0;
        for (int j = 0; j <= d; j++) sum += m[i][j];
        if (abs(sum) > 1e-5 ) std::cout << "Step two: Row " << i << " mismatch: " << sum << std::endl;
        //else std::cout << "Row " << i << " ... OK" << std::endl;
    }
}


