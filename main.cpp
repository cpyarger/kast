#include "mainwindow.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QtGStreamer/QGst/Ui/GraphicsVideoSurface>
#include <QtGStreamer/QGst/Init>

int main(int argc, char *argv[])
{
#if defined(QTVIDEOSINK_PATH)
    //this allows the example to run from the QtGStreamer build tree without installing QtGStreamer
    qputenv("GST_PLUGIN_PATH", QTVIDEOSINK_PATH);
#endif
    QApplication a(argc, argv);
    QGst::init(&argc, &argv);
    QDeclarativeView view;
    QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(&view);
    view.rootContext()->setContextProperty(QLatin1String("videoSurface1"), surface);
    MainWindow w;
    w.show();


    return a.exec();
}
