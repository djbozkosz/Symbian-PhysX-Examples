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
	src/Scene02.cpp \
	src/Scene03.cpp

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
	src/Scene02.h \
	src/Scene03.h

INCLUDEPATH += \
	src/PhysX/PhysX_3.4/include \
	src/PhysX/PxShared/include

FORMS += \
	ui/MainWindow.ui

win32 {
	CONFIG(release, debug|release) {
		DEFINES += NDEBUG
		LIBDIR   = lib/release
	} else {
		DEFINES += _DEBUG
		LIBDIR   = lib/debug
	}

	DEFINES += PX_FOUNDATION_STATIC_LIB PX_PHYSX_STATIC_LIB

	LIBS_PATH_PHYSX    = ../src/PhysX/PhysX_3.4/Source/compiler/make_win32/$${LIBDIR}
	LIBS_PATH_PXSHARED = ../src/PhysX/PxShared/src/compiler/make_win32/$${LIBDIR}

	LIBS += \
		$${LIBS_PATH_PHYSX}/PhysXVehicle.a \
		$${LIBS_PATH_PHYSX}/PhysXCharacterKinematic.a \
		$${LIBS_PATH_PHYSX}/PhysXExtensions.a \
		$${LIBS_PATH_PHYSX}/PhysX.a \
		$${LIBS_PATH_PHYSX}/PhysXSceneQuery.a \
		$${LIBS_PATH_PHYSX}/PhysXSimulationController.a \
		$${LIBS_PATH_PHYSX}/PhysXCooking.a \
		$${LIBS_PATH_PHYSX}/PhysXLowLevelAABB.a \
		$${LIBS_PATH_PHYSX}/PhysXLowLevelCloth.a \
		$${LIBS_PATH_PHYSX}/PhysXLowLevelDynamics.a \
		$${LIBS_PATH_PHYSX}/PhysXLowLevelParticles.a \
		$${LIBS_PATH_PHYSX}/PhysXLowLevel.a \
		$${LIBS_PATH_PHYSX}/PhysXCommon.a \
		$${LIBS_PATH_PXSHARED}/PhysXPvdSDK.a \
		$${LIBS_PATH_PXSHARED}/PhysXTask.a \
		$${LIBS_PATH_PXSHARED}/PhysXFoundation.a \
		$${LIBS_PATH_PXSHARED}/PhysXFastXml.a \
		-lmsvcrt
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
