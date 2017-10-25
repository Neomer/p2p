QT += network
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    Gateway.cpp \
    Pipe.cpp \
    PipeController.cpp \
    KeyLogger.cpp \
    MessageController.cpp \
    PipePackage.cpp \
    web/HttpManager.cpp \
    web/HttpResponse.cpp \
    web/HttpUtils.cpp \
    core/CommandProvider.cpp \
    core/KeyCommand.cpp \
    core/Context.cpp \
    core/Hash.cpp \
    core/EventBus.cpp \
    core/ISerializable.cpp \
    blockchain/Block.cpp \
    blockchain/BlockChainIndex.cpp \
    blockchain/BlockChain.cpp \
    worker/Worker.cpp \
    worker/Miner.cpp

HEADERS += \
    Gateway.h \
    Pipe.h \
    PipeController.h \
    KeyLogger.h \
    Global.h \
    MessageController.h \
    PipePackage.h \
    web/HttpManager.h \
    web/HttpResponse.h \
    web/HttpUtils.h \
    core/CommandProvider.h \
    core/KeyCommand.h \
    core/Context.h \
    core/Hash.h \
    core/IEventDispatcher.h \
    core/EventBus.h \
    core/ISerializable.h \
    blockchain/Block.h \
    blockchain/BlockChainIndex.h \
    blockchain/BlockChain.h \
    worker/Worker.h \
    worker/Miner.h

INCLUDEPATH += $PWD

OBJECTS_DIR = tmp/o
MOC_DIR = tmp/moc
UI_DIR = tmp/ui
