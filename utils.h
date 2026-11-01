#pragma once

#include "HCNetSDK.h"
#include <QString>
#include <vector>

class HK_DVR;
class HK_Error;
class QComboBox;
class QWidget;
class QDateTime;

struct Camera
{
    size_t channel;
    QString name;
};

std::vector<Camera> getOnlineCameras(const HK_DVR &dvr);

void fillCameraComboBox(QComboBox *comboBox, const std::vector<Camera> &cameras);

void showError(QWidget *parent, const HK_Error &error);

NET_DVR_TIME qDateTime2NetDVR(const QDateTime &datetime);
NET_DVR_TIME_V50 qDateTime2NetDVR50(const QDateTime &datetime);
NET_DVR_TIME_EX qDateTime2NetDVREX(const QDateTime &datetime);
NET_DVR_TIME_SEARCH_COND qDateTime2NetDVRSearchCond(const QDateTime &datetime);

QDateTime netDVR2QDateTime(const NET_DVR_TIME &dvr);
QDateTime netDVR2QDateTime(const NET_DVR_TIME_SEARCH &dvr);