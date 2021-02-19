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

    CreateSymbols Symbols(this, DeviceName, m_data);
    Symbols.PublishParameters();
    uint32_t counter = 0;
    // First Loop
    while(!Abort())
    {       
               //Send neu data of published symbol do LabAnalyser
               QString ID = DeviceName + "::Test::Uint32";
               m_data[ID].SetDataKeepType(counter++);
               Messenger.MessageSender("set", ID, m_data[ID]);

               //Send a Message to the output window of Labanalyser
               if(counter % 10 == 0)
                    Messenger.Info("I am alive.");

               //process Events
               QCoreApplication::processEvents();

               //always use a sleep in this loop or the cpu load will be massive
               QThread::msleep(100);


               //Check if Data2Send
               //SendBufferMutex.lock();


               //SendBufferMutex.unlock();

    }
    Messenger.Info("I am dead.");
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

        SendBufferMutex.lock();
        //Send signal ....


        SendBufferMutex.unlock();

    }
    return;
}
