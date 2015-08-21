Windows instructions on building V8
-----------------------------------

Tested on windows 7 using Microsoft Visual Studio 2015

**NOTE:** This may not be 100% correct, I did this at 1am the morning before I wrote these instructions.

**NOTE** yes you will be installing Python 2 in two different locations, its not a mistake in my instructions.

**NOTE** for best results don't have spaces in any of your paths when doing this stuff.

*Install Python 2.7 in C:\Python27\ (you may download the windows installer from python.org)
*Download and install the depot_tools [ link: https://src.chromium.org/svn/trunk/tools/depot_tools.zip ]
  *When extracting, make sure the path doesn't have spaces in its path or else it won't work (ask google, don't ask me)
*Put both the python executable and the depot_tools inside of your PATH found in: startmenu/control pannel/system/advanced settings/environment variables
*open up a cmd prompt
  *cd to the directory where you want v8 to be
  *gclient
  *gclient sync
  *fetch v8
  *git pull
  *gclient sync
  *cd v8
  *svn co http://src.chromium.org/svn/trunk/deps/third_party/cygwin thirdparty\cgywin
  *svn co http://src.chromium.org/svn/trunk/tools/third_party/python_26@89111 thirdparty\python_26
*Generating project files for Visual Studio:
  *cd v8
  *thirdparty/python_26/python.exe build\gyp_v8
    *If doing 64bit build, third_party\python_26\python.exe build\gyp_v8 -Dtarget_arch=x64 [did not test x64 yet]
  *now using explorer, navigate to the build directory and double click ALL.sln. Build that. Then run unit tests. If failure happens, then my instructions are wrong.
  
Mac instructions for V8
------------------------

[TODO, BUT MUCH EASIER]