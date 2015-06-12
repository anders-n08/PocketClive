<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: libspectrum - Win32 (WCE x86em) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP6.tmp" with contents
[
/nologo /W3 /Zi /Od /I "..\..\adapt" /I "..\\" /D "DEBUG" /D _WIN32_WCE=300 /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "WIN32_PLATFORM_PSPC" /D "i486" /D UNDER_CE=300 /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "_LIB" /Fo"X86EMDbg/" /Fd"X86EMDbg/" /Gz /c 
"C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\libspectrum.c"
"C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\sna.c"
"C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tap.c"
"C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tape.c"
"C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c"
"C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\z80.c"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP6.tmp" 
Creating command line "link.exe -lib /nologo /out:"X86EMDbg\libspectrum.lib"  .\X86EMDbg\libspectrum.obj .\X86EMDbg\sna.obj .\X86EMDbg\tap.obj .\X86EMDbg\tape.obj .\X86EMDbg\tzx.obj .\X86EMDbg\z80.obj "
<h3>Output Window</h3>
Compiling...
libspectrum.c
sna.c
tap.c
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tap.c(70) : warning C4018: '<' : signed/unsigned mismatch
tape.c
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tape.c(130) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tape.c(147) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tape.c(165) : warning C4013: 'abort' undefined; assuming extern returning int
tzx.c
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(468) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(486) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(707) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(745) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(941) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1007) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1038) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1099) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1153) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1336) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1362) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1363) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1364) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1365) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1366) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1369) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1394) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1422) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1444) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1445) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1447) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1471) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1559) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1571) : warning C4018: '<' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1774) : warning C4018: '<' : signed/unsigned mismatch
z80.c
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\z80.c(575) : warning C4244: 'function' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\z80.c(576) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned char ', possible loss of data
Generating Code...
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\z80.c(575) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\z80.c(662) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1336) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1362) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1363) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1364) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1365) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1366) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1367) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1369) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1394) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1395) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1422) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1444) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1445) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1447) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1471) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1519) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1540) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1568) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1572) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\libspectrum\tzx.c(1673) : warning C4761: integral size mismatch in argument; conversion supplied
Creating library...



<h3>Results</h3>
libspectrum.lib - 0 error(s), 53 warning(s)
</pre>
</body>
</html>
