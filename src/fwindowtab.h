#ifndef FWINDOWTAB_H
#define FWINDOWTAB_H

#include <QWidget>

//class FData;
//class FDataMap3d;
class FDataPackage;

//enum {NO_DATA_TYPE, FDATA_TYPE, FDATAMAP3D_TYPE};

class FWindowTab : public QWidget
{
    Q_OBJECT

public:
    explicit FWindowTab(QWidget *parent = 0);
    virtual ~FWindowTab() {}

    void SetTabLabel(QString _TabLabel) { TabLabel = _TabLabel; }
    QString GetTabLabel() { return TabLabel; }

    //virtual int GetDataType()=0;
    virtual void RefreshData() {}
    virtual void UpdateData() {}
    virtual void UpdateSettings() {}
    virtual void SetData(FDataPackage *_DataPackage) { Q_UNUSED(_DataPackage) }
    virtual FDataPackage* GetData(){ return 0; }

signals:

public slots:

private:
    QString TabLabel;

};

#endif // FWINDOWTAB_H
