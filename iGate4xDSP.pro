QT -= gui
QT += network
QT += websockets
QT += sql
CONFIG += c++11 console
CONFIG -= app_bundle
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_DEBUG_OUTPUT
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CS42448.cpp \
        ChatClient.cpp \
        ChatServer.cpp \
        Database.cpp \
        Functions.cpp \
        GPIOClass.cpp \
        I2CReadWrite.cpp \
        NetWorkMng.cpp \
        OLEDDriver.cpp \
        PCM3168A.cpp \
        SPI.cpp \
        SigmaStudioFW.cpp \
        SnmpStack.cpp \
        iGate4x.cpp \
        linux_spi.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CS42448.h \
    ChatClient.h \
    ChatServer.h \
    Database.h \
    GPIOClass.h \
    I2CReadWrite.h \
    NetWorkMng.h \
    OLEDDriver.h \
    PCM3168A.h \
    SPI.h \
    SigmaStudioFW.h \
    SnmpStack.h \
    iGate4CH_DSP/FirmwareV3/DesignDSP_IC_1.h \
    iGate4CH_DSP/FirmwareV3/DesignDSP_IC_1_PARAM.h \
    iGate4CH_DSP/FirmwareV3/DesignDSP_IC_1_REG.h \
    iGate4x.h \
    linux_spi.h \
    sampleI2CReadWrite.h


linux-jetson-nano2g-g++:{
    INCLUDEPATH += /home/ubuntu/JETSONData/nano2GB/sysroot/usr/local/include
    LIBS += -L/usr/local/lib -lpjsua2-aarch64-unknown-linux-gnu -lstdc++ -lpjsua-aarch64-unknown-linux-gnu -lpjsip-ua-aarch64-unknown-linux-gnu -lpjsip-simple-aarch64-unknown-linux-gnu -lpjsip-aarch64-unknown-linux-gnu -lpjmedia-codec-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjmedia-videodev-aarch64-unknown-linux-gnu -lpjmedia-audiodev-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjnath-aarch64-unknown-linux-gnu -lpjlib-util-aarch64-unknown-linux-gnu -lsrtp-aarch64-unknown-linux-gnu -lresample-aarch64-unknown-linux-gnu -lg729codec-aarch64-unknown-linux-gnu -lgsmcodec-aarch64-unknown-linux-gnu -lspeex-aarch64-unknown-linux-gnu -lilbccodec-aarch64-unknown-linux-gnu -lg7221codec-aarch64-unknown-linux-gnu -lyuv-aarch64-unknown-linux-gnu -lpj-aarch64-unknown-linux-gnu -lssl -lcrypto -luuid -lm -lrt -lpthread -lasound
}
linux-jetson-nano-g++:{
    INCLUDEPATH += /home/ubuntu/JETSONData/nano/sysroot/usr/local/include
    LIBS += -L/usr/local/lib -lpjsua2-aarch64-unknown-linux-gnu -lstdc++ -lpjsua-aarch64-unknown-linux-gnu -lpjsip-ua-aarch64-unknown-linux-gnu -lpjsip-simple-aarch64-unknown-linux-gnu -lpjsip-aarch64-unknown-linux-gnu -lpjmedia-codec-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjmedia-videodev-aarch64-unknown-linux-gnu -lpjmedia-audiodev-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjnath-aarch64-unknown-linux-gnu -lpjlib-util-aarch64-unknown-linux-gnu -lsrtp-aarch64-unknown-linux-gnu -lresample-aarch64-unknown-linux-gnu -lg729codec-aarch64-unknown-linux-gnu -lgsmcodec-aarch64-unknown-linux-gnu -lspeex-aarch64-unknown-linux-gnu -lilbccodec-aarch64-unknown-linux-gnu -lg7221codec-aarch64-unknown-linux-gnu -lyuv-aarch64-unknown-linux-gnu -lpj-aarch64-unknown-linux-gnu -lssl -lcrypto -luuid -lm -lrt -lpthread -lasound
}
linux-oe-generic-g++:{
    INCLUDEPATH += /opt/b2qt/2.7.2/sysroots/aarch64-poky-linux/usr/local/include
    LIBS += -lpjsua2-aarch64-unknown-linux-gnu -lstdc++ -lpjsua-aarch64-unknown-linux-gnu -lpjsip-ua-aarch64-unknown-linux-gnu -lpjsip-simple-aarch64-unknown-linux-gnu -lpjsip-aarch64-unknown-linux-gnu -lpjmedia-codec-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjmedia-videodev-aarch64-unknown-linux-gnu -lpjmedia-audiodev-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjnath-aarch64-unknown-linux-gnu -lpjlib-util-aarch64-unknown-linux-gnu  -lsrtp-aarch64-unknown-linux-gnu -lresample-aarch64-unknown-linux-gnu -lg729codec-aarch64-unknown-linux-gnu -lgsmcodec-aarch64-unknown-linux-gnu -lspeex-aarch64-unknown-linux-gnu -lilbccodec-aarch64-unknown-linux-gnu -lg7221codec-aarch64-unknown-linux-gnu -lyuv-aarch64-unknown-linux-gnu  -lpj-aarch64-unknown-linux-gnu -lssl -lcrypto -luuid -lm -lrt -lpthread  -lasound
}
linux-jetson-orin-g++:{
    INCLUDEPATH += /home/ubuntu/BackupData/BackupData/OrinNx/Jetson_Linux_R35.3.1_aarch64/QtSource/sysroot/usr/local/include
    LIBS += -L/usr/local/lib -lpjsua2-aarch64-unknown-linux-gnu -lstdc++ -lpjsua-aarch64-unknown-linux-gnu -lpjsip-ua-aarch64-unknown-linux-gnu -lpjsip-simple-aarch64-unknown-linux-gnu -lpjsip-aarch64-unknown-linux-gnu -lpjmedia-codec-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjmedia-videodev-aarch64-unknown-linux-gnu -lpjmedia-audiodev-aarch64-unknown-linux-gnu -lpjmedia-aarch64-unknown-linux-gnu -lpjnath-aarch64-unknown-linux-gnu -lpjlib-util-aarch64-unknown-linux-gnu -lsrtp-aarch64-unknown-linux-gnu -lresample-aarch64-unknown-linux-gnu -lg729codec-aarch64-unknown-linux-gnu -lgsmcodec-aarch64-unknown-linux-gnu -lspeex-aarch64-unknown-linux-gnu -lilbccodec-aarch64-unknown-linux-gnu -lg7221codec-aarch64-unknown-linux-gnu -lyuv-aarch64-unknown-linux-gnu -lpj-aarch64-unknown-linux-gnu -lssl -lcrypto -luuid -lm -lrt -lpthread -lasound
}
linux-g++:{
    LIBS += `pkg-config --libs libpjproject`
}
LIBS += -DPJ_AUTOCONF=1 -O2 -DPJ_IS_BIG_ENDIAN=0 -DPJ_IS_LITTLE_ENDIAN=1


LIBS += -lboost_system -lboost_chrono -lboost_thread -ludev
LIBS += -lnetsnmp -lasound
