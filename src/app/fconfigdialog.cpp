#include "fconfigdialog.h"
#include "fplugininterface.h"

#include <QtGui>
#include <QStyleFactory>

FConfigDialog::FConfigDialog(QList<FPluginInterface*> *_Plugins, QWidget *parent) :
    QDialog(parent)
{

    ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    ListWidget = new QListWidget;
    ListWidget->setMinimumWidth(150);
    StackedLayout = new QStackedLayout;
    connect(ListWidget, SIGNAL(currentRowChanged(int)), StackedLayout, SLOT(setCurrentIndex(int)));

    CreateGeralPage();


    foreach(FPluginInterface* _Plugin, *_Plugins)
    {
        QWidget *_ConfigWdt = _Plugin->SettingsWidget();

        if(_ConfigWdt)
        {
            ListWidget->addItem(_Plugin->Name());
            StackedLayout->addWidget(_ConfigWdt);
        }
    }


    QGridLayout *MainLayout = new QGridLayout(this);
    MainLayout->setColumnStretch(0, 1);
    MainLayout->setColumnStretch(1, 3);
    MainLayout->addWidget(ListWidget, 0, 0);
    MainLayout->addLayout(StackedLayout, 0, 1);
    MainLayout->addWidget(ButtonBox, 1, 0, 1, 2);
    setLayout(MainLayout);

    connect(ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    setWindowTitle(tr("Preferences"));
    ListWidget->setCurrentRow(0);

}


void FConfigDialog::CreateGeralPage()
{
    WdtGeneralPage = new QWidget(this);
    WdtGeneralPage->setFixedSize(400, 300);



    QSettings Settings(qApp->organizationName(), qApp->applicationName());
    QString _CurrentLang = Settings.value("General/Language", "").toString();
    CmbBoxLanguages = new QComboBox(WdtGeneralPage);
    CmbBoxLanguages->addItem(tr("System"));
    CmbBoxLanguages->addItem("en");

    QDir _LangDig(qApp->applicationDirPath() + "/languages");

    foreach (QFileInfo qmFileInfo, _LangDig.entryInfoList(QStringList("fassichip_*.qm"),
                                                          QDir::Files | QDir::Readable, QDir::Name | QDir::IgnoreCase))
    {
            CmbBoxLanguages->addItem(qmFileInfo.completeBaseName().remove("fassichip_"));
    }
    CmbBoxLanguages->setCurrentIndex(CmbBoxLanguages->findText(_CurrentLang, Qt::MatchExactly));
    if(CmbBoxLanguages->currentIndex() < 0) CmbBoxLanguages->setCurrentIndex(0);
    connect(CmbBoxLanguages, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(on_CmbBoxLanguages_CurrentIndexChanged(QString)));

    QRegExp regExp(".(.*)\\+?Style");
    QString defaultStyle = QApplication::style()->metaObject()->className();

    if (regExp.exactMatch(defaultStyle)) defaultStyle = regExp.cap(1);

    CmbBoxStyle = new QComboBox(WdtGeneralPage);
    CmbBoxStyle->addItems(QStyleFactory::keys());
    CmbBoxStyle->setCurrentIndex(CmbBoxStyle->findText(defaultStyle, Qt::MatchContains));

    QVBoxLayout *VLayout = new QVBoxLayout(this);
    VLayout->addWidget(CmbBoxLanguages);
    VLayout->addWidget(CmbBoxStyle);
    WdtGeneralPage->setLayout(VLayout);

    ListWidget->addItem(tr("General"));
    StackedLayout->addWidget(WdtGeneralPage);
}

void FConfigDialog::on_CmbBoxLanguages_CurrentIndexChanged(QString _NewLang)
{
    QMessageBox::information(this, tr("Application Language"),
                             tr("The change will take effect the next time the application is started."));
    Q_UNUSED(_NewLang)
}
