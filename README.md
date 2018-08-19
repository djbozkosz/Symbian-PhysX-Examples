## Symbian PhysX Examples
[![S60: 5.3](https://img.shields.io/badge/s60-5.3-lightgrey.svg)](https://en.wikipedia.org/wiki/S60_(software_platform))
[![Win32: MinGW](https://img.shields.io/badge/Win32-MinGW-lightgrey.svg)](tps://en.wikipedia.org/wiki/S60_(software_pla))
[![PhysX: 3.4.2](https://img.shields.io/badge/physx-3.4.2-brightgreen.svg)](https://github.com/NVIDIAGameWorks/PhysX-3.4)
[![OpenGL ES: 2.0 | 4.3](https://img.shields.io/badge/opengl%20es-2.0-brightgreen.svg)](https://www.google.com/search?q=gpu+drivers)
[![Qt: 4.8](https://img.shields.io/badge/qt-4.8-green.svg)](https://www.qt.io)

### Build
1. For Windows build: Install [Qt for Windows](https://www.qt.io/) with MinGW compiler.
2. For Symbian build: Install Symbian [Qt SDK 1.2.1](https://www.mediafire.com/folder/79jhy594xb3uk/Symbian_Development) (Qt 4.7.4).
3. [Register](https://developer.nvidia.com/physx-source-github) as Nvidia developer with your GitHub account.
4. Clone this repo with [PhysX](https://github.com/djbozkosz/PhysX-3.4) submodule (step 3 needed).

### Qt Windows Build (MinGW)
1. Open Qt Commant Prompt.
2. Make sure that same `g++` compiler is set into `path` environment variable which is used for building Qt applications.
3. If not, use: `set PATH=<path_to_gcc_bin>;%PATH%`.
4. Locate PhysX SDK build folder: `cd <path_to_repo>\src\PhysX\PhysX_3.4\Source\compiler\make_win32`.
5. Build debug libs: `make.cmd DEBUG`.
6. Build release libs: `make.cmd`.
7. Open `PhysXExamples.pro` and setup target platform.
8. Build project in the usual way.

1. todo... (path, cd, build dev, build rel)

### Qt Symbian Build (GCCE)
1. Open Qt Symbian Commant Prompt.
2. Locate PhysX SDK build folder: `cd <path_to_repo>\src\PhysX\PhysX_3.4\Source\compiler\symbian32`.
3. Build debug libs: `sbs -c arm.v5.udeb.gcce4_4_1`.
4. Build release libs: `sbs -c arm.v5.urel.gcce4_4_1`.
5. Open `PhysXExamples.pro`, setup target platform for device and run `qmake`.
6. Open `Makefile` and for `debug-gcce` revert line below to: `$(SBS) -c arm.v5.udeb.gcce4_4_1_custom --configpath=%cd%/configs` (this is temporary solution).
7. Same for `release-gcce`, revert line below to: `$(SBS) -c arm.v5.urel.gcce4_4_1_custom --configpath=%cd%/configs` (this is temporary solution).
8. Build project in the usual way.

### Images:
<img src="https://s15.postimg.cc/qith8huob/Scr_259.jpg" alt="Crates">
