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
    virtual void moreData(const LONG lPlayHandle, const DWORD dwDataType, const BYTE *pBuffer, const DWORD dwBufSize) override;
signals:
    void dataReceived(const size_t data);
private:
    QFile myFile;
};
