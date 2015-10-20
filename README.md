DifViewerGame
=============

A small marble game project based upon DifViewer / DIF. Supports rendering DIFs and rudimentary marble movement and physics.

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

* Windows 7+
* Mac OSX 10.7+
* Dual Core CPU @ 2.0ghz
* GPU:
  * GeForce 8000 series or better GPU with at least 256MB VRAM
  * ATI Radeon HD 2000 series or better GPU with at least 256MB VRAM
  * Intel HD Graphics or better with at least 256MB Shared VRAM
* 2GB Ram

Acknowledgements
-------------------

Credits to Whirligig231 for his sphere-trimesh edge adjacency solver.

License
-------------------

```
Copyright (c) 2014-2015, Glenn Smith
Copyright (c) 2014-2015, Jeff Hutchinson
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
