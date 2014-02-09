#ifndef FCONFIGDIALOG_H
#define FCONFIGDIALOG_H

#include <QDialog>

class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QRadioButton;
class QStackedLayout;
class FPluginInterface;

class FConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FConfigDialog(QList<FPluginInterface*> *_Plugins, QWidget *parent = 0);

    QComboBox *CmbBoxStyle;
    QComboBox *CmbBoxLanguages;

private slots:
    void on_CmbBoxLanguages_CurrentIndexChanged(QString);

private:
    void CreateGeralPage();
    QStackedLayout *StackedLayout;
    QListWidget *ListWidget;
    QDialogButtonBox *ButtonBox;
    QWidget *WdtGeneralPage;

};

#endif // FCONFIGDIALOG_H
