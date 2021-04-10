#ifndef PLAYFRAME_H
#define PLAYFRAME_H

#include <QFrame>
#include <QDateTime>

namespace Ui {
class PlayFrame;
}

class HK_Play;

class PlayFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlayFrame(QWidget *parent = nullptr);
    ~PlayFrame();

    void setPlay(const std::shared_ptr<HK_Play> & play);
    void resizeEvent(QResizeEvent *event) override;

    WId getWindowHandle() const;

protected:
    bool event(QEvent *event) override;
    void timerEvent(QTimerEvent *);

private slots:
    void on_snapshot_clicked();

    void on_record_clicked();

    void on_stop_clicked();

private:
    Ui::PlayFrame *ui;
    std::shared_ptr<HK_Play> myPlay;

    int myTimer;
    QDateTime myStartTime;
};

#endif // PLAYFRAME_H
