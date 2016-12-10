#ifndef KNOTVECTOR_H
#define KNOTVECTOR_H

#include <gmParametricsModule>

class KnotVector
{
    public:
    //KnotVector(GMlib::PCurve<float,3>* c, float start, float end, int n);//order
    KnotVector(GMlib::PCurve<float,3>* c, int n);
    ~KnotVector();

    GMlib::DVector<float> getKnotVector();

private:
    GMlib::DVector<float> _dv;

}; // END class


#endif // KNOTVECTOR_H


