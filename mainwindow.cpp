#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QMessageBox>
#include <QtGui>

//#include <QGst/Init>
//#include <QGst/Bus>
//#include <QGst/Pipeline>
//#include <QGst/Parse>
//#include <QGst/Message>
//#include <QGst/Utils/ApplicationSink>
//#include <QGst/Utils/ApplicationSource>

#include <iostream>
#include <QCoreApplication>
//#include <QGlib/Error>
//#include <QGlib/Connect>
//#include <QGst/Init>
//#include <QGst/Bus>
//#include <QGst/Pipeline>
//#include <QGst/Parse>
//#include <QGst/Message>
//#include <QGst/Utils/ApplicationSink>
//#include <QGst/Utils/ApplicationSource>

#include <QtGStreamer/QGlib/Error>
//#include <QtGStreamer/QGlib/Connect>

#include <QtGStreamer/QGst/Bus>
#include <QtGStreamer/QGst/Pipeline>
#include <QtGStreamer/QGst/Parse>
#include <QtGStreamer/QGst/Message>
#include <QtGStreamer/QGst/Utils/ApplicationSink>
#include <QtGStreamer/QGst/Utils/ApplicationSource>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_StreamQuality_valueChanged(int value)
{
std::cout << " Vol:" << value << "\n" ;
}

void MainWindow::on_buttonCut_pressed()
{
 std::cout << " Cut button Pressed"  << "\n" ;
}

void MainWindow::on_buttonFade_pressed()
{
     std::cout << " Fade button Pressed"  << "\n" ;
}



void MainWindow::on_buttonFade_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.exec();
}


void MainWindow::on_actionAbout_changed()
{
     std::cout << " about button Pressed"  << "\n" ;
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.exec();
}
void MainWindow::displayAbout()
{
  QMessageBox::about(this, tr("About this demo"),
    tr("This is a nice demo with some crazy HTML content."));
}
