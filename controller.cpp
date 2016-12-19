#include "controller.h"

Controller::Controller()
{
    this->setSurroundingSphere(GMlib::Sphere<float,3>(100.0));

    std::vector<GMlib::Color> colorsVec = {GMlib::GMcolor::Blue,GMlib::GMcolor::Red,GMlib::GMcolor::Green,GMlib::GMcolor::Yellow,GMlib::GMcolor::Orange,GMlib::GMcolor::Aqua,
                                           GMlib::GMcolor::AliceBlue,GMlib::GMcolor::AquaMarine,GMlib::GMcolor::Beige,GMlib::GMcolor::BlueViolet,GMlib::GMcolor::BurlyWood,
                                           GMlib::GMcolor::Coral};
    _colorsVec = colorsVec;

    //Original curve for evaluation
    auto mycurve = new MyCurve(2);
    mycurve->toggleDefaultVisualizer();
    //mycurve->insertVisualizer(curve_visualizer);
    mycurve->replot(100,2);
    mycurve->setColor(GMlib::GMcolor::Black);
    _myCurves.push_back(mycurve);
    this->insert(mycurve);

    //ERBS curve
    auto erbs = new CustomERBS(mycurve, 12, 4); //local Bezier curves constructor
    //auto erbs = new CustomERBS(mycurve, 12); //local subcurves constructor
    erbs->toggleDefaultVisualizer();
    //erbs->insertVisualizer(curve_visualizer);
    erbs->replot(500,2);
    erbs->setColor(GMlib::GMcolor::Cyan);
    erbs->translate(GMlib::Vector<float,3>(0,0,3));
    _cerbs.push_back(erbs);
    this->insert(erbs);

    //Old stuff for checks
    //  int parts = 12;
    //  auto kv = new KnotVector(mycurve, parts); //0, M_2PI
    //  GMlib::DVector<float> num_kv = kv->getKnotVector();

    //subcurves
    //  std::vector<SubMyCurve*> _subPartsCurve;
    //  for (int i = 0; i < parts; i++) //(int i = 0; i < parts; i++)
    //  {
    //      //auto submycurve = new SubMyCurve(mycurve,i*M_2PI/parts,(i+1)*M_2PI/parts);
    //      auto submycurve = new SubMyCurve(mycurve,num_kv[i+1],num_kv[i+2]);
    //      //qDebug() << num_kv[i] << num_kv[i+1] << num_kv[i+2];
    //      submycurve->toggleDefaultVisualizer();
    //      //mycurve->insertVisualizer(curve_visualizer);
    //      submycurve->replot(100,2);
    //      submycurve->setColor(colorsVec[i]);
    //      submycurve->translate(GMlib::Vector<float,3>(0,0,1));
    //      _subPartsCurve.push_back(submycurve);
    //      this->insert(submycurve);
    //  }

    //bezier local curves
    //  for (int i = 0; i < parts; i++) //(int i = 0; i < parts; i++)
    //  {
    //      auto bezmycurve = new CustomBezierCurve(mycurve,num_kv[i+1],num_kv[i+3],num_kv[i+2], 2);
    //      //qDebug() << num_kv[i] << num_kv[i+1] << num_kv[i+2];
    //      bezmycurve->toggleDefaultVisualizer();
    //      //bezmycurve->insertVisualizer(curve_visualizer);
    //      bezmycurve->replot(100,2);
    //      bezmycurve->setColor(colorsVec[i]);
    //      bezmycurve->translate(GMlib::Vector<float,3>(0,0,2));
    //      _bezierPartsCurve.push_back(bezmycurve);
    //      this->insert(bezmycurve);
    //  }
}

Controller::~Controller(){}

void Controller::replotERBS()
{
    for(int i = 0; i < _cerbs.size(); i++)
    {
           _cerbs[i]->replot();
    }
}

void Controller::toggleLC()
{
    for(int i = 0; i < _cerbs.size(); i++)
    {
        _cerbs[i]->toggleVisLocalCurves();
    }
}

void Controller::localSimulate(double dt)
{
    //this->rotate(GMlib::Angle(M_PI/4*dt), GMlib::Vector<float,3>(0.0,0.0,1.0));
}
