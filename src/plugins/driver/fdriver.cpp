/****************************************************************************
**
**Copyright (C) 2010-2014 - Rafael Fassi Lobão
**Contact: www.fassi.com.br - rafael@fassi.com.br
**
**This file is part of the Fassichip.
**Fassichip is free software; you can redistribute it and/or modify
**it under the terms of the GNU General Public License as published by
**the Free Software Foundation; either version 2 of the License, or
**(at your option) any later version.
**
**This program is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**GNU General Public License for more details.
**
****************************************************************************/

#include <QMap>

#include "fdriver.h"
#include "fdrivnewelemdialog.h"
#include "script/fscript.h"

FDriver::FDriver(QWidget *parent)
    : QWidget(parent)
{
    DriverDocument = 0;

    CreateForm();
    CreateConnections();

}

FDriver::~FDriver()
{

}

bool FDriver::OpenDriver(QString _DriverFileName)
{
    DriverFile.setFileName(_DriverFileName);

    if (!DriverFile.open(QIODevice::ReadOnly)) return false;

    if(DriverDocument)  delete DriverDocument;
    DriverDocument = new QDomDocument("FassiChip_Driver");

    if (!DriverDocument->setContent(&DriverFile))
    {
        DriverFile.close();
        return false;
    }

    DriverFile.close();

    TreeWidDrivers->setEnabled(true);
    BtnClose->setEnabled(true);
    BtnSaveAs->setEnabled(true);
    RefreshTreeDrivers();
    LabDriverOpen->setText(QFileInfo(DriverFile.fileName()).fileName());

    return true;
}

bool FDriver::CloseDriver()
{
    if(BtnSave->isEnabled())
    {
        int MsgRet = QMessageBox::warning(this, tr("Driver"),
                                        tr("Save changes to \"%0\"?")
                                        .arg(QFileInfo(DriverFile.fileName()).fileName()),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                        QMessageBox::Save);

        switch (MsgRet)
        {
           case QMessageBox::Save:
               SaveDriver("");
               break;
           case QMessageBox::Cancel:
               return false;
               break;
         }
    }

    TreeWidDrivers->clear();
    TreeWidDrivers->setEnabled(false);
    BtnSave->setEnabled(false);
    BtnSaveAs->setEnabled(false);
    BtnClose->setEnabled(false);
    LabDriverOpen->setText(tr("No driver open"));

    if(DriverDocument)
    {
        delete DriverDocument;
        DriverDocument = 0;
    }

    return true;
}

bool FDriver::SaveDriver(QString _DriverFileName)
{
    if(_DriverFileName != "")
    {
        if(QFileInfo(_DriverFileName).suffix().toLower() != "fcd") _DriverFileName += ".fcd";

        if(QFileInfo(_DriverFileName).exists())
        {
            int _Ret = QMessageBox::warning(this, tr("Driver"),
                                            tr("A file already exists with the name \"%0\".\n"
                                               "You want to overwrite the existing file?")
                                            .arg(QFileInfo(_DriverFileName).fileName()),
                                            QMessageBox::Yes | QMessageBox::No,
                                            QMessageBox::No);
            if(_Ret == QMessageBox::No) return false;
        }

        DriverFile.setFileName(_DriverFileName);
    }

    if(!DriverFile.open(QIODevice::WriteOnly)) return false;

    QTextStream out(&DriverFile);
    DriverDocument->save(out, 4);

    DriverFile.close();

    BtnSave->setEnabled(false);

    return true;
}

void FDriver::RefreshTreeDrivers()
{
    TreeWidDrivers->clear();

        // Cria os elementos sem grupo-------------------------------------------------------
        QStringList ElementsNoGrouped = XmlDriver.GetElementsNameByGroup(DriverDocument, "");

        foreach(QString _Element, ElementsNoGrouped)
        {
            QTreeWidgetItem *ItemElem = new QTreeWidgetItem(TreeWidDrivers);

            ItemElem->setText(0, _Element);
            ItemElem->setData(0, Qt::AccessibleTextRole, XmlDriver.GetTagByElementName(DriverDocument, _Element));
            ItemElem->setIcon(0, GetIconToElements(ItemElem->data(0, Qt::AccessibleTextRole).toString()));
        }

        // Cria os grupos---------------------------------------------------------------------
        QStringList Groups = XmlDriver.GetGroups(DriverDocument);

        foreach(QString _Group, Groups)
        {
           QStringList ElementsGrouped = XmlDriver.GetElementsNameByGroup(DriverDocument, _Group);
           if(ElementsGrouped.isEmpty()) continue; // Não exibe o grupo se não existirem elementos para ele.

           QTreeWidgetItem *ItemGroup = new QTreeWidgetItem(TreeWidDrivers);

            ItemGroup->setText(0, _Group);
            ItemGroup->setData(0, Qt::AccessibleTextRole, "Group");
            ItemGroup->setExpanded(true);

            // Cria os elementos para o grupo------------------------------------------------
            foreach(QString _Element, ElementsGrouped)
            {
                QTreeWidgetItem *ItemElem = new QTreeWidgetItem(ItemGroup);

                 ItemElem->setText(0, _Element);
                 ItemElem->setData(0, Qt::AccessibleTextRole, XmlDriver.GetTagByElementName(DriverDocument, _Element));
                 ItemElem->setIcon(0, GetIconToElements(ItemElem->data(0, Qt::AccessibleTextRole).toString()));
            }

        }
}

QIcon FDriver::GetIconToElements(QString _Type)
{
    if(_Type == "Map2D") return QIcon(":/driver/images/TableMap2D.png");
    else if(_Type == "Map3D") return QIcon(":/driver/images/TableMap3D.png");
    else if(_Type == "Script") return QIcon(":/driver/images/Script.png");
    else return QIcon(":/driver/images/Question_16.png");
}

void FDriver::on_BtnNew_Clicked()
{
    if(DriverDocument)
    {
        if(!CloseDriver()) return;
    }

    DriverDocument = new QDomDocument("FassiChip_Driver");
    QDomNode _XmlInstruction = DriverDocument->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

    DriverDocument->insertBefore(_XmlInstruction, DriverDocument->firstChild());

    QDomElement root = DriverDocument->createElement("Driver");
    root.setAttribute("version", "3.0");
    DriverDocument->appendChild(root);

    QString _DriverName = QFileDialog::getSaveFileName(this,
                                                       tr("Save Driver"),
                                                       QApplication::applicationDirPath() + "/drivers",
                                                       "Drivers (*.fcd)");

    bool _Open = false;

    if(!_DriverName.isEmpty())
    {
        if(SaveDriver(_DriverName)) _Open = true;
    }

    if(DriverDocument)
    {
        delete DriverDocument;
        DriverDocument = 0;
    }

    if(_Open) OpenDriver(DriverFile.fileName());

}

void FDriver::on_BtnOpen_Clicked()
{
    if(DriverDocument)
    {
        if(!CloseDriver()) return;
    }

    QString _DriverName = QFileDialog::getOpenFileName(this,
                                                       "Open driver",
                                                       QApplication::applicationDirPath() + "/drivers",
                                                       "Drivers (*.fcd)");

    if(!_DriverName.isEmpty()) OpenDriver(_DriverName);
}

void FDriver::on_BtnClose_Clicked()
{
    CloseDriver();
}

void FDriver::on_BtnSave_Clicked()
{
    SaveDriver("");
}

void FDriver::on_BtnSaveAs_Clicked()
{
    if(!DriverDocument) return;

    QString _DriverName = QFileDialog::getSaveFileName(this,
                                                       tr("Save driver as..."),
                                                       QApplication::applicationDirPath() + "/drivers",
                                                       "Drivers (*.fcd)");

    if(!_DriverName.isEmpty())
    {
        if(SaveDriver(_DriverName))
            LabDriverOpen->setText(QFileInfo(DriverFile.fileName()).fileName());
    }

}

void FDriver::on_ActTreeElementUpDown_Triggered()
{
    QAction *ToggledAct = qobject_cast<QAction *>(sender());
    bool _Sense = ToggledAct->property("Sense").toBool();

   if(XmlDriver.MoveElement(DriverDocument, TreeWidDrivers->selectedItems().first()->text(0), _Sense))
    {
       RefreshTreeDrivers();
       BtnSave->setEnabled(true);
    }
}

void FDriver::on_ActTreeElementRename_Triggered()
{
    QString _ElementName = TreeWidDrivers->selectedItems().first()->text(0);

    bool ok;
         QString _NewElementName = QInputDialog::getText(this, tr("Driver"),
                                              tr("New name:"), QLineEdit::Normal,
                                              _ElementName, &ok);

         if(ok && !_NewElementName.isEmpty() && (_NewElementName != _ElementName))
         {
             if(XmlDriver.RenameElement(DriverDocument, _ElementName, _NewElementName))
             {
                RefreshTreeDrivers();
                BtnSave->setEnabled(true);
             }
             else
             {
                 QMessageBox::warning(qobject_cast<QWidget*>(this), tr("Error"),
                                      tr("Failed to rename!\n"
                                         "Verify that there is another element with that name"));
             }
         }
}

void FDriver::on_ActTreeElementDelete_Triggered()
{
    QString _Element = TreeWidDrivers->selectedItems().first()->text(0);

    int _Ret = QMessageBox::warning(this, tr("Driver"),
                                    tr("Do you really want to delete the element: \"%0\"?")
                                    .arg(_Element),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if(_Ret == QMessageBox::No) return;


    if(XmlDriver.DeleteElement(DriverDocument, _Element))
    {
        RefreshTreeDrivers();
        BtnSave->setEnabled(true);
    }
}

void FDriver::on_ActTreeElementNew_Triggered()
{
    QMap<QString, QIcon> _Options;
    _Options.insert(tr("2D Map"), GetIconToElements("Map2D"));
    _Options.insert(tr("3D Map"), GetIconToElements("Map3D"));
    _Options.insert(tr("Script"), GetIconToElements("Script"));

    FDrivNewElemDialog *DrivNewElemDialog = new FDrivNewElemDialog(_Options, this);

    if(DrivNewElemDialog->exec())
    {
        QString _Type = "";
        QString _Group = "";
        QString _Name = "";
        /*if(!TreeWidDrivers->selectedItems().isEmpty())
        {
            QString _ElementName = TreeWidDrivers->selectedItems().first()->text(0);
            _Group = XmlDriver.GetGroupByElementName(DriverDocument, _ElementName);
        }*/

        _Name = DrivNewElemDialog->GetName();

        switch(DrivNewElemDialog->GetCheckedItemId())
        {
        case 0:
            _Type = "Map2D";
            break;
        case 1:
            _Type = "Map3D";
            break;
        case 2:
            _Type = "Script";
            break;
        }

        if(_Name != "")
        {
                if(XmlDriver.AddNewElement(DriverDocument, _Type, _Name, _Group))
                {
                    RefreshTreeDrivers();
                    BtnSave->setEnabled(true);
                    ExecElement(_Name, _Type, true);
                }
        }
    }

    delete DrivNewElemDialog;

}

void FDriver::on_ActTreeElementExecOrEdit_Triggered()
{
    QAction *_Act = qobject_cast<QAction *>(sender());
    bool _Edit = _Act->property("Edit").toBool();

    QTreeWidgetItem *_ItemElem = TreeWidDrivers->selectedItems().first();

    QString _ElementName = _ItemElem->text(0);
    QString _ElementType = _ItemElem->data(0, Qt::AccessibleTextRole).toString();

    ExecElement(_ElementName, _ElementType, _Edit);
}

void FDriver::on_ActTreeGroupUpDown_Triggered()
{
    QAction *ToggledAct = qobject_cast<QAction *>(sender());
    bool _Sense = ToggledAct->property("Sense").toBool();

   if(XmlDriver.MoveGroup(DriverDocument, TreeWidDrivers->selectedItems().first()->text(0), _Sense))
   {
        RefreshTreeDrivers();
        BtnSave->setEnabled(true);
   }
}

void FDriver::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    QString ElementType = item->data(0, Qt::AccessibleTextRole).toString();
    if(ElementType == "Group") return;

    ExecElement(item->text(0), ElementType, false);
}

void FDriver::on_ActTreeGroupRename_Triggered()
{
    QString _OldNameGroup = TreeWidDrivers->selectedItems().first()->text(0);

    bool ok;
         QString _NewNameGroup = QInputDialog::getText(this, tr("Driver"),
                                              tr("New name for the group:"), QLineEdit::Normal,
                                              _OldNameGroup, &ok);

         if(ok && !_NewNameGroup.isEmpty() && (_NewNameGroup != _OldNameGroup))
         {
             if(XmlDriver.RenameGroup(DriverDocument, _OldNameGroup, _NewNameGroup))
             {
                RefreshTreeDrivers();
                BtnSave->setEnabled(true);
             }
             else
             {
                 QMessageBox::warning(qobject_cast<QWidget*>(this), tr("Error"),
                                      tr("Failed to rename group!\n"
                                         "Verify that there is another group with that name"));
             }
         }
}

void FDriver::on_ActTreeGroupDelete_Triggered()
{
   QString GroupName = TreeWidDrivers->selectedItems().first()->text(0);

   int _Ret = QMessageBox::warning(this, tr("Driver"),
                                   tr("Do you really want to delete the group: \"%0\"?\n"
                                   "The elements belonging to the group are not deleted, but will be without the group.")
                                   .arg(GroupName),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);
   if(_Ret == QMessageBox::No) return;


   if(XmlDriver.DeleteGroup(DriverDocument, GroupName))
   {
        RefreshTreeDrivers();
        BtnSave->setEnabled(true);
   }
}

void FDriver::on_treeWidget_ContextMenuRequested(QPoint _Point)
{
    QTreeWidgetItem *_Item = TreeWidDrivers->itemAt(_Point);
    QMenu *ContextMenu = new QMenu(TreeWidDrivers);

    if(_Item)
    {
        QString ElementType = _Item->data(0, Qt::AccessibleTextRole).toString();

        if(ElementType == "Group")
        {
            ContextMenu->addAction(ActTreeGroupUp);
            ContextMenu->addAction(ActTreeGroupDown);
            ContextMenu->addSeparator();
            ContextMenu->addAction(ActTreeGroupRename);
            ContextMenu->addSeparator();
            ContextMenu->addAction( ActTreeGroupDelete);
        }
        else
        {
            ContextMenu->addAction(ActTreeElementUp);
            ContextMenu->addAction(ActTreeElementDown);
            ContextMenu->addSeparator();
            ContextMenu->addAction(ActTreeElementExec);
            ContextMenu->addAction(ActTreeElementEdit);
            ContextMenu->addSeparator();
            ContextMenu->addAction(ActTreeElementNew);
            ContextMenu->addSeparator();
            ContextMenu->addAction(ActTreeElementRename);
            ContextMenu->addSeparator();
            ContextMenu->addAction(ActTreeElementDelete);
        }
    }
    else
    {
           ContextMenu->addAction(ActTreeElementNew);
    }

    ContextMenu->exec(TreeWidDrivers->mapToGlobal(_Point));

    delete ContextMenu;

    /*QMessageBox MsgBox;
    MsgBox.setText(_Item->text(0));
    MsgBox.exec();*/
}

void FDriver::on_Element_EditingFinished()
{
    RefreshTreeDrivers();
    BtnSave->setEnabled(true);
}

void FDriver::ExecElement(QString _ElementName, QString _ElementType, bool _Edit)
{
    if(_ElementType == "Script")
    {
        // A pasta do Ui é a mesma pasta do driver.
        QDir _UiDir(QFileInfo(DriverFile.fileName()).absoluteDir());
        // Caso exista uma subpasta com o nome do driver, ela será a pasta do Ui.
        _UiDir.cd(QFileInfo(DriverFile.fileName()).baseName());

        FScript *Script = new FScript(_ElementName, Data, this);
        Script->SetDocument(DriverDocument);
        Script->SetUiPath(_UiDir);
        connect(Script, SIGNAL(ScriptReturn(FDataPackage*)), this, SIGNAL(ReturnFunctions(FDataPackage*)));

        if(_Edit)
        {
            connect(Script, SIGNAL(EditingFinished()), this, SLOT(on_Element_EditingFinished()));
            Script->Edit();
        }
        else
        {
            Script->Execute();
        }
    }
}

void FDriver::CreateForm()
{
    BtnNew = new QPushButton(this);
    BtnNew->setIconSize(QSize(16,16));
    BtnNew->setIcon(QIcon(":/driver/images/NewFile_16.png"));
    BtnNew->setToolTip(tr("Create a new driver"));
    BtnNew->setFocusPolicy(Qt::NoFocus);

    BtnOpen = new QPushButton(this);
    BtnOpen->setIconSize(QSize(16,16));
    BtnOpen->setIcon(QIcon(":/driver/images/OpenFile_16.png"));
    BtnOpen->setToolTip(tr("Open a driver"));
    BtnOpen->setFocusPolicy(Qt::NoFocus);

    BtnClose = new QPushButton(this);
    BtnClose->setIconSize(QSize(16,16));
    BtnClose->setIcon(QIcon(":/driver/images/CloseFile_16.png"));
    BtnClose->setToolTip(tr("Close driver"));
    BtnClose->setFocusPolicy(Qt::NoFocus);
    BtnClose->setEnabled(false);

    BtnSave = new QPushButton(this);
    BtnSave->setIconSize(QSize(16,16));
    BtnSave->setIcon(QIcon(":/driver/images/Save_16.png"));
    BtnSave->setToolTip(tr("Save"));
    BtnSave->setFocusPolicy(Qt::NoFocus);
    BtnSave->setEnabled(false);

    BtnSaveAs = new QPushButton(this);
    BtnSaveAs->setIconSize(QSize(16,16));
    BtnSaveAs->setIcon(QIcon(":/driver/images/SaveAs_16.png"));
    BtnSaveAs->setToolTip(tr("Save as..."));
    BtnSaveAs->setFocusPolicy(Qt::NoFocus);
    BtnSaveAs->setEnabled(false);

    BtnFindDriver = new QPushButton(this);
    BtnFindDriver->setIconSize(QSize(16,16));
    BtnFindDriver->setIcon(QIcon(":/driver/images/Find_16.png"));
    BtnFindDriver->setToolTip(tr("Find a driver compatible with the original file open"));
    BtnFindDriver->setFocusPolicy(Qt::NoFocus);

    BtnProperties = new QPushButton(this);
    BtnProperties->setIconSize(QSize(16,16));
    BtnProperties->setIcon(QIcon(":/driver/images/Properties_16.png"));
    BtnProperties->setToolTip(tr("Driver properties"));
    BtnProperties->setFocusPolicy(Qt::NoFocus);
    BtnProperties->setEnabled(false);

    QGridLayout *GridLayBtns = new QGridLayout;
    GridLayBtns->setHorizontalSpacing(1);
    GridLayBtns->addWidget(BtnNew,          0, 0);
    GridLayBtns->addWidget(BtnOpen,         0, 1);
    GridLayBtns->addWidget(BtnClose,        0, 2);
    GridLayBtns->addWidget(BtnSave,         1, 0);
    GridLayBtns->addWidget(BtnSaveAs,       1, 1);
    GridLayBtns->addWidget(BtnFindDriver,   1, 2);
    GridLayBtns->addWidget(BtnProperties,   2, 0);

    GridLayBtns->setContentsMargins(1, 1, 1, 1);


    TreeWidDrivers = new QTreeWidget(this);
    TreeWidDrivers->setHeaderHidden(true);
    TreeWidDrivers->setContextMenuPolicy(Qt::CustomContextMenu);
    TreeWidDrivers->setEnabled(false);

    LabDriverOpen = new QLabel(tr("No driver open"), this);

    QVBoxLayout *VerLayoutMain = new QVBoxLayout;
    VerLayoutMain->addLayout(GridLayBtns);
    VerLayoutMain->addWidget(TreeWidDrivers);
    VerLayoutMain->addWidget(LabDriverOpen);


    this->setLayout(VerLayoutMain);



    ActTreeElementUp = new QAction(QIcon(":/driver/images/ArrowUp_16.png"), tr("Up"), this);
    ActTreeElementUp->setProperty("Sense", true);
    ActTreeElementUp->setIconVisibleInMenu(true);

    ActTreeElementDown = new QAction(QIcon(":/driver/images/ArrowDown_16.png"), tr("Down"), this);
    ActTreeElementDown->setProperty("Sense", false);
    ActTreeElementDown->setIconVisibleInMenu(true);

    ActTreeElementNew = new QAction(QIcon(":/driver/images/Add_16.png"), tr("New"), this);
    ActTreeElementNew->setIconVisibleInMenu(true);

    ActTreeElementRename = new QAction(QIcon(":/driver/images/Rename_16.png"), tr("Rename"), this);
    ActTreeElementRename->setIconVisibleInMenu(true);

    ActTreeElementDelete = new QAction(QIcon(":/driver/images/Delete_16.png"), tr("Delete"), this);
    ActTreeElementDelete->setIconVisibleInMenu(true);

    ActTreeElementExec = new QAction(QIcon(":/driver/images/Run_16.png"), tr("Run"), this);
    ActTreeElementExec->setProperty("Edit", false);
    ActTreeElementExec->setIconVisibleInMenu(true);

    ActTreeElementEdit = new QAction(QIcon(":/driver/images/Edit_16.png"), tr("Edit"), this);
    ActTreeElementEdit->setProperty("Edit", true);
    ActTreeElementEdit->setIconVisibleInMenu(true);

    ActTreeGroupUp = new QAction(QIcon(":/driver/images/ArrowUp_16.png"), tr("Up"), this);
    ActTreeGroupUp->setProperty("Sense", true);
    ActTreeGroupUp->setIconVisibleInMenu(true);

    ActTreeGroupDown = new QAction(QIcon(":/driver/images/ArrowDown_16.png"), tr("Down"), this);
    ActTreeGroupDown->setProperty("Sense", false);
    ActTreeGroupDown->setIconVisibleInMenu(true);

    ActTreeGroupDelete = new QAction(QIcon(":/driver/images/Delete_16.png"), tr("Delete"), this);
    ActTreeGroupDelete->setIconVisibleInMenu(true);

    ActTreeGroupRename = new QAction(QIcon(":/driver/images/Rename_16.png"), tr("Rename"), this);
    ActTreeGroupRename->setIconVisibleInMenu(true);

}

void FDriver::CreateConnections()
{
    connect(BtnNew, SIGNAL(clicked()), this, SLOT(on_BtnNew_Clicked()));
    connect(BtnOpen, SIGNAL(clicked()), this, SLOT(on_BtnOpen_Clicked()));
    connect(BtnClose, SIGNAL(clicked()), this, SLOT(on_BtnClose_Clicked()));
    connect(BtnSave, SIGNAL(clicked()), this, SLOT(on_BtnSave_Clicked()));
    connect(BtnSaveAs, SIGNAL(clicked()), this, SLOT(on_BtnSaveAs_Clicked()));

    connect(ActTreeElementUp, SIGNAL(triggered()), this, SLOT(on_ActTreeElementUpDown_Triggered()));
    connect(ActTreeElementDown, SIGNAL(triggered()), this, SLOT(on_ActTreeElementUpDown_Triggered()));
    connect(ActTreeElementRename, SIGNAL(triggered()), this, SLOT(on_ActTreeElementRename_Triggered()));
    connect(ActTreeElementDelete, SIGNAL(triggered()), this, SLOT(on_ActTreeElementDelete_Triggered()));  
    connect(ActTreeElementNew, SIGNAL(triggered()), this, SLOT(on_ActTreeElementNew_Triggered()));
    connect(ActTreeElementExec, SIGNAL(triggered()), this, SLOT(on_ActTreeElementExecOrEdit_Triggered()));
    connect(ActTreeElementEdit, SIGNAL(triggered()), this, SLOT(on_ActTreeElementExecOrEdit_Triggered()));

    connect(ActTreeGroupUp, SIGNAL(triggered()), this, SLOT(on_ActTreeGroupUpDown_Triggered()));
    connect(ActTreeGroupDown, SIGNAL(triggered()), this, SLOT(on_ActTreeGroupUpDown_Triggered()));
    connect(ActTreeGroupRename, SIGNAL(triggered()), this, SLOT(on_ActTreeGroupRename_Triggered()));
    connect(ActTreeGroupDelete, SIGNAL(triggered()), this, SLOT(on_ActTreeGroupDelete_Triggered()));

    connect(TreeWidDrivers, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
             this, SLOT(on_treeWidget_itemDoubleClicked(QTreeWidgetItem*,int)));

    connect(TreeWidDrivers, SIGNAL(customContextMenuRequested(QPoint)),
             this, SLOT(on_treeWidget_ContextMenuRequested(QPoint)));

}
