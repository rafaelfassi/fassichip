#include "fgraphiceditorcfg.h"

#include <QCheckBox>
#include <QLayout>
#include <QSettings>
#include <QApplication>

FGraphicEditorCfg::FGraphicEditorCfg(QString _Key, QWidget *parent)
    : QWidget(parent)
{
    Key = _Key;

    ChkViewXScale = new QCheckBox(tr("Show X scale"), this);
    ChkViewYScale = new QCheckBox(tr("Show Y scale"), this);

    QVBoxLayout *VLayout = new QVBoxLayout(this);
    VLayout->addWidget(ChkViewXScale);
    VLayout->addWidget(ChkViewYScale);
    setLayout(VLayout);


    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup(Key);
        ChkViewXScale->setChecked(_Settings.value("ViewXScale", true).toBool());
        ChkViewYScale->setChecked(_Settings.value("ViewYScale", false).toBool());
    _Settings.endGroup();

}

void FGraphicEditorCfg::WriteSettings()
{

    QSettings _Settings(qApp->organizationName(), qApp->applicationName());
    _Settings.beginGroup(Key);
        _Settings.setValue("ViewXScale", ChkViewXScale->isChecked());
        _Settings.setValue("ViewYScale", ChkViewYScale->isChecked());
    _Settings.endGroup();

}
