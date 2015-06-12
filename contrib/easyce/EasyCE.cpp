// EasyCE.cpp : API implementation - v1.6
// By Jacco Bikker, a.k.a. "The Phantom"
// Cool windows suggestions by Jaap Suter
// 2-bit dithering code by Jakob Nebeker
// --------------------------------------------------
 
extern "C"
{
#include "..\fuse\config.h"
#include "..\fuse\types.h"
}

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

#ifdef _X86_
#undef MENU_HEIGHT
#define MENU_HEIGHT 0
#endif

HINSTANCE hInst;
TCHAR szTitle[30] = TEXT("easyce");
TCHAR wincls[30] = TEXT("easyce");
HWND hWnd;
HBITMAP hBM;

// Custom button layout stuff
int scan_up, scan_left, scan_right, scan_down;
int scan_A, scan_B, scan_C, scan_D, scan_E, lastKey;

// Framebuffer stuff
PIXEL* buffer, *temp;
char* m_chr[57][7];
int m_transl[256];
#ifdef USE_GAMEX
	GameX* gameX;
#endif
int cursorx, cursory;
int method = 1, BPP, xpitch, ypitch;
bool pendwn = false;
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

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);
extern void __cdecl main( void );
void initcharset( void );

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

extern void pause_emulation( void );
extern void restart_emulation( void );

void ttoa( int v, char* s )
{
	int sec = v % 60;
	int min = (v / 60) % 60;
	int hour = (v / 3600);
	if (hour > 99)
	{
		// overflow
		hour = 88;
		sec = 88;
		min = 88;
	}
	*s = hour / 10 + '0';
	*(s + 1) = hour % 10 + '0';
	*(s + 2) = ':';
	*(s + 3) = min / 10 + '0';
	*(s + 4) = min % 10 + '0';
	*(s + 5) = ':';
	*(s + 6) = sec / 10 + '0';
	*(s + 7) = sec % 10 + '0';
	*(s + 8) = 0;
}

int filesize( char* a_File )
{
	FILE* dummy = fopen( fullpath( a_File ), "rb" );
	if (dummy)
	{
		fseek( dummy, 0, SEEK_END );
		int pos = ftell( dummy );
		fclose( dummy );
		return pos;
	}
	else
	{
		return 0;
	}
}

void itoa( int v, char* s )
{
	if (v == 0)
	{
		*s = '0';
		*(s + 1) = 0;
		return;
	}
	int mul = 10000000;
	bool first = true;	
	char* cur = s;
	if ( v < 0)
	{
		*cur++ = '-';
		v = -v;
	}
	while (mul > 0)
	{
		int digit = (v / mul) % 10;
		if ((digit > 0) || (!first))
		{
			*cur++ = '0' + digit;
			first = false;
		}
		mul /= 10;
	}
	*cur = 0;
}

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
	buffer = new PIXEL[320 * 240];
#ifdef _WIN32_WCE
	taskbar( false );
	resettimer();
   	if (!MyRegisterClass(hInstance)) return FALSE;
	if (!InitInstance (hInstance, nCmdShow)) return FALSE;
	GXOpenDisplay( hWnd, GX_FULLSCREEN );
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
	mcprocessed = true;
	mbutton = false;
	b1 = b2 = b3 = b4 = b5 = 0;
	initcharset();
	cursorx = 2;
	cursory = 2;
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
	for ( int x = 0; x < 57; x++ ) for ( int y = 0; y < 7; y++ ) delete m_chr[x][y];
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
	for ( int x = 0; x < 57; x++ ) for ( int y = 0; y < 7; y++ ) delete m_chr[x][y];
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
		case WM_ENABLE: 
			break;
		case WM_ACTIVATE: 
			if (LOWORD(wParam) == WA_INACTIVE ) 
			{ 
				// set pause flag; 
				pause_emulation();
			} 
			else if (LOWORD(wParam) == WA_ACTIVE ) 
			{
				// clear pause flag;
				restart_emulation();
			}
			break;
		case WM_CREATE: 
		case WM_SETTINGCHANGE: 
			taskbar( false );
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
			if (key == scan_A) b1 = 0;
			if (key == scan_B) b2 = 0;
			if (key == scan_C) b3 = 0;
			if (key == scan_D) b4 = 0;
			if (key == scan_E) b5 = 0;
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
	#ifdef USE_GAMEX
		// The winCE 2.x way
		#define TASKBAR_KEY _T("\\software\\microsoft\\shell") 
		HKEY hKey=0;
		RegOpenKeyEx( HKEY_LOCAL_MACHINE, TASKBAR_KEY, 0, KEY_ALL_ACCESS, &hKey );
		DWORD dwValue = 0;
		unsigned long lSize = sizeof( DWORD );
		DWORD dwType = REG_DWORD;
		RegQueryValueEx( hKey, _T("TBOpt"), 0, &dwType, (BYTE*)&dwValue, &lSize );
		if (show) dwValue &= 0xFFFFFFFF - 8;// reset bit to show taskbar
	 		 else dwValue |= 8;				// set bit to hide taskbar
		RegSetValueEx( hKey, _T("TBOpt"), 0, REG_DWORD, (BYTE*)&dwValue, lSize );
		MSG msg;
		msg.hwnd = FindWindow( _T("HHTaskBar"), NULL );
		::SendMessage( msg.hwnd, WM_COMMAND, 0x03EA, 0 );
		::SetForegroundWindow( hWnd );
	#else
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
#endif
}

void updatedisplay( int ysize )
{
#ifdef _WIN32_WCE
	if (ipaq3800)
	{
		PIXEL* dst = (PIXEL*)0xac0755a0;
		PIXEL* src = getbuffer();
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
			PIXEL* src = getbuffer();
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
				// Let's do this really clean (and slow:)
				for ( int y = 0; y < ysize; y++ )
				{
					PIXEL* ldst = dst;
					for ( int x = 0; x < 240; x++ )
					{
						*(ldst) = *src++;
						ldst += (xpitch >> 1);
					}
					dst += ypitch >> 1;
				}
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
					//int r = (color & (3 << 14)) >> 8;
					//int g = (color & (7 << 8)) >> 5;
					//int b = (color & (3 << 3)) >> 3;

					int r = (color & (7 << 13)) >> 8;
					int g = (color & (7 << 8)) >> 6;
					int b = (color & (3 << 3)) >> 3;

					*dst = r + g + b;
					dst++;
					src++;
					
					// 565 till 332		
					// RRRR RGGG GGGB BBBB
					// 1111 1000 0000 0000
					// 0000 0111 1110 0000
					// 0000 0000 0001 1111
					// RRR0 0GGG 000B B000
					// 0000 0000 RRRG GGBB
				}
			}
			GXEndDraw();
		}
		else if (BPP == 4)
		{
			// Assume this is a monochrome iPaq
			unsigned char* dst = (unsigned char*)GXBeginDraw();
			PIXEL* src = getbuffer();
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
	memcpy( dest, getbuffer(), 240 * ysize * 2 );
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
	while (PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE ) == TRUE)
	{
		GetMessage( &msg, 0, 0, 0 );
		TranslateMessage( &msg );
		DispatchMessage( &msg );
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

#if 0
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
#endif
	
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
//	b1 = 0;
	return retval;
}

bool buttonB()
{
	bool retval = (b2 != 0);
//	b2 = 0;
	return retval;
}

bool buttonC()
{
	bool retval = (b3 != 0);
//	b3 = 0;
	return retval;
}

bool buttonD()
{
	bool retval = (b4 != 0);
//	b4 = 0;
	return retval;
}

bool buttonE()
{
	bool retval = (b5 != 0);
//	b5 = 0;
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

void line( int x1, int y1, int x2, int y2, PIXEL color ) 
{ 
	if (y1>y2) 
	{ 
		int h=y1; 
		y1=y2, y2=h; 
		h=x1, x1=x2, x2=h; 
	} 
	int dx, dy, len = abs( x2-x1 ); 
	if (abs( y2 - y1 ) > len) len = abs( y2 - y1 ); 
	len = len >> FPP; 
	if (len != 0) 
	{ 
		dx = (x2 - x1) / len; 
		dy = (y2 - y1) / len; 
	} 
	if (y1 < 0) 
	{ 
		if (dy > 0) 
		{ 
			int i = -(y1 / dy) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x1 < 0) 
	{ 
		if (dx > 0) 
		{ 
			int i = -(x1 / dx) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (y2 > (240 << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (240 << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (320 << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (320 << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	for ( int i = 0; i < len + 1; i++) 
	{ 
		buffer[((y1 >> FPP) * 240) + (x1 >> FPP)] = color; 
		x1 += dx; 
		y1 += dy; 
	} 
} 

void hline( int x1, int y1, int len, PIXEL color )
{
	PIXEL* addr = getbuffer();
	addr += x1 + (y1 * SCRWIDTH);
	for ( int i = 0; i <= len; i++ ) *addr++ = color;
}

void vline( int x1, int y1, int len, PIXEL color )
{
	PIXEL* addr = getbuffer();
	addr += x1 + (y1 * SCRWIDTH);
	for ( int i = 0; i <= len; i++ ) 
	{
		*addr = color;
		addr += SCRWIDTH;
	}
}

void box( int x1, int y1, int x2, int y2, PIXEL color )
{
	hline( x1, y1, x2 - x1, color );
	vline( x1, y1, y2 - y1, color );
	hline( x1, y2, x2 - x1, color );
	vline( x2, y1, y2 - y1, color );
}

void box( int x1, int y1, int x2, int y2, PIXEL color, PIXEL* buff )
{
	PIXEL* temp = getbuffer();
	buffer = buff;
	box( x1, y1, x2, y2, color );
	buffer = temp;
}

void bar( int x1, int y1, int x2, int y2, PIXEL color )
{
	for ( int i = y1; i < y2; i++ ) hline( x1, i, x2 - x1, color );
}

void bar_a( int x1, int y1, int x2, int y2, PIXEL color )
{
	for ( int i = y1; i < y2; i++ ) hline_a( x1, i, x2 - x1, color );
}

void bar( int x1, int y1, int x2, int y2, PIXEL color, PIXEL* buff )
{
	PIXEL* temp = getbuffer();
	buffer = buff;
	for ( int i = y1; i < y2; i++ ) hline( x1, i, x2 - x1, color );
	buffer = temp;
}

void line_x( int x1, int y1, int x2, int y2, PIXEL color ) 
{ 
	if (y1>y2) 
	{ 
		int h=y1; 
		y1=y2, y2=h; 
		h=x1, x1=x2, x2=h; 
	} 
	int dx, dy, len = abs( x2-x1 ); 
	if (abs( y2 - y1 ) > len) len = abs( y2 - y1 ); 
	len = len >> FPP; 
	if (len != 0) 
	{ 
		dx = (x2 - x1) / len; 
		dy = (y2 - y1) / len; 
	} 
	if (y1 < 0) 
	{ 
		if (dy > 0) 
		{ 
			int i = -(y1 / dy) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x1 < 0) 
	{ 
		if (dx > 0) 
		{ 
			int i = -(x1 / dx) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (y2 > (240 << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (240 << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (320 << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (320 << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	for ( int i = 0; i < len + 1; i++) 
	{ 
		buffer[((y1 >> FPP) * 240) + (x1 >> FPP)] ^= color; 
		x1 += dx; 
		y1 += dy; 
	} 
} 

void line_a( int x1, int y1, int x2, int y2, PIXEL color ) 
{ 
	if (y1>y2) 
	{ 
		int h=y1; 
		y1=y2, y2=h; 
		h=x1, x1=x2, x2=h; 
	} 
	int dx, dy, len = abs( x2-x1 ); 
	if (abs( y2 - y1 ) > len) len = abs( y2 - y1 ); 
	len = len >> FPP; 
	if (len != 0) 
	{ 
		dx = (x2 - x1) / len; 
		dy = (y2 - y1) / len; 
	} 
	if (y1 < 0) 
	{ 
		if (dy > 0) 
		{ 
			int i = -(y1 / dy) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x1 < 0) 
	{ 
		if (dx > 0) 
		{ 
			int i = -(x1 / dx) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (y2 > (240 << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (240 << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (320 << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (320 << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	for ( int i = 0; i < len + 1; i++) 
	{ 
		PIXEL* p = buffer + ((y1 >> FPP) * 240) + (x1 >> FPP);
		*p = addblend( *p, color );
		x1 += dx; 
		y1 += dy; 
	} 
} 

void hline_x( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_x( x1 + i, y1, color );
}

void vline_x( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_x( x1, y1 + i, color );
}

void hline_a( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_a( x1 + i, y1, color );
}

void vline_a( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_a( x1, y1 + i, color );
}

void box_x( int x1, int y1, int x2, int y2, PIXEL color )
{
	hline_x( x1, y1, x2 - x1, color );
	vline_x( x1, y1, y2 - y1, color );
	hline_x( x1, y2, x2 - x1, color );
	vline_x( x2, y1, y2 - y1, color );
}

void box_a( int x1, int y1, int x2, int y2, PIXEL color )
{
	hline_a( x1, y1, x2 - x1, color );
	vline_a( x1, y1, y2 - y1, color );
	hline_a( x1, y2, x2 - x1, color );
	vline_a( x2, y1, y2 - y1, color );
}

void bar_x( int x1, int y1, int x2, int y2, PIXEL color )
{
	for ( int i = y1; i < y2; i++ ) hline_x( x1, i, x2 - x1, color );
}

void plot_x( int x1, int y1, PIXEL color )
{
	PIXEL* dest = getbuffer();
	int pixel = x1 + y1 * 240;
	int shft = 6 - ((pixel & 3) << 1);
	*(dest + (pixel >> 2)) = *(dest + (pixel >> 2)) ^ (color << shft);
}

void plot_a( int x1, int y1, PIXEL color )
{
	PIXEL* dest = getbuffer();
	int pixel = x1 + y1 * 240;
	PIXEL c1 = *(dest + pixel);
	PIXEL c2 = color;
	int r = (c1 & (31 << 11)) + (c2 & (31 << 11));
	int g = (c1 & (63 << 5)) + (c2 & (63 << 5));
	int b = (c1 & 31) + (c2 & 31);
	if (r > (31 << 11)) r = 31 << 11;
	if (g > (63 << 5)) g = 63 << 5;
	if (b > 31) b = 31;
	*(dest + pixel) = (PIXEL)(r + g + b);
}

void plot( int x1, int y1, PIXEL color )
{
	PIXEL* dest = getbuffer();
	*(dest + x1 + y1 * 240) = color;
}

PIXEL getpixel( int x1, int y1 )
{
	PIXEL* dest = getbuffer();
	int pixel = x1 + y1 * 240;
	int shft = 6 - ((pixel & 3) << 1);
	return (*(dest + (pixel >> 2)) & (3 << shft)) >> shft;
}

void clear( PIXEL color )
{
	memset( getbuffer(), color, 320 * 480 );
}

PIXEL* getbuffer( void )
{
	return buffer;
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

void setchar( int c, char* c1, char* c2, char* c3, char* c4, char* c5, char* c6, char* c7 )
{
	strcpy( m_chr[c][0], c1 );
	strcpy( m_chr[c][1], c2 );
	strcpy( m_chr[c][2], c3 );
	strcpy( m_chr[c][3], c4 );
	strcpy( m_chr[c][4], c5 );
	strcpy( m_chr[c][5], c6 );
	strcpy( m_chr[c][6], c7 );
}

void initcharset()
{
	for ( int x = 0; x < 57; x++ ) for ( int y = 0; y < 7; y++ ) m_chr[x][y] = new char[10];
   setchar(  0, ":oooo::","o::::o:","o::::o:","oooooo:","o::::o:","o::::o:",":::::::" );
   setchar(  1, "ooooo::","o::::o:","ooooo::","o::::o:","o::::o:","ooooo::",":::::::" );
   setchar(  2, ":oooo::","o::::o:","o::::::","o::::::","o::::o:",":oooo::",":::::::" );
   setchar(  3, "oooo:::","o:::o::","o::::o:","o::::o:","o:::o::","oooo:::",":::::::" );
   setchar(  4, "oooooo:","o::::::","ooooo::","o::::::","o::::::","oooooo:",":::::::" );
   setchar(  5, "oooooo:","o::::::","ooooo::","o::::::","o::::::","o::::::",":::::::" );
   setchar(  6, ":oooo::","o::::o:","o::::::","o::ooo:","o::::o:",":oooo::",":::::::" );
   setchar(  7, "o::::o:","o::::o:","oooooo:","o::::o:","o::::o:","o::::o:",":::::::" );
   setchar(  8, ":ooooo:",":::o:::",":::o:::",":::o:::",":::o:::",":ooooo:",":::::::" );
   setchar(  9, ":::::o:",":::::o:",":::::o:","o::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 10, "o:::o::","o::o:::","ooo::::","o::o:::","o:::o::","o::::o:",":::::::" );
   setchar( 11, "o::::::","o::::::","o::::::","o::::::","o::::::","oooooo:",":::::::" );
   setchar( 12, "o::::o:","oo::oo:","o:oo:o:","o::::o:","o::::o:","o::::o:",":::::::" );
   setchar( 13, "o::::o:","oo:::o:","o:o::o:","o::o:o:","o:::oo:","o::::o:",":::::::" );
   setchar( 14, ":oooo::","o::::o:","o::::o:","o::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 15, "ooooo::","o::::o:","o::::o:","ooooo::","o::::::","o::::::",":::::::" );
   setchar( 16, ":oooo::","o::::o:","o::::o:","o:o::o:","o::o:o:",":oooo::",":::::::" );
   setchar( 17, "ooooo::","o::::o:","o::::o:","ooooo::","o:::o::","o::::o:",":::::::" );
   setchar( 18, ":oooo::","o::::::",":oooo::",":::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 19, "oooooo:","::o::::","::o::::","::o::::","::o::::","::o::::",":::::::" );
   setchar( 20, "o::::o:","o::::o:","o::::o:","o::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 21, "o::::o:","o::::o:","o::::o:","o::::o:",":o::o::","::oo:::",":::::::" );
   setchar( 22, "o::::o:","o::::o:","o::::o:","o::::o:","o:oo:o:",":o::o::",":::::::" );
   setchar( 23, "o::::o:",":o::o::","::oo:::","::oo:::",":o::o::","o::::o:",":::::::" );
   setchar( 24, ":::::o:","o:::o::",":o:o:::","::o::::","::o::::","::o::::",":::::::" );
   setchar( 25, "oooooo:","::::o::",":::o:::","::o::::",":o:::::","oooooo:",":::::::" );
   setchar( 26, ":oooo::","o:::oo:","o::o:o:","o:o::o:","oo:::o:",":oooo::",":::::::" );
   setchar( 27, "::oo:::",":o:o:::",":::o:::",":::o:::",":::o:::",":ooooo:",":::::::" );
   setchar( 28, ":oooo::","o::::o:",":::::o:",":oooo::","o::::::","oooooo:",":::::::" );
   setchar( 29, ":oooo::","o::::o:",":::oo::",":::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 30, ":::o:::","::oo:::",":o:o:::","o::o:::","oooooo:",":::o:::",":::::::" );
   setchar( 31, "oooooo:","o::::::","ooooo::",":::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 32, ":oooo::","o::::::","ooooo::","o::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 33, "oooooo:",":::::o:","::::o::",":::o:::","::o::::","::o::::",":::::::" );
   setchar( 34, ":oooo::","o::::o:",":oooo::","o::::o:","o::::o:",":oooo::",":::::::" );
   setchar( 35, ":oooo::","o::::o:","o::::o:",":ooooo:",":::::o:",":oooo::",":::::::" );
   setchar( 36, ":::::::",":::::::",":::o:::",":::::::",":::::::",":::o:::",":::::::" );
   setchar( 37, ":::oo::",":::oo::",":::oo::",":::oo::",":::::::",":::oo::",":::oo::" );
   setchar( 38, ":oooo::","::::oo:","::::oo:",":::oo::",":::::::","::oo:::","::oo:::" );
   setchar( 39, ":::::::",":::::::",":::::::","::oooo:",":::::::",":::::::",":::::::" );
   setchar( 40, ":::::::",":::::::",":::::::",":ooooo:",":::::::",":ooooo:",":::::::" );
   setchar( 41, ":::::::",":::o:::",":::o:::",":ooooo:",":::o:::",":::o:::",":::::::" );
   setchar( 42, "::::o::",":::o:::",":::o:::",":::o:::",":::o:::","::::o::",":::::::" );
   setchar( 43, ":o:::::","::o::::","::o::::","::o::::","::o::::",":o:::::",":::::::" );
   setchar( 44, ":::::::",":::::::",":::::::",":::::::",":::::::",":::::::",":::::::" );
   setchar( 45, ":::::::",":::::::",":::::::",":::::::",":::::::",":::oo::",":::oo::" );
   setchar( 46, ":::::::",":::::::",":::::::",":::::::","::::o::","::::o::",":::o:::" );
   setchar( 47, ":o:::::","::o::::",":::o:::","::::o::",":::o:::","::o::::",":o:::::" );
   setchar( 48, "::::o::",":::o:::","::o::::",":o:::::","::o::::",":::o:::","::::o::" );
   setchar( 49, "ooooooo","ooooooo","ooooooo","ooooooo","ooooooo","ooooooo","ooooooo" );
   setchar( 50, ":::o:::",":::o:::","::o::::",":::::::",":::::::",":::::::",":::::::" );
   setchar( 51, "o::::o:","::oo::o",":o::::o",":o::::o","::oo::o","o::::o:",":oooo::" );
   setchar( 52, "::::::o",":::::o:","::::o::",":::o:::","::o::::",":o:::::","o::::::" );
   setchar( 53, "o::::::",":o:::::","::o::::",":::o:::","::::o::",":::::o:","::::::o" );
   // Redir table
   char c[] = "abcdefghijklmnopqrstuvwxyz0123456789:!?-=+() .,><#'@/S";	for ( int i = 0; i < 256; i++ ) m_transl[i] = 44;
	for ( i = 0; i < 57; i++ ) m_transl[c[i]] = i;
}

void printn( char* a_String, int x1, int y1, int color, int nr )
{
   int curx = x1;
   for ( int i = 0; i < nr; i++ )
   {
      if( a_String[ i ] == '\0' )
         break;

      long pos = 0;
      if ((a_String[i] == 10) || (a_String[i] == 13)) curx = 400;
      else if (a_String[i] == 8)
      {
         if (curx > 2) curx -= 8;
      }
      else
      {
         if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = m_transl[a_String[i] - ('A' - 'a')];
                                           else pos = m_transl[a_String[i]];
         for ( int h = 0; h < 7; h++ ) for ( int v = 0; v < 7; v++ )
            if (m_chr[pos][v][h] == 'o') plot( curx + h, y1 + v, color );
         curx += 8;
      }
      if (curx > 230)
      {
         curx = 2;
         if (y1 < 300) y1 += 9;
      }
   }
   cursorx = curx;
   cursory = y1;
}

void print( char* a_String, int x1, int y1, PIXEL color )
{
	print( a_String, x1, y1, color, getbuffer(), 240 );
}

void print( char* a_String, PIXEL color )
{
	print( a_String, cursorx, cursory, color );
}

void print( char* a_String, PIXEL color, PIXEL* buff )
{
	PIXEL* temp = buffer;
	buffer = buff;
	print( a_String, cursorx, cursory, color );
	buffer = temp;
}

void print( char* a_String, int x1, int y1, PIXEL color, PIXEL* buff, int a_Pitch )
{
	int curx = x1;
	for ( int i = 0; i < (int)(strlen( a_String )); i++ )
	{	
		long pos = 0;
		if ((a_String[i] == 10) || (a_String[i] == 13)) curx = 400;
		else if (a_String[i] == 8)
		{
			if (curx > 2) curx -= 8;
		}
		else 
		{
			if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = m_transl[a_String[i] - ('A' - 'a')];
														 else pos = m_transl[a_String[i]];
			for ( int h = 0; h < 7; h++ ) for ( int v = 0; v < 7; v++ )
				if (m_chr[pos][v][h] == 'o') buff[curx + h + (y1 + v) * a_Pitch] = color;
			curx += 8;
		}
		if (curx > 230)
		{
			curx = 2;
			if (y1 < 300) y1 += 9;
		}
	}
	cursorx = curx;
	cursory = y1;
}

void centre( char* a_String, int y, PIXEL color )
{
	print( a_String, (240 - strlen( a_String) * 8) / 2, y, color );
}

// =========================================
// =========================================
// MISSING STUFF FROM WINDOWS FOR WINDOWS CE
// =========================================
// =========================================

double strtod( char* t, char** e )
{
	float mul = 1;
	float retval = 0;
	char* p = t;
	while (((*p >= '0') && (*p <= '9')) || (*p == '.'))
	{
		if (*p == '.')
		{
			if (mul < 1) 
			{
				p++;
				break;
			}
			mul = 0.1f;
		}
		else
		{
			if (mul > 0.1)
			{
				retval = retval * 10 + (*p - '0');
			}
			else
			{
				retval += mul * (float)(*p - '0');
				mul *= 0.1f;
			}
		}
		p++;
	}
	e = &p;
	return (double)retval;
}

int stricmp( char* s1, char* s2 )
{
	char* a = s1;
	char* b = s2;
	while ((*a) || (*b))
	{
		char c1 = *a;
		char c2 = *b;
		if ((c1 >= 'A') && (c1 <= 'Z')) c1 -= 'a' - 'A';
		if ((c2 >= 'A') && (c2 <= 'Z')) c2 -= 'a' - 'A';
		if (c1 == c2)
		{
			a++;
			b++;
		}
		else if (c1 < c2) return -1; else return 1; 
	}
	return 0;
}

unsigned int getdeviceid()
{
#ifdef _WIN32_WCE
	// Fetch owner name
	HKEY key;
	unsigned char* buffer = new unsigned char[4096];
	unsigned short* b1 = (unsigned short*)buffer;
	unsigned long size = 4096;
	RegOpenKeyEx( HKEY_CURRENT_USER, L"ControlPanel\\Owner", 0, 0, &key );
	RegQueryValueEx( key, L"Owner", NULL, NULL, buffer, &size );
	RegCloseKey( key );
	int retval = 0;
	for ( int i = 0; i < 48; i++ ) retval += b1[i] + (i << 3);
	delete buffer;
	return (retval & 0xFFFF);
#else
	return 0x7777;
#endif
}

char* fullpath( char* a_File )
{
#ifdef _WIN32_WCE
	USES_CONVERSION;
	GetModuleFileNameW( 0, wfilename, 100 );
	strcpy( filename, W2A( wfilename ) );
#else
	GetModuleFileNameA( 0, filename, 100 );
#endif
	int pos = strlen( filename );
	while (--pos) if (filename[pos] == '\\') break;
	strcpy( filename + pos + 1, a_File );
	return filename;
}

LPWSTR fullpath( LPWSTR a_File )
{
	GetModuleFileNameW( 0, wfilename, 100 );
	int pos = wcslen( wfilename );
	if (pos) while (--pos) if (wfilename[pos] == '\\') break;
	wcscpy( wfilename + pos + 1, a_File );
	return (LPWSTR)wfilename;
}

void fatalerr( char* t )
{
	bar( 5, 100, 235, 150, 0 );
	box( 5, 100, 235, 150, 0xFFFF );
	centre( "fatal error:", 110, 0xFFFF );
	centre( t, 130, 0xFFFF );
	while (clicked()) update();
	while (!clicked()) update();
	shutdown();
}

// Generic tga class - February 2001 -  The Phantom
static int s_IDLen = 0;
static int s_CMapType = 0;
static int s_ImgType = 0;
static int s_CMapOrig = 0;
static int s_CMapLen = 0;
static int s_CMapSize = 0;
static int s_XPos = 0;
static int s_YPos = 0;
static int s_Width = 0;
static int s_Height = 0;
static int s_PixSize = 0;

void tgaloaderror( char* a_Desc )
{
	bar( 5, 100, 235, 150, 0 );
	box( 5, 100, 235, 150, 0xFFFF );
	centre( "tga failed to load:", 110, 0xFFFF );
	centre( a_Desc, 130, 0xFFFF );
	while (clicked()) update();
	while (!clicked()) update();
	shutdown();
}

bool tgaload( char* a_File, int w, int h, IPIXEL* dest, PIXEL* pal )
{
	char* file = fullpath( a_File );
	int size = w * h + 20 + 768;
	BYTE* tgabuff = new BYTE[size];
	FILE* tga = fopen( fullpath( a_File ), "rb" );
	if (!tga) return false;
	fread( tgabuff, 1, size, tga );
	fclose( tga );
	parseheader( tgabuff );
	// Check if the file is valid
	if ((s_Width != w) ||
	    (s_Height != h) ||
		(s_ImgType != 1) ||
		(s_CMapSize != 24)) return false;
	// Store the data at the specified target address
	if (pal)
	{
		for ( int i = 0; i < 256; i++ )
		{
			int b = *(tgabuff + 18 + i * 3);
			int g = *(tgabuff + 18 + i * 3 + 1);
			int r = *(tgabuff + 18 + i * 3 + 2);
			pal[i] = (PIXEL)(((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3));
		}
	}
	IPIXEL* src = tgabuff + 18 + 768 + (h - 1) * w;
	IPIXEL* dst = (IPIXEL*)dest;
	for ( int y = 0; y < h; y++ )
	{
		memcpy( dst, src, w );
		dst += w;
		src -= w;
	}
	// Release temporary storage
	delete tgabuff;
	return true;
}

bool tgaload( char* a_File, int w, int h, PIXEL* dest )
{
	int size = w * 2 * h + 20;
	BYTE* tgabuff = new BYTE[size];
	FILE* tga = fopen( fullpath( a_File ), "rb" );
	if (!tga) return false;
	fread( tgabuff, 1, size, tga );
	fclose( tga );
	parseheader( tgabuff );
	if (s_ImgType == 1)
	{
		// Palettized image
		PIXEL* pal = new PIXEL[256];
		for ( int i = 0; i < 256; i++ )
		{
			int b = *(tgabuff + 18 + i * 3);
			int g = *(tgabuff + 18 + i * 3 + 1);
			int r = *(tgabuff + 18 + i * 3 + 2);
			pal[i] = (PIXEL)(((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3));
		}
		IPIXEL* src = tgabuff + 18 + 768 + (h - 1) * w;
		PIXEL* dst = (PIXEL*)dest;
		for ( int y = 0; y < h; y++ )
		{
			for ( int x = 0; x < w; x++ )
			{
				int idx = *(src + x);
				*(dst + x) = pal[idx];
			}
			dst += w;
			src -= w;
		}
	}
	else
	{
		// Store the data at the specified target address
		PIXEL* src = (PIXEL*)tgabuff + 9 + (h - 1) * w;
		PIXEL* dst = (PIXEL*)dest;
		for ( int i = 0; i < h; i++ )
		{
			for ( int x = 0; x < w; x++ )
			{
				int red = (*(src + x) & (31 << 10)) >> 10;
				int green = (*(src + x) & (31 << 5)) >> 5;
				int blue = *(src + x) & 31;
				*(dst + x) = (red << 11) + (green << 6) + blue;
			}
			dst += w;
			src -= w;
		}
	}
	// Release temporary storage
	delete tgabuff;
	return true;
}

#ifdef USE_ZLIB
bool tgazload( char* file, int w, int h, IPIXEL* dest, PIXEL* pal )
{
	int size = w * h + 20 + 768;
	BYTE* tgabuff = new BYTE[size];
	gzFile tga = gzopen( fullpath( file ), "rb" );
	gzread( tga, tgabuff, size );
	gzclose( tga );
	parseheader( tgabuff );
	// Store the data at the specified target address
	if (pal)
	{
		for ( int i = 0; i < 256; i++ )
		{
			int b = *(tgabuff + 18 + i * 3);
			int g = *(tgabuff + 18 + i * 3 + 1);
			int r = *(tgabuff + 18 + i * 3 + 2);
			pal[i] = (PIXEL)(((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3));
		}
	}
	IPIXEL* src = tgabuff + 18 + 768 + (h - 1) * w;
	IPIXEL* dst = (IPIXEL*)dest;
	for ( int y = 0; y < h; y++ )
	{
		memcpy( dst, src, w );
		dst += w;
		src -= w;
	}
	// Release temporary storage
	delete tgabuff;
	return true;
}

bool tgazload( char* file, int w, int h, PIXEL* dest )
{
	int size = w * 2 * h + 20;
	BYTE* tgabuff = new BYTE[size];
	gzFile tga = gzopen( fullpath( file ), "rb" );
	gzread( tga, tgabuff, size );
	gzclose( tga );
	parseheader( tgabuff );
	if (s_ImgType == 1)
	{
		// Palettized image
		PIXEL* pal = new PIXEL[256];
		for ( int i = 0; i < 256; i++ )
		{
			int b = *(tgabuff + 18 + i * 3);
			int g = *(tgabuff + 18 + i * 3 + 1);
			int r = *(tgabuff + 18 + i * 3 + 2);
			pal[i] = (PIXEL)(((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3));
		}
		IPIXEL* src = tgabuff + 18 + 768 + (h - 1) * w;
		PIXEL* dst = (PIXEL*)dest;
		for ( int y = 0; y < h; y++ )
		{
			for ( int x = 0; x < w; x++ )
			{
				int idx = *(src + x);
				*(dst + x) = pal[idx];
			}
			dst += w;
			src -= w;
		}
		delete pal;
	}
	else
	{
		// Store the data at the specified target address
		PIXEL* src = (PIXEL*)tgabuff + 9 + (h - 1) * w;
		PIXEL* dst = (PIXEL*)dest;
		for ( int i = 0; i < h; i++ )
		{
			for ( int x = 0; x < w; x++ )
			{
				int red = (*(src + x) & (31 << 10)) >> 10;
				int green = (*(src + x) & (31 << 5)) >> 5;
				int blue = *(src + x) & 31;
				*(dst + x) = (red << 11) + (green << 6) + blue;
			}
			dst += w;
			src -= w;
		}
	}
	// Release temporary storage
	delete tgabuff;
	return true;
}
#endif

void parseheader( BYTE* a_Header )
{
	s_IDLen		= *a_Header;
	s_CMapType	= *(a_Header + 1);
	s_ImgType	= *(a_Header + 2);
	s_CMapOrig	= *(a_Header + 3) + 256 * *(a_Header + 4);
	s_CMapLen	= *(a_Header + 5) + 256 * *(a_Header + 6);
	s_CMapSize	= *(a_Header + 7);
	s_XPos		= *(a_Header + 8) + 256 * *(a_Header + 9);
	s_YPos		= *(a_Header + 10) + 256 * *(a_Header + 11);
	s_Width		= *(a_Header + 12) + 256 * *(a_Header + 13);
	s_Height	= *(a_Header + 14) + 256 * *(a_Header + 15);
	s_PixSize	= *(a_Header + 16);
}

// Generic sprite class
PIXEL* ASprite::s_Dest = 0;
PIXEL* ASprite::s_Backdrop = 0;

ASprite::ASprite()
{
	m_Width = m_Height = m_Pitch = 0;
	m_XPos = m_YPos = 50;
	m_Bitmap = 0;
	m_Flags = 0;
	m_Start = 0;
}

ASprite::ASprite( int a_Width, int a_Height, int a_Pitch, PIXEL* a_Bitmap )
{
	m_Width = a_Width;
	m_Height = a_Height;
	m_Pitch = a_Pitch;
	m_Bitmap = a_Bitmap;
	m_XPos = m_YPos = 50;
	m_Start = new int[a_Height];
	for ( int y = 0; y < a_Height; y++ )
	{
		m_Start[y] = a_Width;
		for ( int x = 0; x < a_Width; x++ )
		{
			if (*(a_Bitmap + x + y * a_Pitch) != 0)
			{
				m_Start[y] = x;
				break;
			}
		}
	}
	m_Flags = 0;
}

ASprite::~ASprite()
{
	delete m_Start;
	m_Start = 0;
	m_Bitmap = 0;
}

void ASprite::draw()
{
	if (m_Flags & SPR_FLASH)
	{
		if (randint( 1 ) == 0) return;
	}
	if (m_Flags & SPR_DISABLED) return;
	PIXEL* src = m_Bitmap;
	int x1 = m_XPos, x2 = m_XPos + m_Width;
	int y1 = m_YPos, y2 = m_YPos + m_Height;
	if (x1 < 0)
	{
		if (!(m_Flags & SPR_MIRROR)) src -= x1;
		x1 = 0;
	}
	if (x2 > (SCRWIDTH - 1)) 
	{
		if (m_Flags & SPR_MIRROR) src += x2 - (SCRWIDTH - 1);
		x2 = SCRWIDTH - 1;
	}
	if (y1 < 0) 
	{
		src -= (y1 * m_Pitch);
		y1 = 0;
	}
	if (y2 > (SCRHEIGHT - 1)) y2 = SCRHEIGHT - 1;
	int xs;
	if ((x2 > x1) && (y2 > y1))
	{
		int addr = y1 * SCRWIDTH + x1;
		int width = x2 - x1;
		int height = y2 - y1;
		for ( int y = 0; y < height; y++ )
		{
			int line = y + (y1 - m_YPos);
			int lsx = m_Start[line] + m_XPos;
			if (m_Flags & SPR_MIRROR)
			{
				if (m_Flags & SPR_FLASHLIGHT)
				{
					for ( int x = 0; x < width; x++ )
					{
						PIXEL c1 = *(src + x);
						if (c1 != 0) 
						{
							PIXEL c2 = *(s_Backdrop + addr + width - (x + 1));
							*(s_Dest + addr + width - (x + 1)) = addblend( c1, c2 );
						}
					}
				}
				else if (m_Flags & SPR_FLARE)
				{
					for ( int x = 0; x < width; x++ )
					{
						PIXEL c1 = *(src + x);
						if (c1 != 0) 
						{
							PIXEL c2 = *(s_Dest + addr + width - (x + 1));
							*(s_Dest + addr + width - (x + 1)) = addblend( c1, c2 );
						}
					}
				}
				else
				{
					xs = (lsx > x1)?lsx - x1:0;
					if (m_Flags & SPR_WHITE)
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(s_Dest + addr + width - (x + 1)) = 255;
						}
					}
					else if (m_Flags & SPR_IREFFECT)
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(s_Dest + addr + width - (x + 1)) = c1 & (PIXEL)GREENMASK;
						}
					}
					else
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(s_Dest + addr + width - (x + 1)) = c1;
						}
					}
				}
			}
			else
			{
				if (m_Flags & SPR_FLASHLIGHT)
				{
					xs = (lsx > x1)?lsx - x1:0;
					for ( int x = xs; x < width; x++ )
					{
						PIXEL c1 = *(src + x);
						if (c1 != 0) 
						{
							PIXEL c2 = *(s_Backdrop + addr + x);
							*(s_Dest + addr + x) = addblend( c1, c2 );
						}
					}
				}
				else if (m_Flags & SPR_FLARE)
				{
					xs = (lsx > x1)?lsx - x1:0;
					for ( int x = xs; x < width; x++ )
					{
						PIXEL c1 = *(src + x);
						if (c1 != 0) 
						{
							PIXEL c2 = *(s_Dest + addr + x);
							*(s_Dest + addr + x) = addblend( c1, c2 );
						}
					}
				}
				else
				{
					xs = (lsx > x1)?lsx - x1:0;
					if (m_Flags & SPR_WHITE)
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(s_Dest + addr + x) = 0xFFFF;
						}
					}
					else if (m_Flags & SPR_IREFFECT)
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(s_Dest + addr + x) = c1 & (PIXEL)GREENMASK;
						}
					}
					else
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(s_Dest + addr + x) = c1;
						}
					}
				}
			}
			addr += SCRWIDTH;
			src += m_Pitch;
		}
	}
}

void ASprite::drawscaled( int a_Width, int a_Height )
{
	PIXEL* src = m_Bitmap;
	int u1 = 0, u2 = (m_Width - 1) << 10;
	int v1 = 0, v2 = (m_Height - 1) << 10;
	int du = u2 / a_Width;
	int dv = v2 / a_Height;
	int addr = m_YPos * 240 + m_XPos;
	for ( int y = 0; y < a_Height; y++ )
	{
		int cv = (v1 >> 10) * 120;
		int cu = u1;
		for ( int x = 0; x < a_Width; x++ )
		{
			PIXEL c1 = *(src + (cu >> 10) + cv );
			{
				PIXEL* d = s_Dest + addr + x;
				*d = addblend( *d, c1 );
			}
			cu += du;
		}
		addr += 240;
		v1 += dv;
	}
}

AAnim::AAnim()
{
	m_Frames = 0;
	m_Frame = 0;
	m_Sprite = 0;
	m_Flags = 0;
	m_XPos = 50;
	m_YPos = 50;
}

AAnim::AAnim( int a_Frames )
{
	m_Frames = 0;
	m_Frame = 0;
	m_Flags = 0;
	m_XPos = m_YPos = 50;
	m_Sprite = new ASprite*[a_Frames];
	for ( int i = 0; i < a_Frames; i++ ) m_Sprite[i] = 0;
}

AAnim::~AAnim()
{
	for ( int i = 0; i < m_Frames; i++ ) 
	{
		delete m_Sprite[i];
		m_Sprite[i] = 0;
	}
	delete m_Sprite;
	m_Sprite = 0;
}

void AAnim::draw()
{ 
	if (m_Sprite[m_Frame])
	{
		m_Sprite[m_Frame]->setpos( m_XPos, m_YPos );
		m_Sprite[m_Frame]->setflags( m_Flags ); 
		m_Sprite[m_Frame]->draw();
	}
}

void AAnim::drawscaled( int a_Width, int a_Height )
{ 
	if (m_Sprite[m_Frame])
	{
		m_Sprite[m_Frame]->setpos( m_XPos, m_YPos );
		m_Sprite[m_Frame]->setflags( m_Flags ); 
		m_Sprite[m_Frame]->drawscaled( a_Width, a_Height ); 
	}
}

AFont::AFont()
{
	m_FData = 0;
	m_Width = 0;
	m_Height = 0;
	m_Pitch = 0;
	m_DPitch = 0;
	m_Chars = 0;
	m_Transl = new int[256];
}

AFont::~AFont()
{
	delete m_Transl;
	delete m_Chars;
}

void AFont::setdata( PIXEL* a_Data, int a_Width, int a_Height, int a_Pitch )
{
	m_FData = a_Data;
	m_Width = a_Width;
	m_Height = a_Height;
	m_Pitch = a_Pitch;
}

void AFont::setchars( char* a_Chars )
{
	delete m_Chars;
	m_Chars = new char[strlen( a_Chars ) + 1];
	strcpy( m_Chars, a_Chars );
	memset( m_Transl, 0, 256 * sizeof( int ) );
	for ( unsigned int i = 0; i < strlen( a_Chars ); i++ )
		m_Transl[(unsigned char)a_Chars[i]] = i;
}

void AFont::centre( char* a_Text, int a_Y )
{
	print( a_Text, (m_DPitch - m_Width * strlen( a_Text )) / 2, a_Y );
}

void AFont::print( char* a_Text, int a_X, int a_Y )
{
	int cx = a_X;
	int cy = a_Y;
	PIXEL* b = m_FData;
	for ( unsigned int i = 0; i < strlen( a_Text ); i++ )
	{
		if (a_Text[i] != ' ')
		{
			int c = m_Transl[a_Text[i]];
			for ( int y = 0; y < m_Height; y++ )
			{
				for ( int x = 2; x < m_Width; x++ )
				{
					PIXEL f = *(m_FData + c * m_Width + x + y * m_Pitch);
					if (f)
					{
						PIXEL* bc = b + cx + x + (cy + y) * m_DPitch;
						*bc = addblend( *bc, f );
					}
				}
			}
		}
		cx += m_Width;
	}
}

int* APBomb::s_VX = 0;
int* APBomb::s_VY = 0;

AActorPool::AActorPool()
{
	m_Poolsize = 50;
	m_Actor = new AActor*[m_Poolsize];
	for ( int i = 0; i < m_Poolsize; i++ ) m_Actor[i] = 0;
}

AActorPool::~AActorPool()
{
	delete m_Actor;
}

void AActorPool::update()
{
	for ( int i = 0; i < m_Poolsize; i++ ) if (m_Actor[i])
	{
		if (!m_Actor[i]->update())
		{
			delete m_Actor[i];
			m_Actor[i] = 0;
		}
	}
}

void AActorPool::addactor( AActor* a_Actor )
{
	int pos = 0;
	while ((pos < m_Poolsize) && (m_Actor[pos])) pos++;
	if (pos != m_Poolsize) m_Actor[pos] = a_Actor; else
	{
		m_Poolsize += 20;
		AActor** newpool = new AActor*[m_Poolsize];
		for ( int i = m_Poolsize - 20; i < m_Poolsize; i++ ) newpool[i] = 0;
		memcpy( newpool, m_Actor, (m_Poolsize - 20) * 4 );
		delete m_Actor;
		m_Actor = newpool;
		addactor( a_Actor );
	}
}

void AActorPool::removeactor( AActor* a_Actor )
{
	for ( int i = 0; i < m_Poolsize; i++ )
	{
		if (m_Actor[i] == a_Actor)
		{
			m_Actor[i] = 0;
			break;
		}
	}
}

void AActorPool::removeall()
{
	for ( int i = 0; i < m_Poolsize; i++ )
	{
		delete m_Actor[i];
		m_Actor[i] = 0;
	}
}

APBomb::APBomb()
{
	// Don't use this one
}

APBomb::APBomb( int a_X, int a_Y, int a_Nr )
{
	if (!s_VX)
	{
		s_VX = new int[200];
		s_VY = new int[200];
		for ( int i = 0; i < 200; i++ )
		{
			float vx = ((float)rand() / RAND_MAX) - 0.5f;
			float vy = ((float)rand() / RAND_MAX) - 0.6f;
			vx += (vx < 0)?-0.2f:0.2f;
			vy += (vy < 0)?-0.2f:0.2f;
			float len = (float)sqrt( vx * vx + vy * vy );
			vx /= len;
			vy /= len;
			float mul = ((float)rand() / RAND_MAX) * 1.0f;
			vx *= mul;
			vy *= mul;
			s_VX[i] = (int)(vx * 4096.0f);
			s_VY[i] = (int)(vy * 4096.0f);
		}
	}
	m_X = new int[a_Nr];
	m_Y = new int[a_Nr];
	m_VX = new int[a_Nr];
	m_VY = new int[a_Nr];
	m_Life = new unsigned char[a_Nr];
	m_Active = new bool[a_Nr];
	m_Particles = m_AParticles = a_Nr;
	memcpy( m_VX, s_VX, 4 * a_Nr );
	memcpy( m_VY, s_VY, 4 * a_Nr );
	for ( int i = 0; i < a_Nr; i++ )
	{
		m_Life[i] = (i >> 1) + 20;
		m_X[i] = a_X << 10;
		m_Y[i] = a_Y << 10;
		m_Active[i] = true;
	}
	m_Color = ((15 << 11) + (31 << 5) + 15);
}

APBomb::~APBomb()
{
	delete m_X;
	delete m_Y;
	delete m_VX;
	delete m_VY;
	delete m_Active;
	m_X = m_Y = m_VX = m_VY = 0;
	m_Active = 0;
}

bool APBomb::update()
{
	PIXEL* b = getbuffer();
	for ( int i = 0; i < m_Particles; i++ ) if (m_Active[i])
	{
		m_X[i] += m_VX[i];
		m_Y[i] += m_VY[i];
		int x = m_X[i] >> 10;
		int y = m_Y[i] >> 10;
		if ((!--m_Life[i]) || (x < 0) || (y < 0) || (x > 511) || (y > 319)) 
		{
			m_Active[i] = false;
			m_AParticles--;
		}
		else
		{
			if ((x > 0) && (x < 240)) 
			{
				PIXEL* a = b + x + y * 240;
				*a = addblend( *a, m_Color );
			}
		}
	}
	return (m_AParticles > 0);
}

ASound::ASound()
{
	m_Buffer = 0;
}

ASound::ASound( char* a_Snd )
{
	m_Buffer = 0;
	load( a_Snd );
}

ASound::~ASound()
{
	delete m_Buffer;
	m_Buffer = 0;
}

void ASound::load( char* a_Snd )
{
	int fsize = filesize( a_Snd );
	delete m_Buffer;
	if (fsize)
	{
		FILE* wav = fopen( fullpath( a_Snd ), "rb" );
		m_Buffer = new unsigned char[fsize];
		fread( m_Buffer, 1, fsize, wav );
		fclose( wav );
	}
	else
	{
		m_Buffer = 0;
	}
}

void ASound::play()
{
	if (m_Buffer) sndPlaySound( (ACHAR*)m_Buffer, SND_ASYNC + SND_MEMORY );
}

// --------------------------------------------------
// EOF