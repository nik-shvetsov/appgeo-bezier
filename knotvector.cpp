#include "knotvector.h"
#include <QDebug>

KnotVector::KnotVector(GMlib::PCurve<float,3>* c, int n) //only for order 2, n - number local curves
{
    bool closed = c->isClosed();
    if(closed) n++;

    int order = 2; //for ERBS
    const float start = c->getParStart();
    const float end = c->getParEnd();

    float inc = (end - start) / (n - 1);

    _dv.setDim(n + order);

    for(int i = 0; i < n; i++)
    {
        _dv[i+1] = start + (i * inc);
    }

    //qDebug() << _dv[n] - _dv[n-1] << _dv[2] - _dv[1];
    float deltadv = (_dv[n] - _dv[n-1]);

    if(c->isClosed())
    {
        _dv[0] = _dv[1] - deltadv;
        _dv[n+1] = _dv[n] + deltadv; //make closed knot vec
    }
    else
    {
        _dv[0]   = _dv[1]; //duplicate knots
        _dv[n+1] = _dv[n];
    }

    std::cout << "Generated _kv " << "for closed -  " << closed << std::endl;
    std::cout << _dv << std::endl;
}

GMlib::DVector<float> KnotVector::getKnotVector()
{
    return _dv;
}
