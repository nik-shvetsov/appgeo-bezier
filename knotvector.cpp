#include "knotvector.h"

KnotVector::KnotVector(GMlib::PCurve<float,3>* c, float start, float end, int n)
{
    bool isClosed = c->isClosed();

    float currentKnotValue = start;
    float increment = (end - start) / (n - 1);

    if(isClosed)
    {
        _dv.append(start - increment);
    }

    else
    {
        _dv.append(start);
    }
    /*
    while (currentKnotValue < end)
    {
        _dv.append(currentKnotValue);
        currentKnotValue += increment;
    }
    */
    for (int i = 0; i < n; i++)
    {
        _dv.append(currentKnotValue);
        currentKnotValue += increment;
    }

    if(isClosed)
    {
        _dv.append(end + increment);
    }
    else
    {
        _dv.append(end);
    }
}

GMlib::DVector<float> KnotVector::getKnotVector()
{
    return _dv;
}
