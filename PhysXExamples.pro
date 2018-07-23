# debug-gcce: c:\Projects\Qt\PhysX\Sample\PhysX.loc bld.inf
#	$(SBS) -c arm.v5.udeb.gcce4_4_1_custom --configpath=%cd%/configs

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhysXExamples
TEMPLATE = app

SOURCES += \
	src/main.cpp \
	src/Splash.cpp \
	src/GlConstants.cpp \
	src/GlWidget.cpp \
	src/MainWindow.cpp \
	src/Physics.cpp \
	src/SceneBase.cpp \
	src/Scene00.cpp \
	src/Scene01.cpp \
	src/Scene02.cpp

HEADERS += \
	src/ISceneProvider.h \
	src/ISceneObjectProvider.h \
	src/Splash.h \
	src/GlConstants.h \
	src/GlWidget.h \
	src/MainWindow.h \
	src/Physics.h \
	src/SceneBase.h \
	src/Scene00.h \
	src/Scene01.h \
	src/Scene02.h

INCLUDEPATH += \
	src/PhysX/PhysX_3.4/include \
	src/PhysX/PxShared/include

FORMS += \
	ui/MainWindow.ui

win32 {
	CONFIG(release, debug|release) {
		DEFINES += NDEBUG
	} else {
		DEFINES += _DEBUG
	}

	DEFINES += PX_FOUNDATION_STATIC_LIB PX_PHYSX_STATIC_LIB

	LIBS += \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXVehicle.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXCharacterKinematic.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXExtensions.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysX.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXSceneQuery.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXSimulationController.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXCooking.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXLowLevelAABB.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXLowLevelCloth.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXLowLevelDynamics.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXLowLevelParticles.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXLowLevel.a \
		../src/PhysX/PhysX_3.4/Source/compiler/make_win32/lib/PhysXCommon.a \
		../src/PhysX/PxShared/src/compiler/make_win32/lib/PhysXPvdSDK.a \
		../src/PhysX/PxShared/src/compiler/make_win32/lib/PhysXTask.a \
		../src/PhysX/PxShared/src/compiler/make_win32/lib/PhysXFoundation.a \
		../src/PhysX/PxShared/src/compiler/make_win32/lib/PhysXFastXml.a
}

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

symbian {
	DEPLOYMENT.display_name = PhysX Examples
	VERSION = 0.0.1
	TARGET.UID3 = 0xAC928603
	TARGET.EPOCSTACKSIZE = 0x16000
	TARGET.EPOCHEAPSIZE = 0x200000 0x20000000
	ICON = images/icon.svg

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
}

RESOURCES += \
    qrc/qresources.qrc
