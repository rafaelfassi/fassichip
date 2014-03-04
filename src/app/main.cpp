/****************************************************************************
**
**Copyright (C) 2010-2014 - Rafael Fassi Lobão
**Contact: www.fassi.com.br - rafael@fassi.com.br
**
**This file is part of the Fassichip.
**Fassichip is free software; you can redistribute it and/or modify
**it under the terms of the GNU General Public License as published by
**the Free Software Foundation; either version 2 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
****************************************************************************/

#include <QApplication>
#include <QSplashScreen>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QSettings>
#include "mainwindow.h"
#include "version.h"

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

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
