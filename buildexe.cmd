SET RSAKey=\netrek_files\rsa\NetrekXP-Mod-RSA-Key-Win32.secret
Set Compiler=BCC
Set InstCWD=\netrek_files
rem Set Compiler=VCC

@echo off
Set MyCWD=%cd%
if /I "%1" == "clean" goto :clean
if /I "%1" == "dist" goto :dist

mkdir %InstCWD%
mkdir %InstCWD%\netrek


if /I "%Compiler%" == "BCC" goto :CompilerBCC
if /I "%Compiler%" == "VCC" goto :CompilerVCC

echo You must set which compiler you have at the top of this batch file.
goto :end

:CompilerVCC
pushd src
%MyCWD%\tools\mkkey -k %RSAKEY%
popd

rem pushd src\cursors
rem cscript %MyCWD%\tools\vcmake.vbs curslib.dsw "Win32 Release"
rem popd

rem pushd src\winkey
rem cscript %MyCWD%\tools\vcmake.vbs winkey.dsw "Win32 Release"
rem popd

rem cscript %MyCWD%\tools\vcmake.vbs clientr.dsw "Win32 Release"

pushd %InstCWD%\netrek
%MyCWD%\tools\stampver -v%MyCWD%\src\stampver.inf netrek.exe
popd
goto :dist

:CompilerBCC
pushd src
pushd cursors
make
popd
make
%MyCWD%\tools\stampver -v%MyCWD%\src\stampver.inf netrek.exe
popd

goto :dist

:dist

if /I "%Compiler%" == "BCC" copy src\netrek.exe %InstCWD%\netrek
if /I "%Compiler%" == "BCC" copy src\cursors\curslib.dll %InstCWD%\netrek\bitmaps

goto end

:clean
del resources\ships\color\fed\fedship.bmp resources\ships\color\fed\fed_??.bmp
del resources\ships\color\ind\indship.bmp resources\ships\color\ind\ind_??.bmp
del resources\ships\color\kli\kliship.bmp resources\ships\color\kli\kli_??.bmp
del resources\ships\color\ori\oriship.bmp resources\ships\color\ori\ori_??.bmp
del resources\ships\color\rom\romship.bmp resources\ships\color\rom\rom_??.bmp

del resources\ships\color1\fed\fedship1.bmp resources\ships\color1\fed\fed_??.bmp
del resources\ships\color1\ind\indship1.bmp resources\ships\color1\ind\ind_??.bmp
del resources\ships\color1\kli\kliship1.bmp resources\ships\color1\kli\kli_??.bmp
del resources\ships\color1\ori\oriship1.bmp resources\ships\color1\ori\ori_??.bmp
del resources\ships\color1\rom\romship1.bmp resources\ships\color1\rom\rom_??.bmp

del resources\ships\mono\fed\fedshipM.bmp resources\ships\mono\fed\fed_??.bmp
del resources\ships\mono\ind\indshipM.bmp resources\ships\mono\ind\ind_??.bmp
del resources\ships\mono\kli\klishipM.bmp resources\ships\mono\kli\kli_??.bmp
del resources\ships\mono\ori\orishipM.bmp resources\ships\mono\ori\ori_??.bmp
del resources\ships\mono\rom\romshipM.bmp resources\ships\mono\rom\rom_??.bmp

del resources\ships\grayscale\fed\fedshipG.bmp resources\ships\grayscale\fed\fed_??.bmp
del resources\ships\grayscale\ind\indshipG.bmp resources\ships\grayscale\ind\ind_??.bmp
del resources\ships\grayscale\kli\klishipG.bmp resources\ships\grayscale\kli\kli_??.bmp
del resources\ships\grayscale\ori\orishipG.bmp resources\ships\grayscale\ori\ori_??.bmp
del resources\ships\grayscale\rom\romshipG.bmp resources\ships\grayscale\rom\rom_??.bmp

del resources\ships\tinted\fed\fedshipT.bmp resources\ships\tinted\fed\fed_??.bmp
del resources\ships\tinted\ind\indshipT.bmp resources\ships\tinted\ind\ind_??.bmp
del resources\ships\tinted\kli\klishipT.bmp resources\ships\tinted\kli\kli_??.bmp
del resources\ships\tinted\ori\orishipT.bmp resources\ships\tinted\ori\ori_??.bmp
del resources\ships\tinted\rom\romshipT.bmp resources\ships\tinted\rom\rom_??.bmp

del resources\planlibm\bronco\plan.bmp
del resources\planlibm\rabbit\planR.bmp
del resources\planlibm\moo\planM.bmp
del resources\planlibm\bronco\mplan.bmp
del resources\planlibm\rabbit\mplanR.bmp
del resources\planlibm\moo\mplanM.bmp

del resources\misclib\mono\sbexplM.bmp
del resources\misclib\mono\shexplM.bmp
del resources\misclib\mono\vshield.bmp
del resources\misclib\color\sbexpl.bmp
del resources\misclib\color\shexpl.bmp

if /I "%Compiler%" == "BCC" goto :CleanBCC
if /I "%Compiler%" == "VCC" goto :CleanVCC
echo You must set which compiler you have at the top of this batch file.
goto :end

:CleanVCC
pushd src
del rsa*.c
pushd cursors
del *.ncb
del *.opt
popd
pushd winkey
del *.ncb
del *.opt
popd
popd
del *.ncb
del *.opt

goto :end

:CleanBCC
pushd src
del *.obj *.res netrek.exe netrek.tds netrek.map
pushd cursors
del curslib.ilf curslib.ilc curslib.ild curslib.ils curslib.tds curslib.dll
popd
pushd winkey
del winkey.ilf winkey.ilc winkey.ild winkey.ils winkey.tds winkey.dll
popd
popd

:end
