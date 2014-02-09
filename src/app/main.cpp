#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QSettings>
#include "mainwindow.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    a.setApplicationName("FassiChip");
    a.setApplicationVersion(FassiChip_Version);
    a.setOrganizationName("Fassi");
    a.setOrganizationDomain("www.fassi.com.br");

    //a.setAttribute(Qt::AA_DontShowIconsInMenus);

    QPixmap _Pixmap(":/images/Splash.png");
    QSplashScreen *SplashScreen = new QSplashScreen(_Pixmap);
    SplashScreen->show();
    a.processEvents();

    QTranslator Translator;
    QSettings Settings(qApp->organizationName(), qApp->applicationName());
    QString Lang = Settings.value("General/Language", "").toString();

    if(Lang != "en")
    {
        if(Lang == "") Lang = QLocale::system().name();
        Translator.load(QString("fassichip_%0").arg(Lang), a.applicationDirPath() + "/languages");
        a.installTranslator(&Translator);
    }

    MainWindow *w = new MainWindow(SplashScreen);

    if(a.arguments().size() > 1) w->OpenFile(a.arguments().last(), 0);

    w->show();
    SplashScreen->finish(w);
    delete SplashScreen;

    return a.exec();
}
