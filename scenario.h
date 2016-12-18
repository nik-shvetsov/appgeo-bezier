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
class Controller;

class Scenario : public GMlibWrapper {
  Q_OBJECT
public:
  using GMlibWrapper::GMlibWrapper;

  void    initializeScenario() override;
  void    cleanupScenario() override;

  void    simulatingReplot();

private:
  Controller* _controller;

};

#endif // SCENARIO_H
