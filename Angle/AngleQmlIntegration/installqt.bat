@echo off

set Q=C:\Qt\5.4\msvc2013_64

PATH=%PATH%;%Q%\bin
"\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
cd C:\git\QtTests\Angle\AngleQmlIntegration\build\bin
%Q%\bin\windeployqt.exe --qmldir C:\git\QtTests\Angle\AngleQmlIntegration\AngleQml AngleQmlRunner.exe
