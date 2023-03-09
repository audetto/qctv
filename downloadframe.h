#pragma once

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class DownloadFrame;
}

class HK_Playback;
class HK_DVR;

class DownloadFrame : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadFrame(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr);
    ~DownloadFrame();

protected:
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void addData(size_t size);

    void on_browse_clicked();

private:
    Ui::DownloadFrame *ui;

    const std::shared_ptr<HK_DVR> myDVR;

    std::shared_ptr<HK_Playback> myPlayback;

    int myTimer;
    size_t mySize;

    QFileDialog mySaveFileDialog;

    void resetDownload();
};
