#include <ntk/ntk.h>
#include <ntk/camera/opencv_grabber.h>
#include <ntk/camera/nite_rgbd_grabber.h>

#include <QApplication>
#include <QMetaType>
#include <QtOpenGL/QGLWidget>

#include "GLWidget.h"
#include "GuiController.h"

using namespace ntk;
using namespace cv;

namespace opt
{
    ntk::arg<bool> high_resolution("--highres", "Low resolution color image.", 0);
}

int main (int argc, char** argv)
{
    arg_base::set_help_option("-h");
    arg_parse(argc, argv);
    ntk_debug_level = 1;
    cv::setBreakOnError(true);

    QApplication::setGraphicsSystem("raster");
    QApplication app (argc, argv);

    ntk::RGBDProcessor* processor = new NiteProcessor();

    QDir prev = QDir::current();
    QDir::setCurrent(QApplication::applicationDirPath());
    NiteRGBDGrabber* grabber = new NiteRGBDGrabber();
    if (opt::high_resolution()) {
        grabber->setHighRgbResolution(true);
    }
    grabber->initialize();
    QDir::setCurrent(prev.absolutePath());

    GuiController gui_controller (*grabber, *processor);
    grabber->addEventListener(&gui_controller);

    grabber->start();

    GLWidget window;
    window.resize(800,600);
    window.show();

    app.exec();
}
