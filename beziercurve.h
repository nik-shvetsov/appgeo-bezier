#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <gmParametricsModule>

class CustomBezierCurve : public GMlib::PCurve<float,3>
{
    GM_SCENEOBJECT(CustomBezierCurve)

public:
    CustomBezierCurve(const GMlib::DVector<GMlib::Vector<float,3>> &controlPoints, float s, float t, float e);
    CustomBezierCurve(const CustomBezierCurve& copy);
    ~CustomBezierCurve(){}

    int getDegree() const;
    void createprepMatrix(GMlib::DMatrix<float> &m, int d, float t, float delta);

    GMlib::DVector<GMlib::Vector<float, 3>> getControlPoints() const;
    void setControlPoints(const GMlib::DVector<GMlib::Vector<float, 3>> &controlPoints);

    bool isClosed() const;
    void setClosed(bool closed);

protected:
    GMlib::DVector<GMlib::Vector<float, 3>> _controlPoints;
    GMlib::DVector<GMlib::DMatrix<float>> _t;
    float _scale = 1;
    bool _preEval = false;
    bool _closed = false;

    void eval(float t, int d = 0, bool l = false);
    float getStartP();
    float getEndP();
};
#endif // BEZIERCURVE_H
