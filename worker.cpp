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

#include "worker.h"

//Needed for compiling:
#define BOOST_SYSTEM_NO_DEPRECATED
#include <boost/system/error_code.hpp>

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include <iomanip>        // std::put_time
#include <thread>         // std::this_thread::sleep_until
#include <chrono>         // std::chrono::system_clock
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime

#include <signal.h>
#include <vector>
#include <mutex>

#include <QtWidgets>
#include <cmath>

#include <QDebug>
#include <functional>

#include "Symbols/createsymbols.h"
#include <ctime>

WorkClass::WorkClass(QString DeviceName_)
    : QObject(nullptr), SymbolsPublished(false), Messenger(this, DeviceName_)
{

    DeviceName = DeviceName_;
}

WorkClass::~WorkClass()
{

}


bool WorkClass::Abort()
{
    return abort||Error;
}

void WorkClass::process()
{
    this->ThreadRunning.lock();
    Messenger.Info( DeviceName + " loaded.");

    std::srand(std::time(nullptr)); // use current time as seed for random generator


    CreateSymbols Symbols(this, DeviceName, m_data);
    Symbols.PublishParameters();
    uint32_t counter = 0;
    SymbolsPublished = true;
    // First Loop
    double omega = 2*M_PI*50;
    double t0 = 0;
    while(!Abort())
    {       
               //Send neu data of published symbol do LabAnalyser
               QString ID = DeviceName + "::Test::Uint32";
               m_data[ID].SetDataKeepType(counter++);
               Messenger.MessageSender("set", ID, m_data[ID]);

               //Send a Message to the output window of Labanalyser
               if(counter % 10 == 0)
                    Messenger.Info("I am alive.");


               t0 += 0.1;

               std::vector<double> x;
               std::vector<double> y, y2;
               for(auto i = 0; i < 10000; i++)
               {
                   double randomn =  (double)(1 +(std::rand()/((RAND_MAX + 1u)/100)))/10000.0;
                   double randomn2 =  (double)(1 +(std::rand()/((RAND_MAX + 1u)/100)))/10000.0;

                   x.push_back(t0 + static_cast<double>( i/100000.));
                   y.push_back(sin((t0+x.back())*omega)+randomn);
                   y2.push_back(cos((t0+x.back())*omega)+randomn2);

               }

               auto Sx = boost::shared_ptr<std::vector<double>>(new std::vector<double>(x));
               auto Sy = boost::shared_ptr<std::vector<double>>(new std::vector<double>(y));
               auto Sy2 = boost::shared_ptr<std::vector<double>>(new std::vector<double>(y2));

               InterfaceData _Data;
               _Data.SetDataType("vector<double>");
               _Data.SetType("Data");
               _Data.SetData(DataPair(Sx,Sy, t0));
               ID = DeviceName + "::Test::Vector";
               emit MessageSender("set", ID,  _Data);

               QThread::msleep(20);
               _Data.SetDataType("vector<double>");
               _Data.SetType("Data");
               _Data.SetData(DataPair(Sx,Sy2, t0));
               ID = DeviceName + "::Test::Vector2";
               emit MessageSender("set", ID,  _Data);

               t0 += 0.1;

               //process Events
               QCoreApplication::processEvents();

               //always use a sleep in this loop or the cpu load will be massive
               QThread::msleep(35);


               //Check if Data2Send
               //SendBufferMutex.lock();


               //SendBufferMutex.unlock();

    }


    Messenger.Info( DeviceName + " closed.");
    this->ThreadRunning.unlock();
    emit ThreadFinished();
    Finished = true;

}




void WorkClass::MessageReceiver(const QString &Command, const QString &ID, InterfaceData Data)
{

    //ID always begins with plugin name
    if(ID.split("::").at(0) != DeviceName)
        return;

    if(Command == "get")
    {
    }
    else if(Command == "load")
    {
        /*XmlReader reader(this,Messenger);
        if(reader.read(Data.GetString()))
        {
            abort = true;
            Error = true;
        }*/
    }
    else if(Command.compare("LoadCustomData")==0)
    {

    }
    else if(Command.compare("save")==0)
    {

    }
    else if(Command == "publish")
    {
        this->m_data[ID] = Data;
        emit MessageSender(Command,ID,Data);
    }
    else if(Command.compare("set")==0)
    {
        //Use if real device has to set it later:
        this->m_data[ID].SetDataTimeOut(Data.GetData(),ID, &Messenger);
        if(Data.IsBool())
            qDebug() << "Received" << Data.GetBool();
        SendBufferMutex.lock();
        //Send signal ....


        SendBufferMutex.unlock();

    }
    return;
}
