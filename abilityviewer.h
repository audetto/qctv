#pragma once

#include <QWidget>
#include "HCNetSDK.h"

namespace Ui {
class AbilityViewer;
}

class HK_DVR;

class AbilityViewer : public QWidget
{
    Q_OBJECT

public:
    explicit AbilityViewer(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr);
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
