#include "misc.h"

QString DoubleToHexS(double _Value, int _Dig)
{
    try
    {
        QString Str1 = QString::number((qint64)fabs(_Value), 16).toUpper();

        while(Str1.length() < _Dig) Str1 = "0" + Str1;

        if(_Value < 0) return "-" + Str1;
        else return Str1;
    }
    catch(...)
    {
        QMessageBox msgBox;
        msgBox.setText("Number is invalid!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return "";
    }
}

qint64 HexToInt(QString HexValue)
{
    bool ok;

    qint64 Val = HexValue.toLongLong(&ok, 16);

    if(!ok)
    {
        QMessageBox msgBox;
        msgBox.setText("The number hexadecimal is invalid!");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return 0;
    }

    return Val;
}

qint64 ApproximateInt(double _Number)
{
 if(_Number > 0) _Number += 0.5;
 if(_Number < 0) _Number -= 0.5;

 return (long int)_Number;
}

FHexValidator::FHexValidator(QObject *parent, qint64 _Min, qint64 _Max, bool _Signal)
    : QValidator(parent)
{
    validator = new QRegExpValidator(QRegExp("[-0-9A-Fa-f]{1,9}"), this);
    Min = _Min;
    Max = _Max;
    Signal = _Signal;
}

QValidator::State FHexValidator::validate(QString &input, int &pos) const
{
    if(input.contains('-'))
    {
        if(!Signal || input[0] != '-' || input.count('-') > 1) return Invalid;
        if(input.size() < 2) return Intermediate; // Se existe somente o sinal na string.
    }

    input = input.toUpper(); // Converte em maiúscula.

    QValidator::State ValidatorStatus = validator->validate(input, pos); // Verifica se os dígitos haxa são válidos.
    if(ValidatorStatus == Invalid) return Invalid;

    if(ValidatorStatus == Acceptable)
    {
        qint64 ValInt = HexToInt(input);
        if(ValInt < Min || ValInt > Max) return Invalid; // Verifica se o range é válido.
    }

    return ValidatorStatus;
}


FHexSpinBox::FHexSpinBox(QWidget *parent)
    : QWidget(parent)
{
    Value = 0;
    Min = 0;
    Max = 0;
    SingleStep = 1;
    EdtDisplay.setText("0");

    BtnDec.setMaximumWidth(20);
    BtnInc.setMaximumWidth(20);

    BtnDec.setText("<");
    BtnInc.setText(">");

    BtnDec.setProperty("Op", 0);
    BtnInc.setProperty("Op", 1);

    BtnDec.setAutoRepeat(true);
    BtnInc.setAutoRepeat(true);

    HLayout.setMargin(0);
    HLayout.setSpacing(0);
    HLayout.addWidget(&EdtDisplay);
    HLayout.addWidget(&BtnDec);
    HLayout.addWidget(&BtnInc);

    EdtDisplay.setValidator(&HexValidator);

    this->setLayout(&HLayout);

    connect(&EdtDisplay, SIGNAL(editingFinished()), this, SLOT(on_EdtDisplayEditingFinished()));
    connect(&BtnDec, SIGNAL(clicked()), this, SLOT(on_BtnClicked()));
    connect(&BtnInc, SIGNAL(clicked()), this, SLOT(on_BtnClicked()));
}

void FHexSpinBox::SetRange(qint64 _Min, qint64 _Max)
{
    Min = _Min;
    Max = _Max;

    if(Min < 0) HexValidator.setSignal(true);
    else HexValidator.setSignal(false);

    HexValidator.setRange(_Min, _Max);
}

void FHexSpinBox::SetValue(qint64 _Value)
{
    if(_Value < Min || _Value > Max) return;

    bool _Change = (Value != _Value);
            
    Value = _Value;

    EdtDisplay.setText(DoubleToHexS(Value, 1));

    if(_Change) emit ValueChanged(Value);
}

void FHexSpinBox::on_EdtDisplayEditingFinished()
{
    SetValue(HexToInt(EdtDisplay.text()));
    emit EditingFinished();
}

void FHexSpinBox::on_BtnClicked()
{
    QPushButton *ClickedButton = qobject_cast<QPushButton *>(sender());
    int _Operation = ClickedButton->property("Op").toInt();

    qint64 _Value = Value;

    if(_Operation) _Value += SingleStep;
    else _Value -= SingleStep;

    SetValue(_Value);
    emit EditingFinished();
}

