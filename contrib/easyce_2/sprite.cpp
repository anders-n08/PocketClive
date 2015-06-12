#include "easyce.h"

// Generic sprite class
ASurface* ASprite::s_Dest = 0;
PIXEL* ASprite::s_Backdrop = 0;

ASprite::ASprite()
{
	m_Width = m_Height = m_Pitch = 0;
	m_XPos = m_YPos = 50;
	m_Bitmap = 0;
	m_Flags = 0;
	m_Start = 0;
}

ASprite::ASprite( int a_Width, int a_Height, ASurface* a_Surf )
{
	m_Width = a_Width;
	m_Height = a_Height;
	m_Pitch = a_Surf->getwidth();
	m_Bitmap = a_Surf->getbuffer();
	m_XPos = m_YPos = 50;
	m_Start = new int[a_Height];
	for ( int y = 0; y < a_Height; y++ )
	{
		m_Start[y] = a_Width;
		for ( int x = 0; x < a_Width; x++ )
		{
			if (*(m_Bitmap + x + y * m_Pitch) != 0)
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
	PIXEL* dest = s_Dest->getbuffer();
	int dpitch = s_Dest->getwidth();
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
							*(dest + addr + width - (x + 1)) = addblend( c1, c2 );
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
							PIXEL c2 = *(dest + addr + width - (x + 1));
							*(dest + addr + width - (x + 1)) = addblend( c1, c2 );
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
							if (c1 != 0) *(dest + addr + width - (x + 1)) = 255;
						}
					}
					else if (m_Flags & SPR_IREFFECT)
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(dest + addr + width - (x + 1)) = c1 & (PIXEL)GREENMASK;
						}
					}
					else
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(dest + addr + width - (x + 1)) = c1;
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
							*(dest + addr + x) = addblend( c1, c2 );
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
							PIXEL c2 = *(dest + addr + x);
							*(dest + addr + x) = addblend( c1, c2 );
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
							if (c1 != 0) *(dest + addr + x) = 0xFFFF;
						}
					}
					else if (m_Flags & SPR_IREFFECT)
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(dest + addr + x) = c1 & (PIXEL)GREENMASK;
						}
					}
					else
					{
						for ( int x = xs; x < width; x++ )
						{
							PIXEL c1 = *(src + x);
							if (c1 != 0) *(dest + addr + x) = c1;
						}
					}
				}
			}
			addr += dpitch;
			src += m_Pitch;
		}
	}
}

void ASprite::drawscaled( int a_Width, int a_Height )
{
	// Experimental
	PIXEL* src = m_Bitmap;
	PIXEL* dst = s_Dest->getbuffer();
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
				PIXEL* d = dst + addr + x;
				*d = addblend( *d, c1 );
			}
			cu += du;
		}
		addr += s_Dest->getwidth();
		v1 += dv;
	}
}

