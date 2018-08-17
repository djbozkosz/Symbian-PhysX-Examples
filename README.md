## Symbian PhysX Examples
[![S60: 5.3](https://img.shields.io/badge/s60-5.3-lightgrey.svg)](https://en.wikipedia.org/wiki/S60_(software_platform))
[![PhysX: 3.4.2](https://img.shields.io/badge/physx-3.4.2-brightgreen.svg)](https://github.com/NVIDIAGameWorks/PhysX-3.4)
[![OpenGL ES: 2.0 | 4.3](https://img.shields.io/badge/opengl%20es-2.0-brightgreen.svg)](https://www.google.com/search?q=gpu+drivers)
[![Qt: 4.8](https://img.shields.io/badge/qt-4.8-green.svg)](https://www.qt.io)

### Prerequisites
- Installed Symbian [Qt SDK 1.2.1](https://www.mediafire.com/folder/79jhy594xb3uk/Symbian_Development) (Qt 4.7.4).

### Windows MinGW Build
1. todo... (path, cd, build dev, build rel)

### Symbian GCCE Build
1. [Register](https://developer.nvidia.com/physx-source-github) as Nvidia developer with your GitHub account.
2. Clone this repo with [PhysX](https://github.com/djbozkosz/PhysX-3.4) submodule (step 1 needed).
3. Open Qt Symbian Commant Prompt.
4. Locate PhysX SDK build folder: `cd <path_to_repo>\src\PhysX\PhysX_3.4\Source\compiler\symbian32`.
5. Build debug libs: `sbs -c arm.v5.udeb.gcce4_4_1`.
6. Build release libs: `sbs -c arm.v5.urel.gcce4_4_1`.
7. Open `PhysXExamples.pro`, setup target platform for device and run `qmake`.
8. Open `Makefile` and for `debug-gcce` revert build to: `$(SBS) -c arm.v5.udeb.gcce4_4_1_custom --configpath=%cd%/configs` (this is temporary solution).
9. Build project in the usual way.

### Images:
<img src="https://s15.postimg.cc/qith8huob/Scr_259.jpg" alt="Crates">
