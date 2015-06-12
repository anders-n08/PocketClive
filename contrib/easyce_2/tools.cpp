#include "easyce.h"

// Static vars for fullpath functions
static char filename[100];
static unsigned short wfilename[100];

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

