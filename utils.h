#pragma once

#include "HCNetSDK.h"

class HK_Error;
class QWidget;
class QDateTime;

void showError(QWidget *parent, const HK_Error & error);

NET_DVR_TIME qDateTime2NetDVR(const QDateTime & datetime);
NET_DVR_TIME_V50 qDateTime2NetDVR50(const QDateTime & datetime);
NET_DVR_TIME_EX qDateTime2NetDVREX(const QDateTime & datetime);
NET_DVR_TIME_SEARCH_COND qDateTime2NetDVRSearchCond(const QDateTime & datetime);

QDateTime netDVR2QDateTime(const NET_DVR_TIME & dvr);
QDateTime netDVR2QDateTime(const NET_DVR_TIME_SEARCH & dvr);