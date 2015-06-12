// EasyCE.h : API
// By Jacco Bikker, a.k.a. "The Phantom"
// --------------------------------------------------

#ifndef EASYCE_H
#define EASYCE_H

#if _MSC_VER > 1000
#pragma once
#endif

#define WIN32_LEAN_AND_MEAN

/* The number of bytes in a char.  */
#undef SIZEOF_CHAR
#define SIZEOF_CHAR 1


/* The number of bytes in a int.  */
#undef SIZEOF_INT
#define SIZEOF_INT 2

/* The number of bytes in a long.  */
#undef SIZEOF_LONG
#define SIZEOF_LONG 4

/* The number of bytes in a short.  */
#undef SIZEOF_SHORT
#define SIZEOF_SHORT 2
#include <windows.h>



#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include "gx.h"
#ifdef _WIN32_WCE
#define USE_GAPI
#include <aygshell.h>
#include <wceatl.h>
#include "atlconv.h"
#endif

// Internal EasyCE stuff
#ifdef _X86_
#define MENU_HEIGHT 0
#else
#define MENU_HEIGHT 26
#endif

// ZLib compression
#define USE_ZLIB

// Landscape
// #define LANDSCAPE
#ifdef LANDSCAPE
	#define SCRWIDTH	320
	#define SCRHEIGHT	240
#else
	#define SCRWIDTH	240
	#define SCRHEIGHT	320
#endif

// Color component masks
#define REDMASK (31<<11)
#define GREENMASK (63<<5)
#define BLUEMASK (31)

// Pixel defines
#define PIXEL unsigned short			// 16bit pixel
#define IPIXEL unsigned char			// palettized pixel

// Shift masks
#define SHFTMASK ((15<<11)+(31<<5)+15)	// for single shifts
#define DSHFTMASK ((7<<11)+(15<<5)+7)	// for double shifts
#define TSHFTMASK ((3<<11)+(7<<5)+3)	// for triple shifts

// Fixed point defines
#define FPP 10
#define FP_ONE (1<<FPP)
#define FPMUL(x,y) (((x)*(y))>>FPP)
#define FPDIV(x,y) (((x)<<FPP)/(y))
#define FP(x) ((int)((x)*FP_ONE+0.5f))
#define IFP(x) ((x)<<FPP)

// Basic math stuff
#define PI 3.14159265359

// Sprite flags
#define SPR_FLARE		1
#define SPR_MIRROR		2
#define SPR_WHITE		4
#define SPR_OPAQUE		8
#define SPR_FLASH		16
#define SPR_IREFFECT	32
#define SPR_DISABLED	64
#define SPR_FLASHLIGHT	128

// WinCE / Windows compatibility
#if _WIN32_WCE==300
typedef unsigned short ACHAR;
#else
typedef char ACHAR;
#endif

// Forward declarations
class ASurface;

// EasyCE API

// Shut down EasyCE gracefully
void shutdown();

// Update: Call this every frame
void update( bool updscr = true, int ysize = 320 );

// Retrieve a pointer to the internal buffer
ASurface* getsecondary();

// Retrieve a pointer to the hardware frame buffer
ASurface* getprimary();

// Retrieve a pointer to the frame buffer
// Note: Hardware dependent! Use with care
PIXEL* getscrptr( void );

// Hide or show the taskbar (internal EasyCE)
void taskbar( bool show );

// Play a sample
void* loadwav( char* a_Snd );
void playfilewav( ACHAR* a_Snd );
void playmemwav( void* a_Snd );

// Button config stuff
void setleftscan( int code );
void setrightscan( int code );
void setupscan( int code );
void setdownscan( int code );
void setAscan( int code );
void setBscan( int code );
void setCscan( int code );
void setDscan( int code );
void setEscan( int code );
int getleftscan();
int getrightscan();
int getupscan();
int getdownscan();
int getAscan();
int getBscan();
int getCscan();
int getDscan();
int getEscan();
int lastkey();
void zerolastkey();

// Mouse and button handling
// Clear all clicks and presses
void clearinput( void );
// Has there been a click since last time we asked?
bool clicked( void );
// Has there been a button press since last time we asked?
bool pressed( bool clear = true );
// What was the location of the last click?
int clickxpos( void );
int clickypos( void );
// What is the current pen location (dragging)?
int penxpos( void );
int penypos( void );
// Is the user dragging with the stylus?
bool pendown( void );
// Is button 'x' currently down?
bool buttonA();
bool buttonB();
bool buttonC();
bool buttonD();
bool buttonE();
bool buttonleft();
bool buttonright();
bool buttonup();
bool buttondown();
// Has button 'x' been pressed since last call?
bool leftpressed();
bool rightpressed();
bool uppressed();
bool downpressed();

// Timer
// Reset the internal timer (millisecond timer)
void resettimer( void );
// Get the clock (milliseconds)
int clock( void );
// Get milliseconds since last resettimer call
int msecs( void );

// Some useful functions
int filesize( char* a_File );
void ttoa( int v, char* s );
void itoa( int v, char* s );
double strtod( char* t, char** e );
int stricmp( char* s1, char* s2 );
unsigned int getdeviceid();
void fatalerr( char* t );
char* fullpath( char* a_File );
LPWSTR fullpath( LPWSTR a_File );
inline int randint( int a_Range ) { return (rand() * (a_Range + 1)) / RAND_MAX; }
// Fast blending inline functions
// addblend: Add two colors with saturation
inline PIXEL addblend( PIXEL c1, PIXEL c2 )
{
	PIXEL c = (PIXEL)(((c1 >> 1) & SHFTMASK) + ((c2 >> 1) & SHFTMASK));
	if (c & 0x8410)
	{
		if (c & 0x8000) c = (c & 0x7FF) + 0x7800;
		if (c & 0x400) c = (c & 0xF81F) + 0x3E0;
		if (c & 0x10) c = (c & 0xFFE0) + 0xF;
	}
	return (PIXEL)(c << 1);
}
// scalecolor: Scale a color. Mul must be 0..31
inline PIXEL scalecolor( PIXEL c, int mul )
{
	unsigned int p = (((c&0xF81F)<<11)|((c&0x7E0)>>5))*mul;
	return (((p>>16)&0xF81F)|(p&0x7E0));
}

// END OF EASYCE API

#define SURF_HICOLOR		1
#define SURF_PALETTIZED		2
#define SURF_OWN			4

class ASurface
{
public:
	ASurface( int a_Width, int a_Height );
	ASurface( int a_Width, int a_Height, PIXEL* a_Buffer );
	ASurface( char* a_File, bool a_Zipped = false );
	~ASurface();
	PIXEL* getbuffer() { return m_Buffer; }
	int getwidth() { return m_Width; }
	int getheight() { return m_Height; }
	void clear( PIXEL color );
	void print( char* a_String, int x1, int y1, PIXEL color );
	void print( char* a_String, PIXEL color );
	void centre( char* a_String, int y, PIXEL color );
	void line( int x1, int y1, int x2, int y2, PIXEL color );
	void plot( int x1, int y1, PIXEL color );
	void hline( int x1, int y1, int len, PIXEL color );
	void vline( int x1, int y1, int len, PIXEL color );
	void box( int x1, int y1, int x2, int y2, PIXEL color );
	void bar( int x1, int y1, int x2, int y2, PIXEL color );
	// Primitives using xor operation
	void line_x( int x1, int y1, int x2, int y2, PIXEL color );
	void hline_x( int x1, int y1, int len, PIXEL color );
	void vline_x( int x1, int y1, int len, PIXEL color );
	void box_x( int x1, int y1, int x2, int y2, PIXEL color );
	void bar_x( int x1, int y1, int x2, int y2, PIXEL color );
	void plot_x( int x1, int y1, PIXEL color );
	// Primitives using additive blending
	void plot_a( int x1, int y1, PIXEL color );
	void line_a( int x1, int y1, int x2, int y2, PIXEL color );
	void bar_a( int x1, int y1, int x2, int y2, PIXEL color );
	void box_a( int x1, int y1, int x2, int y2, PIXEL color );
	void hline_a( int x1, int y1, int len, PIXEL color );
	void vline_a( int x1, int y1, int len, PIXEL color );
	// TGA loader
	void tgaloaderror( char* a_Desc );
	void tgaload( char* file ) { tgaload( file, m_Width, m_Height, m_Buffer ); }
	bool tgaload( char* file, int w, int h, IPIXEL* dest, PIXEL* pal = 0 );
	bool tgaload( char* file, int w, int h, PIXEL* dest );
	void parseheader( unsigned char* a_Header );
	#ifdef USE_ZLIB
		void tgazload( char* file ) { tgazload( file, m_Width, m_Height, m_Buffer ); }
		bool tgazload( char* file, int w, int h, IPIXEL* dest, PIXEL* pal );
		bool tgazload( char* file, int w, int h, PIXEL* dest );
	#endif
	// Copying
	void copyfrom( ASurface* a_Src );
	void copyto( ASurface* a_Dst ) { a_Dst->copyfrom( this ); }
	// Get a pixel at the specified location
	PIXEL getpixel( int x1, int y1 );
	// Font stuff
	static void setchar( int c, char* c1, char* c2, char* c3, char* c4, char* c5, char* c6, char* c7 );
	static void initcharset();
private:
	int m_Flags;
	int m_Width, m_Height;
	PIXEL* m_Buffer;
	static char* m_chr[57][7];
	static int m_transl[256];
	// Member variables for the tga loader
	int m_TgaIDLen;
	int m_TgaCMapType;
	int m_TgaImgType;
	int m_TgaCMapOrig;
	int m_TgaCMapLen;
	int m_TgaCMapSize;
	int m_TgaXPos;
	int m_TgaYPos;
	int m_TgaWidth;
	int m_TgaHeight;
	int m_TgaPixSize;
};

// iPaq sprite engine - February 2001 - The Phantom
class ASprite
{
public:
	ASprite( void );
	ASprite( int a_Width, int a_Height, ASurface* a_Surf );
	~ASprite( void );
	void draw();
	void drawscaled( int a_Width, int a_Height );
	int width( void ) { return m_Width; }
	int height( void ) { return m_Height; }
	int pitch( void) { return m_Pitch; }
	void setflags( int a_Flags ) { m_Flags = a_Flags; }
	static void settarget( ASurface* a_Dst ) { s_Dest = a_Dst; }
	static void setbackdrop( PIXEL* a_Backdrop ) { s_Backdrop = a_Backdrop; }
	void setpos( int a_XPos, int a_YPos ) { m_XPos = a_XPos; m_YPos = a_YPos; }
	PIXEL* getbitmap() { return m_Bitmap; }
	void sety( int a_YPos ) { m_YPos = a_YPos; }
	void setx( int a_XPos ) { m_XPos = a_XPos; }
	int getx() { return m_XPos; }
	int gety() { return m_YPos; }
private:
	int m_Width, m_Height, m_Pitch, m_XPos, m_YPos;
	int m_Flags;
	int* m_Start;
	PIXEL* m_Bitmap;
	static PIXEL* s_Backdrop;
	static ASurface* s_Dest;
};

// Animation container class
class AAnim
{
public:
	AAnim();
	AAnim( int a_Frames );
	~AAnim();
	void addframe( ASprite* a_Frame ) { m_Sprite[m_Frames++] = a_Frame; }
	void draw();
	void drawscaled( int a_Width, int a_Height );
	void setframe( int a_Idx ) { m_Frame = a_Idx; }
	void setpos( int a_XPos, int a_YPos ) { m_XPos = a_XPos; m_YPos = a_YPos; }
	int getx() { return m_XPos; }
	int gety() { return m_YPos; }
	void setx( int a_XPos ) { m_XPos = a_XPos; }
	void sety( int a_YPos ) { m_YPos = a_YPos; }
	void setflags( int a_Flags ) { m_Flags = a_Flags; }
	ASprite* getsprite( int a_Idx ) { return m_Sprite[a_Idx]; }
private:
	int m_Frames;
	int m_Frame;
	int m_XPos, m_YPos;
	int m_Flags;
	ASprite** m_Sprite;
};

// Custom font stuff
class AFont
{
public:
	AFont();
	~AFont();
	void setdata( PIXEL* a_Data, int a_Width, int a_Height, int a_Pitch );
	void setchars( char* a_Chars );
	void settarget( PIXEL* a_Dest, int a_Pitch ) { m_Dest = a_Dest, m_DPitch = a_Pitch; }
	void print( char* a_Text, int a_X, int a_Y );
	void centre( char* a_Text, int a_Y );
private:
	int m_Width, m_Height, m_Pitch;
	int m_DPitch;
	char* m_Chars;
	int* m_Transl;
	PIXEL* m_Dest;
	PIXEL* m_FData;
};

// Generic actor class
class AActor
{
public:
	AActor() {};
	~AActor() {};
	virtual bool update() = 0;
	static void settarget( ASurface* a_Surf ) { s_Target = a_Surf; }
protected:
	bool* m_Active;
	static ASurface* s_Target;
};

// Growing actor pool
class AActorPool
{
public:
	AActorPool();
	~AActorPool();
	void update();
	void addactor( AActor* a_Actor );
	void removeactor( AActor* a_Actor );
	void removeall();
private:
	AActor** m_Actor;
	int m_Poolsize;
};

// Basic particle bomb actor
class APBomb : public AActor
{
public:
	APBomb();
	APBomb( int a_X, int a_Y, int a_Nr );
	~APBomb();
	bool update();
	void setcolor( PIXEL a_Color ) { m_Color = a_Color; }
	int getcolor() { return m_Color; }
private:
	int m_Particles, m_AParticles;
	int* m_X, *m_Y, *m_VX, *m_VY;
	unsigned char* m_Life;
	int m_Color;
	static int* s_VX, *s_VY;
};

// Basic wave file playback class
class ASound
{
public:
	ASound();
	ASound( char* a_Snd );
	~ASound();
	void load( char* a_Snd );
	void play();
private:
	void* m_Buffer;
};

#endif

// --------------------------------------------------
// EOF