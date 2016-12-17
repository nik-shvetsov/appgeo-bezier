#ifndef UEVALUATOR_H
#define UEVALUATOR_H

#include <gmParametricsModule>
#include <gmCoreModule>

class UEvaluator
{
public:
    UEvaluator();
    ~UEvaluator();

    void evalBezier(GMlib::DMatrix<float> &BHm, int d, float t, float scale);
    void evalBSpline(GMlib::DMatrix<float> &BHm, float t, int idx, GMlib::DVector<float> kv, float scale);
//    void evalCERBS(GMlib::DVector<float> &p, GMlib::DVector<float> kv, float t, int d,
//                   GMlib::DVector<GMlib::Vector<float,3>> c1,
//                   GMlib::DVector<GMlib::Vector<float,3>> c2
//                   );

    float getW(float t, int idx, int d, GMlib::DVector<float> kv);
    float getWder(int idx, int d, GMlib::DVector<float> kv);
};

#endif // UEVALUATOR_H
