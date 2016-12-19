#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mycurve.h"
#include "submycurve.h"
#include "beziercurve.h"
#include "knotvector.h"
#include "erbscurve.h"

#include <gmParametricsModule>

class Controller : public GMlib::SceneObject
{
    GM_SCENEOBJECT(Controller)

public:
    Controller();
    ~Controller();

    void                            replotERBS();
    void                            toggleLC();
protected:
    void                            localSimulate(double dt) override;

private:
    std::vector<MyCurve*> _myCurves;
    std::vector<SubMyCurve*> _subPartsCurve;
    std::vector<CustomBezierCurve*> _bezierPartsCurve;
    std::vector<CustomERBS*> _cerbs;

    //std::vector<float> _knotVec;
    std::vector<GMlib::Color> _colorsVec;
};

#endif // CONTROLLER_H
