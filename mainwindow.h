#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class HK_DVR;
class HK_Play;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr);
    ~MainWindow();

private slots:
    void on_actionPlay_triggered();

private:
    void show_error(const char * function);

    const std::shared_ptr<HK_DVR> myDVR;
    Ui::MainWindow *ui;
    std::shared_ptr<HK_Play> myPlay;
};
#endif // MAINWINDOW_H
