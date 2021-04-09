#include "mainwindow.h"

#include <QApplication>
#include <QProcessEnvironment>
#include <iostream>

#include "hk_sdk.h"
#include "hk_dvr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    try
    {
        const std::shared_ptr<const HK_SDK> sdk = std::make_shared<const HK_SDK>();
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        const std::shared_ptr<HK_DVR> dvr = std::make_shared<HK_DVR>(sdk, env.value("HK_HOST").toStdString(), 8000, env.value("HK_USERNAME").toStdString(),
                                                                     env.value("HK_PASSWORD").toStdString());
        MainWindow w(nullptr, dvr);
        w.show();
        return a.exec();
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}
