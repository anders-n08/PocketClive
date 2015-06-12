#include "easyce.h"

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
