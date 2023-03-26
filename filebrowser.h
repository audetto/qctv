#pragma once

#include <QWidget>

namespace Ui {
class FileBrowser;
}

class HK_DVR;

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr);
    ~FileBrowser();

signals:
    void playbackByName(const QString & name, const QDateTime & start, const QDateTime & end);

private slots:
    void on_search_clicked();

    void on_open_clicked();

private:
    Ui::FileBrowser *ui;

    const std::shared_ptr<HK_DVR> myDVR;
};
