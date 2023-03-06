#ifndef DOWNLOADFRAME_H
#define DOWNLOADFRAME_H

#include <QWidget>

namespace Ui {
class DownloadFrame;
}

class DownloadFrame : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadFrame(QWidget *parent = nullptr);
    ~DownloadFrame();

private:
    Ui::DownloadFrame *ui;
};

#endif // DOWNLOADFRAME_H
