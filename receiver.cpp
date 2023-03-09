#include "receiver.h"
#include <stdexcept>

Receiver::Receiver(const QString & name)
    : myFile(name)
{
    if (!myFile.open(QIODeviceBase::WriteOnly))
    {
        throw std::runtime_error("Cannot open: '" + name.toStdString() + "'");
    }
}

void Receiver::moreData(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize)
{
    // would be nice to send the data via the signal, but we do not own it
    // so we would have to copy it

    switch (dwDataType)
    {
    case NET_DVR_STREAMDATA:
    case NET_DVR_SYSHEAD:
      {
        emit dataReceived(dwBufSize);
        const char *data = reinterpret_cast<const char *>(pBuffer);
        QByteArray ba = QByteArray::fromRawData(data, dwBufSize);
        myFile.write(ba);
        break;
      }
    }
}
