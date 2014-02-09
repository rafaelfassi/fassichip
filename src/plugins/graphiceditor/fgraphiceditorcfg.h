#ifndef FGRAPHICEDITORCFG_H
#define FGRAPHICEDITORCFG_H

#include <QWidget>

class QCheckBox;

class FGraphicEditorCfg : public QWidget
{
    Q_OBJECT

public:
    FGraphicEditorCfg(QString _Key, QWidget *parent = 0);  
    void WriteSettings();

private:
    QString Key;
    QCheckBox *ChkViewXScale;
    QCheckBox *ChkViewYScale;

};

#endif // FGRAPHICEDITORCFG_H
