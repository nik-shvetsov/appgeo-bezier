#include "scenario.h"

#include "testtorus.h"
#include "mycurve.h"
#include "submycurve.h"
#include "beziercurve.h"
#include "knotvector.h"
#include "erbscurve.h"

//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>

// qt
#include <QQuickItem>
#include <QDebug>



void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::White, GMlib::GMcolor::White,
                                                     GMlib::GMcolor::White, init_light_pos );
  light->setAttenuation(0.8, 0.002, 0.0008);
  scene()->insertLight( light, false );

  // Insert Sun
  scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
  front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( front_rcpair.camera.get() );
  front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
  side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( side_rcpair.camera.get() );
  side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Top cam
  auto top_rcpair = createRCPair("Top");
  top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
  top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( top_rcpair.camera.get() );
  top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


  //auto curve_visualizer = new GMlib::PCurveVisualizer<float,3>;

  auto mycurve = new MyCurve(2);
  mycurve->toggleDefaultVisualizer();
  //mycurve->insertVisualizer(curve_visualizer);
  mycurve->replot(100,2);
  mycurve->setColor(GMlib::GMcolor::Black);
  scene()->insert(mycurve);


  std::vector<GMlib::Color> colorsVec = {GMlib::GMcolor::Blue,GMlib::GMcolor::Red,GMlib::GMcolor::Green,GMlib::GMcolor::Yellow,GMlib::GMcolor::Orange,GMlib::GMcolor::Aqua,
                                        GMlib::GMcolor::AliceBlue,GMlib::GMcolor::AquaMarine,GMlib::GMcolor::Beige,GMlib::GMcolor::BlueViolet,GMlib::GMcolor::BurlyWood,GMlib::GMcolor::Coral
                                        };
  int parts = 13; //12

  auto kv = new KnotVector(mycurve, 0, M_2PI, parts);
  GMlib::DVector<float> num_kv = kv->getKnotVector();


  int size = num_kv.getDim();
  for (int i = 0; i < size; i++)
  {
      qDebug() << num_kv[i]/M_2PI;
  }



  std::vector<SubMyCurve*> _subPartsCurve;
  for (int i = 0; i < parts-1; i++) //(int i = 0; i < parts; i++)
  {
      //auto submycurve = new SubMyCurve(mycurve,i*M_2PI/parts,(i+1)*M_2PI/parts);
      auto submycurve = new SubMyCurve(mycurve,num_kv[i+1],num_kv[i+2]);
      qDebug() << num_kv[i+1] << num_kv[i+2];
      submycurve->toggleDefaultVisualizer();
      //mycurve->insertVisualizer(curve_visualizer);
      submycurve->replot(100,2);
      submycurve->setColor(colorsVec[i]);
      submycurve->translate(GMlib::Vector<float,3>(0,0,1));
      _subPartsCurve.push_back(submycurve);
      scene()->insert(submycurve);
  }


  auto erbscurve = new CustomERBSCurve(mycurve, 12, 2);
  erbscurve->toggleDefaultVisualizer();
  erbscurve->replot(100,2);
  erbscurve->setColor(GMlib::GMcolor::Silver);
  erbscurve->translate(GMlib::Vector<float,3>(0,0,2));
  scene()->insert(erbscurve);



  //Bezier test
  /*
  GMlib::DVector<GMlib::Vector<float,3> > controlPoints;
  controlPoints.append(GMlib::Vector<float,3>(0.0, 0.0, 0.0));
  controlPoints.append(GMlib::Vector<float,3>(2.0, 0.0, 0.0));
  controlPoints.append(GMlib::Vector<float,3>(0.5, 2.0, 0.0));
  controlPoints.append(GMlib::Vector<float,3>(2.0, 2.0, 0.0));

  auto mybezcurve = new CustomBezierCurve(controlPoints, 0, 0.5, 1);
  mybezcurve->toggleDefaultVisualizer();
  //mycurve->insertVisualizer(curve_visualizer);
  mybezcurve->replot(100,2);
  mybezcurve->setColor(GMlib::GMcolor::White);
  scene()->insert(mybezcurve);
  */


  //Standart GMlib implementation
  /*
  auto mycurve = new GMlib::PCircle<float>(2);
  mycurve->toggleDefaultVisualizer();
  //mycurve->insertVisualizer(curve_visualizer);
  mycurve->replot(100,2);
  mycurve->setColor(GMlib::GMcolor::Black);
  scene()->insert(mycurve);


  auto submycurve = new GMlib::PSubCurve<float>(mycurve, 0.0f, M_2PI, (0.0f + M_2PI) / 2);
  submycurve->toggleDefaultVisualizer();
  //mycurve->insertVisualizer(curve_visualizer);
  submycurve->replot(100,2);
  submycurve->setColor(GMlib::GMcolor::Red);
  submycurve->translate(GMlib::Vector<float,3>(0,0,1));
  scene()->insert(submycurve);
  */

  /*
  // Surface visualizers
  auto surface_visualizer = new GMlib::PSurfNormalsVisualizer<float,3>;

  // Surface
  auto surface = new TestTorus;
  surface->toggleDefaultVisualizer();
  surface->insertVisualizer(surface_visualizer);
  surface->replot(200,200,1,1);
  scene()->insert(surface);

  surface->test01();
  */

}

void Scenario::cleanupScenario() {

}
