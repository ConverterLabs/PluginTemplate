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

#include "createsymbols.h"

CreateSymbols::CreateSymbols(QObject* parent, QString DeviceName_, std::map<QString, DataStorage> &m_data_): QObject(parent), DeviceName(DeviceName_), m_data(m_data_)
{
    connect(this,SIGNAL(MessageSender(const QString, const QString, InterfaceData)),parent,SLOT(MessageReceiver(const QString, const QString, InterfaceData)));
}


void CreateSymbols::PublishGUISelection()
{
    QString ID;
    ID += DeviceName + "::Test::GUISelection";
    InterfaceData _Data;

    QStringList Selection;
    Selection.push_back("Type1");
    Selection.push_back("Type2");
    Selection.push_back("Type3");
    _Data.SetData(GuiSelection("RingBuffer",Selection));
    _Data.SetType("Parameter");
    _Data.SetStateDependency("");

    this->m_data[ID] = _Data;
    emit MessageSender("publish", ID,  _Data);
}


void CreateSymbols::PublishUint32(int index)
{
    //Publish a Signal that resets the trigger condition
    QString ID;
    ID += DeviceName + "::Test::Uint32::" + QString::number(index);
    InterfaceData _Data;
    _Data.SetType("Data");
    _Data.SetData((uint32_t)1);
    _Data.SetStateDependency("");
    this->m_data[ID] = _Data;
    emit MessageSender("publish", ID,  _Data);
    _Data.SetType("Parameter");

    emit MessageSender("publish", ID,  _Data);
}

void CreateSymbols::PublishBool(int index)
{
    //Publish a Signal that resets the trigger condition
    QString ID;
    ID += DeviceName + "::Test::BOOL::" + QString::number(index);
    InterfaceData _Data;
    _Data.SetType("Data");
    _Data.SetData((bool)1);
    _Data.SetStateDependency("");
    this->m_data[ID] = _Data;
    emit MessageSender("publish", ID,  _Data);

}

void CreateSymbols::PublishBoolP(int index)
{
    //Publish a Signal that resets the trigger condition
    QString ID;
    ID += DeviceName + "::Test::BOOLP::" + QString::number(index);
    InterfaceData _Data;
    _Data.SetType("Parameter");
    _Data.SetData((bool)1);
    _Data.SetStateDependency("");
    this->m_data[ID] = _Data;
    emit MessageSender("publish", ID,  _Data);

}

void CreateSymbols::PublishDouble()
{
    //Publish Parameter Buffer SPS Time
    QString ID;
    ID += DeviceName + "::Test::double";
    InterfaceData _Data;
    _Data.SetType("Parameter");
    _Data.SetData(0.5);
    _Data.SetStateDependency("");
    this->m_data[ID] = _Data;
    emit MessageSender("publish", ID,  _Data);
}

void CreateSymbols::PublishVector()
{
    std::vector<double> x;
    std::vector<double> y;
    for(auto i = 0; i < 10000; i++)
    {
        x.push_back(static_cast<double>(i)/1000.);
        y.push_back(x.back()*x.back());
    }
    auto Sx = boost::shared_ptr<std::vector<double>>(new std::vector<double>(x));
    auto Sy = boost::shared_ptr<std::vector<double>>(new std::vector<double>(y));

    InterfaceData _Data;
    _Data.SetDataType("vector<double>");
    _Data.SetType("Data");
    _Data.SetData(DataPair(Sx,Sy));
    QString ID;
    ID += DeviceName + "::Test::Vector";
    emit MessageSender("publish", ID,  _Data);

    _Data.SetDataType("vector<double>");
    _Data.SetType("Data");
    _Data.SetData(DataPair(Sx,Sy));
    ID = DeviceName + "::Test::Vector2";
    emit MessageSender("publish", ID,  _Data);
}

void CreateSymbols::PublishState(QString ID)
{
    InterfaceData _Data;
    _Data.SetDataType("boolean");
    _Data.SetType("State");
    _Data.SetData(false);
    m_data[ID] = _Data;

    emit MessageSender("publish", ID,  _Data);
}

void CreateSymbols::PublishParameters()
{
    InterfaceData Data;
    Data.SetData(0);
    emit MessageSender("publish_start", DeviceName, Data);

    for(int i = 30; i>= 0; i--)
    {
        PublishUint32(i);
        PublishBool(i);
        PublishBoolP(i);

    }

    PublishGUISelection();
    PublishDouble();
    PublishVector();
    PublishState(DeviceName + "::ReadChannels");

    emit MessageSender("publish_finished", DeviceName, Data);
}
