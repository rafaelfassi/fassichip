#ifndef FDRIVNEWELEMDIALOG_H
#define FDRIVNEWELEMDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QRadioButton;
class QDialogButtonBox;

class FDrivNewElemDialog : public QDialog
{
    Q_OBJECT

public:
    FDrivNewElemDialog(const QMap<QString, QIcon> &_Options, QWidget *parent = 0);

    QString GetName();
    QString GetCheckedItemName();
    int GetCheckedItemId();

private:
    QLabel *LabName;
    QLineEdit *EdtName;
    QDialogButtonBox *ButtonBox;
    QList<QRadioButton *> LstRadBot;

};

#endif // FDRIVNEWELEMDIALOG_H
