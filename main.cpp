#include "mainwindow.h"

#include <QApplication>
#include <QProcessEnvironment>
#include <QCommandLineParser>

#include <iostream>

#include "hk_sdk.h"
#include "hk_dvr.h"
#include "hk_error.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("AndSoft");
    QApplication::setApplicationName("HK Camera Viewer");

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    QCommandLineParser parser;
    parser.setApplicationDescription(QApplication::applicationName());
    parser.addHelpOption();

    const QCommandLineOption logOption("log", "Prints log to the console.");
    parser.addOption(logOption);

    const QCommandLineOption hostnameOption("host", "Defaults to HK_HOST.", "hostname", env.value("HK_HOST"));
    parser.addOption(hostnameOption);

    const QCommandLineOption portOption("port", "Defaults to 8000.", "port", QString::number(8000));
    parser.addOption(portOption);

    const QCommandLineOption usernameOption("user", "Defaults to HK_USERNAME.", "username", env.value("HK_USERNAME"));
    parser.addOption(usernameOption);

    const QCommandLineOption passwordOption("pwd", "Defaults to HK_PASSWORD.", "password", env.value("HK_PASSWORD"));
    parser.addOption(passwordOption);

    parser.process(a);

    try
    {
        const std::shared_ptr<const HK_SDK> sdk = std::make_shared<const HK_SDK>(parser.isSet(logOption));
        const std::shared_ptr<HK_DVR> dvr = std::make_shared<HK_DVR>(sdk,
                                                                     parser.value(hostnameOption).toStdString(),
                                                                     parser.value(portOption).toInt(),
                                                                     parser.value(usernameOption).toStdString(),
                                                                     parser.value(passwordOption).toStdString());
        MainWindow w(nullptr, dvr);
        w.show();
        return a.exec();
    }
    catch (const HK_Error & e)
    {
        std::cerr << e.what() << ": " << e.getError() << " = " << e.getMessage() << std::endl;
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
}
