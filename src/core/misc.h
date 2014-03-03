#ifndef MISC_H
#define MISC_H

//#include <QString>
//#include <QValidator>
//#include <QMessageBox>
#include <QtWidgets>
#include <math.h>


QString DoubleToHexS(double _Value, int _Dig = 0);
qint64 HexToInt(QString HexValue);
qint64 ApproximateInt(double _Number);

class CRC32
{
private:
    unsigned long           crc_accum;
    static unsigned long    crc_table[ 256 ];
    static bool             crc_initialized;

public:
    CRC32(unsigned long initialValue = (unsigned long) - 1);

    void            update(unsigned char *buffer, int bufferLen);
    unsigned long   result();
};

class FHexValidator : public QValidator
{
    //Q_OBJECT
public:
    explicit FHexValidator(QObject *parent = 0, qint64 _Min = 0, qint64 _Max = 0, bool _Signal = false);
    void setSignal(bool _Signal) { Signal = _Signal; }
    void setRange(qint64 _Min, qint64 _Max) { Min = _Min; Max = _Max; }
    qint64 getMax() { return Max; }
    qint64 getMin() { return Min; }
    QValidator::State validate(QString &input, int &pos) const;
private:
    QRegExpValidator *validator;
    qint64 Min;
    qint64 Max;
    bool Signal;

};

class FHexSpinBox : public QWidget
{
    Q_OBJECT

public:
    FHexSpinBox(QWidget *parent = 0);
    void SetRange(qint64 _Min, qint64 _Max);
    void SetValue(qint64 _Value);
    qint64 GetValue() { return Value; }
    void SetSingleStep(int _SingleStep) { SingleStep = _SingleStep; }

private slots:
    void on_EdtDisplayEditingFinished();
    void on_BtnClicked();

signals:
    void ValueChanged(qint64 _Value);
    void EditingFinished();

private:
    qint64 Value;
    qint64 Min;
    qint64 Max;
    int SingleStep;
    QLineEdit EdtDisplay;
    QPushButton BtnDec;
    QPushButton BtnInc;
    QHBoxLayout HLayout;
    FHexValidator HexValidator;
};

#endif // MISC_H
