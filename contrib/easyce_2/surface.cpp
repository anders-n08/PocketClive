#include "easyce.h"
#include "zlib.h"

char* ASurface::m_chr[57][7];
int ASurface::m_transl[256];

// Surface class
ASurface::ASurface( int a_Width, int a_Height )
{
	m_Buffer = new PIXEL[a_Width * a_Height];
	m_Width = a_Width;
	m_Height = a_Height;
	m_Flags = SURF_OWN|SURF_HICOLOR;
}

ASurface::ASurface( int a_Width, int a_Height, PIXEL* a_Buffer )
{
	m_Buffer = a_Buffer;
	m_Width = a_Width;
	m_Height = a_Height;
	m_Flags = SURF_HICOLOR;
}

ASurface::ASurface( char* a_File, bool a_Zipped )
{
	BYTE* tgabuff = new BYTE[20];
	bool OK = true;
	if (!a_Zipped)
	{
		FILE* tga = fopen( fullpath( a_File ), "rb" );
		if (!tga) OK = false; else
		{
			fread( tgabuff, 1, 20, tga );
			fclose( tga );
		}
	}
	else
	{
		gzFile tga = gzopen( fullpath( a_File ), "rb" );
		if (!tga) OK = false; else
		{
			gzread( tga, tgabuff, 20 );
			gzclose( tga );
		}
	}
	if (OK)
	{
		parseheader( tgabuff );
		m_Width = m_TgaWidth;
		m_Height = m_TgaHeight;
		m_Buffer = new PIXEL[m_Width * m_Height];
		m_Flags = SURF_OWN|SURF_HICOLOR;
		tgaload( a_File );
	}
	else
	{
		m_Width = 0;
		m_Height = 0;
		m_Buffer = 0;
		m_Flags = 0;
	}	
	delete tgabuff;
}

ASurface::~ASurface()
{
	if (m_Flags & SURF_OWN) delete m_Buffer;
	m_Buffer = 0;
}

void ASurface::copyfrom( ASurface* a_Src )
{
	int width = m_Width;
	int height = m_Height;
	if (a_Src->getwidth() < width) width = a_Src->getwidth();
	if (a_Src->getheight() < height) height = a_Src->getheight();
	PIXEL* src = a_Src->getbuffer();
	PIXEL* dst = m_Buffer;
	if (a_Src->getwidth() == m_Width)
	{
		memcpy( dst, src, width * height * 2 );
	}
	else
	{
		int dpitch = a_Src->getwidth();
		int spitch = width;
		for (int y = 0; y < height; y++ )
		{
			memcpy( dst, src, width * 2 );
			dst += dpitch;
			src += spitch;
		}
	}
}

void ASurface::line( int x1, int y1, int x2, int y2, PIXEL color ) 
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
	if (len > 0) if (y2 > (m_Width << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (m_Width << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (m_Height << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (m_Height << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	for ( int i = 0; i < len + 1; i++) 
	{ 
		m_Buffer[((y1 >> FPP) * m_Width) + (x1 >> FPP)] = color; 
		x1 += dx; 
		y1 += dy; 
	} 
} 

void ASurface::hline( int x1, int y1, int len, PIXEL color )
{
	PIXEL* addr = getbuffer();
	addr += x1 + (y1 * SCRWIDTH);
	for ( int i = 0; i <= len; i++ ) *addr++ = color;
}

void ASurface::vline( int x1, int y1, int len, PIXEL color )
{
	PIXEL* addr = getbuffer();
	addr += x1 + (y1 * SCRWIDTH);
	for ( int i = 0; i <= len; i++ ) 
	{
		*addr = color;
		addr += SCRWIDTH;
	}
}

void ASurface::box( int x1, int y1, int x2, int y2, PIXEL color )
{
	hline( x1, y1, x2 - x1, color );
	vline( x1, y1, y2 - y1, color );
	hline( x1, y2, x2 - x1, color );
	vline( x2, y1, y2 - y1, color );
}

void ASurface::bar( int x1, int y1, int x2, int y2, PIXEL color )
{
	for ( int i = y1; i < y2; i++ ) hline( x1, i, x2 - x1, color );
}

void ASurface::bar_a( int x1, int y1, int x2, int y2, PIXEL color )
{
	for ( int i = y1; i < y2; i++ ) hline_a( x1, i, x2 - x1, color );
}

void ASurface::line_x( int x1, int y1, int x2, int y2, PIXEL color ) 
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
	if (len > 0) if (y2 > (m_Width << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (m_Width << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (m_Height << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (m_Height << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	for ( int i = 0; i < len + 1; i++) 
	{ 
		m_Buffer[((y1 >> FPP) * m_Width) + (x1 >> FPP)] ^= color; 
		x1 += dx; 
		y1 += dy; 
	} 
} 

void ASurface::line_a( int x1, int y1, int x2, int y2, PIXEL color ) 
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
	if (len > 0) if (y2 > (m_Width << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (m_Width << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (m_Height << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (m_Height << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	for ( int i = 0; i < len + 1; i++) 
	{ 
		PIXEL* p = m_Buffer + ((y1 >> FPP) * m_Width) + (x1 >> FPP);
		*p = addblend( *p, color );
		x1 += dx; 
		y1 += dy; 
	} 
} 

void ASurface::hline_x( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_x( x1 + i, y1, color );
}

void ASurface::vline_x( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_x( x1, y1 + i, color );
}

void ASurface::hline_a( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_a( x1 + i, y1, color );
}

void ASurface::vline_a( int x1, int y1, int len, PIXEL color )
{
	for ( int i = 0; i <= len; i++ ) plot_a( x1, y1 + i, color );
}

void ASurface::box_x( int x1, int y1, int x2, int y2, PIXEL color )
{
	hline_x( x1, y1, x2 - x1, color );
	vline_x( x1, y1, y2 - y1, color );
	hline_x( x1, y2, x2 - x1, color );
	vline_x( x2, y1, y2 - y1, color );
}

void ASurface::box_a( int x1, int y1, int x2, int y2, PIXEL color )
{
	hline_a( x1, y1, x2 - x1, color );
	vline_a( x1, y1, y2 - y1, color );
	hline_a( x1, y2, x2 - x1, color );
	vline_a( x2, y1, y2 - y1, color );
}

void ASurface::bar_x( int x1, int y1, int x2, int y2, PIXEL color )
{
	for ( int i = y1; i < y2; i++ ) hline_x( x1, i, x2 - x1, color );
}

void ASurface::plot_x( int x1, int y1, PIXEL color )
{
	PIXEL* dest = getbuffer();
	int pixel = x1 + y1 * m_Width;
	int shft = 6 - ((pixel & 3) << 1);
	*(dest + (pixel >> 2)) = *(dest + (pixel >> 2)) ^ (color << shft);
}

void ASurface::plot_a( int x1, int y1, PIXEL color )
{
	PIXEL* dest = getbuffer();
	int pixel = x1 + y1 * m_Width;
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

void ASurface::plot( int x1, int y1, PIXEL color )
{
	PIXEL* dest = getbuffer();
	*(dest + x1 + y1 * m_Width) = color;
}

PIXEL ASurface::getpixel( int x1, int y1 )
{
	PIXEL* dest = getbuffer();
	int pixel = x1 + y1 * m_Width;
	int shft = 6 - ((pixel & 3) << 1);
	return (*(dest + (pixel >> 2)) & (3 << shft)) >> shft;
}

void ASurface::clear( PIXEL color )
{
	memset( getbuffer(), color, m_Height * m_Height * 2 );
}

void ASurface::print( char* a_String, int x1, int y1, PIXEL color )
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
				if (m_chr[pos][v][h] == 'o') m_Buffer[curx + h + (y1 + v) * m_Width] = color;
			curx += 8;
		}
		if (curx > 230)
		{
			curx = 2;
			if (y1 < 300) y1 += 9;
		}
	}
}

void ASurface::centre( char* a_String, int y, PIXEL color )
{
	print( a_String, (m_Width - strlen( a_String) * 8) / 2, y, color );
}

void ASurface::setchar( int c, char* c1, char* c2, char* c3, char* c4, char* c5, char* c6, char* c7 )
{
	strcpy( m_chr[c][0], c1 );
	strcpy( m_chr[c][1], c2 );
	strcpy( m_chr[c][2], c3 );
	strcpy( m_chr[c][3], c4 );
	strcpy( m_chr[c][4], c5 );
	strcpy( m_chr[c][5], c6 );
	strcpy( m_chr[c][6], c7 );
}

void ASurface::initcharset()
{
	for ( int x = 0; x < 57; x++ ) for ( int y = 0; y < 7; y++ ) m_chr[x][y] = new char[10];
	setchar(  0, "oooo:::","oo:oo::","oo::oo:","oo:::oo","ooooooo","oo:::oo","oo:::oo" );
	setchar(  1, "oooo:::","oo:oo::",":oo::::","ooooo::","oo::oo:","oo:::oo","oooooo:" );
	setchar(  2, "::ooooo",":oo::::","oo:::::","oo:::::","oo:::::","oo:::::",":oooooo" );
	setchar(  3, "oooo:::","oo:oo::","oo::oo:","oo:::oo","oo:::oo","oo:::oo","oooooo:" );
	setchar(  4, "ooooooo","oo:::::","oo:::::","ooooooo","oo:::::","oo:::::","ooooooo" );
	setchar(  5, "ooooooo","oo:::::","oo:::::","ooooo::","oo:::::","oo:::::","oo:::::" );
	setchar(  6, "::oooo:",":oo::::","oo:::::","oo:oooo","oo:::oo","oo:::oo",":oooooo" );
	setchar(  7, "oo::::o","oo::::o","oo::::o","ooooooo","oo::::o","oo::::o","oo::::o" );
	setchar(  8, "::oooo:",":::oo::",":::oo::",":::oo::",":::oo::",":::oo::","::oooo:" );
	setchar(  9, "::ooooo",":::::oo",":::::oo",":::::oo","oo:::oo",":oo:oo:","::ooo::" );
	setchar( 10, "oo::oo:","oo:oo::","oooo:::","ooo::::","ooooo::","oo::oo:","oo:::oo" );
	setchar( 11, "oo:::::","oo:::::","oo:::::","oo:::::","oo:::::","oo:::::","ooooooo" );
	setchar( 12, "o:::::o","oo:::oo","ooo:ooo","ooooooo","oo:o:oo","oo:::oo","oo:::oo" );
	setchar( 13, "oo:::oo","ooo::oo","oooo:oo","oo:oooo","oo::ooo","oo:::oo","oo:::oo" );
	setchar( 14, "::ooo::",":oo:oo:","oo:::oo","oo:::oo","oo:::oo","oo:::oo",":ooooo:" );
	setchar( 15, "ooooo::","oo::oo:","oo:::oo","oo:::oo","oooooo:","oo:::::","oo:::::" );
	setchar( 16, "::ooo::",":oo:oo:","oo:::oo","oo:o:oo","oo:oooo","oo::oo:",":ooo:oo" );
	setchar( 17, "ooooo::","oo::oo:","oo:::oo","oooooo:","oo:oo::","oo::oo:","oo:::oo" );
	setchar( 18, "::ooooo",":oo::::","oo:::::","ooooooo",":::::oo","::::oo:","ooooo::" );
	setchar( 19, "ooooooo",":::oo::",":::oo::",":::oo::",":::oo::",":::oo::",":::oo::" );
	setchar( 20, "oo:::oo","oo:::oo","oo:::oo","oo:::oo","oo:::oo","oo::oo:",":oooo::" );
	setchar( 21, "oo:::oo","oo:::oo",":oo:oo:",":oo:oo:","::ooo::","::ooo::",":::o:::" );
	setchar( 22, "oo:::oo","oo:::oo","oo:::oo","oo:::oo","oo:o:oo",":ooooo:","::o:o::" );
	setchar( 23, "oo::::o",":oo::oo","::oooo:",":::oo::","::oooo:",":oo::oo","oo::::o" );
	setchar( 24, "oo::::o",":oo::oo","::oooo:",":::oo::",":::oo::",":::oo::",":::oo::" );
	setchar( 25, "ooooooo","::::oo:",":::oo::","::oo:::",":oo::::","oo:::::","ooooooo" );
	setchar( 26, ":ooooo:","oo:::oo","oo:::oo",":::::::","oo:::oo","oo:::oo",":ooooo:" );
	setchar( 27, ":::::::",":::::oo",":::::oo",":::::::",":::::oo",":::::oo",":::::::" );
	setchar( 28, ":ooooo:",":::::oo",":::::oo",":ooooo:","oo:::::","oo:::::",":ooooo:" );
	setchar( 29, ":ooooo:",":::::oo",":::::oo",":ooooo:",":::::oo",":::::oo",":ooooo:" );
	setchar( 30, ":::::::","oo:::oo","oo:::oo",":ooooo:",":::::oo",":::::oo",":::::::" );
	setchar( 31, ":ooooo:","oo:::::","oo:::::",":ooooo:",":::::oo",":::::oo",":ooooo:" );
	setchar( 32, ":ooooo:","oo:::::","oo:::::",":ooooo:","oo:::oo","oo:::oo",":ooooo:" );
	setchar( 33, ":ooooo:",":::::oo",":::::oo",":::::::",":::::oo",":::::oo",":::::::" );
	setchar( 34, ":ooooo:","oo:::oo","oo:::oo",":ooooo:","oo:::oo","oo:::oo",":ooooo:" );
	setchar( 35, ":ooooo:","oo:::oo","oo:::oo",":ooooo:",":::::oo",":::::oo",":ooooo:" );
	setchar( 36, ":::::::",":::::::",":::o:::",":::::::",":::::::",":::o:::",":::::::" );
	setchar( 37, ":::oo::",":::oo::",":::oo::",":::oo::",":::::::",":::oo::",":::oo::" );
	setchar( 38, ":oooo::","::::oo:","::::oo:",":::oo::",":::::::","::oo:::","::oo:::" );
	setchar( 39, ":::::::",":::::::",":::::::","::oooo:",":::::::",":::::::",":::::::" );
	setchar( 40, ":::::::",":::::::",":::::::",":ooooo:",":::::::",":ooooo:",":::::::" );
	setchar( 41, ":::::::",":::o:::",":::o:::",":::o:::",":::o:::",":::o:::",":::::::" );
	setchar( 42, "::ooo::","::o::::","::o::::","::o::::","::o::::","::o::::","::ooo::" );
	setchar( 43, "::ooo::","::::o::","::::o::","::::o::","::::o::","::::o::","::ooo::" );
	setchar( 44, ":::::::",":::::::",":::::::",":::::::",":::::::",":::::::",":::::::" );
	setchar( 45, ":::::::",":::::::",":::::::",":::::::",":::::::",":::oo::",":::oo::" );
	setchar( 46, ":::::::",":::::::",":::::::",":::::::","::::o::","::::o::",":::o:::" );
	setchar( 47, ":o:::::","::o::::",":::o:::","::::o::",":::o:::","::o::::",":o:::::" );
	setchar( 48, "::::o::",":::o:::","::o::::",":o:::::","::o::::",":::o:::","::::o::" );
	setchar( 49, "ooooooo","ooooooo","ooooooo","ooooooo","ooooooo","ooooooo","ooooooo" );
	setchar( 50, ":::o:::",":::o:::","::o::::",":::::::",":::::::",":::::::",":::::::" );
	setchar( 51, "::ooo::",":o:::o:","o::oo:o","o:o:::o","o::oooo",":o:::::","::oooo:" );
	setchar( 52, "::::::o",":::::o:","::::o::",":::o:::","::o::::",":o:::::","o::::::" );
	setchar( 53, "o::::::",":o:::::","::o::::",":::o:::","::::o::",":::::o:","::::::o" );
	setchar( 54, "oo:::oo",":::::::","oooo:::","oo::oo:","ooooooo","oo:::oo","oo:::oo" );
	setchar( 55, "oo:::oo",":::::::","oo:::oo","oo:::oo","oo:::oo","oo::oo:",":oooo::" );
	setchar( 56, "oo:::oo",":::::::","::ooo::",":oo:oo:","oo:::oo","oo:::oo",":ooooo:" );
	// Redir table
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789:!?-=+[] .,><#'@/Säüö";
	for ( int i = 0; i < 256; i++ ) m_transl[i] = 44;
	for ( i = 0; i < 57; i++ ) m_transl[c[i]] = i;
}

bool ASurface::tgaload( char* a_File, int w, int h, IPIXEL* dest, PIXEL* pal )
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
	if ((m_TgaWidth != w) ||
	    (m_TgaHeight != h) ||
		(m_TgaImgType != 1) ||
		(m_TgaCMapSize != 24)) return false;
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

bool ASurface::tgaload( char* a_File, int w, int h, PIXEL* dest )
{
	int size = w * 2 * h + 20;
	BYTE* tgabuff = new BYTE[size];
	FILE* tga = fopen( fullpath( a_File ), "rb" );
	if (!tga) return false;
	fread( tgabuff, 1, size, tga );
	fclose( tga );
	parseheader( tgabuff );
	if (m_TgaImgType == 1)
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
bool ASurface::tgazload( char* file, int w, int h, IPIXEL* dest, PIXEL* pal )
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

bool ASurface::tgazload( char* file, int w, int h, PIXEL* dest )
{
	int size = w * 2 * h + 20;
	BYTE* tgabuff = new BYTE[size];
	gzFile tga = gzopen( fullpath( file ), "rb" );
	gzread( tga, tgabuff, size );
	gzclose( tga );
	parseheader( tgabuff );
	if (m_TgaImgType == 1)
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

void ASurface::parseheader( BYTE* a_Header )
{
	m_TgaIDLen		= *a_Header;
	m_TgaCMapType	= *(a_Header + 1);
	m_TgaImgType	= *(a_Header + 2);
	m_TgaCMapOrig	= *(a_Header + 3) + 256 * *(a_Header + 4);
	m_TgaCMapLen	= *(a_Header + 5) + 256 * *(a_Header + 6);
	m_TgaCMapSize	= *(a_Header + 7);
	m_TgaXPos		= *(a_Header + 8) + 256 * *(a_Header + 9);
	m_TgaYPos		= *(a_Header + 10) + 256 * *(a_Header + 11);
	m_TgaWidth		= *(a_Header + 12) + 256 * *(a_Header + 13);
	m_TgaHeight		= *(a_Header + 14) + 256 * *(a_Header + 15);
	m_TgaPixSize	= *(a_Header + 16);
}