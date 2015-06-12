<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: PocketClive - Win32 (WCE x86em) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"X86EMDbg/PocketClive.res" /d UNDER_CE=300 /d _WIN32_WCE=300 /d "UNICODE" /d "_UNICODE" /d "DEBUG" /d "WIN32_PLATFORM_PSPC" /d "_X86_" /d "x86" /d "i486" /r "C:\Projects\PocketClive\2B\PocketClive.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP11.tmp" with contents
[
/nologo /W3 /Zi /Od /I "contrib\easyce" /I "contrib\zlib" /I "contrib\fuse" /I "contrib\fuse\libspectrum" /I "contrib\fuse\z80" /I "contrib\adapt" /D "DEBUG" /D "i486" /D UNDER_CE=300 /D _WIN32_WCE=300 /D "WIN32" /D "STRICT" /D "_WIN32_WCE_EMULATION" /D "INTERNATIONAL" /D "USA" /D "INTLMSG_CODEPAGE" /D "WIN32_PLATFORM_PSPC" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /Fp"X86EMDbg/PocketClive.pch" /YX /Fo"X86EMDbg/" /Fd"X86EMDbg/" /Gz /c 
"C:\Projects\PocketClive\2B\adapt.c"
"C:\Projects\PocketClive\2B\pocket_clive.cpp"
"C:\Projects\PocketClive\2B\pocket_clive_display.cpp"
"C:\Projects\PocketClive\2B\pocket_clive_sound.cpp"
"C:\Projects\PocketClive\2B\contrib\fuse\ay.c"
"C:\Projects\PocketClive\2B\contrib\fuse\display.c"
"C:\Projects\PocketClive\2B\contrib\fuse\event.c"
"C:\Projects\PocketClive\2B\contrib\fuse\keyboard.c"
"C:\Projects\PocketClive\2B\contrib\fuse\keysyms.c"
"C:\Projects\PocketClive\2B\contrib\fuse\machine.c"
"C:\Projects\PocketClive\2B\contrib\fuse\myglib\myglib.c"
"C:\Projects\PocketClive\2B\contrib\fuse\settings.c"
"C:\Projects\PocketClive\2B\contrib\fuse\snapshot.c"
"C:\Projects\PocketClive\2B\contrib\fuse\sound.c"
"C:\Projects\PocketClive\2B\contrib\fuse\spec128.c"
"C:\Projects\PocketClive\2B\contrib\fuse\spec48.c"
"C:\Projects\PocketClive\2B\contrib\fuse\specplus2.c"
"C:\Projects\PocketClive\2B\contrib\fuse\specplus3.c"
"C:\Projects\PocketClive\2B\contrib\fuse\spectrum.c"
"C:\Projects\PocketClive\2B\contrib\fuse\tape.c"
"C:\Projects\PocketClive\2B\contrib\easyce\EasyCE.cpp"
"C:\Projects\PocketClive\2B\contrib\zlib\adler32.c"
"C:\Projects\PocketClive\2B\contrib\zlib\compress.c"
"C:\Projects\PocketClive\2B\contrib\zlib\crc32.c"
"C:\Projects\PocketClive\2B\contrib\zlib\deflate.c"
"C:\Projects\PocketClive\2B\contrib\zlib\gzio.c"
"C:\Projects\PocketClive\2B\contrib\zlib\infblock.c"
"C:\Projects\PocketClive\2B\contrib\zlib\infcodes.c"
"C:\Projects\PocketClive\2B\contrib\zlib\inffast.c"
"C:\Projects\PocketClive\2B\contrib\zlib\inflate.c"
"C:\Projects\PocketClive\2B\contrib\zlib\inftrees.c"
"C:\Projects\PocketClive\2B\contrib\zlib\infutil.c"
"C:\Projects\PocketClive\2B\contrib\zlib\trees.c"
"C:\Projects\PocketClive\2B\contrib\zlib\uncompr.c"
"C:\Projects\PocketClive\2B\contrib\zlib\zutil.c"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP11.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP12.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib aygshell.lib gx.lib libspectrum.lib z80.lib /nologo /stack:0x10000,0x1000 /subsystem:windows /incremental:yes /pdb:"X86EMDbg/PocketClive.pdb" /map:"X86EMDbg/PocketClive.map" /debug /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /nodefaultlib:oldnames.lib /out:"X86EMDbg/PocketClive.exe" /libpath:"gapi\X86" /libpath:"contrib\fuse\libspectrum\X86EMDbg" /libpath:"contrib\fuse\z80\X86EMDbg" /windowsce:emulation /MACHINE:IX86 
.\X86EMDbg\adapt.obj
.\X86EMDbg\pocket_clive.obj
.\X86EMDbg\pocket_clive_display.obj
.\X86EMDbg\pocket_clive_sound.obj
.\X86EMDbg\ay.obj
.\X86EMDbg\display.obj
.\X86EMDbg\event.obj
.\X86EMDbg\keyboard.obj
.\X86EMDbg\keysyms.obj
.\X86EMDbg\machine.obj
.\X86EMDbg\myglib.obj
.\X86EMDbg\settings.obj
.\X86EMDbg\snapshot.obj
.\X86EMDbg\sound.obj
.\X86EMDbg\spec128.obj
.\X86EMDbg\spec48.obj
.\X86EMDbg\specplus2.obj
.\X86EMDbg\specplus3.obj
.\X86EMDbg\spectrum.obj
.\X86EMDbg\tape.obj
.\X86EMDbg\EasyCE.obj
.\X86EMDbg\adler32.obj
.\X86EMDbg\compress.obj
.\X86EMDbg\crc32.obj
.\X86EMDbg\deflate.obj
.\X86EMDbg\gzio.obj
.\X86EMDbg\infblock.obj
.\X86EMDbg\infcodes.obj
.\X86EMDbg\inffast.obj
.\X86EMDbg\inflate.obj
.\X86EMDbg\inftrees.obj
.\X86EMDbg\infutil.obj
.\X86EMDbg\trees.obj
.\X86EMDbg\uncompr.obj
.\X86EMDbg\zutil.obj
.\X86EMDbg\PocketClive.res
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP12.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
adapt.c
pocket_clive.cpp
pocket_clive_display.cpp
pocket_clive_sound.cpp
ay.c
C:\Projects\PocketClive\2B\pocket_clive_sound.cpp(204) : warning C4700: local variable 'sound_buffer_pos' used without having been initialized
display.c
C:\Projects\PocketClive\2B\contrib\fuse\display.c(332) : warning C4244: '=' : conversion from 'long ' to 'unsigned char ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\display.c(336) : warning C4244: '=' : conversion from 'long ' to 'unsigned char ', possible loss of data
event.c
C:\Projects\PocketClive\2B\contrib\fuse\display.c(273) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(278) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(449) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(451) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(462) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(465) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(496) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\display.c(325) : warning C4761: integral size mismatch in argument; conversion supplied
keyboard.c
keysyms.c
machine.c
myglib.c
settings.c
snapshot.c
C:\Projects\PocketClive\2B\contrib\fuse\snapshot.c(71) : warning C4090: 'function' : different 'const' qualifiers
C:\Projects\PocketClive\2B\contrib\fuse\snapshot.c(71) : warning C4024: 'open' : different types for formal and actual parameter 1
C:\Projects\PocketClive\2B\contrib\fuse\snapshot.c(159) : warning C4013: 'strncasecmp' undefined; assuming extern returning int
sound.c
C:\Projects\PocketClive\2B\contrib\fuse\sound.c(414) : warning C4244: '=' : conversion from 'unsigned long ' to 'unsigned short ', possible loss of data
C:\Projects\PocketClive\2B\contrib\fuse\sound.c(739) : warning C4018: '>=' : signed/unsigned mismatch
C:\Projects\PocketClive\2B\contrib\fuse\sound.c(828) : warning C4018: '>=' : signed/unsigned mismatch
spec128.c
C:\Projects\PocketClive\2B\contrib\fuse\spec128.c(60) : warning C4013: 'abort' undefined; assuming extern returning int
C:\Projects\PocketClive\2B\contrib\fuse\spec128.c(98) : warning C4244: 'function' : conversion from 'const double ' to 'unsigned long ', possible loss of data
spec48.c
C:\Projects\PocketClive\2B\contrib\fuse\spec128.c(62) : warning C4715: 'spec128_readbyte' : not all control paths return a value
C:\Projects\PocketClive\2B\contrib\fuse\spec128.c(84) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\spec48.c(60) : warning C4013: 'abort' undefined; assuming extern returning int
C:\Projects\PocketClive\2B\contrib\fuse\spec48.c(99) : warning C4244: 'function' : conversion from 'const double ' to 'unsigned long ', possible loss of data
specplus2.c
C:\Projects\PocketClive\2B\contrib\fuse\spec48.c(62) : warning C4715: 'spec48_readbyte' : not all control paths return a value
C:\Projects\PocketClive\2B\contrib\fuse\specplus2.c(45) : warning C4244: 'function' : conversion from 'const double ' to 'unsigned long ', possible loss of data
specplus3.c
C:\Projects\PocketClive\2B\contrib\fuse\specplus3.c(76) : warning C4013: 'abort' undefined; assuming extern returning int
C:\Projects\PocketClive\2B\contrib\fuse\specplus3.c(143) : warning C4244: 'function' : conversion from 'const double ' to 'unsigned long ', possible loss of data
spectrum.c
C:\Projects\PocketClive\2B\contrib\fuse\specplus3.c(88) : warning C4715: 'specplus3_readbyte' : not all control paths return a value
C:\Projects\PocketClive\2B\contrib\fuse\specplus3.c(130) : warning C4761: integral size mismatch in argument; conversion supplied
tape.c
C:\Projects\PocketClive\2B\contrib\fuse\spectrum.c(109) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(87) : warning C4090: 'function' : different 'const' qualifiers
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(87) : warning C4024: 'open' : different types for formal and actual parameter 1
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(423) : warning C4013: 'abort' undefined; assuming extern returning int
EasyCE.cpp
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(307) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(313) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(373) : warning C4761: integral size mismatch in argument; conversion supplied
C:\Projects\PocketClive\2B\contrib\fuse\tape.c(425) : warning C4715: 'trap_check_rom' : not all control paths return a value
adler32.c
compress.c
crc32.c
deflate.c
gzio.c
infblock.c
infcodes.c
inffast.c
inflate.c
inftrees.c
infutil.c
trees.c
uncompr.c
zutil.c
Linking...
LINK : fatal error LNK1104: cannot open file "gx.lib"
Error executing link.exe.



<h3>Results</h3>
PocketClive.exe - 1 error(s), 37 warning(s)
</pre>
</body>
</html>
