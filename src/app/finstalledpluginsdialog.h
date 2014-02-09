#ifndef FINSTALLEDPLUGINSDIALOG_H
#define FINSTALLEDPLUGINSDIALOG_H

#include <QDialog>

class FPluginInterface;
class QTreeWidget;
class QTreeWidgetItem;

class FInstalledPluginsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FInstalledPluginsDialog(QList<FPluginInterface*> *_Plugins, QWidget *parent = 0);

signals:

private slots:
    void on_TreeWidgetPlugins_itemDoubleClicked(QTreeWidgetItem *_Item, int _Col);

private:
    void ShowAboutPlugin(FPluginInterface *_PluginInterface);
    bool CheckDependencies(FPluginInterface *_PluginInterface);
    QList<FPluginInterface*> *Plugins;
    QTreeWidget *TreeWidgetPlugins;

};

#endif // FINSTALLEDPLUGINSDIALOG_H
