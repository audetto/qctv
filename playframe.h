#ifndef PLAYFRAME_H
#define PLAYFRAME_H

#include <QFrame>

class HK_Play;

class PlayFrame : public QFrame
{
public:
    PlayFrame();

    void setPlay(const std::shared_ptr<HK_Play> & play);
    void resizeEvent(QResizeEvent *event) override;

protected:
    bool event(QEvent *event) override;

private:
    std::shared_ptr<HK_Play> myPlay;
};

#endif // PLAYFRAME_H
