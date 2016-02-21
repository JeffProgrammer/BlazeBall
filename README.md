Blaze Ball
=============

A marble game that is meant to be the successor to Marble Blast.

!["in game image"](http://i.imgur.com/9W5V4rl.jpg "In Game")
Disclaimer: Temporary assets. Will make our own when we are ready.

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
OpenGL 2.1+ (3.0 extensions + GLSL 120)

* Windows XP+
* Mac OSX 10.6.8+
* Ubuntu 14.04
* Intel Core2Duo CPU @ 2.0ghz
* GPU:
  * NVidia GeForce 8000 series
  * ATI Radeon HD 2000 series
  * Intel HD 3000 Graphics
  * 256MB VRAM
* 2GB Ram

Third Party Libraries
-------------------

* Assimp - Model Importer
* Bullet3D - Physics Library
* ENet - Networking Library
* ENetPP - Networking extensions library for ENet
* FreeType - Font Library
* Epoxy - OpenGL Extensions Loader
* GLM - Math Library
* JPEG - Image Library
* LibRocket - GUI library
* MNG - Image Library
* RapidJSON - JSON parser library
* SDL2 - Window and Input System
* srutil - Fast C++ Delegates
* ZLIB - Compression Library

License
-------------------

```
Copyright (c) 2014-2016, Glenn Smith
Copyright (c) 2014-2016, Jeff Hutchinson
All rights reserved.
```
