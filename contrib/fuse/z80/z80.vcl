<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: z80 - Win32 (WCE x86em) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE.tmp" with contents
[
/nologo /W3 /Zi /Od /I "..\\" /I "..\adapt" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "WIN32_PLATFORM_PSPC" /D "i486" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "_LIB" /Fo"X86EMDbg/" /Fd"X86EMDbg/" /Gz /c 
"C:\Projects\PocketClive\2B\contrib\fuse\z80\z80.c"
"C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPE.tmp" 
Creating command line "link.exe -lib /nologo /out:"X86EMDbg\z80.lib"  .\X86EMDbg\z80.obj .\X86EMDbg\z80_ops.obj "
<h3>Output Window</h3>
Compiling...
z80.c
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80.c(119) : warning C4013: 'abort' undefined; assuming extern returning int
z80_ops.c
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(112) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(112) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(112) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(187) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(187) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(187) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(256) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(259) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(271) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(271) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(271) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(352) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(352) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(352) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(634) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(638) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(642) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(646) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(650) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(654) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(660) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(665) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(669) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(673) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(677) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(681) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(685) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(689) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(695) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(700) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(704) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(708) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(712) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(716) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(720) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(724) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(730) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(735) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(739) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(743) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(747) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(751) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(755) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(759) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(765) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(770) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(945) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(991) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(1028) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(1084) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(37) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(37) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(37) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(42) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(42) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(42) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(78) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(78) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(78) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(136) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(136) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(136) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(329) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(334) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(341) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(347) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(352) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(359) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(365) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(370) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(377) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(383) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(388) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(395) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(37) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(37) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(57) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(99) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(99) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(124) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(124) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(156) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(156) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(188) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(188) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(218) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(218) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(251) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(251) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(271) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(271) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(311) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(323) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(361) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(373) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(413) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(426) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(466) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(479) : warning C4244: 'function' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(37) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(37) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(37) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(42) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(42) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(42) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(78) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(78) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(78) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(136) : warning C4244: 'initializing' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(136) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(136) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(329) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(334) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(341) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(347) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(352) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(359) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(365) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(370) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(377) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(383) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(388) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(395) : warning C4244: '=' : conversion from 'unsigned short ' to 'unsigned char ', possible loss of data
Generating Code...
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(1013) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(1054) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(1107) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ops.c(1108) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(752) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(792) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(832) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(872) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(912) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(952) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(992) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1032) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1072) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1112) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1152) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1192) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1232) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1272) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1312) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_cb.c(1352) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(151) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(166) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(181) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(196) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(230) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(269) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(279) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(284) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(289) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(294) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(299) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(304) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(309) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(324) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(340) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(358) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(376) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(394) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(412) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(430) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(448) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(466) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(493) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(495) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(200) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(230) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(311) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(323) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(361) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(373) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(413) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(426) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(466) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ed.c(479) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(151) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(166) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(181) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(196) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(230) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(269) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(279) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(284) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(289) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(294) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(299) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(304) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(309) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(324) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(340) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(358) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(376) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(394) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(412) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(430) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(448) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(466) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(493) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfd.c(495) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(585) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(625) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(665) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(705) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(745) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(785) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(825) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(865) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(905) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(945) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(985) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(1025) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(1065) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(1105) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(1145) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\z80\z80_ddfdcb.c(1185) : warning C4761: integral size mismatch in argument; conversion supplied
Creating library...



<h3>Results</h3>
z80.lib - 0 error(s), 218 warning(s)
</pre>
</body>
</html>
