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

Supported DIF File Formats:
-------------------
* 0 (MB)
* 0 (TGE)
* 0 (TGEA)
* 1 (MBU)
* 2 (MBU)
* 3 (MBU)
* 4 (MBU)
* 10 (T3D)
* 14 (T3D / MBM)
* Possibly those in the middle, unknown and untested.

Requirements
-------------------

C++11 support  
OpenGL 2.1+

* Windows XP SP3+
* Mac OSX 10.6+
* Intel Core2Duo CPU @ 2.0ghz
* GPU:
  * NVidia GeForce 8000 series
  * ATI Radeon HD 2000 series
  * Intel HD 3000 Graphics
  * 256MB VRAM
* 2GB Ram

Third Party Libraries
-------------------

* Assimp Model Importer
* Bullet 3D Physics Library
* DukTape Javascript Engine
* GLEW OpenGL Extensions Loader
* GLM Math Library
* JPEG Image Library
* MNG Image Library
* SDL2 Window and Input System
* ZLIB Compression Library

Acknowledgements
-------------------

Credits to Whirligig231 for his sphere-trimesh edge adjacency solver.

License
-------------------

```
Copyright (c) 2014-2015, Glenn Smith
Copyright (c) 2014-2015, Jeff Hutchinson
All rights reserved.
```
