#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class HK_DVR;
class HK_Error;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr);
    ~MainWindow();

private slots:
    void on_actionPlay_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionAbout_triggered();

private:
    const std::shared_ptr<HK_DVR> myDVR;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
