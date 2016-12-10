#ifndef SCENARIO_H
#define SCENARIO_H

#include "application/gmlibwrapper.h"

// qt
#include <QObject>

using namespace GMlib;
class Color;

class CustomERBS;
class CustomBezierCurve;
class SubMyCurve;
class KnotVector;

class Scenario : public GMlibWrapper {
  Q_OBJECT
public:
  using GMlibWrapper::GMlibWrapper;

  void    initializeScenario() override;
  void    cleanupScenario() override;

  void    simulatingReplot();

private:
  std::vector<float> _knotVec;
  std::vector<Color*> _colorsVec;

  std::vector<SubMyCurve*> _subPartsCurve;
  std::vector<CustomBezierCurve*> _bezierPartsCurve;
  std::vector<CustomERBS*> _cerbs;
};

#endif // SCENARIO_H
