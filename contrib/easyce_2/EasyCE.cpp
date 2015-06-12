// EasyCE.cpp : API implementation - v1.6
// By Jacco Bikker, a.k.a. "The Phantom"
// Cool windows suggestions by Jaap Suter
// 2-bit dithering code by Jakob Nebeker
// --------------------------------------------------
 
#include "math.h"
#include "EasyCE.h"
#include "winbase.h"

#ifdef _WIN32_WCE
	#include "aygshell.h"
	#include "mmsystem.h"
#else
	#include "windows.h"
	#include "ptc.h"
	#include "stdio.h"
	Surface* surface;
	Format* format;
	Console* console;
#endif
#ifdef USE_ZLIB
#include "zlib.h"
#endif

HINSTANCE hInst;
TCHAR szTitle[30] = TEXT("easyce");
TCHAR wincls[30] = TEXT("easyce");
HWND hWnd;
HBITMAP hBM;
HPALETTE hPalettize;
ASurface* primary, *secondary;

// Custom button layout stuff
int scan_up, scan_left, scan_right, scan_down;
int scan_A, scan_B, scan_C, scan_D, scan_E, lastKey;

// Framebuffer stuff
PIXEL* buffer, *temp;
#ifdef USE_GAMEX
	GameX* gameX;
#endif
int method = 1, BPP, xpitch, ypitch;
bool pendwn = false;
bool paused = false;
bool imm = true;
unsigned int basetime = 0;
bool ipaq3800;

// Hardware buttons
#ifdef _WIN32_WCE
	GXKeyList kl;
#endif
int b1, b2, b3, b4, b5;
int bl, br, bu, bd;
int blp, brp, bup, bdp;
bool button1up, button2up, button3up, button4up, button5up;
bool buttonuup, buttondup, buttonlup, buttonrup;

// Stacks
int mxpos, mypos, mcxpos, mcypos;
bool mcprocessed, mbutton;

ASurface* AActor::s_Target = 0;
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);
extern void main( void );

ASurface* getsecondary() 
{
	return secondary; 
}

ASurface* getprimary()
{
	return primary;
}

#ifdef _WIN32_WCE
void createpalette( void )
{
	// Code by L Bailey, modified by Phantom
	LPLOGPALETTE logPalette = NULL;
	WORD palEntries = 256;
	if (BPP == 8) 
	{
		logPalette = (LOGPALETTE *) LocalAlloc (LMEM_FIXED, 
					 sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * palEntries);	
		logPalette->palVersion = (WORD) 0x0300;
		logPalette->palNumEntries = palEntries;
		// Create a 2-3-2 palette
		for (int i = 0; i < palEntries; i++) 
		{
			int r = (i & (3 << 5)) >> 5;
			int g = (i & (7 << 2)) >> 2;
			int b = i & 3;
			logPalette->palPalEntry[i].peRed = r << 6;
			logPalette->palPalEntry[i].peGreen = g << 5;
			logPalette->palPalEntry[i].peBlue = b << 6;
			logPalette->palPalEntry[i].peFlags = 0;
		}
		hPalettize = CreatePalette(logPalette);
		LocalFree ((HLOCAL) logPalette);
	}
	// Activate the palette
	HDC paletteHDC = GetDC(0);
	SelectPalette(paletteHDC, hPalettize, false);
	RealizePalette(paletteHDC);
	ReleaseDC(0, paletteHDC);
}
#endif

#ifdef _WIN32_WCE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#else
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#endif
{
	MSG msg;
	if( msg.hwnd = FindWindow(szTitle,NULL) )
	{
		ShowWindow(msg.hwnd,SW_SHOW);
		return FALSE;
	}
	secondary = new ASurface( 240, 320 );
	primary = new ASurface( 240, 320 );
	AActor::settarget( secondary );
	ASprite::settarget( secondary );
	mcprocessed = true;
	mbutton = false;
	b1 = b2 = b3 = b4 = b5 = 0;
	ASurface::initcharset();
#ifdef _WIN32_WCE
	taskbar( false );
	resettimer();
   	if (!MyRegisterClass(hInstance)) return FALSE;
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;
#endif
#ifdef _WIN32_WCE
#ifdef _X86_ 
	GXOpenDisplay( hWnd, GX_FULLSCREEN ); 
#else 
	GXOpenDisplay( NULL, GX_FULLSCREEN ); 
#endif
	GXOpenInput();
	kl = GXGetDefaultKeys( GX_NORMALKEYS );
	scan_up = kl.vkUp;
	scan_down = kl.vkDown;
	scan_left = kl.vkLeft;
	scan_right = kl.vkRight;
	scan_A = kl.vkA;
	scan_B = kl.vkB;
	scan_C = kl.vkC;
	scan_D = kl.vkStart;
	scan_E = 0;
	GXDisplayProperties dp = GXGetDisplayProperties();
	BPP = dp.cBPP;
	if (BPP == 8) createpalette();
	xpitch = dp.cbxPitch;
	ypitch = dp.cbyPitch;
	if ((!GXIsDisplayDRAMBuffer()) && (ypitch == 480)) 
	{
		// No need to do our own double buffering
		imm = false;
	}
#else
	format  = new Format( 16, 31 << 11, 63 << 5, 31 );
	console = new Console();
	console->option( "Windowed output" );
#ifdef LANDSCAPE
	console->open( "EasyCE", 320, 240, *format );
	surface = new Surface ( 320, 240, *format );
#else
	console->open( "EasyCE", 240, 320, *format );
	surface = new Surface ( 240, 320, *format );
#endif
	BPP = 16;
	xpitch = 1;
	ypitch = 240;
	hWnd = console->window();
#endif
	srand( clock() );
	// detect 3800 models
#ifdef _WIN32_WCE
	WCHAR oemstr[100];
	SystemParametersInfo( SPI_GETOEMINFO, sizeof( oemstr ), oemstr, 0 ); 
	ipaq3800 = false;
	if ((oemstr[12] == 'H') && (oemstr[13] == '3') && (oemstr[14] == '8')) ipaq3800 = true;
#endif
	// call main function (to be provided by user app)
	main();
#ifdef _WIN32_WCE
	taskbar( true );
	GXCloseInput();
	GXCloseDisplay();
#else
	delete console;
	delete format;
	delete surface;
#endif
	delete buffer;
	return 0;
}

void shutdown()
{
	taskbar( true );
#ifdef _WIN32_WCE
	GXCloseInput();
	GXCloseDisplay();
#endif
	delete buffer;
	exit( 0 );
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wcls;
	wcls.style			= 0;
	wcls.lpfnWndProc	= (WNDPROC)WndProc;
	wcls.cbClsExtra		= 0;
	wcls.cbWndExtra		= 0;
	wcls.hInstance		= hInstance;
	wcls.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(1));
	wcls.hCursor		= 0;
	wcls.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	wcls.lpszMenuName	= 0;
	wcls.lpszClassName	= wincls;
	return RegisterClass(&wcls);
}

BOOL InitInstance(HINSTANCE hi, int nCmdShow)
{
	hInst = hi;
	hWnd = CreateWindowEx(WS_EX_TOPMOST, wincls, szTitle, WS_VISIBLE, 0, 0, 240, 320, NULL, NULL, hi, NULL );
	if (!hWnd) return FALSE;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	short key = 0;
#ifdef _WIN32_WCE
	static SHACTIVATEINFO sai;
#endif
	switch (message)
	{
	#ifdef _WIN32_WCE
		// Dan's suggestions
		case WM_ENABLE: 
			break;
		case WM_ACTIVATE: 
			if (LOWORD(wParam) == WA_INACTIVE ) 
			{
//				taskbar( true );
//				GXCloseInput();
//				GXCloseDisplay();
//				paused = true;
//				break; 
			} 
			if (LOWORD(wParam) == WA_ACTIVE )
			{
//				taskbar( false );
//				GXOpenDisplay( NULL, GX_FULLSCREEN );
//				GXOpenInput();
//				paused = false;
			}
			break;
		case WM_CREATE: 
		case WM_SETTINGCHANGE: 
//			taskbar( false ); 
			break; 
	#endif
		case WM_LBUTTONDOWN:
			mcxpos = LOWORD( lParam );
			mcypos = HIWORD( lParam );
			mcprocessed = false;
			mbutton = true;
 			break;
		case WM_LBUTTONUP:
			mbutton = false;
			break;
		case WM_MOUSEMOVE:
			mxpos = LOWORD( lParam );
			mypos = HIWORD( lParam );
			// mbutton = (wParam == MK_LBUTTON);
			break;
		case WM_DESTROY:
			// ReleaseCapture();
			PostQuitMessage(0);
			break;
		case WM_KEYUP:
			key = (short)wParam;
		#ifdef _WIN32_WCE
			if (key == scan_left) bl = 0;
			if (key == scan_right) br = 0;
			if (key == scan_up) bu = 0;
			if (key == scan_down) bd = 0;
		#endif
			break;
		case WM_KEYDOWN:
			key = (short)wParam;
		#ifdef _WIN32_WCE
			if (key == scan_A) b1 = 1;
			if (key == scan_B) b2 = 1;
			if (key == scan_C) b3 = 1;
			if (key == scan_D) b4 = 1;
			if (key == scan_E) b5 = 1;
			if (key == scan_left) bl = blp = 1;
			if (key == scan_right) br = brp = 1;
			if (key == scan_up) bu = bup = 1;
			if (key == scan_down) bd = bdp = 1;
			lastKey = key;
		#else
			if (key == VK_INSERT) b1 = 1;
			if (key == VK_HOME) b2 = 1;
			if (key == VK_DELETE) b3 = 1;
			if (key == VK_END) b4 = 1;
			if (key == VK_CONTROL) b5 = 1;
			if (key == VK_LEFT) bl = blp = 1;
			if (key == VK_RIGHT) br = brp = 1;
			if (key == VK_UP) bu = bup = 1;
			if (key == VK_DOWN) bd = bdp = 1;
		#endif
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void taskbar( bool show )
{
#ifdef _WIN32_WCE
	// The Pocket PC way
	RECT rc;
	GetWindowRect( hWnd, &rc );
	HWND hWndTB=FindWindow(TEXT("HHTaskbar"),NULL);
	if (show)
	{
		SHFullScreen( hWnd, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON + SHFS_SHOWSTARTICON );
		ShowWindow( hWndTB, SW_SHOW );
		MoveWindow( hWnd, 
					rc.left, 
					rc.top + MENU_HEIGHT, 
					rc.right, 
					rc.bottom - 2 * MENU_HEIGHT,
					TRUE
				  );
	}
	else
	{
		SHFullScreen( hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON + SHFS_HIDESTARTICON );
		ShowWindow( hWndTB, SW_HIDE );
		MoveWindow( hWnd, 
					rc.left, 
					rc.top - MENU_HEIGHT, 
					rc.right, 
					rc.bottom + MENU_HEIGHT,
					TRUE
				  );
	}
#endif
}

void updatedisplay( int ysize )
{
#ifdef _WIN32_WCE
	if (ipaq3800)
	{
		PIXEL* dst = (PIXEL*)0xac0755a0;
		PIXEL* src = secondary->getbuffer();
		for ( int y = 0; y < ysize; y++ )
		{
			PIXEL* ldst = dst;
			for ( int x = 0; x < 240; x++ )
			{
				*(ldst) = *src++;
				ldst -= 320;
			}
			dst++;
		}
	}
	else
	{
		if (BPP == 16)
		{
			PIXEL* dst = (PIXEL*)GXBeginDraw();
			PIXEL* src = secondary->getbuffer();
			if (xpitch == 2)
			{
				if (ypitch == 480)
				{
					// This is a really nice display (casio's)
					memcpy( dst, src, 240 * ysize * 2 );
				}
				else
				{
					for ( int y = 0; y < ysize; y++ )
					{
						memcpy( dst, src, 480 );
						dst += 256;
						src += 240;
					}
				}
			}
			else
			{
			#if 0
				// Dan's way
				dst -= 319;
				src += 319 * 240;
				for ( int y = 0; y < 240; y++ )
				{
					PIXEL* lsrc = src;
					unsigned int* ldst = (unsigned int*)dst;
					for ( int x = 0; x < 160; x++ )
					{
						*ldst++ = *lsrc + (*(lsrc - 240) << 16);
						lsrc -= 480;
					}
					dst += 320;
					src++;
				}
			#else
				// original way
				for ( int y = 0; y < ysize; y++ )
				{
					PIXEL* ldst = dst;
					for ( int x = 0; x < 240; x++ )
					{
						*(ldst) = *src++;
						ldst += 320; // (xpitch >> 1);
					}
					dst += -1; // ypitch >> 1;
				}
			#endif
			}
			GXEndDraw();
		}
		else if (BPP == 8)
		{
			// Use the default palette
			unsigned char* dst = (unsigned char*)GXBeginDraw();
			unsigned short* src = (unsigned short*)buffer;
			for ( int y = 0; y < ysize; y++ )
			{
				for ( int x = 0; x < 240; x++ )
				{
					PIXEL color = *src;
					int r = (color & (3 << 14)) >> 9;
					int g = (color & (7 << 8)) >> 5;
					int b = (color & (3 << 3)) >> 3;
					*dst = r + g + b;
					dst++;
					src++;
				}
			}
			GXEndDraw();
		}
		else if (BPP == 4)
		{
			// Assume this is a monochrome iPaq
			unsigned char* dst = (unsigned char*)GXBeginDraw();
			PIXEL* src = secondary->getbuffer();
			src += 319 * 240; // start at lower left corner
			for ( int y = 0; y < 240; y++ )
			{
				unsigned char* ldst = dst;
				PIXEL* lsrc = src;
				for ( int x = 0; x < 160; x++ )
				{
					PIXEL c1 = *(lsrc - 240);
					PIXEL c2 = *lsrc;
					c1 = ((c1 & REDMASK) >> 11) + ((c1 & GREENMASK) >> 5) + (c1 & BLUEMASK);
					c2 = ((c2 & REDMASK) >> 11) + ((c2 & GREENMASK) >> 5) + (c2 & BLUEMASK);
					*(ldst) = (c1 >> 3) + ((c2 >> 3) << 4);
					ldst++;
					lsrc -= 480;
				}
				dst += 160;
				src++;
			}
			GXEndDraw();
		}
	}
#else
	PIXEL* dest = (PIXEL*)surface->lock();
	memcpy( dest, secondary->getbuffer(), 240 * ysize * 2 );
	surface->unlock();
	surface->copy( *console );
	console->update();
#endif
}

void update( bool updscr, int ysize )
{
	if (updscr) updatedisplay( ysize );
#ifdef _WIN32_WCE
	MSG msg;
	if (paused)
	{
		while (PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) == TRUE)
		{
			GetMessage( &msg, 0, 0, 0 );
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		Sleep( 1 );
	}
	else
	{
		while (PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) == TRUE)
		{
			GetMessage( &msg, 0, 0, 0 );
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
#else
	POINT p;
	RECT r;
	GetWindowRect( hWnd, &r );
	GetCursorPos( &p );
	mxpos = p.x - r.left;
	mypos = p.y - r.top - MENU_HEIGHT;
	if (GetAsyncKeyState( VK_LBUTTON ))
	{
		if (!mbutton)
		{
			mcxpos = p.x - r.left;
			mcypos = p.y - r.top - MENU_HEIGHT;
			mcprocessed = false;
			mbutton = true;
		}
	}
	else mbutton = false;
	short bt1 = GetAsyncKeyState( VK_INSERT );
	short bt2 = GetAsyncKeyState( VK_HOME );
	short bt3 = GetAsyncKeyState( VK_DELETE );
	short bt4 = GetAsyncKeyState( VK_END );
	short bt5 = GetAsyncKeyState( VK_TAB );
	if (bt1) if (button1up) { b1 = 1; button1up = false; }
	if (bt2) if (button2up) { b2 = 1; button2up = false; }
	if (bt3) if (button3up) { b3 = 1; button3up = false; }
	if (bt4) if (button4up) { b4 = 1; button4up = false; }
	if (bt5) if (button5up) { b5 = 1; button5up = false; }
	if (!bt1) button1up = true;
	if (!bt2) button2up = true;
	if (!bt3) button3up = true;
	if (!bt4) button4up = true;
	if (!bt5) button5up = true;
	if (GetAsyncKeyState( VK_LEFT )) bl = 1; else bl = 0;
	if (GetAsyncKeyState( VK_RIGHT )) br = 1; else br = 0;
	if (GetAsyncKeyState( VK_UP )) bu = 1; else bu = 0;
	if (GetAsyncKeyState( VK_DOWN )) bd = 1; else bd = 0;
	if (bl) if (buttonlup) { blp = 1; buttonlup = false; }
	if (br) if (buttonrup) { brp = 1; buttonrup = false; }
	if (bu) if (buttonuup) { bup = 1; buttonuup = false; }
	if (bd) if (buttondup) { bdp = 1; buttondup = false; }
	if (!bl) buttonlup = true;
	if (!br) buttonrup = true;
	if (!bu) buttonuup = true;
	if (!bd) buttondup = true;
#endif
}

bool buttonA()
{
	bool retval = (b1 != 0);
	b1 = 0;
	return retval;
}

bool buttonB()
{
	bool retval = (b2 != 0);
	b2 = 0;
	return retval;
}

bool buttonC()
{
	bool retval = (b3 != 0);
	b3 = 0;
	return retval;
}

bool buttonD()
{
	bool retval = (b4 != 0);
	b4 = 0;
	return retval;
}

bool buttonE()
{
	bool retval = (b5 != 0);
	b5 = 0;
	return retval;
}

bool leftpressed()
{
	bool retval = (blp != 0);
	blp = 0;
	return retval;
}

bool rightpressed()
{
	bool retval = (brp != 0);
	brp = 0;
	return retval;
}

bool uppressed()
{
	bool retval = (bup != 0);
	bup = 0;
	return retval;
}

bool downpressed()
{
	bool retval = (bdp != 0);
	bdp = 0;
	return retval;
}

bool buttonleft()
{
	bool retval = (bl != 0);
	return retval;
}

bool buttonright()
{
	bool retval = (br != 0);
	return retval;
}

bool buttonup()
{
	bool retval = (bu != 0);
	return retval;
}

bool buttondown()
{
	bool retval = (bd != 0);
	return retval;
}

bool clicked( void )
{
	bool retval = mcprocessed;
	mcprocessed = true;
	return !retval;
}

void clearinput()
{
	blp = brp = bup = bdp = b1 = b2 = b3 = b4 = b5 = bl = bu = br = bd = 0;
	mcprocessed = true;
}

bool pressed( bool clear )
{
	bool retval = false;
	if (buttonA() || buttonB() || buttonC() || buttonD() || buttonE()) retval = true;
	if (!clear) if ((blp != 0) || (brp != 0) || (bup != 0) || (bdp != 0)) retval = true;
	if (clear) if (uppressed() || downpressed() || leftpressed() || rightpressed()) retval = true;
	return retval;
}

int clickxpos( void )
{
	return mcxpos;
}

int clickypos( void )
{
	return mcypos;
}

int penxpos( void )
{
	return mxpos;
}

int penypos( void )
{
	return mypos;
}

bool pendown( void )
{
	return mbutton;
}

void resettimer( void )
{
	basetime = GetTickCount();
}

int clock( void )
{
	return GetTickCount();
}

int msecs( void )
{
	return GetTickCount() - basetime;
}

void fatalerr( char* t )
{
	secondary->bar( 5, 100, 235, 150, 0 );
	secondary->box( 5, 100, 235, 150, 0xFFFF );
	secondary->centre( "fatal error:", 110, 0xFFFF );
	secondary->centre( t, 130, 0xFFFF );
	while (clicked()) update();
	while (!clicked()) update();
	shutdown();
}

int lastkey() { return lastKey; }
void zerolastkey() { lastKey = 0; }
void setleftscan( int code ) { scan_left = code; }
void setrightscan( int code ) { scan_right = code; }
void setupscan( int code ) { scan_up = code; }
void setdownscan( int code ) { scan_down = code; }
void setAscan( int code ) { scan_A = code; }
void setBscan( int code ) { scan_B = code; }
void setCscan( int code ) { scan_C = code; }
void setDscan( int code ) { scan_D = code; }
void setEscan( int code ) { scan_E = code; }
int getleftscan() { return scan_left; }
int getrightscan() { return scan_right; }
int getupscan() { return scan_up; }
int getdownscan() { return scan_down; }
int getAscan() { return scan_A; }
int getBscan() { return scan_B; }
int getCscan() { return scan_C; }
int getDscan() { return scan_D; }
int getEscan() { return scan_E; }

// --------------------------------------------------
// EOF