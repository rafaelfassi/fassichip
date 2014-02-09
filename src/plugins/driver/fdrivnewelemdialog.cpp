#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QLayout>
#include <QMap>


#include "fdrivnewelemdialog.h"

FDrivNewElemDialog::FDrivNewElemDialog(const QMap<QString, QIcon> &_Options, QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("New Element"));

    LabName = new QLabel(tr("Name") ,this);

    EdtName = new  QLineEdit(this);

    QGroupBox *GroupBox = new QGroupBox(tr("Element type"), this);

    QVBoxLayout *VLayGroup = new QVBoxLayout;

    for (int i = 0; i < _Options.size(); ++i)
    {
        LstRadBot << new QRadioButton(_Options.keys().at(i), GroupBox);
        LstRadBot.last()->setIcon(_Options.values().at(i));
        LstRadBot.last()->setFocusPolicy(Qt::NoFocus);
        VLayGroup->addWidget(LstRadBot.last());
    }

    LstRadBot[0]->setChecked(true);

    GroupBox->setLayout(VLayGroup);

    ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *VLayMain = new QVBoxLayout;
    VLayMain->addWidget(LabName);
    VLayMain->addWidget(EdtName);
    VLayMain->addWidget(GroupBox);
    VLayMain->addWidget(ButtonBox);

    this->setLayout(VLayMain);
}

QString FDrivNewElemDialog::GetName()
{
    return EdtName->text();
}

QString FDrivNewElemDialog::GetCheckedItemName()
{
    foreach(QRadioButton *RadioButton, LstRadBot)
    {
        if(RadioButton->isChecked()) return RadioButton->text();
    }
    return "";
}

int FDrivNewElemDialog::GetCheckedItemId()
{
    for(int i=0; i<LstRadBot.size(); i++)
    {
        if(LstRadBot[i]->isChecked()) return i;
    }
    return 0;
}
