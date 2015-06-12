Service Release 1B

First of all thanks to all the people that has given me support and feedback, 
especially Peter Duyts and Vasyl Tsvikunov who has given me detailed
error reports and success stories.

Keep it coming!

This is a in-between release to fix some of the more obvious bugs in the 
R1A release. The following issues has been addressed : 

- It is now possible to only have .z80 files in the \Roms directory, i.e.
  no .sna file is needed.
- The program does not crash if you press up and down if no snapshots
  are available.
- PocketClive does not use absolute paths anymore, so if you want to move
  the program after installation to e.g. a flash card this should work fine.
  But the snapshots has to reside in a directory called Roms that is present
  at the same location as the executable PocketClive.exe.
- The hardware button issue has hopefully been addressed, i.e. there should
  not be any delay anymore, but since I have an iPAQ it is hard to test. 

Gothenburg Sweden 2001-10-15, Anders Holmberg

---

ReadMe for PocketClive

PocketClive is a Sinclair ZX Spectrum emulator for the PocketPC 
operating system. The goal of the emulator is for me to have fun programming
(it was more fun programming the 80's, don't you think?), and to make it 
possible for me to play my favourite games during those boring meetings. 

1. Features

PocketClive Release 1 supports the following features : 

- ZX Sinclair ZX Spectrum 48k (issue 3)
- ZX Sinclair ZX Spectrum 128k
- Kempston joystick support (both PocketPC joypad and virtual joypad)
- Loads snapshots in .sna and .z80 format
- Frame skips configurable (but emulator runs at 100% without frame
  skips on iPAQ)
- Virtual keyboard

2. Left to do

- Sound, sound and sound is totally missing in this release
- Tape support
- Remove limitation that all snapshots must reside in the directory
  \Program Files\PocketClive\Roms
- Landscape mode
- More joystick "modes", e.g. firing when moving to make some games more
  playable on the iPAQ

3. Bugs

- Loading of 128k .sna snapshots seems to result in the error   
  "corrupt memory". PocketClive uses something called libspectrum for 
  loading of snapshots and I have had no time to look for this bug.  
  128k .sna files are pretty rare though.
- If the emulator is running when ActiveSync is trying to connect to
  the PocketPC the display flickers, and responses to "key" presses
  can be disturbed. Not sure if this is PocketClive bug or a 
  PocketPC feature. 
- Not all buttons work on my iPAQ (I am mapping all buttons that GAPI
  gives me as joystick fire buttons).
- It is not possible to load a 128k snapshot when in 48k mode and vice verse.

4. Bug report
Please report any bugs or comments to aagholmberg@yahoo.se.

5. Installation instructions
If you have downloaded the installation zip program 
(PocketClive_Install_R1A.zip), unzip it and run the installation program, 
and follow the instructions. 

You must have GAPI (Microsoft Game API) installed on your PocketPC to make
PocketClive work, and this can be found at http:\\www.pocketpc.com. The gx.dll
file for your system must be downloaded to the \Windows directory.

If you have downloaded the executable zip files, unzip the on your
desktop computer and download them (using ActiveSync) to your PocketPC. 
The files must reside in \Program Files\PocketClive (sorry for that)

Note : 

I only have access to an iPAQ so the Mips and SH version are not tested by me. The use of the EasyCe graphical framework shall in theory make it possible to use the Mips and SH version. Please report if they are working 
(aagholmberg@yahoo.se). 

6. Snapshots
All snapshots must be stored in the directory \Program Files\PocketClive\Roms.

7. Emulator instructions
The emulator has three configuration buttons on the bottom left : 

7.1 Disk button
This button lists all .z80 and .sna files stored in the \Program Files\PocketClive\Roms. Just press the snapshot you want to load and it will be loaded. Any error message should really be interpreted as "I failed to load your file, wait for next version of PocketClive". 

If you do not want to load a snapshot just press the disk button again. 

7.2 Joystick/Keyboard button
This button switches between the virtual joystick and the virtual keyboard. 

7.3 Configuration button
This button brings up the configuration screen. Currently it is possible to choose which target platform you want to work with, and how many frame skips you want. It is also possible to reset and to exit the emulator. 

If you want to leave the configuration screen just press the configuration button again. 

Enjoy! Gothenburg, Sweden 2001-10-12 Anders Holmberg
