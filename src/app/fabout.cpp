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

#include "fabout.h"

#include <QTextBrowser>
#include <QTabWidget>
#include <QLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFile>
#include <QPushButton>
#include <QApplication>

FAbout::FAbout(QWidget *parent)
    : QDialog(parent)
{

    setWindowTitle(tr("About"));
    setWindowIcon(QIcon(":/images/About_16.png"));
    resize(600, 400);

    AboutFilesPath = qApp->applicationDirPath() + "//doc//";

    QLabel *LabAppIco = new QLabel(this);
    LabAppIco->setPixmap(QIcon(":/images/Chip1.png").pixmap(64, 64));

    QLabel *LabAppName = new QLabel("FassiChip - " + qApp->applicationVersion(), this);
    QFont _Font = LabAppName->font();
    _Font.setPointSize(_Font.pointSize() + 4);
    _Font.setBold(true);
    LabAppName->setFont(_Font);
    //LabAppName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    LabAppName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QPushButton *BtnAboutQt = new QPushButton(this);
    BtnAboutQt->setFlat(true);
    BtnAboutQt->setIcon(QIcon(":/images/Qt.png").pixmap(32, 32));
    BtnAboutQt->setFocusPolicy(Qt::NoFocus);
    connect(BtnAboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));

    QHBoxLayout* HLayTitle = new QHBoxLayout;
    HLayTitle->addWidget(LabAppIco);
    HLayTitle->addWidget(LabAppName);
    HLayTitle->addWidget(BtnAboutQt);

    TabWidget = new QTabWidget(this);
    CreateInfo();

    ButtonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(ButtonBox, SIGNAL(rejected()), this, SLOT(close()));

    QVBoxLayout *VLayMain = new QVBoxLayout;
    VLayMain->addLayout(HLayTitle);
    VLayMain->addWidget(TabWidget);
    VLayMain->addWidget(ButtonBox);

    this->setLayout(VLayMain);
}

void FAbout::AddTab(QString _Name, QString _Text, bool _IsHtml)
{
    QPalette _Pal = palette();
    _Pal.setColor(QPalette::Base, this->palette().button().color());

    QTextBrowser *TextBrowser = new QTextBrowser(this);
    TextBrowser->setPalette(_Pal);
    TextBrowser->setOpenLinks(true);
    TextBrowser->setOpenExternalLinks(true);
    if(_IsHtml)TextBrowser->setHtml(_Text);
    else TextBrowser->setText(_Text);

    TabWidget->addTab(TextBrowser, _Name);
}

void FAbout::CreateInfo()
{
    // About ------------------------------------------------------------------------------------------------
    AddTab(tr("About"),
            "<br>"
            "<b>" + tr("Advanced binary file editor") + "</b><br>" +
            tr("The FassiChip has a main focus on reverse engineering of embedded devices, "
            "especially fuel injection ECUs. However, it is a flexible software and can "
            "be used for various other purposes.") +
            "<br><br>"
            "<div align=\"center\">Copyright &copy; 2010-2011 Rafael Fassi Lobão<br><br>"
            "<a href=\"http://www.fassi.com.br\">www.fassi.com.br</a><br>"
            "</div>"
            );


    // Authors ----------------------------------------------------------------------------------------------
    AddTab(tr("Authors"),
           "<b>Rafael Fassi Lobão</b><br>"
           "<a href=\"mailto:rflobao@gmail.com\">rflobao@gmail.com</a><br>"
           );


    // Contributors -----------------------------------------------------------------------------------------
    QFile ContributorsFile(AboutFilesPath + tr("Contributors.html"));

    if (ContributorsFile.open(QFile::ReadOnly | QFile::Text))
    {
        AddTab(tr("Contributors"), ContributorsFile.readAll(), true);
        ContributorsFile.close();
    }


    // Thanks -----------------------------------------------------------------------------------------------
    QFile ThanksFile(AboutFilesPath + tr("Thanks.html"));

    if (ThanksFile.open(QFile::ReadOnly | QFile::Text))
    {
        AddTab(tr("Thanks"), ThanksFile.readAll(), true);
        ThanksFile.close();
    }


    // Other credits ----------------------------------------------------------------------------------------
    QFile OtherCreditsFile(AboutFilesPath + tr("OtherCredits.html"));

    if (OtherCreditsFile.open(QFile::ReadOnly | QFile::Text))
    {
        AddTab(tr("Other Credits"), OtherCreditsFile.readAll(), true);
        OtherCreditsFile.close();
    }

    // Help this project ------------------------------------------------------------------------------------
    QFile HelpThisProjectFile(AboutFilesPath + tr("HelpThisProject.html"));

    if (HelpThisProjectFile.open(QFile::ReadOnly | QFile::Text))
    {
        AddTab(tr("Help This Project"), HelpThisProjectFile.readAll(), true);
        HelpThisProjectFile.close();
    }


    // License ----------------------------------------------------------------------------------------------
    AddTab(tr("License"),
           "Copyright © 2010-2011 Rafael Fassi Lobão\n\n" +
           tr(
           "FassiChip is free software: you can redistribute it and/or modify it under the terms of "
           "the GNU General Public License as published by the Free Software Foundation, either version "
           "3 of the License, or (at your option) any later version."
           "\n\n"
           "FassiChip is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without "
           "even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU "
           "General Public License for more details."
           "\n\n"
           "You should have received a copy of the GNU General Public License along with FassiChip. If not, "
           "see <http://www.gnu.org/licenses/>."
           ), false);
}
