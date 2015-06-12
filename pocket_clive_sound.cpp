/*
    (C) 2001 Anders Holmberg

    This file is part of PocketClive.

    PocketClive is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PocketClive is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PocketClive; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Author contact information : aagholmberg@yahoo.se
 
*/

#include <winbase.h>
#include <mmsystem.h>

#include "contrib\fuse\config.h"

extern "C"
{
#include "contrib\fuse\display.h"
#include "contrib\fuse\tape.h"
}

#include "contrib\easyce\EasyCE.h"

#include "pocket_clive.h"
#include "pocket_clive_types.h"
#include "pocket_clive_sound.h"

#define NR_SOUND_BUFFERS 10
#define SOUND_BUFFER_SIZE 882
#define AMPL_BEEPER 31

static HWAVEOUT hwo;
static WAVEHDR whdr[ NR_SOUND_BUFFERS ];
static MMRESULT mmres;
static UINT devId;
static DWORD dwOldVolume;
static int wave_index = 0;
static WAVEFORMATEX Format;

char *sound_buf, *sound_buf_1, *sound_buf_2;
int sound_buffer_index;
int old_sound_val = 128 - AMPL_BEEPER;

int pc_sound_enabled = SOUND_OFF;

static char silent_sound[ SOUND_BUFFER_SIZE ];

void CALLBACK callback_function( HWAVEOUT hwo, 
								 UINT			uMsg, 
								 DWORD		dwInstance, 
								 DWORD		dwParam1, 
								 DWORD		dwParam2 )
{
	if( uMsg == MM_WOM_DONE )
	{
		/* Update sound buffer from Spectrum */
		memcpy( whdr[ wave_index ].lpData, pc_get_sound_buffer(), SOUND_BUFFER_SIZE );
		
		/* Assume that no sound buffers will pass another in the list */
		waveOutWrite( hwo, &whdr[ wave_index++ ], sizeof(WAVEHDR) );	
		if( wave_index == NR_SOUND_BUFFERS )
			wave_index = 0;
	}
}

MMRESULT pc_sound_init( void )
{
	int i;
	
	sound_buf_1     = new char[ SOUND_BUFFER_SIZE ];
	sound_buf_2     = new char[ SOUND_BUFFER_SIZE ];
	sound_buf       = sound_buf_1;
	sound_buffer_index = 1;

	memset( sound_buf_1, 128 - AMPL_BEEPER, SOUND_BUFFER_SIZE );
	memset( sound_buf_2, 128 - AMPL_BEEPER, SOUND_BUFFER_SIZE );

	Format.wFormatTag      = WAVE_FORMAT_PCM;
	Format.nChannels       = 1;
//	Format.nSamplesPerSec  = 32000;
//	Format.nAvgBytesPerSec = 32000;
	Format.nSamplesPerSec  = 44100;
	Format.nAvgBytesPerSec = 44100;
	Format.nBlockAlign     = 1;
	Format.wBitsPerSample  = 8;
	Format.cbSize          = 0;

	for (devId = 0; devId < waveOutGetNumDevs(); devId++) 
	{
//		mmres = waveOutOpen( &hwo, devId, &Format, (DWORD) callback_function, 0, CALLBACK_FUNCTION );
		mmres = waveOutOpen( &hwo, devId, &Format, 0, 0, CALLBACK_NULL );
		if (mmres == MMSYSERR_NOERROR) 
		{
			break;
		}
	}
	if (mmres != MMSYSERR_NOERROR) 
	{
		return mmres;
	}


	// Set volume
	mmres = waveOutGetVolume(hwo, &dwOldVolume);
	if (mmres != MMSYSERR_NOERROR) 
	{
		return mmres;
	}

#if 0
	waveOutSetVolume(hwo, 0xFFFFFFFF);
	if (mmres != MMSYSERR_NOERROR) 
	{
		return mmres;
	}
#endif

	for( i=0; i<NR_SOUND_BUFFERS; i++ )
	{
		ZeroMemory(&whdr[ i ], sizeof(WAVEHDR));
		whdr[ i ].lpData = new char[SOUND_BUFFER_SIZE];
		whdr[ i ].dwBufferLength = SOUND_BUFFER_SIZE;
		whdr[ i ].dwUser = 0;
		whdr[ i ].dwFlags =	0;
		whdr[ i ].dwLoops = 0;
		whdr[ i ].dwBytesRecorded = 0;
		whdr[ i ].lpNext = 0;
		whdr[ i ].reserved = 0;

		memset( whdr[ i ].lpData, 127, SOUND_BUFFER_SIZE );

		mmres = waveOutPrepareHeader( hwo, &whdr[ i ], sizeof( WAVEHDR ) );	
		if (mmres != MMSYSERR_NOERROR) 
		{
			return mmres;
		}
	}


	for( i=0; i<NR_SOUND_BUFFERS; i++ )
	{
		mmres = waveOutWrite(hwo, &whdr[ i ], sizeof(WAVEHDR));	
		if (mmres != MMSYSERR_NOERROR) 
		{
			return mmres;
		}
	}
 

	memset( silent_sound, 127, SOUND_BUFFER_SIZE );

	return MMSYSERR_NOERROR;
}

MMRESULT pc_sound_close( void )
{
	int i;

	waveOutReset( hwo );

	/* TBD - restore volume */
	waveOutSetVolume(hwo, dwOldVolume);

	for( i=0; i<NR_SOUND_BUFFERS; i++ )
	{
		waveOutUnprepareHeader( hwo, &whdr[ i ], sizeof( WAVEHDR ) );

		delete [] whdr[ i ].lpData;
	}

	delete [] sound_buf_1;
	delete [] sound_buf_2;

	waveOutClose( hwo );

	return MMSYSERR_NOERROR;
}

void pc_sound_beeper( int on )
{
	int val;
	int sound_buffer_pos;

	val=( on ? 128 + AMPL_BEEPER : 128 - AMPL_BEEPER );

	old_sound_val = val;

//	sound_buffer_pos = ( tstates * SOUND_BUFFER_SIZE ) / machine.cycles_per_frame;
	
	/* Fill rest of buffer with this value */	
	memset( sound_buf + sound_buffer_pos, val, SOUND_BUFFER_SIZE - sound_buffer_pos );
}

void pc_sound_frame( void )
{
	if( sound_buffer_index == 1 )
	{
		sound_buffer_index = 2;
		sound_buf = sound_buf_2;
		memset( sound_buf_2, old_sound_val, SOUND_BUFFER_SIZE );
	}
	else
	{
		sound_buffer_index = 1;
		sound_buf = sound_buf_1;
		memset( sound_buf_1, old_sound_val, SOUND_BUFFER_SIZE );
	}

#if 0

	for( i=0; i<NR_SOUND_BUFFERS; i++ )
	{
		if( whdr[ i ].dwFlags & MM_WOM_DONE )
		{
			/* Update sound buffer from Spectrum */
			memcpy( whdr[ wave_index ].lpData, pc_get_sound_buffer(), SOUND_BUFFER_SIZE );
			
			/* Assume that no sound buffers will pass another in the list */
			waveOutWrite( hwo, &whdr[ i ], sizeof(WAVEHDR) );	
	
		}
	}

#endif

}

char *pc_get_sound_buffer( void )
{
	if( sound_buffer_index == 1 )
		return sound_buf_2;
	else
		return sound_buf_1;
}

void pc_set_sound_volume( enum sound_mode_t sound_volume )
{
	switch( sound_volume )
	{
	case sound_auto: 
		pc_sound_enabled = SOUND_ON;
		waveOutSetVolume(hwo, dwOldVolume);
		break;
	case sound_max: 
		pc_sound_enabled = SOUND_ON;
		waveOutSetVolume(hwo, 0xF000F000);
		break;
	case sound_medium: 
		pc_sound_enabled = SOUND_ON;
		waveOutSetVolume(hwo, 0x50005000);
		break;
	case sound_minimum: 
		pc_sound_enabled = SOUND_ON;
		waveOutSetVolume(hwo, 0x10001000);
		break;
	case sound_off:
		pc_sound_enabled = SOUND_OFF;
		break;
	default: 
		pc_sound_enabled = SOUND_OFF;
		break;
	}
}

extern "C" int osssound_init(int *freqptr,int *stereoptr)
{
	if( pc_sound_init() == MMSYSERR_NOERROR )
		pc_sound_enabled = SOUND_ON;
	else
		pc_sound_enabled = SOUND_OFF;

	return 0;
}

extern "C" void osssound_end(void)
{
	pc_sound_close();
}

extern "C" void vanta( void )
{
	char temp[14];
	int sound_written = 0;
	static int tiden = 0;
	static int max_tiden = 0;
	static int raknare = 0;
	static int refresh_tiden = 0;

	if( tiden < msecs() )
		tiden = msecs();

//	display_refresh_all();
	display_refresh_all_dirty();

	if( refresh_tiden < msecs() )
		refresh_tiden = msecs();

	bar( 0, 0, 100, 10, 0x0000 );
	_itoa( tiden, temp, 10 );
	print( temp, 1, 1, 0xffff ); 
	_itoa( max_tiden, temp, 10 );
	print( temp, 80, 1, 0xffff ); 
	_itoa( refresh_tiden, temp, 10 );
	print( temp, 40, 1, 0xffff ); 

	update( true, 200 );

	if( max_tiden < msecs() )
		max_tiden = msecs();

	if( ( raknare++ ) > 100 )
	{
		raknare = 0;
		max_tiden = 0;
		refresh_tiden = 0;
		tiden = 0;
	}

	while( msecs() < 20 )
	{
		update( false, 0 );
	}

	resettimer();
}

void write_silent_sound( void )
{
	int i;

	for( i=0; i<NR_SOUND_BUFFERS; i++ )
	{
		if( whdr[ i ].dwFlags & WHDR_DONE )
		{
			memcpy( whdr[ i ].lpData, silent_sound, SOUND_BUFFER_SIZE );
			/* Assume that no sound buffers will pass another in the list */
			waveOutWrite( hwo, &whdr[ i ], sizeof(WAVEHDR) );
		}
	}

}


extern "C" void osssound_frame(unsigned char *data,int len)
{
	/* FUSE uses this function for sync (which is a good idea)!
	   */

	   /* len should be 640 (mono) or 2*640 (stereo), we do not want
	   stereo (I only have one speaker on my iPAQ;-) */

	char temp[14];

	int i;
	static int sound_written = 0;
	static int max_sound = 0;

	int hits;
	static int tiden = 0;
	static int max_tiden = 0;
	static int raknare = 0;
	static int refresh_tiden = 0;
	static int max_hits = 0;


	static frame_skip = 0;

	static int backup = 0;
	static char backup_copy[ SOUND_BUFFER_SIZE ];

	if( tape_playing )
		SystemIdleTimerReset();

	if( tiden < msecs() )
		tiden = msecs();

//	display_refresh_all();
	hits = display_refresh_all_dirty();

	if( max_hits < hits )
		max_hits = hits;

	if( refresh_tiden < msecs() )
		refresh_tiden = msecs();
	bar( 0, 0, 100, 10, 0x0000 );
	_itoa( max_sound, temp, 10 );
	print( temp, 1, 1, 0xffff ); 
	_itoa( max_hits, temp, 10 );
	print( temp, 80, 1, 0xffff ); 
	_itoa( max_tiden, temp, 10 );
	print( temp, 40, 1, 0xffff ); 

	frame_skip++;

	if( frame_skip >= (int)(current_settings.speed_setting) )
	{
		update( true, 200 );
	}

	sound_written = 0;

	if( pc_sound_enabled == SOUND_ON )
	{
//		for(;;)
//		{

			for( i=0; i<NR_SOUND_BUFFERS; i++ )
			{
				if( whdr[ i ].dwFlags & WHDR_DONE )
				{

					if( backup )
					{
					/* Update sound buffer from Spectrum */
						memcpy( whdr[ i ].lpData, backup_copy, SOUND_BUFFER_SIZE );
						
						/* Assume that no sound buffers will pass another in the list */
						waveOutWrite( hwo, &whdr[ i ], sizeof(WAVEHDR) );
						
						backup = 0;
					}
					else
					{

						/* Update sound buffer from Spectrum */
						memcpy( whdr[ i ].lpData, data, SOUND_BUFFER_SIZE );
						
						/* Assume that no sound buffers will pass another in the list */
						waveOutWrite( hwo, &whdr[ i ], sizeof(WAVEHDR) );	

						sound_written++;
					}
					
				}
			}

//			if( sound_written )
//				break;
//		}	
	}

	if( !sound_written )
	{
		backup = 1;
		memcpy( backup_copy, data, SOUND_BUFFER_SIZE );
	}
	else
	{
		backup = 0;
	}

	if( max_tiden < msecs() )
		max_tiden = msecs();

	if( max_sound < sound_written )
		max_sound = sound_written;

	if( ( raknare++ ) > 100 )
	{
		raknare = 0;
		max_tiden = 0;
		refresh_tiden = 0;
		tiden = 0;
		max_hits = 0;
		max_sound = 0;
	}

	if( frame_skip >= (int)(current_settings.speed_setting) )
	{
		frame_skip = 0;
	
		while( msecs() < 20 * (int)(current_settings.speed_setting) )
		{
			update( false, 0 );
		}

		resettimer();
	}
}



















