#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_StreamQuality_valueChanged(int value);
    void on_actionAbout_triggered();

    void on_buttonCut_clicked();

    void on_buttonCut_pressed();

    void on_buttonFade_pressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
