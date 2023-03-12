#pragma once

#include "HCNetSDK.h"

class HK_Error;
class QWidget;
class QDateTime;

void showError(QWidget *parent, const HK_Error & error);

NET_DVR_TIME_V50 qDateTime2NetDVR50(const QDateTime & datetime);
NET_DVR_TIME qDateTime2NetDVR(const QDateTime & datetime);

QDateTime netDVR2QDateTime(const NET_DVR_TIME & dvr);
