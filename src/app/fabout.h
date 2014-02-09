#ifndef FABOUT_H
#define FABOUT_H

#include <QDialog>

class QTabWidget;
class QDialogButtonBox;

class FAbout : public QDialog
{
    Q_OBJECT

public:
    FAbout(QWidget *parent = 0);
    void CreateInfo();

private:
    void AddTab(QString _Name, QString _Text, bool _IsHtml = true);
    QTabWidget *TabWidget;
    QDialogButtonBox *ButtonBox;
    QString AboutFilesPath;
};

#endif // FABOUT_H
