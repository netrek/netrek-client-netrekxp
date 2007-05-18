set RSAKey="\netrek_files\rsa\NetrekXP-2006v3-RSA-Key-Win32.secret"
set InstCWD="\netrek_files"
set HelpCompiler="C:\Program Files\HTML Help Workshop\hhc.exe"
rem set Compiler=VCC
rem set Compiler=BCC
set Compiler=CYGWIN

@echo off
Set MyCWD=%cd%
if /I "%1" == "clean" goto :clean
if /I "%1" == "cleanrsa" goto :cleanrsa
if /I "%1" == "dist" goto :dist
if /I "%1" == "build" goto :build
if /I "%1" == "pics" goto :pics

mkdir %InstCWD%
mkdir %InstCWD%\netrek

:pics

pushd resources\ships\color\fed
echo Making resources\ships\color\fed...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_as01.bmp fed_as02.bmp fed_as03.bmp fed_as04.bmp fed_as05.bmp fed_as06.bmp fed_as07.bmp fed_as08.bmp fed_as09.bmp fed_as10.bmp fed_as11.bmp fed_as12.bmp fed_as13.bmp fed_as14.bmp fed_as15.bmp fed_as16.bmp fed_as17.bmp fed_as18.bmp fed_as19.bmp fed_as20.bmp fed_as21.bmp fed_as22.bmp fed_as23.bmp fed_as24.bmp fed_as25.bmp fed_as26.bmp fed_as27.bmp fed_as28.bmp fed_as29.bmp fed_as30.bmp fed_as31.bmp fed_as32.bmp fed_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_at01.bmp fed_at02.bmp fed_at03.bmp fed_at04.bmp fed_at05.bmp fed_at06.bmp fed_at07.bmp fed_at08.bmp fed_at09.bmp fed_at10.bmp fed_at11.bmp fed_at12.bmp fed_at13.bmp fed_at14.bmp fed_at15.bmp fed_at16.bmp fed_at17.bmp fed_at18.bmp fed_at19.bmp fed_at20.bmp fed_at21.bmp fed_at22.bmp fed_at23.bmp fed_at24.bmp fed_at25.bmp fed_at26.bmp fed_at27.bmp fed_at28.bmp fed_at29.bmp fed_at30.bmp fed_at31.bmp fed_at32.bmp fed_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_bb01.bmp fed_bb02.bmp fed_bb03.bmp fed_bb04.bmp fed_bb05.bmp fed_bb06.bmp fed_bb07.bmp fed_bb08.bmp fed_bb09.bmp fed_bb10.bmp fed_bb11.bmp fed_bb12.bmp fed_bb13.bmp fed_bb14.bmp fed_bb15.bmp fed_bb16.bmp fed_bb17.bmp fed_bb18.bmp fed_bb19.bmp fed_bb20.bmp fed_bb21.bmp fed_bb22.bmp fed_bb23.bmp fed_bb24.bmp fed_bb25.bmp fed_bb26.bmp fed_bb27.bmp fed_bb28.bmp fed_bb29.bmp fed_bb30.bmp fed_bb31.bmp fed_bb32.bmp fed_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ca01.bmp fed_ca02.bmp fed_ca03.bmp fed_ca04.bmp fed_ca05.bmp fed_ca06.bmp fed_ca07.bmp fed_ca08.bmp fed_ca09.bmp fed_ca10.bmp fed_ca11.bmp fed_ca12.bmp fed_ca13.bmp fed_ca14.bmp fed_ca15.bmp fed_ca16.bmp fed_ca17.bmp fed_ca18.bmp fed_ca19.bmp fed_ca20.bmp fed_ca21.bmp fed_ca22.bmp fed_ca23.bmp fed_ca24.bmp fed_ca25.bmp fed_ca26.bmp fed_ca27.bmp fed_ca28.bmp fed_ca29.bmp fed_ca30.bmp fed_ca31.bmp fed_ca32.bmp fed_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_dd01.bmp fed_dd02.bmp fed_dd03.bmp fed_dd04.bmp fed_dd05.bmp fed_dd06.bmp fed_dd07.bmp fed_dd08.bmp fed_dd09.bmp fed_dd10.bmp fed_dd11.bmp fed_dd12.bmp fed_dd13.bmp fed_dd14.bmp fed_dd15.bmp fed_dd16.bmp fed_dd17.bmp fed_dd18.bmp fed_dd19.bmp fed_dd20.bmp fed_dd21.bmp fed_dd22.bmp fed_dd23.bmp fed_dd24.bmp fed_dd25.bmp fed_dd26.bmp fed_dd27.bmp fed_dd28.bmp fed_dd29.bmp fed_dd30.bmp fed_dd31.bmp fed_dd32.bmp fed_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ga01.bmp fed_ga02.bmp fed_ga03.bmp fed_ga04.bmp fed_ga05.bmp fed_ga06.bmp fed_ga07.bmp fed_ga08.bmp fed_ga09.bmp fed_ga10.bmp fed_ga11.bmp fed_ga12.bmp fed_ga13.bmp fed_ga14.bmp fed_ga15.bmp fed_ga16.bmp fed_ga17.bmp fed_ga18.bmp fed_ga19.bmp fed_ga20.bmp fed_ga21.bmp fed_ga22.bmp fed_ga23.bmp fed_ga24.bmp fed_ga25.bmp fed_ga26.bmp fed_ga27.bmp fed_ga28.bmp fed_ga29.bmp fed_ga30.bmp fed_ga31.bmp fed_ga32.bmp fed_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sb01.bmp fed_sb02.bmp fed_sb03.bmp fed_sb04.bmp fed_sb05.bmp fed_sb06.bmp fed_sb07.bmp fed_sb08.bmp fed_sb09.bmp fed_sb10.bmp fed_sb11.bmp fed_sb12.bmp fed_sb13.bmp fed_sb14.bmp fed_sb15.bmp fed_sb16.bmp fed_sb17.bmp fed_sb18.bmp fed_sb19.bmp fed_sb20.bmp fed_sb21.bmp fed_sb22.bmp fed_sb23.bmp fed_sb24.bmp fed_sb25.bmp fed_sb26.bmp fed_sb27.bmp fed_sb28.bmp fed_sb29.bmp fed_sb30.bmp fed_sb31.bmp fed_sb32.bmp fed_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sc01.bmp fed_sc02.bmp fed_sc03.bmp fed_sc04.bmp fed_sc05.bmp fed_sc06.bmp fed_sc07.bmp fed_sc08.bmp fed_sc09.bmp fed_sc10.bmp fed_sc11.bmp fed_sc12.bmp fed_sc13.bmp fed_sc14.bmp fed_sc15.bmp fed_sc16.bmp fed_sc17.bmp fed_sc18.bmp fed_sc19.bmp fed_sc20.bmp fed_sc21.bmp fed_sc22.bmp fed_sc23.bmp fed_sc24.bmp fed_sc25.bmp fed_sc26.bmp fed_sc27.bmp fed_sc28.bmp fed_sc29.bmp fed_sc30.bmp fed_sc31.bmp fed_sc32.bmp fed_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 fed_sc.bmp fed_dd.bmp fed_ca.bmp fed_bb.bmp fed_as.bmp  fed_sb.bmp   fed_ga.bmp  fed_at.bmp fedship.bmp
popd
pushd resources\ships\color\ind
echo Making resources\ships\color\ind...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_as01.bmp ind_as02.bmp ind_as03.bmp ind_as04.bmp ind_as05.bmp ind_as06.bmp ind_as07.bmp ind_as08.bmp ind_as09.bmp ind_as10.bmp ind_as11.bmp ind_as12.bmp ind_as13.bmp ind_as14.bmp ind_as15.bmp ind_as16.bmp ind_as17.bmp ind_as18.bmp ind_as19.bmp ind_as20.bmp ind_as21.bmp ind_as22.bmp ind_as23.bmp ind_as24.bmp ind_as25.bmp ind_as26.bmp ind_as27.bmp ind_as28.bmp ind_as29.bmp ind_as30.bmp ind_as31.bmp ind_as32.bmp ind_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_at01.bmp ind_at02.bmp ind_at03.bmp ind_at04.bmp ind_at05.bmp ind_at06.bmp ind_at07.bmp ind_at08.bmp ind_at09.bmp ind_at10.bmp ind_at11.bmp ind_at12.bmp ind_at13.bmp ind_at14.bmp ind_at15.bmp ind_at16.bmp ind_at17.bmp ind_at18.bmp ind_at19.bmp ind_at20.bmp ind_at21.bmp ind_at22.bmp ind_at23.bmp ind_at24.bmp ind_at25.bmp ind_at26.bmp ind_at27.bmp ind_at28.bmp ind_at29.bmp ind_at30.bmp ind_at31.bmp ind_at32.bmp ind_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_bb01.bmp ind_bb02.bmp ind_bb03.bmp ind_bb04.bmp ind_bb05.bmp ind_bb06.bmp ind_bb07.bmp ind_bb08.bmp ind_bb09.bmp ind_bb10.bmp ind_bb11.bmp ind_bb12.bmp ind_bb13.bmp ind_bb14.bmp ind_bb15.bmp ind_bb16.bmp ind_bb17.bmp ind_bb18.bmp ind_bb19.bmp ind_bb20.bmp ind_bb21.bmp ind_bb22.bmp ind_bb23.bmp ind_bb24.bmp ind_bb25.bmp ind_bb26.bmp ind_bb27.bmp ind_bb28.bmp ind_bb29.bmp ind_bb30.bmp ind_bb31.bmp ind_bb32.bmp ind_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ca01.bmp ind_ca02.bmp ind_ca03.bmp ind_ca04.bmp ind_ca05.bmp ind_ca06.bmp ind_ca07.bmp ind_ca08.bmp ind_ca09.bmp ind_ca10.bmp ind_ca11.bmp ind_ca12.bmp ind_ca13.bmp ind_ca14.bmp ind_ca15.bmp ind_ca16.bmp ind_ca17.bmp ind_ca18.bmp ind_ca19.bmp ind_ca20.bmp ind_ca21.bmp ind_ca22.bmp ind_ca23.bmp ind_ca24.bmp ind_ca25.bmp ind_ca26.bmp ind_ca27.bmp ind_ca28.bmp ind_ca29.bmp ind_ca30.bmp ind_ca31.bmp ind_ca32.bmp ind_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_dd01.bmp ind_dd02.bmp ind_dd03.bmp ind_dd04.bmp ind_dd05.bmp ind_dd06.bmp ind_dd07.bmp ind_dd08.bmp ind_dd09.bmp ind_dd10.bmp ind_dd11.bmp ind_dd12.bmp ind_dd13.bmp ind_dd14.bmp ind_dd15.bmp ind_dd16.bmp ind_dd17.bmp ind_dd18.bmp ind_dd19.bmp ind_dd20.bmp ind_dd21.bmp ind_dd22.bmp ind_dd23.bmp ind_dd24.bmp ind_dd25.bmp ind_dd26.bmp ind_dd27.bmp ind_dd28.bmp ind_dd29.bmp ind_dd30.bmp ind_dd31.bmp ind_dd32.bmp ind_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ga01.bmp ind_ga02.bmp ind_ga03.bmp ind_ga04.bmp ind_ga05.bmp ind_ga06.bmp ind_ga07.bmp ind_ga08.bmp ind_ga09.bmp ind_ga10.bmp ind_ga11.bmp ind_ga12.bmp ind_ga13.bmp ind_ga14.bmp ind_ga15.bmp ind_ga16.bmp ind_ga17.bmp ind_ga18.bmp ind_ga19.bmp ind_ga20.bmp ind_ga21.bmp ind_ga22.bmp ind_ga23.bmp ind_ga24.bmp ind_ga25.bmp ind_ga26.bmp ind_ga27.bmp ind_ga28.bmp ind_ga29.bmp ind_ga30.bmp ind_ga31.bmp ind_ga32.bmp ind_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sb01.bmp ind_sb02.bmp ind_sb03.bmp ind_sb04.bmp ind_sb05.bmp ind_sb06.bmp ind_sb07.bmp ind_sb08.bmp ind_sb09.bmp ind_sb10.bmp ind_sb11.bmp ind_sb12.bmp ind_sb13.bmp ind_sb14.bmp ind_sb15.bmp ind_sb16.bmp ind_sb17.bmp ind_sb18.bmp ind_sb19.bmp ind_sb20.bmp ind_sb21.bmp ind_sb22.bmp ind_sb23.bmp ind_sb24.bmp ind_sb25.bmp ind_sb26.bmp ind_sb27.bmp ind_sb28.bmp ind_sb29.bmp ind_sb30.bmp ind_sb31.bmp ind_sb32.bmp ind_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sc01.bmp ind_sc02.bmp ind_sc03.bmp ind_sc04.bmp ind_sc05.bmp ind_sc06.bmp ind_sc07.bmp ind_sc08.bmp ind_sc09.bmp ind_sc10.bmp ind_sc11.bmp ind_sc12.bmp ind_sc13.bmp ind_sc14.bmp ind_sc15.bmp ind_sc16.bmp ind_sc17.bmp ind_sc18.bmp ind_sc19.bmp ind_sc20.bmp ind_sc21.bmp ind_sc22.bmp ind_sc23.bmp ind_sc24.bmp ind_sc25.bmp ind_sc26.bmp ind_sc27.bmp ind_sc28.bmp ind_sc29.bmp ind_sc30.bmp ind_sc31.bmp ind_sc32.bmp ind_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ind_sc.bmp ind_dd.bmp ind_ca.bmp ind_bb.bmp ind_as.bmp  ind_sb.bmp   ind_ga.bmp  ind_at.bmp indship.bmp
popd
pushd resources\ships\color\kli
echo Making resources\ships\color\kli...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_as01.bmp kli_as02.bmp kli_as03.bmp kli_as04.bmp kli_as05.bmp kli_as06.bmp kli_as07.bmp kli_as08.bmp kli_as09.bmp kli_as10.bmp kli_as11.bmp kli_as12.bmp kli_as13.bmp kli_as14.bmp kli_as15.bmp kli_as16.bmp kli_as17.bmp kli_as18.bmp kli_as19.bmp kli_as20.bmp kli_as21.bmp kli_as22.bmp kli_as23.bmp kli_as24.bmp kli_as25.bmp kli_as26.bmp kli_as27.bmp kli_as28.bmp kli_as29.bmp kli_as30.bmp kli_as31.bmp kli_as32.bmp kli_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_at01.bmp kli_at02.bmp kli_at03.bmp kli_at04.bmp kli_at05.bmp kli_at06.bmp kli_at07.bmp kli_at08.bmp kli_at09.bmp kli_at10.bmp kli_at11.bmp kli_at12.bmp kli_at13.bmp kli_at14.bmp kli_at15.bmp kli_at16.bmp kli_at17.bmp kli_at18.bmp kli_at19.bmp kli_at20.bmp kli_at21.bmp kli_at22.bmp kli_at23.bmp kli_at24.bmp kli_at25.bmp kli_at26.bmp kli_at27.bmp kli_at28.bmp kli_at29.bmp kli_at30.bmp kli_at31.bmp kli_at32.bmp kli_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_bb01.bmp kli_bb02.bmp kli_bb03.bmp kli_bb04.bmp kli_bb05.bmp kli_bb06.bmp kli_bb07.bmp kli_bb08.bmp kli_bb09.bmp kli_bb10.bmp kli_bb11.bmp kli_bb12.bmp kli_bb13.bmp kli_bb14.bmp kli_bb15.bmp kli_bb16.bmp kli_bb17.bmp kli_bb18.bmp kli_bb19.bmp kli_bb20.bmp kli_bb21.bmp kli_bb22.bmp kli_bb23.bmp kli_bb24.bmp kli_bb25.bmp kli_bb26.bmp kli_bb27.bmp kli_bb28.bmp kli_bb29.bmp kli_bb30.bmp kli_bb31.bmp kli_bb32.bmp kli_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ca01.bmp kli_ca02.bmp kli_ca03.bmp kli_ca04.bmp kli_ca05.bmp kli_ca06.bmp kli_ca07.bmp kli_ca08.bmp kli_ca09.bmp kli_ca10.bmp kli_ca11.bmp kli_ca12.bmp kli_ca13.bmp kli_ca14.bmp kli_ca15.bmp kli_ca16.bmp kli_ca17.bmp kli_ca18.bmp kli_ca19.bmp kli_ca20.bmp kli_ca21.bmp kli_ca22.bmp kli_ca23.bmp kli_ca24.bmp kli_ca25.bmp kli_ca26.bmp kli_ca27.bmp kli_ca28.bmp kli_ca29.bmp kli_ca30.bmp kli_ca31.bmp kli_ca32.bmp kli_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_dd01.bmp kli_dd02.bmp kli_dd03.bmp kli_dd04.bmp kli_dd05.bmp kli_dd06.bmp kli_dd07.bmp kli_dd08.bmp kli_dd09.bmp kli_dd10.bmp kli_dd11.bmp kli_dd12.bmp kli_dd13.bmp kli_dd14.bmp kli_dd15.bmp kli_dd16.bmp kli_dd17.bmp kli_dd18.bmp kli_dd19.bmp kli_dd20.bmp kli_dd21.bmp kli_dd22.bmp kli_dd23.bmp kli_dd24.bmp kli_dd25.bmp kli_dd26.bmp kli_dd27.bmp kli_dd28.bmp kli_dd29.bmp kli_dd30.bmp kli_dd31.bmp kli_dd32.bmp kli_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ga01.bmp kli_ga02.bmp kli_ga03.bmp kli_ga04.bmp kli_ga05.bmp kli_ga06.bmp kli_ga07.bmp kli_ga08.bmp kli_ga09.bmp kli_ga10.bmp kli_ga11.bmp kli_ga12.bmp kli_ga13.bmp kli_ga14.bmp kli_ga15.bmp kli_ga16.bmp kli_ga17.bmp kli_ga18.bmp kli_ga19.bmp kli_ga20.bmp kli_ga21.bmp kli_ga22.bmp kli_ga23.bmp kli_ga24.bmp kli_ga25.bmp kli_ga26.bmp kli_ga27.bmp kli_ga28.bmp kli_ga29.bmp kli_ga30.bmp kli_ga31.bmp kli_ga32.bmp kli_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sb01.bmp kli_sb02.bmp kli_sb03.bmp kli_sb04.bmp kli_sb05.bmp kli_sb06.bmp kli_sb07.bmp kli_sb08.bmp kli_sb09.bmp kli_sb10.bmp kli_sb11.bmp kli_sb12.bmp kli_sb13.bmp kli_sb14.bmp kli_sb15.bmp kli_sb16.bmp kli_sb17.bmp kli_sb18.bmp kli_sb19.bmp kli_sb20.bmp kli_sb21.bmp kli_sb22.bmp kli_sb23.bmp kli_sb24.bmp kli_sb25.bmp kli_sb26.bmp kli_sb27.bmp kli_sb28.bmp kli_sb29.bmp kli_sb30.bmp kli_sb31.bmp kli_sb32.bmp kli_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sc01.bmp kli_sc02.bmp kli_sc03.bmp kli_sc04.bmp kli_sc05.bmp kli_sc06.bmp kli_sc07.bmp kli_sc08.bmp kli_sc09.bmp kli_sc10.bmp kli_sc11.bmp kli_sc12.bmp kli_sc13.bmp kli_sc14.bmp kli_sc15.bmp kli_sc16.bmp kli_sc17.bmp kli_sc18.bmp kli_sc19.bmp kli_sc20.bmp kli_sc21.bmp kli_sc22.bmp kli_sc23.bmp kli_sc24.bmp kli_sc25.bmp kli_sc26.bmp kli_sc27.bmp kli_sc28.bmp kli_sc29.bmp kli_sc30.bmp kli_sc31.bmp kli_sc32.bmp kli_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 kli_sc.bmp kli_dd.bmp kli_ca.bmp kli_bb.bmp kli_as.bmp  kli_sb.bmp   kli_ga.bmp  kli_at.bmp kliship.bmp
popd
pushd resources\ships\color\ori
echo Making resources\ships\color\ori...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_as01.bmp ori_as02.bmp ori_as03.bmp ori_as04.bmp ori_as05.bmp ori_as06.bmp ori_as07.bmp ori_as08.bmp ori_as09.bmp ori_as10.bmp ori_as11.bmp ori_as12.bmp ori_as13.bmp ori_as14.bmp ori_as15.bmp ori_as16.bmp ori_as17.bmp ori_as18.bmp ori_as19.bmp ori_as20.bmp ori_as21.bmp ori_as22.bmp ori_as23.bmp ori_as24.bmp ori_as25.bmp ori_as26.bmp ori_as27.bmp ori_as28.bmp ori_as29.bmp ori_as30.bmp ori_as31.bmp ori_as32.bmp ori_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_at01.bmp ori_at02.bmp ori_at03.bmp ori_at04.bmp ori_at05.bmp ori_at06.bmp ori_at07.bmp ori_at08.bmp ori_at09.bmp ori_at10.bmp ori_at11.bmp ori_at12.bmp ori_at13.bmp ori_at14.bmp ori_at15.bmp ori_at16.bmp ori_at17.bmp ori_at18.bmp ori_at19.bmp ori_at20.bmp ori_at21.bmp ori_at22.bmp ori_at23.bmp ori_at24.bmp ori_at25.bmp ori_at26.bmp ori_at27.bmp ori_at28.bmp ori_at29.bmp ori_at30.bmp ori_at31.bmp ori_at32.bmp ori_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_bb01.bmp ori_bb02.bmp ori_bb03.bmp ori_bb04.bmp ori_bb05.bmp ori_bb06.bmp ori_bb07.bmp ori_bb08.bmp ori_bb09.bmp ori_bb10.bmp ori_bb11.bmp ori_bb12.bmp ori_bb13.bmp ori_bb14.bmp ori_bb15.bmp ori_bb16.bmp ori_bb17.bmp ori_bb18.bmp ori_bb19.bmp ori_bb20.bmp ori_bb21.bmp ori_bb22.bmp ori_bb23.bmp ori_bb24.bmp ori_bb25.bmp ori_bb26.bmp ori_bb27.bmp ori_bb28.bmp ori_bb29.bmp ori_bb30.bmp ori_bb31.bmp ori_bb32.bmp ori_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ca01.bmp ori_ca02.bmp ori_ca03.bmp ori_ca04.bmp ori_ca05.bmp ori_ca06.bmp ori_ca07.bmp ori_ca08.bmp ori_ca09.bmp ori_ca10.bmp ori_ca11.bmp ori_ca12.bmp ori_ca13.bmp ori_ca14.bmp ori_ca15.bmp ori_ca16.bmp ori_ca17.bmp ori_ca18.bmp ori_ca19.bmp ori_ca20.bmp ori_ca21.bmp ori_ca22.bmp ori_ca23.bmp ori_ca24.bmp ori_ca25.bmp ori_ca26.bmp ori_ca27.bmp ori_ca28.bmp ori_ca29.bmp ori_ca30.bmp ori_ca31.bmp ori_ca32.bmp ori_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_dd01.bmp ori_dd02.bmp ori_dd03.bmp ori_dd04.bmp ori_dd05.bmp ori_dd06.bmp ori_dd07.bmp ori_dd08.bmp ori_dd09.bmp ori_dd10.bmp ori_dd11.bmp ori_dd12.bmp ori_dd13.bmp ori_dd14.bmp ori_dd15.bmp ori_dd16.bmp ori_dd17.bmp ori_dd18.bmp ori_dd19.bmp ori_dd20.bmp ori_dd21.bmp ori_dd22.bmp ori_dd23.bmp ori_dd24.bmp ori_dd25.bmp ori_dd26.bmp ori_dd27.bmp ori_dd28.bmp ori_dd29.bmp ori_dd30.bmp ori_dd31.bmp ori_dd32.bmp ori_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ga01.bmp ori_ga02.bmp ori_ga03.bmp ori_ga04.bmp ori_ga05.bmp ori_ga06.bmp ori_ga07.bmp ori_ga08.bmp ori_ga09.bmp ori_ga10.bmp ori_ga11.bmp ori_ga12.bmp ori_ga13.bmp ori_ga14.bmp ori_ga15.bmp ori_ga16.bmp ori_ga17.bmp ori_ga18.bmp ori_ga19.bmp ori_ga20.bmp ori_ga21.bmp ori_ga22.bmp ori_ga23.bmp ori_ga24.bmp ori_ga25.bmp ori_ga26.bmp ori_ga27.bmp ori_ga28.bmp ori_ga29.bmp ori_ga30.bmp ori_ga31.bmp ori_ga32.bmp ori_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sb01.bmp ori_sb02.bmp ori_sb03.bmp ori_sb04.bmp ori_sb05.bmp ori_sb06.bmp ori_sb07.bmp ori_sb08.bmp ori_sb09.bmp ori_sb10.bmp ori_sb11.bmp ori_sb12.bmp ori_sb13.bmp ori_sb14.bmp ori_sb15.bmp ori_sb16.bmp ori_sb17.bmp ori_sb18.bmp ori_sb19.bmp ori_sb20.bmp ori_sb21.bmp ori_sb22.bmp ori_sb23.bmp ori_sb24.bmp ori_sb25.bmp ori_sb26.bmp ori_sb27.bmp ori_sb28.bmp ori_sb29.bmp ori_sb30.bmp ori_sb31.bmp ori_sb32.bmp ori_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sc01.bmp ori_sc02.bmp ori_sc03.bmp ori_sc04.bmp ori_sc05.bmp ori_sc06.bmp ori_sc07.bmp ori_sc08.bmp ori_sc09.bmp ori_sc10.bmp ori_sc11.bmp ori_sc12.bmp ori_sc13.bmp ori_sc14.bmp ori_sc15.bmp ori_sc16.bmp ori_sc17.bmp ori_sc18.bmp ori_sc19.bmp ori_sc20.bmp ori_sc21.bmp ori_sc22.bmp ori_sc23.bmp ori_sc24.bmp ori_sc25.bmp ori_sc26.bmp ori_sc27.bmp ori_sc28.bmp ori_sc29.bmp ori_sc30.bmp ori_sc31.bmp ori_sc32.bmp ori_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ori_sc.bmp ori_dd.bmp ori_ca.bmp ori_bb.bmp ori_as.bmp  ori_sb.bmp   ori_ga.bmp  ori_at.bmp oriship.bmp
popd
pushd resources\ships\color\rom
echo Making resources\ships\color\rom...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_as01.bmp rom_as02.bmp rom_as03.bmp rom_as04.bmp rom_as05.bmp rom_as06.bmp rom_as07.bmp rom_as08.bmp rom_as09.bmp rom_as10.bmp rom_as11.bmp rom_as12.bmp rom_as13.bmp rom_as14.bmp rom_as15.bmp rom_as16.bmp rom_as17.bmp rom_as18.bmp rom_as19.bmp rom_as20.bmp rom_as21.bmp rom_as22.bmp rom_as23.bmp rom_as24.bmp rom_as25.bmp rom_as26.bmp rom_as27.bmp rom_as28.bmp rom_as29.bmp rom_as30.bmp rom_as31.bmp rom_as32.bmp rom_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_at01.bmp rom_at02.bmp rom_at03.bmp rom_at04.bmp rom_at05.bmp rom_at06.bmp rom_at07.bmp rom_at08.bmp rom_at09.bmp rom_at10.bmp rom_at11.bmp rom_at12.bmp rom_at13.bmp rom_at14.bmp rom_at15.bmp rom_at16.bmp rom_at17.bmp rom_at18.bmp rom_at19.bmp rom_at20.bmp rom_at21.bmp rom_at22.bmp rom_at23.bmp rom_at24.bmp rom_at25.bmp rom_at26.bmp rom_at27.bmp rom_at28.bmp rom_at29.bmp rom_at30.bmp rom_at31.bmp rom_at32.bmp rom_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_bb01.bmp rom_bb02.bmp rom_bb03.bmp rom_bb04.bmp rom_bb05.bmp rom_bb06.bmp rom_bb07.bmp rom_bb08.bmp rom_bb09.bmp rom_bb10.bmp rom_bb11.bmp rom_bb12.bmp rom_bb13.bmp rom_bb14.bmp rom_bb15.bmp rom_bb16.bmp rom_bb17.bmp rom_bb18.bmp rom_bb19.bmp rom_bb20.bmp rom_bb21.bmp rom_bb22.bmp rom_bb23.bmp rom_bb24.bmp rom_bb25.bmp rom_bb26.bmp rom_bb27.bmp rom_bb28.bmp rom_bb29.bmp rom_bb30.bmp rom_bb31.bmp rom_bb32.bmp rom_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ca01.bmp rom_ca02.bmp rom_ca03.bmp rom_ca04.bmp rom_ca05.bmp rom_ca06.bmp rom_ca07.bmp rom_ca08.bmp rom_ca09.bmp rom_ca10.bmp rom_ca11.bmp rom_ca12.bmp rom_ca13.bmp rom_ca14.bmp rom_ca15.bmp rom_ca16.bmp rom_ca17.bmp rom_ca18.bmp rom_ca19.bmp rom_ca20.bmp rom_ca21.bmp rom_ca22.bmp rom_ca23.bmp rom_ca24.bmp rom_ca25.bmp rom_ca26.bmp rom_ca27.bmp rom_ca28.bmp rom_ca29.bmp rom_ca30.bmp rom_ca31.bmp rom_ca32.bmp rom_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_dd01.bmp rom_dd02.bmp rom_dd03.bmp rom_dd04.bmp rom_dd05.bmp rom_dd06.bmp rom_dd07.bmp rom_dd08.bmp rom_dd09.bmp rom_dd10.bmp rom_dd11.bmp rom_dd12.bmp rom_dd13.bmp rom_dd14.bmp rom_dd15.bmp rom_dd16.bmp rom_dd17.bmp rom_dd18.bmp rom_dd19.bmp rom_dd20.bmp rom_dd21.bmp rom_dd22.bmp rom_dd23.bmp rom_dd24.bmp rom_dd25.bmp rom_dd26.bmp rom_dd27.bmp rom_dd28.bmp rom_dd29.bmp rom_dd30.bmp rom_dd31.bmp rom_dd32.bmp rom_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ga01.bmp rom_ga02.bmp rom_ga03.bmp rom_ga04.bmp rom_ga05.bmp rom_ga06.bmp rom_ga07.bmp rom_ga08.bmp rom_ga09.bmp rom_ga10.bmp rom_ga11.bmp rom_ga12.bmp rom_ga13.bmp rom_ga14.bmp rom_ga15.bmp rom_ga16.bmp rom_ga17.bmp rom_ga18.bmp rom_ga19.bmp rom_ga20.bmp rom_ga21.bmp rom_ga22.bmp rom_ga23.bmp rom_ga24.bmp rom_ga25.bmp rom_ga26.bmp rom_ga27.bmp rom_ga28.bmp rom_ga29.bmp rom_ga30.bmp rom_ga31.bmp rom_ga32.bmp rom_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sb01.bmp rom_sb02.bmp rom_sb03.bmp rom_sb04.bmp rom_sb05.bmp rom_sb06.bmp rom_sb07.bmp rom_sb08.bmp rom_sb09.bmp rom_sb10.bmp rom_sb11.bmp rom_sb12.bmp rom_sb13.bmp rom_sb14.bmp rom_sb15.bmp rom_sb16.bmp rom_sb17.bmp rom_sb18.bmp rom_sb19.bmp rom_sb20.bmp rom_sb21.bmp rom_sb22.bmp rom_sb23.bmp rom_sb24.bmp rom_sb25.bmp rom_sb26.bmp rom_sb27.bmp rom_sb28.bmp rom_sb29.bmp rom_sb30.bmp rom_sb31.bmp rom_sb32.bmp rom_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sc01.bmp rom_sc02.bmp rom_sc03.bmp rom_sc04.bmp rom_sc05.bmp rom_sc06.bmp rom_sc07.bmp rom_sc08.bmp rom_sc09.bmp rom_sc10.bmp rom_sc11.bmp rom_sc12.bmp rom_sc13.bmp rom_sc14.bmp rom_sc15.bmp rom_sc16.bmp rom_sc17.bmp rom_sc18.bmp rom_sc19.bmp rom_sc20.bmp rom_sc21.bmp rom_sc22.bmp rom_sc23.bmp rom_sc24.bmp rom_sc25.bmp rom_sc26.bmp rom_sc27.bmp rom_sc28.bmp rom_sc29.bmp rom_sc30.bmp rom_sc31.bmp rom_sc32.bmp rom_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 rom_sc.bmp rom_dd.bmp rom_ca.bmp rom_bb.bmp rom_as.bmp  rom_sb.bmp   rom_ga.bmp  rom_at.bmp romship.bmp
popd

pushd resources\ships\color1\fed
echo Making resources\ships\color1\fed...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_as01.bmp fed_as02.bmp fed_as03.bmp fed_as04.bmp fed_as05.bmp fed_as06.bmp fed_as07.bmp fed_as08.bmp fed_as09.bmp fed_as10.bmp fed_as11.bmp fed_as12.bmp fed_as13.bmp fed_as14.bmp fed_as15.bmp fed_as16.bmp fed_as17.bmp fed_as18.bmp fed_as19.bmp fed_as20.bmp fed_as21.bmp fed_as22.bmp fed_as23.bmp fed_as24.bmp fed_as25.bmp fed_as26.bmp fed_as27.bmp fed_as28.bmp fed_as29.bmp fed_as30.bmp fed_as31.bmp fed_as32.bmp fed_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_at01.bmp fed_at02.bmp fed_at03.bmp fed_at04.bmp fed_at05.bmp fed_at06.bmp fed_at07.bmp fed_at08.bmp fed_at09.bmp fed_at10.bmp fed_at11.bmp fed_at12.bmp fed_at13.bmp fed_at14.bmp fed_at15.bmp fed_at16.bmp fed_at17.bmp fed_at18.bmp fed_at19.bmp fed_at20.bmp fed_at21.bmp fed_at22.bmp fed_at23.bmp fed_at24.bmp fed_at25.bmp fed_at26.bmp fed_at27.bmp fed_at28.bmp fed_at29.bmp fed_at30.bmp fed_at31.bmp fed_at32.bmp fed_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_bb01.bmp fed_bb02.bmp fed_bb03.bmp fed_bb04.bmp fed_bb05.bmp fed_bb06.bmp fed_bb07.bmp fed_bb08.bmp fed_bb09.bmp fed_bb10.bmp fed_bb11.bmp fed_bb12.bmp fed_bb13.bmp fed_bb14.bmp fed_bb15.bmp fed_bb16.bmp fed_bb17.bmp fed_bb18.bmp fed_bb19.bmp fed_bb20.bmp fed_bb21.bmp fed_bb22.bmp fed_bb23.bmp fed_bb24.bmp fed_bb25.bmp fed_bb26.bmp fed_bb27.bmp fed_bb28.bmp fed_bb29.bmp fed_bb30.bmp fed_bb31.bmp fed_bb32.bmp fed_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ca01.bmp fed_ca02.bmp fed_ca03.bmp fed_ca04.bmp fed_ca05.bmp fed_ca06.bmp fed_ca07.bmp fed_ca08.bmp fed_ca09.bmp fed_ca10.bmp fed_ca11.bmp fed_ca12.bmp fed_ca13.bmp fed_ca14.bmp fed_ca15.bmp fed_ca16.bmp fed_ca17.bmp fed_ca18.bmp fed_ca19.bmp fed_ca20.bmp fed_ca21.bmp fed_ca22.bmp fed_ca23.bmp fed_ca24.bmp fed_ca25.bmp fed_ca26.bmp fed_ca27.bmp fed_ca28.bmp fed_ca29.bmp fed_ca30.bmp fed_ca31.bmp fed_ca32.bmp fed_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_dd01.bmp fed_dd02.bmp fed_dd03.bmp fed_dd04.bmp fed_dd05.bmp fed_dd06.bmp fed_dd07.bmp fed_dd08.bmp fed_dd09.bmp fed_dd10.bmp fed_dd11.bmp fed_dd12.bmp fed_dd13.bmp fed_dd14.bmp fed_dd15.bmp fed_dd16.bmp fed_dd17.bmp fed_dd18.bmp fed_dd19.bmp fed_dd20.bmp fed_dd21.bmp fed_dd22.bmp fed_dd23.bmp fed_dd24.bmp fed_dd25.bmp fed_dd26.bmp fed_dd27.bmp fed_dd28.bmp fed_dd29.bmp fed_dd30.bmp fed_dd31.bmp fed_dd32.bmp fed_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ga01.bmp fed_ga02.bmp fed_ga03.bmp fed_ga04.bmp fed_ga05.bmp fed_ga06.bmp fed_ga07.bmp fed_ga08.bmp fed_ga09.bmp fed_ga10.bmp fed_ga11.bmp fed_ga12.bmp fed_ga13.bmp fed_ga14.bmp fed_ga15.bmp fed_ga16.bmp fed_ga17.bmp fed_ga18.bmp fed_ga19.bmp fed_ga20.bmp fed_ga21.bmp fed_ga22.bmp fed_ga23.bmp fed_ga24.bmp fed_ga25.bmp fed_ga26.bmp fed_ga27.bmp fed_ga28.bmp fed_ga29.bmp fed_ga30.bmp fed_ga31.bmp fed_ga32.bmp fed_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sb01.bmp fed_sb02.bmp fed_sb03.bmp fed_sb04.bmp fed_sb05.bmp fed_sb06.bmp fed_sb07.bmp fed_sb08.bmp fed_sb09.bmp fed_sb10.bmp fed_sb11.bmp fed_sb12.bmp fed_sb13.bmp fed_sb14.bmp fed_sb15.bmp fed_sb16.bmp fed_sb17.bmp fed_sb18.bmp fed_sb19.bmp fed_sb20.bmp fed_sb21.bmp fed_sb22.bmp fed_sb23.bmp fed_sb24.bmp fed_sb25.bmp fed_sb26.bmp fed_sb27.bmp fed_sb28.bmp fed_sb29.bmp fed_sb30.bmp fed_sb31.bmp fed_sb32.bmp fed_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sc01.bmp fed_sc02.bmp fed_sc03.bmp fed_sc04.bmp fed_sc05.bmp fed_sc06.bmp fed_sc07.bmp fed_sc08.bmp fed_sc09.bmp fed_sc10.bmp fed_sc11.bmp fed_sc12.bmp fed_sc13.bmp fed_sc14.bmp fed_sc15.bmp fed_sc16.bmp fed_sc17.bmp fed_sc18.bmp fed_sc19.bmp fed_sc20.bmp fed_sc21.bmp fed_sc22.bmp fed_sc23.bmp fed_sc24.bmp fed_sc25.bmp fed_sc26.bmp fed_sc27.bmp fed_sc28.bmp fed_sc29.bmp fed_sc30.bmp fed_sc31.bmp fed_sc32.bmp fed_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 fed_sc.bmp fed_dd.bmp fed_ca.bmp fed_bb.bmp fed_as.bmp  fed_sb.bmp   fed_ga.bmp  fed_at.bmp fedship1.bmp
popd
pushd resources\ships\color1\ind
echo Making resources\ships\color1\ind...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_as01.bmp ind_as02.bmp ind_as03.bmp ind_as04.bmp ind_as05.bmp ind_as06.bmp ind_as07.bmp ind_as08.bmp ind_as09.bmp ind_as10.bmp ind_as11.bmp ind_as12.bmp ind_as13.bmp ind_as14.bmp ind_as15.bmp ind_as16.bmp ind_as17.bmp ind_as18.bmp ind_as19.bmp ind_as20.bmp ind_as21.bmp ind_as22.bmp ind_as23.bmp ind_as24.bmp ind_as25.bmp ind_as26.bmp ind_as27.bmp ind_as28.bmp ind_as29.bmp ind_as30.bmp ind_as31.bmp ind_as32.bmp ind_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_at01.bmp ind_at02.bmp ind_at03.bmp ind_at04.bmp ind_at05.bmp ind_at06.bmp ind_at07.bmp ind_at08.bmp ind_at09.bmp ind_at10.bmp ind_at11.bmp ind_at12.bmp ind_at13.bmp ind_at14.bmp ind_at15.bmp ind_at16.bmp ind_at17.bmp ind_at18.bmp ind_at19.bmp ind_at20.bmp ind_at21.bmp ind_at22.bmp ind_at23.bmp ind_at24.bmp ind_at25.bmp ind_at26.bmp ind_at27.bmp ind_at28.bmp ind_at29.bmp ind_at30.bmp ind_at31.bmp ind_at32.bmp ind_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_bb01.bmp ind_bb02.bmp ind_bb03.bmp ind_bb04.bmp ind_bb05.bmp ind_bb06.bmp ind_bb07.bmp ind_bb08.bmp ind_bb09.bmp ind_bb10.bmp ind_bb11.bmp ind_bb12.bmp ind_bb13.bmp ind_bb14.bmp ind_bb15.bmp ind_bb16.bmp ind_bb17.bmp ind_bb18.bmp ind_bb19.bmp ind_bb20.bmp ind_bb21.bmp ind_bb22.bmp ind_bb23.bmp ind_bb24.bmp ind_bb25.bmp ind_bb26.bmp ind_bb27.bmp ind_bb28.bmp ind_bb29.bmp ind_bb30.bmp ind_bb31.bmp ind_bb32.bmp ind_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ca01.bmp ind_ca02.bmp ind_ca03.bmp ind_ca04.bmp ind_ca05.bmp ind_ca06.bmp ind_ca07.bmp ind_ca08.bmp ind_ca09.bmp ind_ca10.bmp ind_ca11.bmp ind_ca12.bmp ind_ca13.bmp ind_ca14.bmp ind_ca15.bmp ind_ca16.bmp ind_ca17.bmp ind_ca18.bmp ind_ca19.bmp ind_ca20.bmp ind_ca21.bmp ind_ca22.bmp ind_ca23.bmp ind_ca24.bmp ind_ca25.bmp ind_ca26.bmp ind_ca27.bmp ind_ca28.bmp ind_ca29.bmp ind_ca30.bmp ind_ca31.bmp ind_ca32.bmp ind_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_dd01.bmp ind_dd02.bmp ind_dd03.bmp ind_dd04.bmp ind_dd05.bmp ind_dd06.bmp ind_dd07.bmp ind_dd08.bmp ind_dd09.bmp ind_dd10.bmp ind_dd11.bmp ind_dd12.bmp ind_dd13.bmp ind_dd14.bmp ind_dd15.bmp ind_dd16.bmp ind_dd17.bmp ind_dd18.bmp ind_dd19.bmp ind_dd20.bmp ind_dd21.bmp ind_dd22.bmp ind_dd23.bmp ind_dd24.bmp ind_dd25.bmp ind_dd26.bmp ind_dd27.bmp ind_dd28.bmp ind_dd29.bmp ind_dd30.bmp ind_dd31.bmp ind_dd32.bmp ind_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ga01.bmp ind_ga02.bmp ind_ga03.bmp ind_ga04.bmp ind_ga05.bmp ind_ga06.bmp ind_ga07.bmp ind_ga08.bmp ind_ga09.bmp ind_ga10.bmp ind_ga11.bmp ind_ga12.bmp ind_ga13.bmp ind_ga14.bmp ind_ga15.bmp ind_ga16.bmp ind_ga17.bmp ind_ga18.bmp ind_ga19.bmp ind_ga20.bmp ind_ga21.bmp ind_ga22.bmp ind_ga23.bmp ind_ga24.bmp ind_ga25.bmp ind_ga26.bmp ind_ga27.bmp ind_ga28.bmp ind_ga29.bmp ind_ga30.bmp ind_ga31.bmp ind_ga32.bmp ind_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sb01.bmp ind_sb02.bmp ind_sb03.bmp ind_sb04.bmp ind_sb05.bmp ind_sb06.bmp ind_sb07.bmp ind_sb08.bmp ind_sb09.bmp ind_sb10.bmp ind_sb11.bmp ind_sb12.bmp ind_sb13.bmp ind_sb14.bmp ind_sb15.bmp ind_sb16.bmp ind_sb17.bmp ind_sb18.bmp ind_sb19.bmp ind_sb20.bmp ind_sb21.bmp ind_sb22.bmp ind_sb23.bmp ind_sb24.bmp ind_sb25.bmp ind_sb26.bmp ind_sb27.bmp ind_sb28.bmp ind_sb29.bmp ind_sb30.bmp ind_sb31.bmp ind_sb32.bmp ind_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sc01.bmp ind_sc02.bmp ind_sc03.bmp ind_sc04.bmp ind_sc05.bmp ind_sc06.bmp ind_sc07.bmp ind_sc08.bmp ind_sc09.bmp ind_sc10.bmp ind_sc11.bmp ind_sc12.bmp ind_sc13.bmp ind_sc14.bmp ind_sc15.bmp ind_sc16.bmp ind_sc17.bmp ind_sc18.bmp ind_sc19.bmp ind_sc20.bmp ind_sc21.bmp ind_sc22.bmp ind_sc23.bmp ind_sc24.bmp ind_sc25.bmp ind_sc26.bmp ind_sc27.bmp ind_sc28.bmp ind_sc29.bmp ind_sc30.bmp ind_sc31.bmp ind_sc32.bmp ind_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ind_sc.bmp ind_dd.bmp ind_ca.bmp ind_bb.bmp ind_as.bmp  ind_sb.bmp   ind_ga.bmp  ind_at.bmp indship1.bmp
popd
pushd resources\ships\color1\kli
echo Making resources\ships\color1\kli...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_as01.bmp kli_as02.bmp kli_as03.bmp kli_as04.bmp kli_as05.bmp kli_as06.bmp kli_as07.bmp kli_as08.bmp kli_as09.bmp kli_as10.bmp kli_as11.bmp kli_as12.bmp kli_as13.bmp kli_as14.bmp kli_as15.bmp kli_as16.bmp kli_as17.bmp kli_as18.bmp kli_as19.bmp kli_as20.bmp kli_as21.bmp kli_as22.bmp kli_as23.bmp kli_as24.bmp kli_as25.bmp kli_as26.bmp kli_as27.bmp kli_as28.bmp kli_as29.bmp kli_as30.bmp kli_as31.bmp kli_as32.bmp kli_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_at01.bmp kli_at02.bmp kli_at03.bmp kli_at04.bmp kli_at05.bmp kli_at06.bmp kli_at07.bmp kli_at08.bmp kli_at09.bmp kli_at10.bmp kli_at11.bmp kli_at12.bmp kli_at13.bmp kli_at14.bmp kli_at15.bmp kli_at16.bmp kli_at17.bmp kli_at18.bmp kli_at19.bmp kli_at20.bmp kli_at21.bmp kli_at22.bmp kli_at23.bmp kli_at24.bmp kli_at25.bmp kli_at26.bmp kli_at27.bmp kli_at28.bmp kli_at29.bmp kli_at30.bmp kli_at31.bmp kli_at32.bmp kli_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_bb01.bmp kli_bb02.bmp kli_bb03.bmp kli_bb04.bmp kli_bb05.bmp kli_bb06.bmp kli_bb07.bmp kli_bb08.bmp kli_bb09.bmp kli_bb10.bmp kli_bb11.bmp kli_bb12.bmp kli_bb13.bmp kli_bb14.bmp kli_bb15.bmp kli_bb16.bmp kli_bb17.bmp kli_bb18.bmp kli_bb19.bmp kli_bb20.bmp kli_bb21.bmp kli_bb22.bmp kli_bb23.bmp kli_bb24.bmp kli_bb25.bmp kli_bb26.bmp kli_bb27.bmp kli_bb28.bmp kli_bb29.bmp kli_bb30.bmp kli_bb31.bmp kli_bb32.bmp kli_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ca01.bmp kli_ca02.bmp kli_ca03.bmp kli_ca04.bmp kli_ca05.bmp kli_ca06.bmp kli_ca07.bmp kli_ca08.bmp kli_ca09.bmp kli_ca10.bmp kli_ca11.bmp kli_ca12.bmp kli_ca13.bmp kli_ca14.bmp kli_ca15.bmp kli_ca16.bmp kli_ca17.bmp kli_ca18.bmp kli_ca19.bmp kli_ca20.bmp kli_ca21.bmp kli_ca22.bmp kli_ca23.bmp kli_ca24.bmp kli_ca25.bmp kli_ca26.bmp kli_ca27.bmp kli_ca28.bmp kli_ca29.bmp kli_ca30.bmp kli_ca31.bmp kli_ca32.bmp kli_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_dd01.bmp kli_dd02.bmp kli_dd03.bmp kli_dd04.bmp kli_dd05.bmp kli_dd06.bmp kli_dd07.bmp kli_dd08.bmp kli_dd09.bmp kli_dd10.bmp kli_dd11.bmp kli_dd12.bmp kli_dd13.bmp kli_dd14.bmp kli_dd15.bmp kli_dd16.bmp kli_dd17.bmp kli_dd18.bmp kli_dd19.bmp kli_dd20.bmp kli_dd21.bmp kli_dd22.bmp kli_dd23.bmp kli_dd24.bmp kli_dd25.bmp kli_dd26.bmp kli_dd27.bmp kli_dd28.bmp kli_dd29.bmp kli_dd30.bmp kli_dd31.bmp kli_dd32.bmp kli_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ga01.bmp kli_ga02.bmp kli_ga03.bmp kli_ga04.bmp kli_ga05.bmp kli_ga06.bmp kli_ga07.bmp kli_ga08.bmp kli_ga09.bmp kli_ga10.bmp kli_ga11.bmp kli_ga12.bmp kli_ga13.bmp kli_ga14.bmp kli_ga15.bmp kli_ga16.bmp kli_ga17.bmp kli_ga18.bmp kli_ga19.bmp kli_ga20.bmp kli_ga21.bmp kli_ga22.bmp kli_ga23.bmp kli_ga24.bmp kli_ga25.bmp kli_ga26.bmp kli_ga27.bmp kli_ga28.bmp kli_ga29.bmp kli_ga30.bmp kli_ga31.bmp kli_ga32.bmp kli_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sb01.bmp kli_sb02.bmp kli_sb03.bmp kli_sb04.bmp kli_sb05.bmp kli_sb06.bmp kli_sb07.bmp kli_sb08.bmp kli_sb09.bmp kli_sb10.bmp kli_sb11.bmp kli_sb12.bmp kli_sb13.bmp kli_sb14.bmp kli_sb15.bmp kli_sb16.bmp kli_sb17.bmp kli_sb18.bmp kli_sb19.bmp kli_sb20.bmp kli_sb21.bmp kli_sb22.bmp kli_sb23.bmp kli_sb24.bmp kli_sb25.bmp kli_sb26.bmp kli_sb27.bmp kli_sb28.bmp kli_sb29.bmp kli_sb30.bmp kli_sb31.bmp kli_sb32.bmp kli_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sc01.bmp kli_sc02.bmp kli_sc03.bmp kli_sc04.bmp kli_sc05.bmp kli_sc06.bmp kli_sc07.bmp kli_sc08.bmp kli_sc09.bmp kli_sc10.bmp kli_sc11.bmp kli_sc12.bmp kli_sc13.bmp kli_sc14.bmp kli_sc15.bmp kli_sc16.bmp kli_sc17.bmp kli_sc18.bmp kli_sc19.bmp kli_sc20.bmp kli_sc21.bmp kli_sc22.bmp kli_sc23.bmp kli_sc24.bmp kli_sc25.bmp kli_sc26.bmp kli_sc27.bmp kli_sc28.bmp kli_sc29.bmp kli_sc30.bmp kli_sc31.bmp kli_sc32.bmp kli_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 kli_sc.bmp kli_dd.bmp kli_ca.bmp kli_bb.bmp kli_as.bmp  kli_sb.bmp   kli_ga.bmp  kli_at.bmp kliship1.bmp
popd
pushd resources\ships\color1\ori
echo Making resources\ships\color1\ori...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_as01.bmp ori_as02.bmp ori_as03.bmp ori_as04.bmp ori_as05.bmp ori_as06.bmp ori_as07.bmp ori_as08.bmp ori_as09.bmp ori_as10.bmp ori_as11.bmp ori_as12.bmp ori_as13.bmp ori_as14.bmp ori_as15.bmp ori_as16.bmp ori_as17.bmp ori_as18.bmp ori_as19.bmp ori_as20.bmp ori_as21.bmp ori_as22.bmp ori_as23.bmp ori_as24.bmp ori_as25.bmp ori_as26.bmp ori_as27.bmp ori_as28.bmp ori_as29.bmp ori_as30.bmp ori_as31.bmp ori_as32.bmp ori_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_at01.bmp ori_at02.bmp ori_at03.bmp ori_at04.bmp ori_at05.bmp ori_at06.bmp ori_at07.bmp ori_at08.bmp ori_at09.bmp ori_at10.bmp ori_at11.bmp ori_at12.bmp ori_at13.bmp ori_at14.bmp ori_at15.bmp ori_at16.bmp ori_at17.bmp ori_at18.bmp ori_at19.bmp ori_at20.bmp ori_at21.bmp ori_at22.bmp ori_at23.bmp ori_at24.bmp ori_at25.bmp ori_at26.bmp ori_at27.bmp ori_at28.bmp ori_at29.bmp ori_at30.bmp ori_at31.bmp ori_at32.bmp ori_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_bb01.bmp ori_bb02.bmp ori_bb03.bmp ori_bb04.bmp ori_bb05.bmp ori_bb06.bmp ori_bb07.bmp ori_bb08.bmp ori_bb09.bmp ori_bb10.bmp ori_bb11.bmp ori_bb12.bmp ori_bb13.bmp ori_bb14.bmp ori_bb15.bmp ori_bb16.bmp ori_bb17.bmp ori_bb18.bmp ori_bb19.bmp ori_bb20.bmp ori_bb21.bmp ori_bb22.bmp ori_bb23.bmp ori_bb24.bmp ori_bb25.bmp ori_bb26.bmp ori_bb27.bmp ori_bb28.bmp ori_bb29.bmp ori_bb30.bmp ori_bb31.bmp ori_bb32.bmp ori_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ca01.bmp ori_ca02.bmp ori_ca03.bmp ori_ca04.bmp ori_ca05.bmp ori_ca06.bmp ori_ca07.bmp ori_ca08.bmp ori_ca09.bmp ori_ca10.bmp ori_ca11.bmp ori_ca12.bmp ori_ca13.bmp ori_ca14.bmp ori_ca15.bmp ori_ca16.bmp ori_ca17.bmp ori_ca18.bmp ori_ca19.bmp ori_ca20.bmp ori_ca21.bmp ori_ca22.bmp ori_ca23.bmp ori_ca24.bmp ori_ca25.bmp ori_ca26.bmp ori_ca27.bmp ori_ca28.bmp ori_ca29.bmp ori_ca30.bmp ori_ca31.bmp ori_ca32.bmp ori_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_dd01.bmp ori_dd02.bmp ori_dd03.bmp ori_dd04.bmp ori_dd05.bmp ori_dd06.bmp ori_dd07.bmp ori_dd08.bmp ori_dd09.bmp ori_dd10.bmp ori_dd11.bmp ori_dd12.bmp ori_dd13.bmp ori_dd14.bmp ori_dd15.bmp ori_dd16.bmp ori_dd17.bmp ori_dd18.bmp ori_dd19.bmp ori_dd20.bmp ori_dd21.bmp ori_dd22.bmp ori_dd23.bmp ori_dd24.bmp ori_dd25.bmp ori_dd26.bmp ori_dd27.bmp ori_dd28.bmp ori_dd29.bmp ori_dd30.bmp ori_dd31.bmp ori_dd32.bmp ori_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ga01.bmp ori_ga02.bmp ori_ga03.bmp ori_ga04.bmp ori_ga05.bmp ori_ga06.bmp ori_ga07.bmp ori_ga08.bmp ori_ga09.bmp ori_ga10.bmp ori_ga11.bmp ori_ga12.bmp ori_ga13.bmp ori_ga14.bmp ori_ga15.bmp ori_ga16.bmp ori_ga17.bmp ori_ga18.bmp ori_ga19.bmp ori_ga20.bmp ori_ga21.bmp ori_ga22.bmp ori_ga23.bmp ori_ga24.bmp ori_ga25.bmp ori_ga26.bmp ori_ga27.bmp ori_ga28.bmp ori_ga29.bmp ori_ga30.bmp ori_ga31.bmp ori_ga32.bmp ori_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sb01.bmp ori_sb02.bmp ori_sb03.bmp ori_sb04.bmp ori_sb05.bmp ori_sb06.bmp ori_sb07.bmp ori_sb08.bmp ori_sb09.bmp ori_sb10.bmp ori_sb11.bmp ori_sb12.bmp ori_sb13.bmp ori_sb14.bmp ori_sb15.bmp ori_sb16.bmp ori_sb17.bmp ori_sb18.bmp ori_sb19.bmp ori_sb20.bmp ori_sb21.bmp ori_sb22.bmp ori_sb23.bmp ori_sb24.bmp ori_sb25.bmp ori_sb26.bmp ori_sb27.bmp ori_sb28.bmp ori_sb29.bmp ori_sb30.bmp ori_sb31.bmp ori_sb32.bmp ori_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sc01.bmp ori_sc02.bmp ori_sc03.bmp ori_sc04.bmp ori_sc05.bmp ori_sc06.bmp ori_sc07.bmp ori_sc08.bmp ori_sc09.bmp ori_sc10.bmp ori_sc11.bmp ori_sc12.bmp ori_sc13.bmp ori_sc14.bmp ori_sc15.bmp ori_sc16.bmp ori_sc17.bmp ori_sc18.bmp ori_sc19.bmp ori_sc20.bmp ori_sc21.bmp ori_sc22.bmp ori_sc23.bmp ori_sc24.bmp ori_sc25.bmp ori_sc26.bmp ori_sc27.bmp ori_sc28.bmp ori_sc29.bmp ori_sc30.bmp ori_sc31.bmp ori_sc32.bmp ori_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ori_sc.bmp ori_dd.bmp ori_ca.bmp ori_bb.bmp ori_as.bmp  ori_sb.bmp   ori_ga.bmp  ori_at.bmp oriship1.bmp
popd
pushd resources\ships\color1\rom
echo Making resources\ships\color1\rom...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_as01.bmp rom_as02.bmp rom_as03.bmp rom_as04.bmp rom_as05.bmp rom_as06.bmp rom_as07.bmp rom_as08.bmp rom_as09.bmp rom_as10.bmp rom_as11.bmp rom_as12.bmp rom_as13.bmp rom_as14.bmp rom_as15.bmp rom_as16.bmp rom_as17.bmp rom_as18.bmp rom_as19.bmp rom_as20.bmp rom_as21.bmp rom_as22.bmp rom_as23.bmp rom_as24.bmp rom_as25.bmp rom_as26.bmp rom_as27.bmp rom_as28.bmp rom_as29.bmp rom_as30.bmp rom_as31.bmp rom_as32.bmp rom_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_at01.bmp rom_at02.bmp rom_at03.bmp rom_at04.bmp rom_at05.bmp rom_at06.bmp rom_at07.bmp rom_at08.bmp rom_at09.bmp rom_at10.bmp rom_at11.bmp rom_at12.bmp rom_at13.bmp rom_at14.bmp rom_at15.bmp rom_at16.bmp rom_at17.bmp rom_at18.bmp rom_at19.bmp rom_at20.bmp rom_at21.bmp rom_at22.bmp rom_at23.bmp rom_at24.bmp rom_at25.bmp rom_at26.bmp rom_at27.bmp rom_at28.bmp rom_at29.bmp rom_at30.bmp rom_at31.bmp rom_at32.bmp rom_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_bb01.bmp rom_bb02.bmp rom_bb03.bmp rom_bb04.bmp rom_bb05.bmp rom_bb06.bmp rom_bb07.bmp rom_bb08.bmp rom_bb09.bmp rom_bb10.bmp rom_bb11.bmp rom_bb12.bmp rom_bb13.bmp rom_bb14.bmp rom_bb15.bmp rom_bb16.bmp rom_bb17.bmp rom_bb18.bmp rom_bb19.bmp rom_bb20.bmp rom_bb21.bmp rom_bb22.bmp rom_bb23.bmp rom_bb24.bmp rom_bb25.bmp rom_bb26.bmp rom_bb27.bmp rom_bb28.bmp rom_bb29.bmp rom_bb30.bmp rom_bb31.bmp rom_bb32.bmp rom_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ca01.bmp rom_ca02.bmp rom_ca03.bmp rom_ca04.bmp rom_ca05.bmp rom_ca06.bmp rom_ca07.bmp rom_ca08.bmp rom_ca09.bmp rom_ca10.bmp rom_ca11.bmp rom_ca12.bmp rom_ca13.bmp rom_ca14.bmp rom_ca15.bmp rom_ca16.bmp rom_ca17.bmp rom_ca18.bmp rom_ca19.bmp rom_ca20.bmp rom_ca21.bmp rom_ca22.bmp rom_ca23.bmp rom_ca24.bmp rom_ca25.bmp rom_ca26.bmp rom_ca27.bmp rom_ca28.bmp rom_ca29.bmp rom_ca30.bmp rom_ca31.bmp rom_ca32.bmp rom_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_dd01.bmp rom_dd02.bmp rom_dd03.bmp rom_dd04.bmp rom_dd05.bmp rom_dd06.bmp rom_dd07.bmp rom_dd08.bmp rom_dd09.bmp rom_dd10.bmp rom_dd11.bmp rom_dd12.bmp rom_dd13.bmp rom_dd14.bmp rom_dd15.bmp rom_dd16.bmp rom_dd17.bmp rom_dd18.bmp rom_dd19.bmp rom_dd20.bmp rom_dd21.bmp rom_dd22.bmp rom_dd23.bmp rom_dd24.bmp rom_dd25.bmp rom_dd26.bmp rom_dd27.bmp rom_dd28.bmp rom_dd29.bmp rom_dd30.bmp rom_dd31.bmp rom_dd32.bmp rom_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ga01.bmp rom_ga02.bmp rom_ga03.bmp rom_ga04.bmp rom_ga05.bmp rom_ga06.bmp rom_ga07.bmp rom_ga08.bmp rom_ga09.bmp rom_ga10.bmp rom_ga11.bmp rom_ga12.bmp rom_ga13.bmp rom_ga14.bmp rom_ga15.bmp rom_ga16.bmp rom_ga17.bmp rom_ga18.bmp rom_ga19.bmp rom_ga20.bmp rom_ga21.bmp rom_ga22.bmp rom_ga23.bmp rom_ga24.bmp rom_ga25.bmp rom_ga26.bmp rom_ga27.bmp rom_ga28.bmp rom_ga29.bmp rom_ga30.bmp rom_ga31.bmp rom_ga32.bmp rom_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sb01.bmp rom_sb02.bmp rom_sb03.bmp rom_sb04.bmp rom_sb05.bmp rom_sb06.bmp rom_sb07.bmp rom_sb08.bmp rom_sb09.bmp rom_sb10.bmp rom_sb11.bmp rom_sb12.bmp rom_sb13.bmp rom_sb14.bmp rom_sb15.bmp rom_sb16.bmp rom_sb17.bmp rom_sb18.bmp rom_sb19.bmp rom_sb20.bmp rom_sb21.bmp rom_sb22.bmp rom_sb23.bmp rom_sb24.bmp rom_sb25.bmp rom_sb26.bmp rom_sb27.bmp rom_sb28.bmp rom_sb29.bmp rom_sb30.bmp rom_sb31.bmp rom_sb32.bmp rom_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sc01.bmp rom_sc02.bmp rom_sc03.bmp rom_sc04.bmp rom_sc05.bmp rom_sc06.bmp rom_sc07.bmp rom_sc08.bmp rom_sc09.bmp rom_sc10.bmp rom_sc11.bmp rom_sc12.bmp rom_sc13.bmp rom_sc14.bmp rom_sc15.bmp rom_sc16.bmp rom_sc17.bmp rom_sc18.bmp rom_sc19.bmp rom_sc20.bmp rom_sc21.bmp rom_sc22.bmp rom_sc23.bmp rom_sc24.bmp rom_sc25.bmp rom_sc26.bmp rom_sc27.bmp rom_sc28.bmp rom_sc29.bmp rom_sc30.bmp rom_sc31.bmp rom_sc32.bmp rom_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 rom_sc.bmp rom_dd.bmp rom_ca.bmp rom_bb.bmp rom_as.bmp  rom_sb.bmp   rom_ga.bmp  rom_at.bmp romship1.bmp
popd

pushd resources\ships\mono\fed
echo Making resources\ships\mono\fed...
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_as01.bmp fed_as02.bmp fed_as03.bmp fed_as04.bmp fed_as05.bmp fed_as06.bmp fed_as07.bmp fed_as08.bmp fed_as09.bmp fed_as10.bmp fed_as11.bmp fed_as12.bmp fed_as13.bmp fed_as14.bmp fed_as15.bmp fed_as16.bmp fed_as17.bmp fed_as18.bmp fed_as19.bmp fed_as20.bmp fed_as21.bmp fed_as22.bmp fed_as23.bmp fed_as24.bmp fed_as25.bmp fed_as26.bmp fed_as27.bmp fed_as28.bmp fed_as29.bmp fed_as30.bmp fed_as31.bmp fed_as32.bmp fed_as.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_at01.bmp fed_at02.bmp fed_at03.bmp fed_at04.bmp fed_at05.bmp fed_at06.bmp fed_at07.bmp fed_at08.bmp fed_at09.bmp fed_at10.bmp fed_at11.bmp fed_at12.bmp fed_at13.bmp fed_at14.bmp fed_at15.bmp fed_at16.bmp fed_at17.bmp fed_at18.bmp fed_at19.bmp fed_at20.bmp fed_at21.bmp fed_at22.bmp fed_at23.bmp fed_at24.bmp fed_at25.bmp fed_at26.bmp fed_at27.bmp fed_at28.bmp fed_at29.bmp fed_at30.bmp fed_at31.bmp fed_at32.bmp fed_at.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_bb01.bmp fed_bb02.bmp fed_bb03.bmp fed_bb04.bmp fed_bb05.bmp fed_bb06.bmp fed_bb07.bmp fed_bb08.bmp fed_bb09.bmp fed_bb10.bmp fed_bb11.bmp fed_bb12.bmp fed_bb13.bmp fed_bb14.bmp fed_bb15.bmp fed_bb16.bmp fed_bb17.bmp fed_bb18.bmp fed_bb19.bmp fed_bb20.bmp fed_bb21.bmp fed_bb22.bmp fed_bb23.bmp fed_bb24.bmp fed_bb25.bmp fed_bb26.bmp fed_bb27.bmp fed_bb28.bmp fed_bb29.bmp fed_bb30.bmp fed_bb31.bmp fed_bb32.bmp fed_bb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_ca01.bmp fed_ca02.bmp fed_ca03.bmp fed_ca04.bmp fed_ca05.bmp fed_ca06.bmp fed_ca07.bmp fed_ca08.bmp fed_ca09.bmp fed_ca10.bmp fed_ca11.bmp fed_ca12.bmp fed_ca13.bmp fed_ca14.bmp fed_ca15.bmp fed_ca16.bmp fed_ca17.bmp fed_ca18.bmp fed_ca19.bmp fed_ca20.bmp fed_ca21.bmp fed_ca22.bmp fed_ca23.bmp fed_ca24.bmp fed_ca25.bmp fed_ca26.bmp fed_ca27.bmp fed_ca28.bmp fed_ca29.bmp fed_ca30.bmp fed_ca31.bmp fed_ca32.bmp fed_ca.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_dd01.bmp fed_dd02.bmp fed_dd03.bmp fed_dd04.bmp fed_dd05.bmp fed_dd06.bmp fed_dd07.bmp fed_dd08.bmp fed_dd09.bmp fed_dd10.bmp fed_dd11.bmp fed_dd12.bmp fed_dd13.bmp fed_dd14.bmp fed_dd15.bmp fed_dd16.bmp fed_dd17.bmp fed_dd18.bmp fed_dd19.bmp fed_dd20.bmp fed_dd21.bmp fed_dd22.bmp fed_dd23.bmp fed_dd24.bmp fed_dd25.bmp fed_dd26.bmp fed_dd27.bmp fed_dd28.bmp fed_dd29.bmp fed_dd30.bmp fed_dd31.bmp fed_dd32.bmp fed_dd.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_ga01.bmp fed_ga02.bmp fed_ga03.bmp fed_ga04.bmp fed_ga05.bmp fed_ga06.bmp fed_ga07.bmp fed_ga08.bmp fed_ga09.bmp fed_ga10.bmp fed_ga11.bmp fed_ga12.bmp fed_ga13.bmp fed_ga14.bmp fed_ga15.bmp fed_ga16.bmp fed_ga17.bmp fed_ga18.bmp fed_ga19.bmp fed_ga20.bmp fed_ga21.bmp fed_ga22.bmp fed_ga23.bmp fed_ga24.bmp fed_ga25.bmp fed_ga26.bmp fed_ga27.bmp fed_ga28.bmp fed_ga29.bmp fed_ga30.bmp fed_ga31.bmp fed_ga32.bmp fed_ga.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_sb01.bmp fed_sb02.bmp fed_sb03.bmp fed_sb04.bmp fed_sb05.bmp fed_sb06.bmp fed_sb07.bmp fed_sb08.bmp fed_sb09.bmp fed_sb10.bmp fed_sb11.bmp fed_sb12.bmp fed_sb13.bmp fed_sb14.bmp fed_sb15.bmp fed_sb16.bmp fed_sb17.bmp fed_sb18.bmp fed_sb19.bmp fed_sb20.bmp fed_sb21.bmp fed_sb22.bmp fed_sb23.bmp fed_sb24.bmp fed_sb25.bmp fed_sb26.bmp fed_sb27.bmp fed_sb28.bmp fed_sb29.bmp fed_sb30.bmp fed_sb31.bmp fed_sb32.bmp fed_sb.bmp
::%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb01.bmp fed_sb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 fed_sc01.bmp fed_sc02.bmp fed_sc03.bmp fed_sc04.bmp fed_sc05.bmp fed_sc06.bmp fed_sc07.bmp fed_sc08.bmp fed_sc09.bmp fed_sc10.bmp fed_sc11.bmp fed_sc12.bmp fed_sc13.bmp fed_sc14.bmp fed_sc15.bmp fed_sc16.bmp fed_sc17.bmp fed_sc18.bmp fed_sc19.bmp fed_sc20.bmp fed_sc21.bmp fed_sc22.bmp fed_sc23.bmp fed_sc24.bmp fed_sc25.bmp fed_sc26.bmp fed_sc27.bmp fed_sc28.bmp fed_sc29.bmp fed_sc30.bmp fed_sc31.bmp fed_sc32.bmp fed_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 8x1 fed_sc.bmp fed_dd.bmp fed_ca.bmp fed_bb.bmp fed_as.bmp  fed_sb.bmp   fed_ga.bmp  fed_at.bmp fedshipM.bmp
popd
pushd resources\ships\mono\ind
echo Making resources\ships\mono\ind...
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_as01.bmp ind_as02.bmp ind_as03.bmp ind_as04.bmp ind_as05.bmp ind_as06.bmp ind_as07.bmp ind_as08.bmp ind_as09.bmp ind_as10.bmp ind_as11.bmp ind_as12.bmp ind_as13.bmp ind_as14.bmp ind_as15.bmp ind_as16.bmp ind_as17.bmp ind_as18.bmp ind_as19.bmp ind_as20.bmp ind_as21.bmp ind_as22.bmp ind_as23.bmp ind_as24.bmp ind_as25.bmp ind_as26.bmp ind_as27.bmp ind_as28.bmp ind_as29.bmp ind_as30.bmp ind_as31.bmp ind_as32.bmp ind_as.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_at01.bmp ind_at02.bmp ind_at03.bmp ind_at04.bmp ind_at05.bmp ind_at06.bmp ind_at07.bmp ind_at08.bmp ind_at09.bmp ind_at10.bmp ind_at11.bmp ind_at12.bmp ind_at13.bmp ind_at14.bmp ind_at15.bmp ind_at16.bmp ind_at17.bmp ind_at18.bmp ind_at19.bmp ind_at20.bmp ind_at21.bmp ind_at22.bmp ind_at23.bmp ind_at24.bmp ind_at25.bmp ind_at26.bmp ind_at27.bmp ind_at28.bmp ind_at29.bmp ind_at30.bmp ind_at31.bmp ind_at32.bmp ind_at.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_bb01.bmp ind_bb02.bmp ind_bb03.bmp ind_bb04.bmp ind_bb05.bmp ind_bb06.bmp ind_bb07.bmp ind_bb08.bmp ind_bb09.bmp ind_bb10.bmp ind_bb11.bmp ind_bb12.bmp ind_bb13.bmp ind_bb14.bmp ind_bb15.bmp ind_bb16.bmp ind_bb17.bmp ind_bb18.bmp ind_bb19.bmp ind_bb20.bmp ind_bb21.bmp ind_bb22.bmp ind_bb23.bmp ind_bb24.bmp ind_bb25.bmp ind_bb26.bmp ind_bb27.bmp ind_bb28.bmp ind_bb29.bmp ind_bb30.bmp ind_bb31.bmp ind_bb32.bmp ind_bb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_ca01.bmp ind_ca02.bmp ind_ca03.bmp ind_ca04.bmp ind_ca05.bmp ind_ca06.bmp ind_ca07.bmp ind_ca08.bmp ind_ca09.bmp ind_ca10.bmp ind_ca11.bmp ind_ca12.bmp ind_ca13.bmp ind_ca14.bmp ind_ca15.bmp ind_ca16.bmp ind_ca17.bmp ind_ca18.bmp ind_ca19.bmp ind_ca20.bmp ind_ca21.bmp ind_ca22.bmp ind_ca23.bmp ind_ca24.bmp ind_ca25.bmp ind_ca26.bmp ind_ca27.bmp ind_ca28.bmp ind_ca29.bmp ind_ca30.bmp ind_ca31.bmp ind_ca32.bmp ind_ca.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_dd01.bmp ind_dd02.bmp ind_dd03.bmp ind_dd04.bmp ind_dd05.bmp ind_dd06.bmp ind_dd07.bmp ind_dd08.bmp ind_dd09.bmp ind_dd10.bmp ind_dd11.bmp ind_dd12.bmp ind_dd13.bmp ind_dd14.bmp ind_dd15.bmp ind_dd16.bmp ind_dd17.bmp ind_dd18.bmp ind_dd19.bmp ind_dd20.bmp ind_dd21.bmp ind_dd22.bmp ind_dd23.bmp ind_dd24.bmp ind_dd25.bmp ind_dd26.bmp ind_dd27.bmp ind_dd28.bmp ind_dd29.bmp ind_dd30.bmp ind_dd31.bmp ind_dd32.bmp ind_dd.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_ga01.bmp ind_ga02.bmp ind_ga03.bmp ind_ga04.bmp ind_ga05.bmp ind_ga06.bmp ind_ga07.bmp ind_ga08.bmp ind_ga09.bmp ind_ga10.bmp ind_ga11.bmp ind_ga12.bmp ind_ga13.bmp ind_ga14.bmp ind_ga15.bmp ind_ga16.bmp ind_ga17.bmp ind_ga18.bmp ind_ga19.bmp ind_ga20.bmp ind_ga21.bmp ind_ga22.bmp ind_ga23.bmp ind_ga24.bmp ind_ga25.bmp ind_ga26.bmp ind_ga27.bmp ind_ga28.bmp ind_ga29.bmp ind_ga30.bmp ind_ga31.bmp ind_ga32.bmp ind_ga.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_sb01.bmp ind_sb02.bmp ind_sb03.bmp ind_sb04.bmp ind_sb05.bmp ind_sb06.bmp ind_sb07.bmp ind_sb08.bmp ind_sb09.bmp ind_sb10.bmp ind_sb11.bmp ind_sb12.bmp ind_sb13.bmp ind_sb14.bmp ind_sb15.bmp ind_sb16.bmp ind_sb17.bmp ind_sb18.bmp ind_sb19.bmp ind_sb20.bmp ind_sb21.bmp ind_sb22.bmp ind_sb23.bmp ind_sb24.bmp ind_sb25.bmp ind_sb26.bmp ind_sb27.bmp ind_sb28.bmp ind_sb29.bmp ind_sb30.bmp ind_sb31.bmp ind_sb32.bmp ind_sb.bmp
::%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb01.bmp ind_sb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ind_sc01.bmp ind_sc02.bmp ind_sc03.bmp ind_sc04.bmp ind_sc05.bmp ind_sc06.bmp ind_sc07.bmp ind_sc08.bmp ind_sc09.bmp ind_sc10.bmp ind_sc11.bmp ind_sc12.bmp ind_sc13.bmp ind_sc14.bmp ind_sc15.bmp ind_sc16.bmp ind_sc17.bmp ind_sc18.bmp ind_sc19.bmp ind_sc20.bmp ind_sc21.bmp ind_sc22.bmp ind_sc23.bmp ind_sc24.bmp ind_sc25.bmp ind_sc26.bmp ind_sc27.bmp ind_sc28.bmp ind_sc29.bmp ind_sc30.bmp ind_sc31.bmp ind_sc32.bmp ind_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 8x1 ind_sc.bmp ind_dd.bmp ind_ca.bmp ind_bb.bmp ind_as.bmp  ind_sb.bmp   ind_ga.bmp  ind_at.bmp indshipM.bmp
popd
pushd resources\ships\mono\kli
echo Making resources\ships\mono\kli...
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_as01.bmp kli_as02.bmp kli_as03.bmp kli_as04.bmp kli_as05.bmp kli_as06.bmp kli_as07.bmp kli_as08.bmp kli_as09.bmp kli_as10.bmp kli_as11.bmp kli_as12.bmp kli_as13.bmp kli_as14.bmp kli_as15.bmp kli_as16.bmp kli_as17.bmp kli_as18.bmp kli_as19.bmp kli_as20.bmp kli_as21.bmp kli_as22.bmp kli_as23.bmp kli_as24.bmp kli_as25.bmp kli_as26.bmp kli_as27.bmp kli_as28.bmp kli_as29.bmp kli_as30.bmp kli_as31.bmp kli_as32.bmp kli_as.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_at01.bmp kli_at02.bmp kli_at03.bmp kli_at04.bmp kli_at05.bmp kli_at06.bmp kli_at07.bmp kli_at08.bmp kli_at09.bmp kli_at10.bmp kli_at11.bmp kli_at12.bmp kli_at13.bmp kli_at14.bmp kli_at15.bmp kli_at16.bmp kli_at17.bmp kli_at18.bmp kli_at19.bmp kli_at20.bmp kli_at21.bmp kli_at22.bmp kli_at23.bmp kli_at24.bmp kli_at25.bmp kli_at26.bmp kli_at27.bmp kli_at28.bmp kli_at29.bmp kli_at30.bmp kli_at31.bmp kli_at32.bmp kli_at.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_bb01.bmp kli_bb02.bmp kli_bb03.bmp kli_bb04.bmp kli_bb05.bmp kli_bb06.bmp kli_bb07.bmp kli_bb08.bmp kli_bb09.bmp kli_bb10.bmp kli_bb11.bmp kli_bb12.bmp kli_bb13.bmp kli_bb14.bmp kli_bb15.bmp kli_bb16.bmp kli_bb17.bmp kli_bb18.bmp kli_bb19.bmp kli_bb20.bmp kli_bb21.bmp kli_bb22.bmp kli_bb23.bmp kli_bb24.bmp kli_bb25.bmp kli_bb26.bmp kli_bb27.bmp kli_bb28.bmp kli_bb29.bmp kli_bb30.bmp kli_bb31.bmp kli_bb32.bmp kli_bb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_ca01.bmp kli_ca02.bmp kli_ca03.bmp kli_ca04.bmp kli_ca05.bmp kli_ca06.bmp kli_ca07.bmp kli_ca08.bmp kli_ca09.bmp kli_ca10.bmp kli_ca11.bmp kli_ca12.bmp kli_ca13.bmp kli_ca14.bmp kli_ca15.bmp kli_ca16.bmp kli_ca17.bmp kli_ca18.bmp kli_ca19.bmp kli_ca20.bmp kli_ca21.bmp kli_ca22.bmp kli_ca23.bmp kli_ca24.bmp kli_ca25.bmp kli_ca26.bmp kli_ca27.bmp kli_ca28.bmp kli_ca29.bmp kli_ca30.bmp kli_ca31.bmp kli_ca32.bmp kli_ca.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_dd01.bmp kli_dd02.bmp kli_dd03.bmp kli_dd04.bmp kli_dd05.bmp kli_dd06.bmp kli_dd07.bmp kli_dd08.bmp kli_dd09.bmp kli_dd10.bmp kli_dd11.bmp kli_dd12.bmp kli_dd13.bmp kli_dd14.bmp kli_dd15.bmp kli_dd16.bmp kli_dd17.bmp kli_dd18.bmp kli_dd19.bmp kli_dd20.bmp kli_dd21.bmp kli_dd22.bmp kli_dd23.bmp kli_dd24.bmp kli_dd25.bmp kli_dd26.bmp kli_dd27.bmp kli_dd28.bmp kli_dd29.bmp kli_dd30.bmp kli_dd31.bmp kli_dd32.bmp kli_dd.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_ga01.bmp kli_ga02.bmp kli_ga03.bmp kli_ga04.bmp kli_ga05.bmp kli_ga06.bmp kli_ga07.bmp kli_ga08.bmp kli_ga09.bmp kli_ga10.bmp kli_ga11.bmp kli_ga12.bmp kli_ga13.bmp kli_ga14.bmp kli_ga15.bmp kli_ga16.bmp kli_ga17.bmp kli_ga18.bmp kli_ga19.bmp kli_ga20.bmp kli_ga21.bmp kli_ga22.bmp kli_ga23.bmp kli_ga24.bmp kli_ga25.bmp kli_ga26.bmp kli_ga27.bmp kli_ga28.bmp kli_ga29.bmp kli_ga30.bmp kli_ga31.bmp kli_ga32.bmp kli_ga.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_sb01.bmp kli_sb02.bmp kli_sb03.bmp kli_sb04.bmp kli_sb05.bmp kli_sb06.bmp kli_sb07.bmp kli_sb08.bmp kli_sb09.bmp kli_sb10.bmp kli_sb11.bmp kli_sb12.bmp kli_sb13.bmp kli_sb14.bmp kli_sb15.bmp kli_sb16.bmp kli_sb17.bmp kli_sb18.bmp kli_sb19.bmp kli_sb20.bmp kli_sb21.bmp kli_sb22.bmp kli_sb23.bmp kli_sb24.bmp kli_sb25.bmp kli_sb26.bmp kli_sb27.bmp kli_sb28.bmp kli_sb29.bmp kli_sb30.bmp kli_sb31.bmp kli_sb32.bmp kli_sb.bmp
::%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb01.bmp kli_sb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 kli_sc01.bmp kli_sc02.bmp kli_sc03.bmp kli_sc04.bmp kli_sc05.bmp kli_sc06.bmp kli_sc07.bmp kli_sc08.bmp kli_sc09.bmp kli_sc10.bmp kli_sc11.bmp kli_sc12.bmp kli_sc13.bmp kli_sc14.bmp kli_sc15.bmp kli_sc16.bmp kli_sc17.bmp kli_sc18.bmp kli_sc19.bmp kli_sc20.bmp kli_sc21.bmp kli_sc22.bmp kli_sc23.bmp kli_sc24.bmp kli_sc25.bmp kli_sc26.bmp kli_sc27.bmp kli_sc28.bmp kli_sc29.bmp kli_sc30.bmp kli_sc31.bmp kli_sc32.bmp kli_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 8x1 kli_sc.bmp kli_dd.bmp kli_ca.bmp kli_bb.bmp kli_as.bmp  kli_sb.bmp   kli_ga.bmp  kli_at.bmp klishipM.bmp
popd
pushd resources\ships\mono\ori
echo Making resources\ships\mono\ori...
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_as01.bmp ori_as02.bmp ori_as03.bmp ori_as04.bmp ori_as05.bmp ori_as06.bmp ori_as07.bmp ori_as08.bmp ori_as09.bmp ori_as10.bmp ori_as11.bmp ori_as12.bmp ori_as13.bmp ori_as14.bmp ori_as15.bmp ori_as16.bmp ori_as17.bmp ori_as18.bmp ori_as19.bmp ori_as20.bmp ori_as21.bmp ori_as22.bmp ori_as23.bmp ori_as24.bmp ori_as25.bmp ori_as26.bmp ori_as27.bmp ori_as28.bmp ori_as29.bmp ori_as30.bmp ori_as31.bmp ori_as32.bmp ori_as.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_at01.bmp ori_at02.bmp ori_at03.bmp ori_at04.bmp ori_at05.bmp ori_at06.bmp ori_at07.bmp ori_at08.bmp ori_at09.bmp ori_at10.bmp ori_at11.bmp ori_at12.bmp ori_at13.bmp ori_at14.bmp ori_at15.bmp ori_at16.bmp ori_at17.bmp ori_at18.bmp ori_at19.bmp ori_at20.bmp ori_at21.bmp ori_at22.bmp ori_at23.bmp ori_at24.bmp ori_at25.bmp ori_at26.bmp ori_at27.bmp ori_at28.bmp ori_at29.bmp ori_at30.bmp ori_at31.bmp ori_at32.bmp ori_at.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_bb01.bmp ori_bb02.bmp ori_bb03.bmp ori_bb04.bmp ori_bb05.bmp ori_bb06.bmp ori_bb07.bmp ori_bb08.bmp ori_bb09.bmp ori_bb10.bmp ori_bb11.bmp ori_bb12.bmp ori_bb13.bmp ori_bb14.bmp ori_bb15.bmp ori_bb16.bmp ori_bb17.bmp ori_bb18.bmp ori_bb19.bmp ori_bb20.bmp ori_bb21.bmp ori_bb22.bmp ori_bb23.bmp ori_bb24.bmp ori_bb25.bmp ori_bb26.bmp ori_bb27.bmp ori_bb28.bmp ori_bb29.bmp ori_bb30.bmp ori_bb31.bmp ori_bb32.bmp ori_bb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_ca01.bmp ori_ca02.bmp ori_ca03.bmp ori_ca04.bmp ori_ca05.bmp ori_ca06.bmp ori_ca07.bmp ori_ca08.bmp ori_ca09.bmp ori_ca10.bmp ori_ca11.bmp ori_ca12.bmp ori_ca13.bmp ori_ca14.bmp ori_ca15.bmp ori_ca16.bmp ori_ca17.bmp ori_ca18.bmp ori_ca19.bmp ori_ca20.bmp ori_ca21.bmp ori_ca22.bmp ori_ca23.bmp ori_ca24.bmp ori_ca25.bmp ori_ca26.bmp ori_ca27.bmp ori_ca28.bmp ori_ca29.bmp ori_ca30.bmp ori_ca31.bmp ori_ca32.bmp ori_ca.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_dd01.bmp ori_dd02.bmp ori_dd03.bmp ori_dd04.bmp ori_dd05.bmp ori_dd06.bmp ori_dd07.bmp ori_dd08.bmp ori_dd09.bmp ori_dd10.bmp ori_dd11.bmp ori_dd12.bmp ori_dd13.bmp ori_dd14.bmp ori_dd15.bmp ori_dd16.bmp ori_dd17.bmp ori_dd18.bmp ori_dd19.bmp ori_dd20.bmp ori_dd21.bmp ori_dd22.bmp ori_dd23.bmp ori_dd24.bmp ori_dd25.bmp ori_dd26.bmp ori_dd27.bmp ori_dd28.bmp ori_dd29.bmp ori_dd30.bmp ori_dd31.bmp ori_dd32.bmp ori_dd.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_ga01.bmp ori_ga02.bmp ori_ga03.bmp ori_ga04.bmp ori_ga05.bmp ori_ga06.bmp ori_ga07.bmp ori_ga08.bmp ori_ga09.bmp ori_ga10.bmp ori_ga11.bmp ori_ga12.bmp ori_ga13.bmp ori_ga14.bmp ori_ga15.bmp ori_ga16.bmp ori_ga17.bmp ori_ga18.bmp ori_ga19.bmp ori_ga20.bmp ori_ga21.bmp ori_ga22.bmp ori_ga23.bmp ori_ga24.bmp ori_ga25.bmp ori_ga26.bmp ori_ga27.bmp ori_ga28.bmp ori_ga29.bmp ori_ga30.bmp ori_ga31.bmp ori_ga32.bmp ori_ga.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_sb01.bmp ori_sb02.bmp ori_sb03.bmp ori_sb04.bmp ori_sb05.bmp ori_sb06.bmp ori_sb07.bmp ori_sb08.bmp ori_sb09.bmp ori_sb10.bmp ori_sb11.bmp ori_sb12.bmp ori_sb13.bmp ori_sb14.bmp ori_sb15.bmp ori_sb16.bmp ori_sb17.bmp ori_sb18.bmp ori_sb19.bmp ori_sb20.bmp ori_sb21.bmp ori_sb22.bmp ori_sb23.bmp ori_sb24.bmp ori_sb25.bmp ori_sb26.bmp ori_sb27.bmp ori_sb28.bmp ori_sb29.bmp ori_sb30.bmp ori_sb31.bmp ori_sb32.bmp ori_sb.bmp
::%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb01.bmp ori_sb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 ori_sc01.bmp ori_sc02.bmp ori_sc03.bmp ori_sc04.bmp ori_sc05.bmp ori_sc06.bmp ori_sc07.bmp ori_sc08.bmp ori_sc09.bmp ori_sc10.bmp ori_sc11.bmp ori_sc12.bmp ori_sc13.bmp ori_sc14.bmp ori_sc15.bmp ori_sc16.bmp ori_sc17.bmp ori_sc18.bmp ori_sc19.bmp ori_sc20.bmp ori_sc21.bmp ori_sc22.bmp ori_sc23.bmp ori_sc24.bmp ori_sc25.bmp ori_sc26.bmp ori_sc27.bmp ori_sc28.bmp ori_sc29.bmp ori_sc30.bmp ori_sc31.bmp ori_sc32.bmp ori_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 8x1 ori_sc.bmp ori_dd.bmp ori_ca.bmp ori_bb.bmp ori_as.bmp  ori_sb.bmp   ori_ga.bmp  ori_at.bmp orishipM.bmp
popd
pushd resources\ships\mono\rom
echo Making resources\ships\mono\rom...
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_as01.bmp rom_as02.bmp rom_as03.bmp rom_as04.bmp rom_as05.bmp rom_as06.bmp rom_as07.bmp rom_as08.bmp rom_as09.bmp rom_as10.bmp rom_as11.bmp rom_as12.bmp rom_as13.bmp rom_as14.bmp rom_as15.bmp rom_as16.bmp rom_as17.bmp rom_as18.bmp rom_as19.bmp rom_as20.bmp rom_as21.bmp rom_as22.bmp rom_as23.bmp rom_as24.bmp rom_as25.bmp rom_as26.bmp rom_as27.bmp rom_as28.bmp rom_as29.bmp rom_as30.bmp rom_as31.bmp rom_as32.bmp rom_as.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_at01.bmp rom_at02.bmp rom_at03.bmp rom_at04.bmp rom_at05.bmp rom_at06.bmp rom_at07.bmp rom_at08.bmp rom_at09.bmp rom_at10.bmp rom_at11.bmp rom_at12.bmp rom_at13.bmp rom_at14.bmp rom_at15.bmp rom_at16.bmp rom_at17.bmp rom_at18.bmp rom_at19.bmp rom_at20.bmp rom_at21.bmp rom_at22.bmp rom_at23.bmp rom_at24.bmp rom_at25.bmp rom_at26.bmp rom_at27.bmp rom_at28.bmp rom_at29.bmp rom_at30.bmp rom_at31.bmp rom_at32.bmp rom_at.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_bb01.bmp rom_bb02.bmp rom_bb03.bmp rom_bb04.bmp rom_bb05.bmp rom_bb06.bmp rom_bb07.bmp rom_bb08.bmp rom_bb09.bmp rom_bb10.bmp rom_bb11.bmp rom_bb12.bmp rom_bb13.bmp rom_bb14.bmp rom_bb15.bmp rom_bb16.bmp rom_bb17.bmp rom_bb18.bmp rom_bb19.bmp rom_bb20.bmp rom_bb21.bmp rom_bb22.bmp rom_bb23.bmp rom_bb24.bmp rom_bb25.bmp rom_bb26.bmp rom_bb27.bmp rom_bb28.bmp rom_bb29.bmp rom_bb30.bmp rom_bb31.bmp rom_bb32.bmp rom_bb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_ca01.bmp rom_ca02.bmp rom_ca03.bmp rom_ca04.bmp rom_ca05.bmp rom_ca06.bmp rom_ca07.bmp rom_ca08.bmp rom_ca09.bmp rom_ca10.bmp rom_ca11.bmp rom_ca12.bmp rom_ca13.bmp rom_ca14.bmp rom_ca15.bmp rom_ca16.bmp rom_ca17.bmp rom_ca18.bmp rom_ca19.bmp rom_ca20.bmp rom_ca21.bmp rom_ca22.bmp rom_ca23.bmp rom_ca24.bmp rom_ca25.bmp rom_ca26.bmp rom_ca27.bmp rom_ca28.bmp rom_ca29.bmp rom_ca30.bmp rom_ca31.bmp rom_ca32.bmp rom_ca.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_dd01.bmp rom_dd02.bmp rom_dd03.bmp rom_dd04.bmp rom_dd05.bmp rom_dd06.bmp rom_dd07.bmp rom_dd08.bmp rom_dd09.bmp rom_dd10.bmp rom_dd11.bmp rom_dd12.bmp rom_dd13.bmp rom_dd14.bmp rom_dd15.bmp rom_dd16.bmp rom_dd17.bmp rom_dd18.bmp rom_dd19.bmp rom_dd20.bmp rom_dd21.bmp rom_dd22.bmp rom_dd23.bmp rom_dd24.bmp rom_dd25.bmp rom_dd26.bmp rom_dd27.bmp rom_dd28.bmp rom_dd29.bmp rom_dd30.bmp rom_dd31.bmp rom_dd32.bmp rom_dd.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_ga01.bmp rom_ga02.bmp rom_ga03.bmp rom_ga04.bmp rom_ga05.bmp rom_ga06.bmp rom_ga07.bmp rom_ga08.bmp rom_ga09.bmp rom_ga10.bmp rom_ga11.bmp rom_ga12.bmp rom_ga13.bmp rom_ga14.bmp rom_ga15.bmp rom_ga16.bmp rom_ga17.bmp rom_ga18.bmp rom_ga19.bmp rom_ga20.bmp rom_ga21.bmp rom_ga22.bmp rom_ga23.bmp rom_ga24.bmp rom_ga25.bmp rom_ga26.bmp rom_ga27.bmp rom_ga28.bmp rom_ga29.bmp rom_ga30.bmp rom_ga31.bmp rom_ga32.bmp rom_ga.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_sb01.bmp rom_sb02.bmp rom_sb03.bmp rom_sb04.bmp rom_sb05.bmp rom_sb06.bmp rom_sb07.bmp rom_sb08.bmp rom_sb09.bmp rom_sb10.bmp rom_sb11.bmp rom_sb12.bmp rom_sb13.bmp rom_sb14.bmp rom_sb15.bmp rom_sb16.bmp rom_sb17.bmp rom_sb18.bmp rom_sb19.bmp rom_sb20.bmp rom_sb21.bmp rom_sb22.bmp rom_sb23.bmp rom_sb24.bmp rom_sb25.bmp rom_sb26.bmp rom_sb27.bmp rom_sb28.bmp rom_sb29.bmp rom_sb30.bmp rom_sb31.bmp rom_sb32.bmp rom_sb.bmp
::%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb01.bmp rom_sb.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x32 rom_sc01.bmp rom_sc02.bmp rom_sc03.bmp rom_sc04.bmp rom_sc05.bmp rom_sc06.bmp rom_sc07.bmp rom_sc08.bmp rom_sc09.bmp rom_sc10.bmp rom_sc11.bmp rom_sc12.bmp rom_sc13.bmp rom_sc14.bmp rom_sc15.bmp rom_sc16.bmp rom_sc17.bmp rom_sc18.bmp rom_sc19.bmp rom_sc20.bmp rom_sc21.bmp rom_sc22.bmp rom_sc23.bmp rom_sc24.bmp rom_sc25.bmp rom_sc26.bmp rom_sc27.bmp rom_sc28.bmp rom_sc29.bmp rom_sc30.bmp rom_sc31.bmp rom_sc32.bmp rom_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 8x1 rom_sc.bmp rom_dd.bmp rom_ca.bmp rom_bb.bmp rom_as.bmp  rom_sb.bmp   rom_ga.bmp  rom_at.bmp romshipM.bmp
popd

pushd resources\ships\tinted\fed
echo Making resources\ships\tinted\fed...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_as01.bmp fed_as02.bmp fed_as03.bmp fed_as04.bmp fed_as05.bmp fed_as06.bmp fed_as07.bmp fed_as08.bmp fed_as09.bmp fed_as10.bmp fed_as11.bmp fed_as12.bmp fed_as13.bmp fed_as14.bmp fed_as15.bmp fed_as16.bmp fed_as17.bmp fed_as18.bmp fed_as19.bmp fed_as20.bmp fed_as21.bmp fed_as22.bmp fed_as23.bmp fed_as24.bmp fed_as25.bmp fed_as26.bmp fed_as27.bmp fed_as28.bmp fed_as29.bmp fed_as30.bmp fed_as31.bmp fed_as32.bmp fed_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_at01.bmp fed_at02.bmp fed_at03.bmp fed_at04.bmp fed_at05.bmp fed_at06.bmp fed_at07.bmp fed_at08.bmp fed_at09.bmp fed_at10.bmp fed_at11.bmp fed_at12.bmp fed_at13.bmp fed_at14.bmp fed_at15.bmp fed_at16.bmp fed_at17.bmp fed_at18.bmp fed_at19.bmp fed_at20.bmp fed_at21.bmp fed_at22.bmp fed_at23.bmp fed_at24.bmp fed_at25.bmp fed_at26.bmp fed_at27.bmp fed_at28.bmp fed_at29.bmp fed_at30.bmp fed_at31.bmp fed_at32.bmp fed_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_bb01.bmp fed_bb02.bmp fed_bb03.bmp fed_bb04.bmp fed_bb05.bmp fed_bb06.bmp fed_bb07.bmp fed_bb08.bmp fed_bb09.bmp fed_bb10.bmp fed_bb11.bmp fed_bb12.bmp fed_bb13.bmp fed_bb14.bmp fed_bb15.bmp fed_bb16.bmp fed_bb17.bmp fed_bb18.bmp fed_bb19.bmp fed_bb20.bmp fed_bb21.bmp fed_bb22.bmp fed_bb23.bmp fed_bb24.bmp fed_bb25.bmp fed_bb26.bmp fed_bb27.bmp fed_bb28.bmp fed_bb29.bmp fed_bb30.bmp fed_bb31.bmp fed_bb32.bmp fed_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ca01.bmp fed_ca02.bmp fed_ca03.bmp fed_ca04.bmp fed_ca05.bmp fed_ca06.bmp fed_ca07.bmp fed_ca08.bmp fed_ca09.bmp fed_ca10.bmp fed_ca11.bmp fed_ca12.bmp fed_ca13.bmp fed_ca14.bmp fed_ca15.bmp fed_ca16.bmp fed_ca17.bmp fed_ca18.bmp fed_ca19.bmp fed_ca20.bmp fed_ca21.bmp fed_ca22.bmp fed_ca23.bmp fed_ca24.bmp fed_ca25.bmp fed_ca26.bmp fed_ca27.bmp fed_ca28.bmp fed_ca29.bmp fed_ca30.bmp fed_ca31.bmp fed_ca32.bmp fed_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_dd01.bmp fed_dd02.bmp fed_dd03.bmp fed_dd04.bmp fed_dd05.bmp fed_dd06.bmp fed_dd07.bmp fed_dd08.bmp fed_dd09.bmp fed_dd10.bmp fed_dd11.bmp fed_dd12.bmp fed_dd13.bmp fed_dd14.bmp fed_dd15.bmp fed_dd16.bmp fed_dd17.bmp fed_dd18.bmp fed_dd19.bmp fed_dd20.bmp fed_dd21.bmp fed_dd22.bmp fed_dd23.bmp fed_dd24.bmp fed_dd25.bmp fed_dd26.bmp fed_dd27.bmp fed_dd28.bmp fed_dd29.bmp fed_dd30.bmp fed_dd31.bmp fed_dd32.bmp fed_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ga01.bmp fed_ga02.bmp fed_ga03.bmp fed_ga04.bmp fed_ga05.bmp fed_ga06.bmp fed_ga07.bmp fed_ga08.bmp fed_ga09.bmp fed_ga10.bmp fed_ga11.bmp fed_ga12.bmp fed_ga13.bmp fed_ga14.bmp fed_ga15.bmp fed_ga16.bmp fed_ga17.bmp fed_ga18.bmp fed_ga19.bmp fed_ga20.bmp fed_ga21.bmp fed_ga22.bmp fed_ga23.bmp fed_ga24.bmp fed_ga25.bmp fed_ga26.bmp fed_ga27.bmp fed_ga28.bmp fed_ga29.bmp fed_ga30.bmp fed_ga31.bmp fed_ga32.bmp fed_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sb01.bmp fed_sb02.bmp fed_sb03.bmp fed_sb04.bmp fed_sb05.bmp fed_sb06.bmp fed_sb07.bmp fed_sb08.bmp fed_sb09.bmp fed_sb10.bmp fed_sb11.bmp fed_sb12.bmp fed_sb13.bmp fed_sb14.bmp fed_sb15.bmp fed_sb16.bmp fed_sb17.bmp fed_sb18.bmp fed_sb19.bmp fed_sb20.bmp fed_sb21.bmp fed_sb22.bmp fed_sb23.bmp fed_sb24.bmp fed_sb25.bmp fed_sb26.bmp fed_sb27.bmp fed_sb28.bmp fed_sb29.bmp fed_sb30.bmp fed_sb31.bmp fed_sb32.bmp fed_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sc01.bmp fed_sc02.bmp fed_sc03.bmp fed_sc04.bmp fed_sc05.bmp fed_sc06.bmp fed_sc07.bmp fed_sc08.bmp fed_sc09.bmp fed_sc10.bmp fed_sc11.bmp fed_sc12.bmp fed_sc13.bmp fed_sc14.bmp fed_sc15.bmp fed_sc16.bmp fed_sc17.bmp fed_sc18.bmp fed_sc19.bmp fed_sc20.bmp fed_sc21.bmp fed_sc22.bmp fed_sc23.bmp fed_sc24.bmp fed_sc25.bmp fed_sc26.bmp fed_sc27.bmp fed_sc28.bmp fed_sc29.bmp fed_sc30.bmp fed_sc31.bmp fed_sc32.bmp fed_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 fed_sc.bmp fed_dd.bmp fed_ca.bmp fed_bb.bmp fed_as.bmp  fed_sb.bmp   fed_ga.bmp  fed_at.bmp fedshipT.bmp
popd
pushd resources\ships\tinted\ind
echo Making resources\ships\tinted\ind...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_as01.bmp ind_as02.bmp ind_as03.bmp ind_as04.bmp ind_as05.bmp ind_as06.bmp ind_as07.bmp ind_as08.bmp ind_as09.bmp ind_as10.bmp ind_as11.bmp ind_as12.bmp ind_as13.bmp ind_as14.bmp ind_as15.bmp ind_as16.bmp ind_as17.bmp ind_as18.bmp ind_as19.bmp ind_as20.bmp ind_as21.bmp ind_as22.bmp ind_as23.bmp ind_as24.bmp ind_as25.bmp ind_as26.bmp ind_as27.bmp ind_as28.bmp ind_as29.bmp ind_as30.bmp ind_as31.bmp ind_as32.bmp ind_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_at01.bmp ind_at02.bmp ind_at03.bmp ind_at04.bmp ind_at05.bmp ind_at06.bmp ind_at07.bmp ind_at08.bmp ind_at09.bmp ind_at10.bmp ind_at11.bmp ind_at12.bmp ind_at13.bmp ind_at14.bmp ind_at15.bmp ind_at16.bmp ind_at17.bmp ind_at18.bmp ind_at19.bmp ind_at20.bmp ind_at21.bmp ind_at22.bmp ind_at23.bmp ind_at24.bmp ind_at25.bmp ind_at26.bmp ind_at27.bmp ind_at28.bmp ind_at29.bmp ind_at30.bmp ind_at31.bmp ind_at32.bmp ind_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_bb01.bmp ind_bb02.bmp ind_bb03.bmp ind_bb04.bmp ind_bb05.bmp ind_bb06.bmp ind_bb07.bmp ind_bb08.bmp ind_bb09.bmp ind_bb10.bmp ind_bb11.bmp ind_bb12.bmp ind_bb13.bmp ind_bb14.bmp ind_bb15.bmp ind_bb16.bmp ind_bb17.bmp ind_bb18.bmp ind_bb19.bmp ind_bb20.bmp ind_bb21.bmp ind_bb22.bmp ind_bb23.bmp ind_bb24.bmp ind_bb25.bmp ind_bb26.bmp ind_bb27.bmp ind_bb28.bmp ind_bb29.bmp ind_bb30.bmp ind_bb31.bmp ind_bb32.bmp ind_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ca01.bmp ind_ca02.bmp ind_ca03.bmp ind_ca04.bmp ind_ca05.bmp ind_ca06.bmp ind_ca07.bmp ind_ca08.bmp ind_ca09.bmp ind_ca10.bmp ind_ca11.bmp ind_ca12.bmp ind_ca13.bmp ind_ca14.bmp ind_ca15.bmp ind_ca16.bmp ind_ca17.bmp ind_ca18.bmp ind_ca19.bmp ind_ca20.bmp ind_ca21.bmp ind_ca22.bmp ind_ca23.bmp ind_ca24.bmp ind_ca25.bmp ind_ca26.bmp ind_ca27.bmp ind_ca28.bmp ind_ca29.bmp ind_ca30.bmp ind_ca31.bmp ind_ca32.bmp ind_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_dd01.bmp ind_dd02.bmp ind_dd03.bmp ind_dd04.bmp ind_dd05.bmp ind_dd06.bmp ind_dd07.bmp ind_dd08.bmp ind_dd09.bmp ind_dd10.bmp ind_dd11.bmp ind_dd12.bmp ind_dd13.bmp ind_dd14.bmp ind_dd15.bmp ind_dd16.bmp ind_dd17.bmp ind_dd18.bmp ind_dd19.bmp ind_dd20.bmp ind_dd21.bmp ind_dd22.bmp ind_dd23.bmp ind_dd24.bmp ind_dd25.bmp ind_dd26.bmp ind_dd27.bmp ind_dd28.bmp ind_dd29.bmp ind_dd30.bmp ind_dd31.bmp ind_dd32.bmp ind_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ga01.bmp ind_ga02.bmp ind_ga03.bmp ind_ga04.bmp ind_ga05.bmp ind_ga06.bmp ind_ga07.bmp ind_ga08.bmp ind_ga09.bmp ind_ga10.bmp ind_ga11.bmp ind_ga12.bmp ind_ga13.bmp ind_ga14.bmp ind_ga15.bmp ind_ga16.bmp ind_ga17.bmp ind_ga18.bmp ind_ga19.bmp ind_ga20.bmp ind_ga21.bmp ind_ga22.bmp ind_ga23.bmp ind_ga24.bmp ind_ga25.bmp ind_ga26.bmp ind_ga27.bmp ind_ga28.bmp ind_ga29.bmp ind_ga30.bmp ind_ga31.bmp ind_ga32.bmp ind_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sb01.bmp ind_sb02.bmp ind_sb03.bmp ind_sb04.bmp ind_sb05.bmp ind_sb06.bmp ind_sb07.bmp ind_sb08.bmp ind_sb09.bmp ind_sb10.bmp ind_sb11.bmp ind_sb12.bmp ind_sb13.bmp ind_sb14.bmp ind_sb15.bmp ind_sb16.bmp ind_sb17.bmp ind_sb18.bmp ind_sb19.bmp ind_sb20.bmp ind_sb21.bmp ind_sb22.bmp ind_sb23.bmp ind_sb24.bmp ind_sb25.bmp ind_sb26.bmp ind_sb27.bmp ind_sb28.bmp ind_sb29.bmp ind_sb30.bmp ind_sb31.bmp ind_sb32.bmp ind_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sc01.bmp ind_sc02.bmp ind_sc03.bmp ind_sc04.bmp ind_sc05.bmp ind_sc06.bmp ind_sc07.bmp ind_sc08.bmp ind_sc09.bmp ind_sc10.bmp ind_sc11.bmp ind_sc12.bmp ind_sc13.bmp ind_sc14.bmp ind_sc15.bmp ind_sc16.bmp ind_sc17.bmp ind_sc18.bmp ind_sc19.bmp ind_sc20.bmp ind_sc21.bmp ind_sc22.bmp ind_sc23.bmp ind_sc24.bmp ind_sc25.bmp ind_sc26.bmp ind_sc27.bmp ind_sc28.bmp ind_sc29.bmp ind_sc30.bmp ind_sc31.bmp ind_sc32.bmp ind_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ind_sc.bmp ind_dd.bmp ind_ca.bmp ind_bb.bmp ind_as.bmp  ind_sb.bmp   ind_ga.bmp  ind_at.bmp indshipT.bmp
popd
pushd resources\ships\tinted\kli
echo Making resources\ships\tinted\kli...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_as01.bmp kli_as02.bmp kli_as03.bmp kli_as04.bmp kli_as05.bmp kli_as06.bmp kli_as07.bmp kli_as08.bmp kli_as09.bmp kli_as10.bmp kli_as11.bmp kli_as12.bmp kli_as13.bmp kli_as14.bmp kli_as15.bmp kli_as16.bmp kli_as17.bmp kli_as18.bmp kli_as19.bmp kli_as20.bmp kli_as21.bmp kli_as22.bmp kli_as23.bmp kli_as24.bmp kli_as25.bmp kli_as26.bmp kli_as27.bmp kli_as28.bmp kli_as29.bmp kli_as30.bmp kli_as31.bmp kli_as32.bmp kli_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_at01.bmp kli_at02.bmp kli_at03.bmp kli_at04.bmp kli_at05.bmp kli_at06.bmp kli_at07.bmp kli_at08.bmp kli_at09.bmp kli_at10.bmp kli_at11.bmp kli_at12.bmp kli_at13.bmp kli_at14.bmp kli_at15.bmp kli_at16.bmp kli_at17.bmp kli_at18.bmp kli_at19.bmp kli_at20.bmp kli_at21.bmp kli_at22.bmp kli_at23.bmp kli_at24.bmp kli_at25.bmp kli_at26.bmp kli_at27.bmp kli_at28.bmp kli_at29.bmp kli_at30.bmp kli_at31.bmp kli_at32.bmp kli_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_bb01.bmp kli_bb02.bmp kli_bb03.bmp kli_bb04.bmp kli_bb05.bmp kli_bb06.bmp kli_bb07.bmp kli_bb08.bmp kli_bb09.bmp kli_bb10.bmp kli_bb11.bmp kli_bb12.bmp kli_bb13.bmp kli_bb14.bmp kli_bb15.bmp kli_bb16.bmp kli_bb17.bmp kli_bb18.bmp kli_bb19.bmp kli_bb20.bmp kli_bb21.bmp kli_bb22.bmp kli_bb23.bmp kli_bb24.bmp kli_bb25.bmp kli_bb26.bmp kli_bb27.bmp kli_bb28.bmp kli_bb29.bmp kli_bb30.bmp kli_bb31.bmp kli_bb32.bmp kli_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ca01.bmp kli_ca02.bmp kli_ca03.bmp kli_ca04.bmp kli_ca05.bmp kli_ca06.bmp kli_ca07.bmp kli_ca08.bmp kli_ca09.bmp kli_ca10.bmp kli_ca11.bmp kli_ca12.bmp kli_ca13.bmp kli_ca14.bmp kli_ca15.bmp kli_ca16.bmp kli_ca17.bmp kli_ca18.bmp kli_ca19.bmp kli_ca20.bmp kli_ca21.bmp kli_ca22.bmp kli_ca23.bmp kli_ca24.bmp kli_ca25.bmp kli_ca26.bmp kli_ca27.bmp kli_ca28.bmp kli_ca29.bmp kli_ca30.bmp kli_ca31.bmp kli_ca32.bmp kli_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_dd01.bmp kli_dd02.bmp kli_dd03.bmp kli_dd04.bmp kli_dd05.bmp kli_dd06.bmp kli_dd07.bmp kli_dd08.bmp kli_dd09.bmp kli_dd10.bmp kli_dd11.bmp kli_dd12.bmp kli_dd13.bmp kli_dd14.bmp kli_dd15.bmp kli_dd16.bmp kli_dd17.bmp kli_dd18.bmp kli_dd19.bmp kli_dd20.bmp kli_dd21.bmp kli_dd22.bmp kli_dd23.bmp kli_dd24.bmp kli_dd25.bmp kli_dd26.bmp kli_dd27.bmp kli_dd28.bmp kli_dd29.bmp kli_dd30.bmp kli_dd31.bmp kli_dd32.bmp kli_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ga01.bmp kli_ga02.bmp kli_ga03.bmp kli_ga04.bmp kli_ga05.bmp kli_ga06.bmp kli_ga07.bmp kli_ga08.bmp kli_ga09.bmp kli_ga10.bmp kli_ga11.bmp kli_ga12.bmp kli_ga13.bmp kli_ga14.bmp kli_ga15.bmp kli_ga16.bmp kli_ga17.bmp kli_ga18.bmp kli_ga19.bmp kli_ga20.bmp kli_ga21.bmp kli_ga22.bmp kli_ga23.bmp kli_ga24.bmp kli_ga25.bmp kli_ga26.bmp kli_ga27.bmp kli_ga28.bmp kli_ga29.bmp kli_ga30.bmp kli_ga31.bmp kli_ga32.bmp kli_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sb01.bmp kli_sb02.bmp kli_sb03.bmp kli_sb04.bmp kli_sb05.bmp kli_sb06.bmp kli_sb07.bmp kli_sb08.bmp kli_sb09.bmp kli_sb10.bmp kli_sb11.bmp kli_sb12.bmp kli_sb13.bmp kli_sb14.bmp kli_sb15.bmp kli_sb16.bmp kli_sb17.bmp kli_sb18.bmp kli_sb19.bmp kli_sb20.bmp kli_sb21.bmp kli_sb22.bmp kli_sb23.bmp kli_sb24.bmp kli_sb25.bmp kli_sb26.bmp kli_sb27.bmp kli_sb28.bmp kli_sb29.bmp kli_sb30.bmp kli_sb31.bmp kli_sb32.bmp kli_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sc01.bmp kli_sc02.bmp kli_sc03.bmp kli_sc04.bmp kli_sc05.bmp kli_sc06.bmp kli_sc07.bmp kli_sc08.bmp kli_sc09.bmp kli_sc10.bmp kli_sc11.bmp kli_sc12.bmp kli_sc13.bmp kli_sc14.bmp kli_sc15.bmp kli_sc16.bmp kli_sc17.bmp kli_sc18.bmp kli_sc19.bmp kli_sc20.bmp kli_sc21.bmp kli_sc22.bmp kli_sc23.bmp kli_sc24.bmp kli_sc25.bmp kli_sc26.bmp kli_sc27.bmp kli_sc28.bmp kli_sc29.bmp kli_sc30.bmp kli_sc31.bmp kli_sc32.bmp kli_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 kli_sc.bmp kli_dd.bmp kli_ca.bmp kli_bb.bmp kli_as.bmp  kli_sb.bmp   kli_ga.bmp  kli_at.bmp klishipT.bmp
popd
pushd resources\ships\tinted\ori
echo Making resources\ships\tinted\ori...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_as01.bmp ori_as02.bmp ori_as03.bmp ori_as04.bmp ori_as05.bmp ori_as06.bmp ori_as07.bmp ori_as08.bmp ori_as09.bmp ori_as10.bmp ori_as11.bmp ori_as12.bmp ori_as13.bmp ori_as14.bmp ori_as15.bmp ori_as16.bmp ori_as17.bmp ori_as18.bmp ori_as19.bmp ori_as20.bmp ori_as21.bmp ori_as22.bmp ori_as23.bmp ori_as24.bmp ori_as25.bmp ori_as26.bmp ori_as27.bmp ori_as28.bmp ori_as29.bmp ori_as30.bmp ori_as31.bmp ori_as32.bmp ori_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_at01.bmp ori_at02.bmp ori_at03.bmp ori_at04.bmp ori_at05.bmp ori_at06.bmp ori_at07.bmp ori_at08.bmp ori_at09.bmp ori_at10.bmp ori_at11.bmp ori_at12.bmp ori_at13.bmp ori_at14.bmp ori_at15.bmp ori_at16.bmp ori_at17.bmp ori_at18.bmp ori_at19.bmp ori_at20.bmp ori_at21.bmp ori_at22.bmp ori_at23.bmp ori_at24.bmp ori_at25.bmp ori_at26.bmp ori_at27.bmp ori_at28.bmp ori_at29.bmp ori_at30.bmp ori_at31.bmp ori_at32.bmp ori_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_bb01.bmp ori_bb02.bmp ori_bb03.bmp ori_bb04.bmp ori_bb05.bmp ori_bb06.bmp ori_bb07.bmp ori_bb08.bmp ori_bb09.bmp ori_bb10.bmp ori_bb11.bmp ori_bb12.bmp ori_bb13.bmp ori_bb14.bmp ori_bb15.bmp ori_bb16.bmp ori_bb17.bmp ori_bb18.bmp ori_bb19.bmp ori_bb20.bmp ori_bb21.bmp ori_bb22.bmp ori_bb23.bmp ori_bb24.bmp ori_bb25.bmp ori_bb26.bmp ori_bb27.bmp ori_bb28.bmp ori_bb29.bmp ori_bb30.bmp ori_bb31.bmp ori_bb32.bmp ori_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ca01.bmp ori_ca02.bmp ori_ca03.bmp ori_ca04.bmp ori_ca05.bmp ori_ca06.bmp ori_ca07.bmp ori_ca08.bmp ori_ca09.bmp ori_ca10.bmp ori_ca11.bmp ori_ca12.bmp ori_ca13.bmp ori_ca14.bmp ori_ca15.bmp ori_ca16.bmp ori_ca17.bmp ori_ca18.bmp ori_ca19.bmp ori_ca20.bmp ori_ca21.bmp ori_ca22.bmp ori_ca23.bmp ori_ca24.bmp ori_ca25.bmp ori_ca26.bmp ori_ca27.bmp ori_ca28.bmp ori_ca29.bmp ori_ca30.bmp ori_ca31.bmp ori_ca32.bmp ori_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_dd01.bmp ori_dd02.bmp ori_dd03.bmp ori_dd04.bmp ori_dd05.bmp ori_dd06.bmp ori_dd07.bmp ori_dd08.bmp ori_dd09.bmp ori_dd10.bmp ori_dd11.bmp ori_dd12.bmp ori_dd13.bmp ori_dd14.bmp ori_dd15.bmp ori_dd16.bmp ori_dd17.bmp ori_dd18.bmp ori_dd19.bmp ori_dd20.bmp ori_dd21.bmp ori_dd22.bmp ori_dd23.bmp ori_dd24.bmp ori_dd25.bmp ori_dd26.bmp ori_dd27.bmp ori_dd28.bmp ori_dd29.bmp ori_dd30.bmp ori_dd31.bmp ori_dd32.bmp ori_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ga01.bmp ori_ga02.bmp ori_ga03.bmp ori_ga04.bmp ori_ga05.bmp ori_ga06.bmp ori_ga07.bmp ori_ga08.bmp ori_ga09.bmp ori_ga10.bmp ori_ga11.bmp ori_ga12.bmp ori_ga13.bmp ori_ga14.bmp ori_ga15.bmp ori_ga16.bmp ori_ga17.bmp ori_ga18.bmp ori_ga19.bmp ori_ga20.bmp ori_ga21.bmp ori_ga22.bmp ori_ga23.bmp ori_ga24.bmp ori_ga25.bmp ori_ga26.bmp ori_ga27.bmp ori_ga28.bmp ori_ga29.bmp ori_ga30.bmp ori_ga31.bmp ori_ga32.bmp ori_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sb01.bmp ori_sb02.bmp ori_sb03.bmp ori_sb04.bmp ori_sb05.bmp ori_sb06.bmp ori_sb07.bmp ori_sb08.bmp ori_sb09.bmp ori_sb10.bmp ori_sb11.bmp ori_sb12.bmp ori_sb13.bmp ori_sb14.bmp ori_sb15.bmp ori_sb16.bmp ori_sb17.bmp ori_sb18.bmp ori_sb19.bmp ori_sb20.bmp ori_sb21.bmp ori_sb22.bmp ori_sb23.bmp ori_sb24.bmp ori_sb25.bmp ori_sb26.bmp ori_sb27.bmp ori_sb28.bmp ori_sb29.bmp ori_sb30.bmp ori_sb31.bmp ori_sb32.bmp ori_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sc01.bmp ori_sc02.bmp ori_sc03.bmp ori_sc04.bmp ori_sc05.bmp ori_sc06.bmp ori_sc07.bmp ori_sc08.bmp ori_sc09.bmp ori_sc10.bmp ori_sc11.bmp ori_sc12.bmp ori_sc13.bmp ori_sc14.bmp ori_sc15.bmp ori_sc16.bmp ori_sc17.bmp ori_sc18.bmp ori_sc19.bmp ori_sc20.bmp ori_sc21.bmp ori_sc22.bmp ori_sc23.bmp ori_sc24.bmp ori_sc25.bmp ori_sc26.bmp ori_sc27.bmp ori_sc28.bmp ori_sc29.bmp ori_sc30.bmp ori_sc31.bmp ori_sc32.bmp ori_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ori_sc.bmp ori_dd.bmp ori_ca.bmp ori_bb.bmp ori_as.bmp  ori_sb.bmp   ori_ga.bmp  ori_at.bmp orishipT.bmp
popd
pushd resources\ships\tinted\rom
echo Making resources\ships\tinted\rom...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_as01.bmp rom_as02.bmp rom_as03.bmp rom_as04.bmp rom_as05.bmp rom_as06.bmp rom_as07.bmp rom_as08.bmp rom_as09.bmp rom_as10.bmp rom_as11.bmp rom_as12.bmp rom_as13.bmp rom_as14.bmp rom_as15.bmp rom_as16.bmp rom_as17.bmp rom_as18.bmp rom_as19.bmp rom_as20.bmp rom_as21.bmp rom_as22.bmp rom_as23.bmp rom_as24.bmp rom_as25.bmp rom_as26.bmp rom_as27.bmp rom_as28.bmp rom_as29.bmp rom_as30.bmp rom_as31.bmp rom_as32.bmp rom_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_at01.bmp rom_at02.bmp rom_at03.bmp rom_at04.bmp rom_at05.bmp rom_at06.bmp rom_at07.bmp rom_at08.bmp rom_at09.bmp rom_at10.bmp rom_at11.bmp rom_at12.bmp rom_at13.bmp rom_at14.bmp rom_at15.bmp rom_at16.bmp rom_at17.bmp rom_at18.bmp rom_at19.bmp rom_at20.bmp rom_at21.bmp rom_at22.bmp rom_at23.bmp rom_at24.bmp rom_at25.bmp rom_at26.bmp rom_at27.bmp rom_at28.bmp rom_at29.bmp rom_at30.bmp rom_at31.bmp rom_at32.bmp rom_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_bb01.bmp rom_bb02.bmp rom_bb03.bmp rom_bb04.bmp rom_bb05.bmp rom_bb06.bmp rom_bb07.bmp rom_bb08.bmp rom_bb09.bmp rom_bb10.bmp rom_bb11.bmp rom_bb12.bmp rom_bb13.bmp rom_bb14.bmp rom_bb15.bmp rom_bb16.bmp rom_bb17.bmp rom_bb18.bmp rom_bb19.bmp rom_bb20.bmp rom_bb21.bmp rom_bb22.bmp rom_bb23.bmp rom_bb24.bmp rom_bb25.bmp rom_bb26.bmp rom_bb27.bmp rom_bb28.bmp rom_bb29.bmp rom_bb30.bmp rom_bb31.bmp rom_bb32.bmp rom_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ca01.bmp rom_ca02.bmp rom_ca03.bmp rom_ca04.bmp rom_ca05.bmp rom_ca06.bmp rom_ca07.bmp rom_ca08.bmp rom_ca09.bmp rom_ca10.bmp rom_ca11.bmp rom_ca12.bmp rom_ca13.bmp rom_ca14.bmp rom_ca15.bmp rom_ca16.bmp rom_ca17.bmp rom_ca18.bmp rom_ca19.bmp rom_ca20.bmp rom_ca21.bmp rom_ca22.bmp rom_ca23.bmp rom_ca24.bmp rom_ca25.bmp rom_ca26.bmp rom_ca27.bmp rom_ca28.bmp rom_ca29.bmp rom_ca30.bmp rom_ca31.bmp rom_ca32.bmp rom_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_dd01.bmp rom_dd02.bmp rom_dd03.bmp rom_dd04.bmp rom_dd05.bmp rom_dd06.bmp rom_dd07.bmp rom_dd08.bmp rom_dd09.bmp rom_dd10.bmp rom_dd11.bmp rom_dd12.bmp rom_dd13.bmp rom_dd14.bmp rom_dd15.bmp rom_dd16.bmp rom_dd17.bmp rom_dd18.bmp rom_dd19.bmp rom_dd20.bmp rom_dd21.bmp rom_dd22.bmp rom_dd23.bmp rom_dd24.bmp rom_dd25.bmp rom_dd26.bmp rom_dd27.bmp rom_dd28.bmp rom_dd29.bmp rom_dd30.bmp rom_dd31.bmp rom_dd32.bmp rom_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ga01.bmp rom_ga02.bmp rom_ga03.bmp rom_ga04.bmp rom_ga05.bmp rom_ga06.bmp rom_ga07.bmp rom_ga08.bmp rom_ga09.bmp rom_ga10.bmp rom_ga11.bmp rom_ga12.bmp rom_ga13.bmp rom_ga14.bmp rom_ga15.bmp rom_ga16.bmp rom_ga17.bmp rom_ga18.bmp rom_ga19.bmp rom_ga20.bmp rom_ga21.bmp rom_ga22.bmp rom_ga23.bmp rom_ga24.bmp rom_ga25.bmp rom_ga26.bmp rom_ga27.bmp rom_ga28.bmp rom_ga29.bmp rom_ga30.bmp rom_ga31.bmp rom_ga32.bmp rom_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sb01.bmp rom_sb02.bmp rom_sb03.bmp rom_sb04.bmp rom_sb05.bmp rom_sb06.bmp rom_sb07.bmp rom_sb08.bmp rom_sb09.bmp rom_sb10.bmp rom_sb11.bmp rom_sb12.bmp rom_sb13.bmp rom_sb14.bmp rom_sb15.bmp rom_sb16.bmp rom_sb17.bmp rom_sb18.bmp rom_sb19.bmp rom_sb20.bmp rom_sb21.bmp rom_sb22.bmp rom_sb23.bmp rom_sb24.bmp rom_sb25.bmp rom_sb26.bmp rom_sb27.bmp rom_sb28.bmp rom_sb29.bmp rom_sb30.bmp rom_sb31.bmp rom_sb32.bmp rom_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sc01.bmp rom_sc02.bmp rom_sc03.bmp rom_sc04.bmp rom_sc05.bmp rom_sc06.bmp rom_sc07.bmp rom_sc08.bmp rom_sc09.bmp rom_sc10.bmp rom_sc11.bmp rom_sc12.bmp rom_sc13.bmp rom_sc14.bmp rom_sc15.bmp rom_sc16.bmp rom_sc17.bmp rom_sc18.bmp rom_sc19.bmp rom_sc20.bmp rom_sc21.bmp rom_sc22.bmp rom_sc23.bmp rom_sc24.bmp rom_sc25.bmp rom_sc26.bmp rom_sc27.bmp rom_sc28.bmp rom_sc29.bmp rom_sc30.bmp rom_sc31.bmp rom_sc32.bmp rom_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 rom_sc.bmp rom_dd.bmp rom_ca.bmp rom_bb.bmp rom_as.bmp  rom_sb.bmp   rom_ga.bmp  rom_at.bmp romshipT.bmp
popd

pushd resources\ships\grayscale\fed
echo Making resources\ships\grayscale\fed...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_as01.bmp fed_as02.bmp fed_as03.bmp fed_as04.bmp fed_as05.bmp fed_as06.bmp fed_as07.bmp fed_as08.bmp fed_as09.bmp fed_as10.bmp fed_as11.bmp fed_as12.bmp fed_as13.bmp fed_as14.bmp fed_as15.bmp fed_as16.bmp fed_as17.bmp fed_as18.bmp fed_as19.bmp fed_as20.bmp fed_as21.bmp fed_as22.bmp fed_as23.bmp fed_as24.bmp fed_as25.bmp fed_as26.bmp fed_as27.bmp fed_as28.bmp fed_as29.bmp fed_as30.bmp fed_as31.bmp fed_as32.bmp fed_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_at01.bmp fed_at02.bmp fed_at03.bmp fed_at04.bmp fed_at05.bmp fed_at06.bmp fed_at07.bmp fed_at08.bmp fed_at09.bmp fed_at10.bmp fed_at11.bmp fed_at12.bmp fed_at13.bmp fed_at14.bmp fed_at15.bmp fed_at16.bmp fed_at17.bmp fed_at18.bmp fed_at19.bmp fed_at20.bmp fed_at21.bmp fed_at22.bmp fed_at23.bmp fed_at24.bmp fed_at25.bmp fed_at26.bmp fed_at27.bmp fed_at28.bmp fed_at29.bmp fed_at30.bmp fed_at31.bmp fed_at32.bmp fed_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_bb01.bmp fed_bb02.bmp fed_bb03.bmp fed_bb04.bmp fed_bb05.bmp fed_bb06.bmp fed_bb07.bmp fed_bb08.bmp fed_bb09.bmp fed_bb10.bmp fed_bb11.bmp fed_bb12.bmp fed_bb13.bmp fed_bb14.bmp fed_bb15.bmp fed_bb16.bmp fed_bb17.bmp fed_bb18.bmp fed_bb19.bmp fed_bb20.bmp fed_bb21.bmp fed_bb22.bmp fed_bb23.bmp fed_bb24.bmp fed_bb25.bmp fed_bb26.bmp fed_bb27.bmp fed_bb28.bmp fed_bb29.bmp fed_bb30.bmp fed_bb31.bmp fed_bb32.bmp fed_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ca01.bmp fed_ca02.bmp fed_ca03.bmp fed_ca04.bmp fed_ca05.bmp fed_ca06.bmp fed_ca07.bmp fed_ca08.bmp fed_ca09.bmp fed_ca10.bmp fed_ca11.bmp fed_ca12.bmp fed_ca13.bmp fed_ca14.bmp fed_ca15.bmp fed_ca16.bmp fed_ca17.bmp fed_ca18.bmp fed_ca19.bmp fed_ca20.bmp fed_ca21.bmp fed_ca22.bmp fed_ca23.bmp fed_ca24.bmp fed_ca25.bmp fed_ca26.bmp fed_ca27.bmp fed_ca28.bmp fed_ca29.bmp fed_ca30.bmp fed_ca31.bmp fed_ca32.bmp fed_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_dd01.bmp fed_dd02.bmp fed_dd03.bmp fed_dd04.bmp fed_dd05.bmp fed_dd06.bmp fed_dd07.bmp fed_dd08.bmp fed_dd09.bmp fed_dd10.bmp fed_dd11.bmp fed_dd12.bmp fed_dd13.bmp fed_dd14.bmp fed_dd15.bmp fed_dd16.bmp fed_dd17.bmp fed_dd18.bmp fed_dd19.bmp fed_dd20.bmp fed_dd21.bmp fed_dd22.bmp fed_dd23.bmp fed_dd24.bmp fed_dd25.bmp fed_dd26.bmp fed_dd27.bmp fed_dd28.bmp fed_dd29.bmp fed_dd30.bmp fed_dd31.bmp fed_dd32.bmp fed_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_ga01.bmp fed_ga02.bmp fed_ga03.bmp fed_ga04.bmp fed_ga05.bmp fed_ga06.bmp fed_ga07.bmp fed_ga08.bmp fed_ga09.bmp fed_ga10.bmp fed_ga11.bmp fed_ga12.bmp fed_ga13.bmp fed_ga14.bmp fed_ga15.bmp fed_ga16.bmp fed_ga17.bmp fed_ga18.bmp fed_ga19.bmp fed_ga20.bmp fed_ga21.bmp fed_ga22.bmp fed_ga23.bmp fed_ga24.bmp fed_ga25.bmp fed_ga26.bmp fed_ga27.bmp fed_ga28.bmp fed_ga29.bmp fed_ga30.bmp fed_ga31.bmp fed_ga32.bmp fed_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sb01.bmp fed_sb02.bmp fed_sb03.bmp fed_sb04.bmp fed_sb05.bmp fed_sb06.bmp fed_sb07.bmp fed_sb08.bmp fed_sb09.bmp fed_sb10.bmp fed_sb11.bmp fed_sb12.bmp fed_sb13.bmp fed_sb14.bmp fed_sb15.bmp fed_sb16.bmp fed_sb17.bmp fed_sb18.bmp fed_sb19.bmp fed_sb20.bmp fed_sb21.bmp fed_sb22.bmp fed_sb23.bmp fed_sb24.bmp fed_sb25.bmp fed_sb26.bmp fed_sb27.bmp fed_sb28.bmp fed_sb29.bmp fed_sb30.bmp fed_sb31.bmp fed_sb32.bmp fed_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 fed_sc01.bmp fed_sc02.bmp fed_sc03.bmp fed_sc04.bmp fed_sc05.bmp fed_sc06.bmp fed_sc07.bmp fed_sc08.bmp fed_sc09.bmp fed_sc10.bmp fed_sc11.bmp fed_sc12.bmp fed_sc13.bmp fed_sc14.bmp fed_sc15.bmp fed_sc16.bmp fed_sc17.bmp fed_sc18.bmp fed_sc19.bmp fed_sc20.bmp fed_sc21.bmp fed_sc22.bmp fed_sc23.bmp fed_sc24.bmp fed_sc25.bmp fed_sc26.bmp fed_sc27.bmp fed_sc28.bmp fed_sc29.bmp fed_sc30.bmp fed_sc31.bmp fed_sc32.bmp fed_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 fed_sc.bmp fed_dd.bmp fed_ca.bmp fed_bb.bmp fed_as.bmp  fed_sb.bmp   fed_ga.bmp  fed_at.bmp fedshipG.bmp
popd
pushd resources\ships\grayscale\ind
echo Making resources\ships\grayscale\ind...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_as01.bmp ind_as02.bmp ind_as03.bmp ind_as04.bmp ind_as05.bmp ind_as06.bmp ind_as07.bmp ind_as08.bmp ind_as09.bmp ind_as10.bmp ind_as11.bmp ind_as12.bmp ind_as13.bmp ind_as14.bmp ind_as15.bmp ind_as16.bmp ind_as17.bmp ind_as18.bmp ind_as19.bmp ind_as20.bmp ind_as21.bmp ind_as22.bmp ind_as23.bmp ind_as24.bmp ind_as25.bmp ind_as26.bmp ind_as27.bmp ind_as28.bmp ind_as29.bmp ind_as30.bmp ind_as31.bmp ind_as32.bmp ind_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_at01.bmp ind_at02.bmp ind_at03.bmp ind_at04.bmp ind_at05.bmp ind_at06.bmp ind_at07.bmp ind_at08.bmp ind_at09.bmp ind_at10.bmp ind_at11.bmp ind_at12.bmp ind_at13.bmp ind_at14.bmp ind_at15.bmp ind_at16.bmp ind_at17.bmp ind_at18.bmp ind_at19.bmp ind_at20.bmp ind_at21.bmp ind_at22.bmp ind_at23.bmp ind_at24.bmp ind_at25.bmp ind_at26.bmp ind_at27.bmp ind_at28.bmp ind_at29.bmp ind_at30.bmp ind_at31.bmp ind_at32.bmp ind_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_bb01.bmp ind_bb02.bmp ind_bb03.bmp ind_bb04.bmp ind_bb05.bmp ind_bb06.bmp ind_bb07.bmp ind_bb08.bmp ind_bb09.bmp ind_bb10.bmp ind_bb11.bmp ind_bb12.bmp ind_bb13.bmp ind_bb14.bmp ind_bb15.bmp ind_bb16.bmp ind_bb17.bmp ind_bb18.bmp ind_bb19.bmp ind_bb20.bmp ind_bb21.bmp ind_bb22.bmp ind_bb23.bmp ind_bb24.bmp ind_bb25.bmp ind_bb26.bmp ind_bb27.bmp ind_bb28.bmp ind_bb29.bmp ind_bb30.bmp ind_bb31.bmp ind_bb32.bmp ind_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ca01.bmp ind_ca02.bmp ind_ca03.bmp ind_ca04.bmp ind_ca05.bmp ind_ca06.bmp ind_ca07.bmp ind_ca08.bmp ind_ca09.bmp ind_ca10.bmp ind_ca11.bmp ind_ca12.bmp ind_ca13.bmp ind_ca14.bmp ind_ca15.bmp ind_ca16.bmp ind_ca17.bmp ind_ca18.bmp ind_ca19.bmp ind_ca20.bmp ind_ca21.bmp ind_ca22.bmp ind_ca23.bmp ind_ca24.bmp ind_ca25.bmp ind_ca26.bmp ind_ca27.bmp ind_ca28.bmp ind_ca29.bmp ind_ca30.bmp ind_ca31.bmp ind_ca32.bmp ind_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_dd01.bmp ind_dd02.bmp ind_dd03.bmp ind_dd04.bmp ind_dd05.bmp ind_dd06.bmp ind_dd07.bmp ind_dd08.bmp ind_dd09.bmp ind_dd10.bmp ind_dd11.bmp ind_dd12.bmp ind_dd13.bmp ind_dd14.bmp ind_dd15.bmp ind_dd16.bmp ind_dd17.bmp ind_dd18.bmp ind_dd19.bmp ind_dd20.bmp ind_dd21.bmp ind_dd22.bmp ind_dd23.bmp ind_dd24.bmp ind_dd25.bmp ind_dd26.bmp ind_dd27.bmp ind_dd28.bmp ind_dd29.bmp ind_dd30.bmp ind_dd31.bmp ind_dd32.bmp ind_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_ga01.bmp ind_ga02.bmp ind_ga03.bmp ind_ga04.bmp ind_ga05.bmp ind_ga06.bmp ind_ga07.bmp ind_ga08.bmp ind_ga09.bmp ind_ga10.bmp ind_ga11.bmp ind_ga12.bmp ind_ga13.bmp ind_ga14.bmp ind_ga15.bmp ind_ga16.bmp ind_ga17.bmp ind_ga18.bmp ind_ga19.bmp ind_ga20.bmp ind_ga21.bmp ind_ga22.bmp ind_ga23.bmp ind_ga24.bmp ind_ga25.bmp ind_ga26.bmp ind_ga27.bmp ind_ga28.bmp ind_ga29.bmp ind_ga30.bmp ind_ga31.bmp ind_ga32.bmp ind_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sb01.bmp ind_sb02.bmp ind_sb03.bmp ind_sb04.bmp ind_sb05.bmp ind_sb06.bmp ind_sb07.bmp ind_sb08.bmp ind_sb09.bmp ind_sb10.bmp ind_sb11.bmp ind_sb12.bmp ind_sb13.bmp ind_sb14.bmp ind_sb15.bmp ind_sb16.bmp ind_sb17.bmp ind_sb18.bmp ind_sb19.bmp ind_sb20.bmp ind_sb21.bmp ind_sb22.bmp ind_sb23.bmp ind_sb24.bmp ind_sb25.bmp ind_sb26.bmp ind_sb27.bmp ind_sb28.bmp ind_sb29.bmp ind_sb30.bmp ind_sb31.bmp ind_sb32.bmp ind_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ind_sc01.bmp ind_sc02.bmp ind_sc03.bmp ind_sc04.bmp ind_sc05.bmp ind_sc06.bmp ind_sc07.bmp ind_sc08.bmp ind_sc09.bmp ind_sc10.bmp ind_sc11.bmp ind_sc12.bmp ind_sc13.bmp ind_sc14.bmp ind_sc15.bmp ind_sc16.bmp ind_sc17.bmp ind_sc18.bmp ind_sc19.bmp ind_sc20.bmp ind_sc21.bmp ind_sc22.bmp ind_sc23.bmp ind_sc24.bmp ind_sc25.bmp ind_sc26.bmp ind_sc27.bmp ind_sc28.bmp ind_sc29.bmp ind_sc30.bmp ind_sc31.bmp ind_sc32.bmp ind_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ind_sc.bmp ind_dd.bmp ind_ca.bmp ind_bb.bmp ind_as.bmp  ind_sb.bmp   ind_ga.bmp  ind_at.bmp indshipG.bmp
popd
pushd resources\ships\grayscale\kli
echo Making resources\ships\grayscale\kli...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_as01.bmp kli_as02.bmp kli_as03.bmp kli_as04.bmp kli_as05.bmp kli_as06.bmp kli_as07.bmp kli_as08.bmp kli_as09.bmp kli_as10.bmp kli_as11.bmp kli_as12.bmp kli_as13.bmp kli_as14.bmp kli_as15.bmp kli_as16.bmp kli_as17.bmp kli_as18.bmp kli_as19.bmp kli_as20.bmp kli_as21.bmp kli_as22.bmp kli_as23.bmp kli_as24.bmp kli_as25.bmp kli_as26.bmp kli_as27.bmp kli_as28.bmp kli_as29.bmp kli_as30.bmp kli_as31.bmp kli_as32.bmp kli_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_at01.bmp kli_at02.bmp kli_at03.bmp kli_at04.bmp kli_at05.bmp kli_at06.bmp kli_at07.bmp kli_at08.bmp kli_at09.bmp kli_at10.bmp kli_at11.bmp kli_at12.bmp kli_at13.bmp kli_at14.bmp kli_at15.bmp kli_at16.bmp kli_at17.bmp kli_at18.bmp kli_at19.bmp kli_at20.bmp kli_at21.bmp kli_at22.bmp kli_at23.bmp kli_at24.bmp kli_at25.bmp kli_at26.bmp kli_at27.bmp kli_at28.bmp kli_at29.bmp kli_at30.bmp kli_at31.bmp kli_at32.bmp kli_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_bb01.bmp kli_bb02.bmp kli_bb03.bmp kli_bb04.bmp kli_bb05.bmp kli_bb06.bmp kli_bb07.bmp kli_bb08.bmp kli_bb09.bmp kli_bb10.bmp kli_bb11.bmp kli_bb12.bmp kli_bb13.bmp kli_bb14.bmp kli_bb15.bmp kli_bb16.bmp kli_bb17.bmp kli_bb18.bmp kli_bb19.bmp kli_bb20.bmp kli_bb21.bmp kli_bb22.bmp kli_bb23.bmp kli_bb24.bmp kli_bb25.bmp kli_bb26.bmp kli_bb27.bmp kli_bb28.bmp kli_bb29.bmp kli_bb30.bmp kli_bb31.bmp kli_bb32.bmp kli_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ca01.bmp kli_ca02.bmp kli_ca03.bmp kli_ca04.bmp kli_ca05.bmp kli_ca06.bmp kli_ca07.bmp kli_ca08.bmp kli_ca09.bmp kli_ca10.bmp kli_ca11.bmp kli_ca12.bmp kli_ca13.bmp kli_ca14.bmp kli_ca15.bmp kli_ca16.bmp kli_ca17.bmp kli_ca18.bmp kli_ca19.bmp kli_ca20.bmp kli_ca21.bmp kli_ca22.bmp kli_ca23.bmp kli_ca24.bmp kli_ca25.bmp kli_ca26.bmp kli_ca27.bmp kli_ca28.bmp kli_ca29.bmp kli_ca30.bmp kli_ca31.bmp kli_ca32.bmp kli_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_dd01.bmp kli_dd02.bmp kli_dd03.bmp kli_dd04.bmp kli_dd05.bmp kli_dd06.bmp kli_dd07.bmp kli_dd08.bmp kli_dd09.bmp kli_dd10.bmp kli_dd11.bmp kli_dd12.bmp kli_dd13.bmp kli_dd14.bmp kli_dd15.bmp kli_dd16.bmp kli_dd17.bmp kli_dd18.bmp kli_dd19.bmp kli_dd20.bmp kli_dd21.bmp kli_dd22.bmp kli_dd23.bmp kli_dd24.bmp kli_dd25.bmp kli_dd26.bmp kli_dd27.bmp kli_dd28.bmp kli_dd29.bmp kli_dd30.bmp kli_dd31.bmp kli_dd32.bmp kli_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_ga01.bmp kli_ga02.bmp kli_ga03.bmp kli_ga04.bmp kli_ga05.bmp kli_ga06.bmp kli_ga07.bmp kli_ga08.bmp kli_ga09.bmp kli_ga10.bmp kli_ga11.bmp kli_ga12.bmp kli_ga13.bmp kli_ga14.bmp kli_ga15.bmp kli_ga16.bmp kli_ga17.bmp kli_ga18.bmp kli_ga19.bmp kli_ga20.bmp kli_ga21.bmp kli_ga22.bmp kli_ga23.bmp kli_ga24.bmp kli_ga25.bmp kli_ga26.bmp kli_ga27.bmp kli_ga28.bmp kli_ga29.bmp kli_ga30.bmp kli_ga31.bmp kli_ga32.bmp kli_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sb01.bmp kli_sb02.bmp kli_sb03.bmp kli_sb04.bmp kli_sb05.bmp kli_sb06.bmp kli_sb07.bmp kli_sb08.bmp kli_sb09.bmp kli_sb10.bmp kli_sb11.bmp kli_sb12.bmp kli_sb13.bmp kli_sb14.bmp kli_sb15.bmp kli_sb16.bmp kli_sb17.bmp kli_sb18.bmp kli_sb19.bmp kli_sb20.bmp kli_sb21.bmp kli_sb22.bmp kli_sb23.bmp kli_sb24.bmp kli_sb25.bmp kli_sb26.bmp kli_sb27.bmp kli_sb28.bmp kli_sb29.bmp kli_sb30.bmp kli_sb31.bmp kli_sb32.bmp kli_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 kli_sc01.bmp kli_sc02.bmp kli_sc03.bmp kli_sc04.bmp kli_sc05.bmp kli_sc06.bmp kli_sc07.bmp kli_sc08.bmp kli_sc09.bmp kli_sc10.bmp kli_sc11.bmp kli_sc12.bmp kli_sc13.bmp kli_sc14.bmp kli_sc15.bmp kli_sc16.bmp kli_sc17.bmp kli_sc18.bmp kli_sc19.bmp kli_sc20.bmp kli_sc21.bmp kli_sc22.bmp kli_sc23.bmp kli_sc24.bmp kli_sc25.bmp kli_sc26.bmp kli_sc27.bmp kli_sc28.bmp kli_sc29.bmp kli_sc30.bmp kli_sc31.bmp kli_sc32.bmp kli_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 kli_sc.bmp kli_dd.bmp kli_ca.bmp kli_bb.bmp kli_as.bmp  kli_sb.bmp   kli_ga.bmp  kli_at.bmp klishipG.bmp
popd
pushd resources\ships\grayscale\ori
echo Making resources\ships\grayscale\ori...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_as01.bmp ori_as02.bmp ori_as03.bmp ori_as04.bmp ori_as05.bmp ori_as06.bmp ori_as07.bmp ori_as08.bmp ori_as09.bmp ori_as10.bmp ori_as11.bmp ori_as12.bmp ori_as13.bmp ori_as14.bmp ori_as15.bmp ori_as16.bmp ori_as17.bmp ori_as18.bmp ori_as19.bmp ori_as20.bmp ori_as21.bmp ori_as22.bmp ori_as23.bmp ori_as24.bmp ori_as25.bmp ori_as26.bmp ori_as27.bmp ori_as28.bmp ori_as29.bmp ori_as30.bmp ori_as31.bmp ori_as32.bmp ori_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_at01.bmp ori_at02.bmp ori_at03.bmp ori_at04.bmp ori_at05.bmp ori_at06.bmp ori_at07.bmp ori_at08.bmp ori_at09.bmp ori_at10.bmp ori_at11.bmp ori_at12.bmp ori_at13.bmp ori_at14.bmp ori_at15.bmp ori_at16.bmp ori_at17.bmp ori_at18.bmp ori_at19.bmp ori_at20.bmp ori_at21.bmp ori_at22.bmp ori_at23.bmp ori_at24.bmp ori_at25.bmp ori_at26.bmp ori_at27.bmp ori_at28.bmp ori_at29.bmp ori_at30.bmp ori_at31.bmp ori_at32.bmp ori_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_bb01.bmp ori_bb02.bmp ori_bb03.bmp ori_bb04.bmp ori_bb05.bmp ori_bb06.bmp ori_bb07.bmp ori_bb08.bmp ori_bb09.bmp ori_bb10.bmp ori_bb11.bmp ori_bb12.bmp ori_bb13.bmp ori_bb14.bmp ori_bb15.bmp ori_bb16.bmp ori_bb17.bmp ori_bb18.bmp ori_bb19.bmp ori_bb20.bmp ori_bb21.bmp ori_bb22.bmp ori_bb23.bmp ori_bb24.bmp ori_bb25.bmp ori_bb26.bmp ori_bb27.bmp ori_bb28.bmp ori_bb29.bmp ori_bb30.bmp ori_bb31.bmp ori_bb32.bmp ori_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ca01.bmp ori_ca02.bmp ori_ca03.bmp ori_ca04.bmp ori_ca05.bmp ori_ca06.bmp ori_ca07.bmp ori_ca08.bmp ori_ca09.bmp ori_ca10.bmp ori_ca11.bmp ori_ca12.bmp ori_ca13.bmp ori_ca14.bmp ori_ca15.bmp ori_ca16.bmp ori_ca17.bmp ori_ca18.bmp ori_ca19.bmp ori_ca20.bmp ori_ca21.bmp ori_ca22.bmp ori_ca23.bmp ori_ca24.bmp ori_ca25.bmp ori_ca26.bmp ori_ca27.bmp ori_ca28.bmp ori_ca29.bmp ori_ca30.bmp ori_ca31.bmp ori_ca32.bmp ori_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_dd01.bmp ori_dd02.bmp ori_dd03.bmp ori_dd04.bmp ori_dd05.bmp ori_dd06.bmp ori_dd07.bmp ori_dd08.bmp ori_dd09.bmp ori_dd10.bmp ori_dd11.bmp ori_dd12.bmp ori_dd13.bmp ori_dd14.bmp ori_dd15.bmp ori_dd16.bmp ori_dd17.bmp ori_dd18.bmp ori_dd19.bmp ori_dd20.bmp ori_dd21.bmp ori_dd22.bmp ori_dd23.bmp ori_dd24.bmp ori_dd25.bmp ori_dd26.bmp ori_dd27.bmp ori_dd28.bmp ori_dd29.bmp ori_dd30.bmp ori_dd31.bmp ori_dd32.bmp ori_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_ga01.bmp ori_ga02.bmp ori_ga03.bmp ori_ga04.bmp ori_ga05.bmp ori_ga06.bmp ori_ga07.bmp ori_ga08.bmp ori_ga09.bmp ori_ga10.bmp ori_ga11.bmp ori_ga12.bmp ori_ga13.bmp ori_ga14.bmp ori_ga15.bmp ori_ga16.bmp ori_ga17.bmp ori_ga18.bmp ori_ga19.bmp ori_ga20.bmp ori_ga21.bmp ori_ga22.bmp ori_ga23.bmp ori_ga24.bmp ori_ga25.bmp ori_ga26.bmp ori_ga27.bmp ori_ga28.bmp ori_ga29.bmp ori_ga30.bmp ori_ga31.bmp ori_ga32.bmp ori_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sb01.bmp ori_sb02.bmp ori_sb03.bmp ori_sb04.bmp ori_sb05.bmp ori_sb06.bmp ori_sb07.bmp ori_sb08.bmp ori_sb09.bmp ori_sb10.bmp ori_sb11.bmp ori_sb12.bmp ori_sb13.bmp ori_sb14.bmp ori_sb15.bmp ori_sb16.bmp ori_sb17.bmp ori_sb18.bmp ori_sb19.bmp ori_sb20.bmp ori_sb21.bmp ori_sb22.bmp ori_sb23.bmp ori_sb24.bmp ori_sb25.bmp ori_sb26.bmp ori_sb27.bmp ori_sb28.bmp ori_sb29.bmp ori_sb30.bmp ori_sb31.bmp ori_sb32.bmp ori_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 ori_sc01.bmp ori_sc02.bmp ori_sc03.bmp ori_sc04.bmp ori_sc05.bmp ori_sc06.bmp ori_sc07.bmp ori_sc08.bmp ori_sc09.bmp ori_sc10.bmp ori_sc11.bmp ori_sc12.bmp ori_sc13.bmp ori_sc14.bmp ori_sc15.bmp ori_sc16.bmp ori_sc17.bmp ori_sc18.bmp ori_sc19.bmp ori_sc20.bmp ori_sc21.bmp ori_sc22.bmp ori_sc23.bmp ori_sc24.bmp ori_sc25.bmp ori_sc26.bmp ori_sc27.bmp ori_sc28.bmp ori_sc29.bmp ori_sc30.bmp ori_sc31.bmp ori_sc32.bmp ori_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 ori_sc.bmp ori_dd.bmp ori_ca.bmp ori_bb.bmp ori_as.bmp  ori_sb.bmp   ori_ga.bmp  ori_at.bmp orishipG.bmp
popd
pushd resources\ships\grayscale\rom
echo Making resources\ships\grayscale\rom...
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_as01.bmp rom_as02.bmp rom_as03.bmp rom_as04.bmp rom_as05.bmp rom_as06.bmp rom_as07.bmp rom_as08.bmp rom_as09.bmp rom_as10.bmp rom_as11.bmp rom_as12.bmp rom_as13.bmp rom_as14.bmp rom_as15.bmp rom_as16.bmp rom_as17.bmp rom_as18.bmp rom_as19.bmp rom_as20.bmp rom_as21.bmp rom_as22.bmp rom_as23.bmp rom_as24.bmp rom_as25.bmp rom_as26.bmp rom_as27.bmp rom_as28.bmp rom_as29.bmp rom_as30.bmp rom_as31.bmp rom_as32.bmp rom_as.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_at01.bmp rom_at02.bmp rom_at03.bmp rom_at04.bmp rom_at05.bmp rom_at06.bmp rom_at07.bmp rom_at08.bmp rom_at09.bmp rom_at10.bmp rom_at11.bmp rom_at12.bmp rom_at13.bmp rom_at14.bmp rom_at15.bmp rom_at16.bmp rom_at17.bmp rom_at18.bmp rom_at19.bmp rom_at20.bmp rom_at21.bmp rom_at22.bmp rom_at23.bmp rom_at24.bmp rom_at25.bmp rom_at26.bmp rom_at27.bmp rom_at28.bmp rom_at29.bmp rom_at30.bmp rom_at31.bmp rom_at32.bmp rom_at.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_bb01.bmp rom_bb02.bmp rom_bb03.bmp rom_bb04.bmp rom_bb05.bmp rom_bb06.bmp rom_bb07.bmp rom_bb08.bmp rom_bb09.bmp rom_bb10.bmp rom_bb11.bmp rom_bb12.bmp rom_bb13.bmp rom_bb14.bmp rom_bb15.bmp rom_bb16.bmp rom_bb17.bmp rom_bb18.bmp rom_bb19.bmp rom_bb20.bmp rom_bb21.bmp rom_bb22.bmp rom_bb23.bmp rom_bb24.bmp rom_bb25.bmp rom_bb26.bmp rom_bb27.bmp rom_bb28.bmp rom_bb29.bmp rom_bb30.bmp rom_bb31.bmp rom_bb32.bmp rom_bb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ca01.bmp rom_ca02.bmp rom_ca03.bmp rom_ca04.bmp rom_ca05.bmp rom_ca06.bmp rom_ca07.bmp rom_ca08.bmp rom_ca09.bmp rom_ca10.bmp rom_ca11.bmp rom_ca12.bmp rom_ca13.bmp rom_ca14.bmp rom_ca15.bmp rom_ca16.bmp rom_ca17.bmp rom_ca18.bmp rom_ca19.bmp rom_ca20.bmp rom_ca21.bmp rom_ca22.bmp rom_ca23.bmp rom_ca24.bmp rom_ca25.bmp rom_ca26.bmp rom_ca27.bmp rom_ca28.bmp rom_ca29.bmp rom_ca30.bmp rom_ca31.bmp rom_ca32.bmp rom_ca.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_dd01.bmp rom_dd02.bmp rom_dd03.bmp rom_dd04.bmp rom_dd05.bmp rom_dd06.bmp rom_dd07.bmp rom_dd08.bmp rom_dd09.bmp rom_dd10.bmp rom_dd11.bmp rom_dd12.bmp rom_dd13.bmp rom_dd14.bmp rom_dd15.bmp rom_dd16.bmp rom_dd17.bmp rom_dd18.bmp rom_dd19.bmp rom_dd20.bmp rom_dd21.bmp rom_dd22.bmp rom_dd23.bmp rom_dd24.bmp rom_dd25.bmp rom_dd26.bmp rom_dd27.bmp rom_dd28.bmp rom_dd29.bmp rom_dd30.bmp rom_dd31.bmp rom_dd32.bmp rom_dd.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_ga01.bmp rom_ga02.bmp rom_ga03.bmp rom_ga04.bmp rom_ga05.bmp rom_ga06.bmp rom_ga07.bmp rom_ga08.bmp rom_ga09.bmp rom_ga10.bmp rom_ga11.bmp rom_ga12.bmp rom_ga13.bmp rom_ga14.bmp rom_ga15.bmp rom_ga16.bmp rom_ga17.bmp rom_ga18.bmp rom_ga19.bmp rom_ga20.bmp rom_ga21.bmp rom_ga22.bmp rom_ga23.bmp rom_ga24.bmp rom_ga25.bmp rom_ga26.bmp rom_ga27.bmp rom_ga28.bmp rom_ga29.bmp rom_ga30.bmp rom_ga31.bmp rom_ga32.bmp rom_ga.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sb01.bmp rom_sb02.bmp rom_sb03.bmp rom_sb04.bmp rom_sb05.bmp rom_sb06.bmp rom_sb07.bmp rom_sb08.bmp rom_sb09.bmp rom_sb10.bmp rom_sb11.bmp rom_sb12.bmp rom_sb13.bmp rom_sb14.bmp rom_sb15.bmp rom_sb16.bmp rom_sb17.bmp rom_sb18.bmp rom_sb19.bmp rom_sb20.bmp rom_sb21.bmp rom_sb22.bmp rom_sb23.bmp rom_sb24.bmp rom_sb25.bmp rom_sb26.bmp rom_sb27.bmp rom_sb28.bmp rom_sb29.bmp rom_sb30.bmp rom_sb31.bmp rom_sb32.bmp rom_sb.bmp
%MyCWD%\tools\imagemagick\montage -mode concatenate -tile 1x32 rom_sc01.bmp rom_sc02.bmp rom_sc03.bmp rom_sc04.bmp rom_sc05.bmp rom_sc06.bmp rom_sc07.bmp rom_sc08.bmp rom_sc09.bmp rom_sc10.bmp rom_sc11.bmp rom_sc12.bmp rom_sc13.bmp rom_sc14.bmp rom_sc15.bmp rom_sc16.bmp rom_sc17.bmp rom_sc18.bmp rom_sc19.bmp rom_sc20.bmp rom_sc21.bmp rom_sc22.bmp rom_sc23.bmp rom_sc24.bmp rom_sc25.bmp rom_sc26.bmp rom_sc27.bmp rom_sc28.bmp rom_sc29.bmp rom_sc30.bmp rom_sc31.bmp rom_sc32.bmp rom_sc.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 8x1 rom_sc.bmp rom_dd.bmp rom_ca.bmp rom_bb.bmp rom_as.bmp  rom_sb.bmp   rom_ga.bmp  rom_at.bmp romshipG.bmp
popd

pushd resources\planlibm\bronco
echo Making resources\planlibm\bronco
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x9 mplan000.bmp mplan001.bmp mplan010.bmp mplan011.bmp mplan100.bmp mplan101.bmp mplan110.bmp mplan111.bmp mplanunkn.bmp mplan.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x9 plan000.bmp plan001.bmp plan010.bmp plan011.bmp plan100.bmp plan101.bmp plan110.bmp plan111.bmp planunkn.bmp plan.bmp
popd
pushd resources\planlibm\rabbit
echo Making resources\planlibm\rabbit
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x9 mplan000.bmp mplan001.bmp mplan010.bmp mplan011.bmp mplan100.bmp mplan101.bmp mplan110.bmp mplan111.bmp mplanunkn.bmp mplanR.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x9 plan000.bmp plan001.bmp plan010.bmp plan011.bmp plan100.bmp plan101.bmp plan110.bmp plan111.bmp planunkn.bmp planR.bmp
popd
pushd resources\planlibm\moo
echo Making resources\planlibm\moo
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x9 mplan000.bmp mplan001.bmp mplan010.bmp mplan011.bmp mplan100.bmp mplan101.bmp mplan110.bmp mplan111.bmp mplanunkn.bmp mplanM.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x9 plan000.bmp plan001.bmp plan010.bmp plan011.bmp plan100.bmp plan101.bmp plan110.bmp plan111.bmp planunkn.bmp planM.bmp
popd
pushd resources\misclib\mono
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x7 sbexpl0.bmp sbexpl1.bmp sbexpl2.bmp sbexpl3.bmp sbexpl4.bmp sbexpl5.bmp sbexpl6.bmp sbexplM.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x5 shexpl0.bmp shexpl1.bmp shexpl2.bmp shexpl3.bmp shexpl4.bmp shexplM.bmp
%MyCWD%\tools\imagemagick\montage -colors 2 -mode concatenate -tile 1x5 shield1.bmp shield2.bmp shield3.bmp shield4.bmp shield5.bmp vshield.bmp
popd
pushd resources\misclib\color
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 1x7 sbexpl0.bmp sbexpl1.bmp sbexpl2.bmp sbexpl3.bmp sbexpl4.bmp sbexpl5.bmp sbexpl6.bmp sbexpl.bmp
%MyCWD%\tools\imagemagick\montage -colors 256 -mode concatenate -tile 1x5 shexpl0.bmp shexpl1.bmp shexpl2.bmp shexpl3.bmp shexpl4.bmp shexpl.bmp
popd

if /I "%1" == "pics" goto :end

:build

pushd resources
pushd htmlhelp
%HelpCompiler% netrek.hhp
popd
popd

pushd src
%MyCWD%\tools\mkkey -k %RSAKEY%
popd

if /I "%Compiler%" == "BCC" goto :CompilerBCC
if /I "%Compiler%" == "VCC" goto :CompilerVCC
if /I "%Compiler%" == "CYGWIN" goto :CompilerCYGWIN

echo You must set which compiler you have at the top of this batch file.
goto :end

:CompilerVCC
pushd src\cursors
cscript %MyCWD%\tools\vcmake.vbs curslib.dsw "Win32 Release"
popd

pushd src\winkey
cscript %MyCWD%\tools\vcmake.vbs winkey.dsw "Win32 Release"
popd

cscript %MyCWD%\tools\vcmake.vbs clientr.dsw "Win32 Release"

pushd %InstCWD%\netrek
%MyCWD%\tools\stampver -v%MyCWD%\src\stampver.inf netrek.exe
popd

goto :dist

:CompilerBCC
pushd src
pushd cursors
make
popd
pushd winkey
make
popd
make
%MyCWD%\tools\stampver -v%MyCWD%\src\stampver.inf netrek.exe
popd

goto :dist

:CompilerCYGWIN
pushd src
pushd cursors
make --makefile=makecyg
popd
pushd winkey
make --makefile=makecyg
popd
make --makefile=makecyg
%MyCWD%\tools\stampver -v%MyCWD%\src\stampver.inf netrek.exe
popd

goto :dist

:dist

copy resources\htmlhelp\netrek.chm %InstCWD%\netrek

mkdir %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color\fed\fedship.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color\ind\indship.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color\kli\kliship.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color\ori\oriship.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color\rom\romship.bmp %InstCWD%\netrek\bitmaps\shiplib

copy resources\ships\color1\fed\fedship1.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color1\ind\indship1.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color1\kli\kliship1.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color1\ori\oriship1.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\color1\rom\romship1.bmp %InstCWD%\netrek\bitmaps\shiplib

copy resources\ships\mono\fed\fedshipM.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\mono\ind\indshipM.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\mono\kli\klishipM.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\mono\ori\orishipM.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\mono\rom\romshipM.bmp %InstCWD%\netrek\bitmaps\shiplib

copy resources\ships\grayscale\fed\fedshipG.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\grayscale\ind\indshipG.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\grayscale\kli\klishipG.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\grayscale\ori\orishipG.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\grayscale\rom\romshipG.bmp %InstCWD%\netrek\bitmaps\shiplib

copy resources\ships\tinted\fed\fedshipT.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\tinted\ind\indshipT.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\tinted\kli\klishipT.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\tinted\ori\orishipT.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\tinted\rom\romshipT.bmp %InstCWD%\netrek\bitmaps\shiplib

copy resources\ships\highres\fedshipHR.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\highres\indshipHR.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\highres\klishipHR.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\highres\orishipHR.bmp %InstCWD%\netrek\bitmaps\shiplib
copy resources\ships\highres\romshipHR.bmp %InstCWD%\netrek\bitmaps\shiplib

mkdir %InstCWD%\netrek\bitmaps\weaplibm
mkdir %InstCWD%\netrek\bitmaps\weaplibm\mono
copy resources\weaplibm\mono\*.bmp %InstCWD%\netrek\bitmaps\weaplibm\mono

mkdir %InstCWD%\netrek\bitmaps\weaplibm\color
copy resources\weaplibm\color\mtorpC.bmp %InstCWD%\netrek\bitmaps\weaplibm\color
copy resources\weaplibm\color\torpC.bmp %InstCWD%\netrek\bitmaps\weaplibm\color
copy resources\weaplibm\color\mplasmaC.bmp %InstCWD%\netrek\bitmaps\weaplibm\color
copy resources\weaplibm\color\plasmaC.bmp %InstCWD%\netrek\bitmaps\weaplibm\color
copy resources\weaplibm\color\cloudC.bmp %InstCWD%\netrek\bitmaps\weaplibm\color
copy resources\weaplibm\color\plcloudC.bmp %InstCWD%\netrek\bitmaps\weaplibm\color

mkdir %InstCWD%\netrek\bitmaps\planlibm
copy resources\planlibm\bronco\plan.bmp %InstCWD%\netrek\bitmaps\planlibm
copy resources\planlibm\rabbit\planR.bmp %InstCWD%\netrek\bitmaps\planlibm
copy resources\planlibm\moo\planM.bmp %InstCWD%\netrek\bitmaps\planlibm
copy resources\planlibm\bronco\mplan.bmp %InstCWD%\netrek\bitmaps\planlibm
copy resources\planlibm\rabbit\mplanR.bmp %InstCWD%\netrek\bitmaps\planlibm
copy resources\planlibm\moo\mplanM.bmp %InstCWD%\netrek\bitmaps\planlibm

mkdir %InstCWD%\netrek\bitmaps\planlibm\color
copy resources\planlibm\color\*.bmp %InstCWD%\netrek\bitmaps\planlibm\color

mkdir %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\sbexplM.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\shexplM.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\color\sbexpl.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\color\shexpl.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\cloak.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\clock.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\shield.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\stipple.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\varyhull.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\vshield.bmp %InstCWD%\netrek\bitmaps\misclib
copy resources\misclib\mono\beeplite*.bmp %InstCWD%\netrek\bitmaps\misclib

mkdir %InstCWD%\netrek\bitmaps\misclib\color
copy resources\misclib\color\clock.bmp %InstCWD%\netrek\bitmaps\misclib\color
copy resources\misclib\color\clockhand.bmp %InstCWD%\netrek\bitmaps\misclib\color
copy resources\misclib\color\genocide.bmp %InstCWD%\netrek\bitmaps\misclib\color
copy resources\misclib\color\genocide2.bmp %InstCWD%\netrek\bitmaps\misclib\color
copy resources\misclib\color\*team.bmp %InstCWD%\netrek\bitmaps\misclib\color

mkdir %InstCWD%\netrek\bitmaps\paradise
copy resources\paradise\paradise_*.bmp %InstCWD%\netrek\bitmaps\paradise
copy resources\paradise\star*.bmp %InstCWD%\netrek\bitmaps\paradise
copy resources\paradise\mdroneC.bmp %InstCWD%\netrek\bitmaps\paradise
copy resources\paradise\drone*.bmp %InstCWD%\netrek\bitmaps\paradise
copy resources\paradise\gear.bmp %InstCWD%\netrek\bitmaps\paradise

copy resources\fonts\ntfonts2.fon %InstCWD%\netrek

mkdir %InstCWD%\netrek\docs
copy resources\docs\*.doc %InstCWD%\netrek\docs
copy resources\docs\*.dat %InstCWD%\netrek\docs
copy resources\docs\*.txt %InstCWD%\netrek\docs

copy resources\netrekrc\netrekrc.txt %InstCWD%\netrek

mkdir %InstCWD%\netrek\sounds
copy resources\sounds\*.wav %InstCWD%\netrek\sounds
copy resources\sounds\*.ogg %InstCWD%\netrek\sounds
copy win32\SDL\*.dll %InstCWD%\netrek

if /I "%Compiler%" == "BCC" copy src\cursors\curslib.dll %InstCWD%\netrek\bitmaps
if /I "%Compiler%" == "BCC" copy src\winkey\winkey.dll %InstCWD%\netrek\bitmaps
if /I "%Compiler%" == "BCC" copy src\netrek.exe %InstCWD%\netrek

if /I "%Compiler%" == "CYGWIN" copy src\cursors\curslib.dll %InstCWD%\netrek\bitmaps
if /I "%Compiler%" == "CYGWIN" copy src\winkey\winkey.dll %InstCWD%\netrek\bitmaps
if /I "%Compiler%" == "CYGWIN" copy src\netrek.exe %InstCWD%\netrek

goto end

:clean
del resources\htmlhelp\netrek.chm

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
if /I "%Compiler%" == "CYGWIN" goto :CleanCYGWIN

echo You must set which compiler you have at the top of this batch file.

goto :end

:CleanVCC
pushd src
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
make clean
pushd cursors
make clean
popd
pushd winkey
make clean
popd
popd

goto :end

:CleanCYGWIN
pushd src
make clean --makefile=makecyg
pushd cursors
make clean --makefile=makecyg
popd
pushd winkey
make clean --makefile=makecyg
popd
popd

goto :end

:cleanrsa
pushd src
del rsa*.c
popd

goto :end

:end
