#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <gmParametricsModule>

class CustomBezierCurve : public GMlib::PCurve<float,3>
{
    GM_SCENEOBJECT(CustomBezierCurve)

public:
    CustomBezierCurve(const GMlib::DVector<GMlib::Vector<float,3>> &controlPoints, float s, float t, float e);

    CustomBezierCurve(PCurve<float,3>* curve, float s, float t, float e, int d);
    //CustomBezierCurve(const CustomBezierCurve& copy);
    ~CustomBezierCurve(){}


    int getDegree() const;
    void createBHMatrix(GMlib::DMatrix<float> &BHm, int d, float t, float scale);

    GMlib::DVector<GMlib::Vector<float, 3>> getControlPoints() const;
    void setControlPoints(const GMlib::DVector<GMlib::Vector<float, 3>> &controlPoints);

    bool isClosed() const;
    void setClosed(bool closed);

protected:
    GMlib::DVector<GMlib::Vector<float, 3>> _controlPoints;
    //GMlib::DVector<GMlib::DMatrix<float>> _t;

    PCurve<float,3>* _c;
    float _s;
    float _e;
    //float _t;
    GMlib::Vector<float,3>     _trans;

    float _scale = 1;
    //bool _preEval = false;
    bool _closed = false;

    void eval(float t, int d = 0, bool l = false);
    float getStartP();
    float getEndP();

private:
    void set(PCurve<float,3>* curve, float s, float t, float e, int d);
};
#endif // BEZIERCURVE_H
