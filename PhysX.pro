# debug-gcce: c:\Projects\Qt\PhysX\Sample\PhysX.loc bld.inf
#	$(SBS) -c arm.v5.udeb.gcce4_4_1_custom --configpath=%cd%/configs

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhysX
TEMPLATE = app

SOURCES += \
	src/main.cpp \
	src/Splash.cpp \
	src/GlWidget.cpp \
	src/SampleBase.cpp \
	src/Sample00.cpp

HEADERS += \
	src/ISceneObjectProvider.h \
	src/Splash.h \
	src/GlWidget.h \
	src/SampleBase.h \
	src/Sample00.h

INCLUDEPATH += \
	../PhysX_3.4/include \
	../PxShared/include

QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wno-invalid-offsetof
QMAKE_CXXFLAGS += -Wno-psabi
QMAKE_CXXFLAGS += -fno-strict-aliasing
QMAKE_CXXFLAGS += -U__STRICT_ANSI__

CONFIG(release, debug|release) {
	QMAKE_CXXFLAGS += -O3
} else {
	QMAKE_CXXFLAGS += -O0
}

LIBS += \
	-lPhysXVehicle.lib \
	-lPhysXCharacterKinematic.lib \
	-lPhysXExtensions.lib \
	-lPhysX.lib \
	-lPhysXSceneQuery.lib \
	-lPhysXSimulationController.lib \
	-lPhysXCooking.lib \
	-lPhysXLowLevelAABB.lib \
	-lPhysXLowLevelCloth.lib \
	-lPhysXLowLevelDynamics.lib \
	-lPhysXLowLevelParticles.lib \
	-lPhysXLowLevel.lib \
	-lPhysXCommon.lib \
	-lPhysXPvdSDK.lib \
	-lPhysXTask.lib \
	-lPhysXFoundation.lib \
	-lPhysXFastXml.lib \
	-llibstdcpp \
	-llibc \
	-llibm \
	-llibpthread \
	-lusrt3_1.lib

symbian {
	DEPLOYMENT.display_name = PhysX
	VERSION = 0.0.1
	TARGET.UID3 = 0xAC928603
	TARGET.EPOCSTACKSIZE = 0x16000
	TARGET.EPOCHEAPSIZE = 0x200000 0x20000000
}

RESOURCES += \
    qrc/qresources.qrc
