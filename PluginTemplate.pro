#/******************************************************************************
#**                                                                           **
#**  Template Plugin for LabAnlyser                                           **
#**  Copyright (C) 2015-2021 Andreas Hoffmann                                 **
#**                                                                           **
#**  Template  Plugin is free software: you can redistribute it and/or modify **
#**  it under the terms of the GNU General Public License as published by     **
#**  the Free Software Foundation, either version 3 of the License, or        **
#**  (at your option) any later version.                                      **
#**                                                                           **
#**  This program is distributed in the hope that it will be useful,          **
#**  but WITHOUT ANY WARRANTY; without even the implied warranty of           **
#**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
#**  GNU General Public License for more details.                             **
#**                                                                           **
#**  You should have received a copy of the GNU General Public License        **
#**  along with this program.  If not, see http://www.gnu.org/licenses/.      **
#**                                                                           **
#*******************************************************************************
#*******************************************************************************/

QT       -= gui

TARGET = PluginTemplate
TEMPLATE = lib

CONFIG         += plugin
QT             += widgets

OBJECTS_DIR=$$builddir
DESTDIR=$$builddir

INCLUDEPATH += C:/libraries/boost_1_59_0

DEFINES +=  PluginTemplate_LIBRARY
DEFINES +=  WIN32_LEAN_AND_MEAN

SOURCES +=\
            ../LabAnalyser/plugins/InterfaceDataType.cpp \
            DataType/datastorage.cpp \
            Messenger/messengerclass.cpp \
            Symbols/createsymbols.cpp \
            PluginTemplate.cpp \
            worker.cpp

HEADERS += \
            ../LabAnalyser/plugins/platforminterface.h\
            ../LabAnalyser/plugins/InterfaceDataType.h \
            DataType/datastorage.h \
            Messenger/messengerclass.h \
            Symbols/createsymbols.h \
            PluginTemplate.h \
            worker.h

TARGET          = $$qtLibraryTarget(PluginTemplate)
Debug:DESTDIR         = ../../plugins
Release:DESTDIR         = C:/LEGui

#QMAKE_CXXFLAGS_RELEASE += -O1
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE += -O3
