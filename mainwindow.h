#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QtGStreamer/QGst/Pipeline>
#include <QtGStreamer/QGst/Message>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void displayAbout();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:
    void on_StreamQuality_valueChanged(int value);
    void on_actionAbout_triggered();

    void on_buttonCut_clicked();

    void on_buttonCut_pressed();

    void on_buttonFade_pressed();



    void on_buttonFade_clicked();

    void on_actionAbout_changed();

private:
    Ui::MainWindow *ui;
};


class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    void setVideoSink(const QGst::ElementPtr & sink);
public Q_SLOTS:
    void play();
    void stop();
    void open();
private:
    void openFile(const QString & fileName);
    void setUri(const QString & uri);
    void onBusMessage(const QGst::MessagePtr & message);
    QGst::PipelinePtr m_pipeline;
    QGst::ElementPtr m_videoSink;
    QString m_baseDir;
};
#endif // MAINWINDOW_H
