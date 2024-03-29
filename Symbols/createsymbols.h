/******************************************************************************
**                                                                           **
**  Template Plugin for LabAnlyser                                           **
**  Copyright (C) 2015-2021 Andreas Hoffmann                                 **
**                                                                           **
**  Template  Plugin is free software: you can redistribute it and/or modify **
**  it under the terms of the GNU General Public License as published by     **
**  the Free Software Foundation, either version 3 of the License, or        **
**  (at your option) any later version.                                      **
**                                                                           **
**  This program is distributed in the hope that it will be useful,          **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**  GNU General Public License for more details.                             **
**                                                                           **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                           **
*******************************************************************************
*******************************************************************************/
#pragma once

#include <QObject>
#include "DataType/datastorage.h"

class CreateSymbols: public QObject
{
   Q_OBJECT
public:
    CreateSymbols(QObject* parent, QString DeviceName_, std::map<QString, DataStorage> &m_data_);
    void PublishParameters();


signals:
    void MessageSender(const QString &Command, const QString &ID, InterfaceData Data);

private:
    QString DeviceName;
    std::map<QString, DataStorage> &m_data;

    void PublishGUISelection();
    void PublishUint32(int index);
    void PublishBool(int index);
    void PublishBoolP(int index);

    void PublishDouble();
    void PublishVector();
    void PublishState(QString ID);


};

