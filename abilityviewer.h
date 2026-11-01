#pragma once

#include "HCNetSDK.h"

#include "utils.h"
#include <QWidget>

namespace Ui
{
class AbilityViewer;
}

class HK_DVR;

class AbilityViewer : public QWidget
{
    Q_OBJECT

public:
    explicit AbilityViewer(QWidget *parent, const std::shared_ptr<HK_DVR> &dvr, const std::vector<Camera> &cameras);
    ~AbilityViewer();

private slots:
    void on_ability_currentIndexChanged(int index);

    void on_channel_currentIndexChanged(int index);

private:
    Ui::AbilityViewer *ui;

    const std::shared_ptr<HK_DVR> myDVR;

    void updateAbilityData();
    void retrieveAbility(DWORD ability, size_t channel);
};
