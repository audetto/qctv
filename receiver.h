#pragma once

#include "sdk/hk_callback.h"
#include <QObject>
#include <QString>
#include <QFile>

class Receiver : public QObject, public HK_Callback_V40
{
    Q_OBJECT
public:
    Receiver(const QString & name);
    virtual void moreData(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize) override;
signals:
    void dataReceived(size_t data);
private:
    QFile myFile;
};
