#include "playframe.h"

#include "hk_play.h"

#include <QEvent>

PlayFrame::PlayFrame()
{

}

void PlayFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if (myPlay)
    {
        myPlay->resize();
    }
}

void PlayFrame::setPlay(const std::shared_ptr<HK_Play> & play)
{
    myPlay = play;
}

bool PlayFrame::event(QEvent *event)
{
    if (event->type() == QEvent::WinIdChange)
    {
        // I know of no way to tell NET_DVR that the window id has changed
        myPlay.reset();
    }
    return QFrame::event(event);
}
