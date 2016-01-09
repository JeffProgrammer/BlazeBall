Blaze Ball
=============

A marble game that is meant to be the successor to Marble Blast.

Building
-------------------

Run CMake on the root project directory. It is recommended to build project/binary
files to another directory, such as `build/`.

Running
-------------------

Make the "install" target from the CMakeFile and run the `Game` executable in `bin/`.  
The executable is run as `Game <dif file> [dif file 2, ...]`. Files can be in any compatible DIF format.

Requirements
-------------------

C++11 support  
OpenGL 3.3+

* Windows 7+
* Mac OSX 10.9+
* Intel Core2Duo CPU @ 2.0ghz
* GPU:
  * NVidia GeForce 8000 series
  * ATI Radeon HD 2000 series
  * Intel HD 3000 Graphics [OSX]
  * Intel HD 4000 Graphics [Windows]
  * 256MB VRAM
* 2GB Ram

Third Party Libraries
-------------------

* Assimp - Model Importer
* Bullet3D - Physics Library
* ChaiScript - Scripting Language Library
* ENet - Networking Library
* ENetPP - Networking extensions library for ENet
* GLEW - OpenGL Extensions Loader
* GLM - Math Library
* JPEG - Image Library
* MNG - Image Library
* RapidJSON - JSON parser library
* SDL2 - Window and Input System
* srutil - Fast C++ Delegates
* ZLIB - Compression Library

License
-------------------

```
Copyright (c) 2014-2015, Glenn Smith
Copyright (c) 2014-2015, Jeff Hutchinson
All rights reserved.
```
