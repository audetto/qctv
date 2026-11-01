#include "abilityviewer.h"
#include "sdk/hk_dvr.h"
#include "sdk/hk_error.h"
#include "ui_abilityviewer.h"
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

AbilityViewer::AbilityViewer(QWidget *parent, const std::shared_ptr<HK_DVR> &dvr, const std::vector<Camera> &cameras)
    : QWidget(parent)
    , ui(new Ui::AbilityViewer)
    , myDVR(dvr)
{
    ui->setupUi(this);
    fillCameraComboBox(ui->channel, cameras);
}

AbilityViewer::~AbilityViewer()
{
    delete ui;
}

void AbilityViewer::retrieveAbility(DWORD ability, size_t channel)
{
    std::vector<char> out(1024 * 100);
    std::vector<char> in;
    QFile file;
    switch (ability)
    {
    case DEVICE_SOFTHARDWARE_ABILITY:
    case DEVICE_NETWORK_ABILITY:
    case DEVICE_RAID_ABILITY:
    {
        break;
    }
    case DEVICE_DYNCHAN_ABILITY:
    {
        file.setFileName(":/resources/dynchan_ability_input.xml");
        break;
    }
    case DEVICE_USER_ABILITY:
    {
        file.setFileName(":/resources/user_ability_input.xml");
        break;
    }
    case DEVICE_VIDEOPIC_ABILITY:
    {
        file.setFileName(":/resources/videopic_ability_input.xml");
        break;
    }
    case DEVICE_JPEG_CAP_ABILITY:
    {
        file.setFileName(":/resources/jpeg_cap_ability_input.xml");
        break;
    }
    case STREAM_ABILITY:
    {
        file.setFileName(":/resources/stream_ability_input.xml");
        break;
    }
    }

    if (!file.fileName().isEmpty() && file.open(QFile::ReadOnly))
    {
        // this is "copy central"
        QTextStream text(&file);
        const LONG digital = myDVR->getDigitalChannel(channel);
        const QString content = text.readAll().replace("${CHANNEL}", QString::number(digital));
        const std::string str = content.toStdString();
        in.assign(str.begin(), str.end());
    }

    myDVR->getDeviceAbility(ability, in, out);
    const QByteArray reply(out.data(), qstrnlen(out.data(), out.size()));

    QXmlStreamReader reader(reply);

    QString formatted;
    QXmlStreamWriter writer(&formatted);

    writer.setAutoFormatting(true);

    while (!reader.atEnd())
    {
        reader.readNext();
        if (!reader.isWhitespace())
        {
            writer.writeCurrentToken(reader);
        }
    }

    if (reader.hasError())
    {
        ui->text->setPlainText("Error parsing XML: " + reader.errorString());
    }
    else
    {
        ui->text->setPlainText(formatted);
    }
}

void AbilityViewer::updateAbilityData()
{
    const std::vector<DWORD> abilities = {DEVICE_SOFTHARDWARE_ABILITY, DEVICE_NETWORK_ABILITY, DEVICE_RAID_ABILITY,
                                          DEVICE_DYNCHAN_ABILITY,      DEVICE_USER_ABILITY,    DEVICE_VIDEOPIC_ABILITY,
                                          DEVICE_JPEG_CAP_ABILITY,     STREAM_ABILITY};

    const int abilityIndex = ui->ability->currentIndex();
    const int channelIndex = ui->channel->currentIndex();

    if (abilityIndex >= 0 && abilityIndex < static_cast<int>(abilities.size()) && channelIndex >= 0)
    {
        const DWORD ability = abilities[abilityIndex];
        const size_t channel = ui->channel->currentData().toULongLong();
        try
        {
            retrieveAbility(ability, channel);
        }
        catch (const HK_Error &error)
        {
            ui->text->setPlainText(error.what());
        }
    }
}

void AbilityViewer::on_channel_currentIndexChanged(int /* index */)
{
    updateAbilityData();
}

void AbilityViewer::on_ability_currentIndexChanged(int /* index */)
{
    updateAbilityData();
}
