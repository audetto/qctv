#pragma once

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
    void on_actionLive_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionAbout_triggered();

    void on_actionPlayback_triggered();

    void on_actionDownload_triggered();

private:
    const std::shared_ptr<HK_DVR> myDVR;
    Ui::MainWindow *ui;

    void livePlay(const size_t channel);
    void playback(const size_t channel);

};
