#include "finstalledpluginsdialog.h"
#include "fplugininterface.h"

#include <QTreeWidget>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QListWidget>

FInstalledPluginsDialog::FInstalledPluginsDialog(QList<FPluginInterface*> *_Plugins, QWidget *parent) :
    QDialog(parent)
{
    Plugins = _Plugins;

    setWindowTitle(tr("Installed plugins"));

    TreeWidgetPlugins = new QTreeWidget(this);
    TreeWidgetPlugins->setColumnCount(5);
    TreeWidgetPlugins->setIndentation(2);
    TreeWidgetPlugins->setFocusPolicy(Qt::NoFocus);
    TreeWidgetPlugins->setAlternatingRowColors(true);
    TreeWidgetPlugins->setHeaderLabels(QStringList() << tr("State") << tr("Name") << tr("Version")
                                       << tr("Author") << tr("File"));
    connect(TreeWidgetPlugins, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(on_TreeWidgetPlugins_itemDoubleClicked(QTreeWidgetItem*,int)));

    foreach(FPluginInterface* _Plugin, *Plugins)
    {
        QTreeWidgetItem *TreeWidgetItemPlugin = new QTreeWidgetItem(TreeWidgetPlugins);

        if(CheckDependencies(_Plugin)) TreeWidgetItemPlugin->setIcon(0, QIcon(":/images/Ok_16.png"));
        else TreeWidgetItemPlugin->setIcon(0, QIcon(":/images/Warning_16.png"));

        TreeWidgetItemPlugin->setText(1, _Plugin->Name());
        TreeWidgetItemPlugin->setText(2, _Plugin->Version());
        TreeWidgetItemPlugin->setText(3, _Plugin->Author());
        TreeWidgetItemPlugin->setText(4, _Plugin->FileName());
    }


    TreeWidgetPlugins->resizeColumnToContents(0);
    TreeWidgetPlugins->resizeColumnToContents(1);
    TreeWidgetPlugins->resizeColumnToContents(2);
    TreeWidgetPlugins->resizeColumnToContents(3);
    TreeWidgetPlugins->resizeColumnToContents(4);

    QVBoxLayout *VerLayout = new QVBoxLayout(this);
    VerLayout->setContentsMargins(0,0,0,0);
    VerLayout->setSpacing(0);
    VerLayout->addWidget(TreeWidgetPlugins);
    setLayout(VerLayout);

    //this->resize(TreeWidgetPlugins->sizeHint()*1.5);
    this->resize(640, 480);
}

void FInstalledPluginsDialog::on_TreeWidgetPlugins_itemDoubleClicked(QTreeWidgetItem *_Item, int)
{

    foreach(FPluginInterface* _Plugin, *Plugins)
    {

        if(_Plugin->Name() == _Item->text(1)) { ShowAboutPlugin(_Plugin); break; }

    }

}

void FInstalledPluginsDialog::ShowAboutPlugin(FPluginInterface *_PluginInterface)
{
    QDialog *AboutPlugin = new QDialog(this);
    AboutPlugin->setWindowTitle(tr("Plugin"));

    QLabel *LabName = new QLabel(tr("Name: %0").arg(_PluginInterface->Name()), AboutPlugin);
    QLabel *LabVersion = new QLabel(tr("Version: %0").arg(_PluginInterface->Version()), AboutPlugin);
    QLabel *LabAuthor = new QLabel(tr("Author: %0").arg(_PluginInterface->Author()), AboutPlugin);
    QLabel *LabAuthorContact = new QLabel(tr("Contact: <a href=\"mailto:%0\">%0</a>")
                                          .arg(_PluginInterface->AuthorContact()), AboutPlugin);
    LabAuthorContact->setOpenExternalLinks(true);
    //LabAuthorContact->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

    QLabel *LabDescription = new QLabel(tr("Description:"), AboutPlugin);
    QTextEdit *TEdtDescription = new QTextEdit(_PluginInterface->Description(), AboutPlugin);
    TEdtDescription->setReadOnly(true);

    QLabel *LabLicense = new QLabel(tr("License:"), AboutPlugin);
    QTextEdit *TEdtLicense = new QTextEdit(_PluginInterface->License(), AboutPlugin);
    TEdtLicense->setReadOnly(true);

    QLabel *LabDependencies = new QLabel(tr("Dependencies:"), AboutPlugin);
    QListWidget *LstWdtDependencies = new QListWidget(AboutPlugin);

    // Verivica as dependências do plugin.
    foreach(QString _Dependence, _PluginInterface->Dependencies())
    {
        QListWidgetItem *_Item = new QListWidgetItem;
        _Item->setText(_Dependence);
        _Item->setBackgroundColor(Qt::red);

        foreach(FPluginInterface* _Plugin, *Plugins)
        {
            if(_Plugin->Name() == _Dependence)
            {
                _Item->setBackgroundColor(Qt::green);
                break;
            }
        }
        LstWdtDependencies->addItem(_Item);
    }


    QVBoxLayout *VerLayout = new QVBoxLayout(AboutPlugin);
    VerLayout->addWidget(LabName);
    VerLayout->addWidget(LabVersion);
    VerLayout->addWidget(LabAuthor);
    VerLayout->addWidget(LabAuthorContact);
    VerLayout->addWidget(LabDescription);
    VerLayout->addWidget(TEdtDescription);
    VerLayout->addWidget(LabLicense);
    VerLayout->addWidget(TEdtLicense);
    VerLayout->addWidget(LabDependencies);
    VerLayout->addWidget(LstWdtDependencies);

    AboutPlugin->setLayout(VerLayout);

    AboutPlugin->exec();
    delete AboutPlugin;
}

bool FInstalledPluginsDialog::CheckDependencies(FPluginInterface *_PluginInterface)
{
    foreach(QString _Dependence, _PluginInterface->Dependencies())
    {
        bool _DepOk = false;

        foreach(FPluginInterface* _Plugin, *Plugins)
        {
            if(_Plugin->Name() == _Dependence) { _DepOk = true; break; }
        }
        if(!_DepOk) return false;
    }
    return true;
}
