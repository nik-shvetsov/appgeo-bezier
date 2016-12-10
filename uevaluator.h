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
    float evalCERBS();

    float getW(float t, int idx, int d, GMlib::DVector<float> kv);
    float getWder(int idx, int d, GMlib::DVector<float> kv);
};

#endif // UEVALUATOR_H
