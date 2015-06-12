/*
    (C) 2002 Anders Holmberg

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

// ---------------------------------------------------------------------------- 
//  Includes 
// ---------------------------------------------------------------------------- 
#include "pocket_clive.h"
#include "pocket_clive_types.h"
#include "pocket_clive_display.h"
#include "pocket_clive_sound.h"

extern "C"
{
#include "contrib\fuse\config.h"
#include "contrib\fuse\types.h"
#include "contrib\fuse\z80\z80.h"
#include "contrib\fuse\display.h"
#include "contrib\fuse\event.h"
#include "contrib\fuse\keyboard.h"
#include "contrib\fuse\machine.h"
#include "contrib\fuse\settings.h"
#include "contrib\fuse\snapshot.h"
#include "contrib\fuse\sound.h"
#include "contrib\fuse\spectrum.h"
#include "contrib\fuse\tape.h"
#include "contrib\fuse\ui.h"
}

#include "contrib\easyce\EasyCE.h"
#include "Winbase.h"
#include "Winreg.h"
#include "tchar.h"
#include "Afx.h" 

// ---------------------------------------------------------------------------- 
//  Defines
// ---------------------------------------------------------------------------- 

// Picture file names
#define KEYBOARD	"keyboard.tga.gz"
#define LOGO		"CliveLogo.tga.gz"
#define LOAD		"load.tga.gz"
#define CONFIGURE	"configure.tga.gz"
#define JOYSTICK	"virt_joy.tga.gz"
#define TAPE		"tape.tga.gz"

// Modes of emulator operation
#define EMULATION_MODE      1
#define CONFIGURATION_MODE  2
#define DISASSEMBLER_MODE   3  // Never implemented
#define LOAD_MODE	        4
#define TAPE_MODE           5
#define TAPE_CHOOSEN_MODE   6  // Not implemented (should indicate that a tape
                               // file is being examined)
// Types of emulator input
#define JOYSTICK_EMULATION 1
#define KEYBOARD_EMULATION 2

// UNCHANGED is used by the function set_mode to indicate that
// either the mode or input type (see above) shall stay unchanged
#define UNCHANGED           100

// Subscreen definitions used by the configuration screen
#define NO_SUB_SCREEN					0
#define TARGET_SUB_SCREEN				1
#define SOUND_SUB_SCREEN				3
#define SPEED_SUB_SCREEN				4
#define SCREEN_SUB_SCREEN				5
#define SAVE_SNAPSHOT_SUB_SCREEN		6
#define CONFIGURE_JOYSTICK_SUB_SCREEN	7
#define JOYSTICK_SUB_SCREEN				8

// Max size of names in the list of tape or rom files
// TODO : Should really use memory allocation and a pointer
#define DIR_ELEMENT_NAME_SIZE 64

// Used by the caps and symbol shift processing
#define NOT_ACTIVATED 0
#define ACTIVATED     1

// ----------------------------------------------------------------------------
//  Structs and typedefs
// ----------------------------------------------------------------------------

// Node element in the linked list of roms or tapes (or any linked list of 
// strings actually)
struct dir_element_t 
{
		char   name[ DIR_ELEMENT_NAME_SIZE ];
		char   complete_name;     /* 1 if name fitted within DIR_ELEMENT_NAME_SIZE characters */
		struct dir_element_t *next;
};

typedef struct dir_element_t *dir_element_ptr;

// ----------------------------------------------------------------------------
//  Global variables (Didn't I tell you that this program is a hack!)
// ----------------------------------------------------------------------------

// This is the current settings of the emulator (e.g. if the sound is enabled
// or not). This is used in other parts of the emulator (e.g. in the sound
// module).
struct settings_t current_settings = { 0, speed_full, screen_zoom, sound_auto };

// String with directory to roms directory (this is used by FUSE). This is
// a #define in the FUSE Linux environment that can be changed at compile 
// time, but since no-one is compiling PocketClive except me I have changed
// it into a variable that is filled at runtime with the path to the 
// PocketClive ex-ecutable with '\Roms' added.  
char ROMSDIR[ MAX_PATH ];

// ----------------------------------------------------------------------------
//  Local variables 
// ----------------------------------------------------------------------------

// The last key that the user has pressed (this is used to "unpress" a key when
// the user removes the stylus from the touchscreen)
static keyboard_key_name last_key;

// Where are the keys locates on the PocketClive keyboard
// KEYBOARD_End simulates the Extended key and KEYBOARD_Home simulates 
// backspace
static keyboard_key_name keyboard_loc[ 5 ][ 10 ] = 
{
	{ KEYBOARD_1, KEYBOARD_2, KEYBOARD_3, KEYBOARD_4, KEYBOARD_5, 
	  KEYBOARD_6, KEYBOARD_7, KEYBOARD_8, KEYBOARD_9, KEYBOARD_0  }, 
	{ KEYBOARD_q, KEYBOARD_w, KEYBOARD_e, KEYBOARD_r, KEYBOARD_t, 
	  KEYBOARD_y, KEYBOARD_u, KEYBOARD_i, KEYBOARD_o, KEYBOARD_p  }, 

	{ KEYBOARD_a, KEYBOARD_s, KEYBOARD_d, KEYBOARD_f, KEYBOARD_g, 
	  KEYBOARD_h, KEYBOARD_j, KEYBOARD_k, KEYBOARD_l, KEYBOARD_Enter  }, 
	{ KEYBOARD_Caps, KEYBOARD_z, KEYBOARD_x, KEYBOARD_c, KEYBOARD_v, 
	  KEYBOARD_b, KEYBOARD_n, KEYBOARD_m, KEYBOARD_Symbol, KEYBOARD_End  }, 
	{ KEYBOARD_NONE, KEYBOARD_NONE, KEYBOARD_NONE, KEYBOARD_NONE, KEYBOARD_NONE, 
	  KEYBOARD_NONE, KEYBOARD_NONE, KEYBOARD_space, KEYBOARD_space, KEYBOARD_Home	 }
};

// Both the CAPS and SYMBOL SHIFT buttons are sticky, i.e. they stay pressed
// even after the user releases the button. These variables indicate the 
// status of both
static int caps_activated = NOT_ACTIVATED;
static int symb_activated = NOT_ACTIVATED;

// Pointers used by the linked list of roms or tapes
static dir_element_ptr first_dir_element = NULL;
static dir_element_ptr current_dir_element = NULL;
static dir_element_ptr last_dir_element = NULL;

// Input type indicates if the emulator is displaying the keyboard or the 
// virtual joystick 
static int input_type = KEYBOARD_EMULATION;

// Emulator mode
static int mode = EMULATION_MODE;

// Which sub configuration screen is viewed at the moment (only valid when
// the emulator mode is equal to CONFIGURATION_MODE)
static int sub_screen = NO_SUB_SCREEN;


// Set to TRUE if the application wants to terminate PocketClive
static int Terminated = FALSE;

// Copyright text (from splash screen)
static char copyright[][32] = {
	"                               ", 
	"                               ",
	"                               ",
	"           Release 2B          ",
	"   (C) 2002 Anders Holmberg    ", 
	"                               ", 
	"                               "  };

// Configuration screen text
static char configure[][32] = {
	"TARGET     SPECTRUM 48K", 
	"SCREEN     ZOOM",
	"SPEED      FULL",
	"SOUND      AUTO",
	"JOYSTICK	KEMPSTON", 
	"", 
	"CONFIGURE JOYSTICK",
	"SAVE SNAPSHOT",
	"RESET POCKETCLIVE", 
	"EXIT POCKETCLIVE",
	"", 
	" POCKETCLIVE RELEASE 2B", 
	"(C) 2002 ANDERS HOLMBERG", 
};

// Target platform configuration text
static char target_text[][32] = {
	"SPECTRUM 48K", 
	"SPECTRUM 128K", 
	"PLUS 2", 
	"PLUS 3"
};

// Screen configuration text
static char screen_text[][32] = 
{
	"SHRINK", 
	"ALIGN MIDDLE", 
	"ALIGN LEFT", 
	"ALIGN RIGHT"
};

// Speed configuration text
static char speed_text[][32] = 
{
	"FULL", 
	"1 FRAME SKIP", 
	"2 FRAME SKIPS", 
	"3 FRAME SKIPS"
};

// Sound configuration text
static char sound_text[][32] = 
{
	"AUTO", 
	"MAX", 
	"MEDIUM",
	"MINIMUM", 
	"OFF"
};

// Joystick configuration text
static char joystick_text[][32] = 
{
	"KEMPSTON",
	"USER DEFINED"
};
	
// Index into the linked list of roms or tapes indicating which
// item is currently the top item on screen
static int top_dir_index = 0;

// These variables are holders for the last top item for the
// tape and roms screen (used for returning to the same
// position the next time the tape or rom screen is viewed) 
static int top_dir_tape_index = 0;
static int top_dir_rom_index = 0;

// mod_path is a Unicode string indicating the path to the PocketClive
// executable (all roms and tapes are stored relatively to this 
// location). This string ends with the '\' character.
//
// TODO : This shall be removed in favour of the wModuleFileName string
//        defined below  
static WORD mod_path[ MAX_PATH ];

// wModuleFileName is a Unicode string indicating the path to the 
// Pocketclive executable. This string does not include the '\'
// character
static wchar_t wModuleFileName[ MAX_PATH ];

// Storage location and index of save snapshot name (typed in by the user in
// the configuration screen)
static int snapshot_name_index = 0;
static char snapshot_name[ 15 ];

// Storage location and index for user definition of joystick (typed in by
// the user in the configuration screen). 
static int config_joystick_index = 0;
static char config_joystick_char[6] = { '\0', '\0', '\0', '\0', '\0', '\0' };

// Some problem with the tga image file loader makes it impossible for me to 
// load small images from file. Therefore I have added a large image area from
// which I can blit smaller portions onto the PocketPC screen. 
static unsigned short image_buffer[ 128 * 128 ];

// This variable pauses the execution of the emulator (used by the EasyCE 
// library when PocketClive looses and regains focus).
static int paused = 0;

// ----------------------------------------------------------------------------
//  External definitions
// ----------------------------------------------------------------------------

// The joystick variable is declared and updated by FUSE
extern "C" BYTE joystick;

// ----------------------------------------------------------------------------
//  Local functions
// ----------------------------------------------------------------------------

// Set the joystick to a certain value (the value is implemented just like the
// Kempston interface)
static void joystick_set( BYTE value );

// Reset the movement part of the joystick
static void joystick_reset_movement( void );

// Reset the fire part of the joystick
static void joystick_reset_fire( void );

// Load and save configuration data from the Windows registry
static void load_preferences( void );
static void save_preferences( void );

// Press the key located at xpos, ypos
static void keyboard_down( int xpos, int ypos );

// Release the last key pressed
static void keyboard_up( void );

// Fill the linked list with all roms
static void dir_snapshot( void );

// Fill the linked list with all tapes
static void dir_tape( void );

// Show the copyright splash screen
static void show_copyright( void );

// Setup a sub screen (one of the small windows in the configuration mode)
// This is is generic function that is used for all configuration items
// that does not need special treatment, see below for specific functions 
static void setup_sub_screen( char *title, char items[][32], int nr_items, int ypos );

// Setup the save snapshot sub screen
static void setup_save_snapshot_screen( int ypos );

// Setup the configure user defined joystick subscreen
static void setup_configure_joystick_screen( int ypos );

// Add directory element (rom are tape filename) to linked list. The element
// is added in alphabetic order
static void add_dir_element( char *name_ptr );

// Get first element in list or NULL if list is empty
static dir_element_ptr get_first_dir_element( void );

// Get next element in list or NULL if end of list is reached. Requires 
// that the get_first_dir_element is called first. 
static dir_element_ptr get_next_dir_element( void );

// Remove the first element in the linked list
static void delete_first_dir_element( void );

// Get the indicated directory element or NULL if index points to a
// non existing element
static dir_element_ptr get_sel_dir_element( int index );

// How many elements are there currently in the list
static int nr_dir_elements( void );

// Check (using EasyCE) if the keyboard is pressed or clicked. If so call
// the appropriate callback function
static void check_keyboard( void );

// Set both the emulation mode and the input mode
static void set_mode( int new_mode, int new_input );

// Find the "root" path, i.e. the path to the PocketClive executable
static void set_root_path( void );

// Build an Unicode absolute path to the filename (the directory is
// the same as the directory in which the PocketClive executable resides.
static void lbuild_absolute_filename( WORD *output, WORD *filename );

// Build a character absolute path to the filename (the directory is
// the same as the directory in which the PocketClive executable resides.
static void build_absolute_filename( char *output, char *filename );

// Blit the speaker symbol to the lower right corner
static void show_speaker( int activated );

// Show either the TZX tape player or the Clive Sinclair logo (the tape player
// is only shown if the last tape file is a .tzx and not a .tap file)
static void show_tape_player( int activated );

// Update both the tape player and speaker symbol 
static void update_keyboard_image( void );

// PocketClive uses callback functions for its interface. For each mode of 
// operation (such as emulation mode or configuration mode) the screen of
// the PocketPC is divided into different areas. When the user presses one 
// of these areas the corresponding callback function is executed. Each 
// callback function takes for parameters. The xpos and ypos is the actual
// coordinates the user pressed. The xchar and ychar is a block position
// within the area (the block size is defined for each area). 
static void cb_save_snap( int xpos, int ypos, int xchar, int ychar );  
static void cb_load_tape( int xpos, int ypos, int xchar, int ychar );  
static void cb_config_screen( int xpos, int ypos, int xchar, int ychar );
static void cb_sub_config_screen( int xpos, int ypos, int xchar, int ychar );
static void cb_load_screen( int xpos, int ypos, int xchar, int ychar );
static void cb_tape_screen( int xpos, int ypos, int xchar, int ychar );
static void cb_config_up( int xpos, int ypos, int xchar, int ychar );
static void cb_config_down( int xpos, int ypos, int xchar, int ychar );
static void cb_config_key_1( int xpos, int ypos, int xchar, int ychar );
static void cb_config_key_2( int xpos, int ypos, int xchar, int ychar );
static void cb_config_key_3( int xpos, int ypos, int xchar, int ychar );
static void cb_config_key_4( int xpos, int ypos, int xchar, int ychar );
static void cb_keyboard( int xpos, int ypos, int xchar, int ychar );
static void cb_joy_n( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_ne( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_e( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_se( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_s( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_sw( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_w( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_nw( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_n_fire( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_ne_fire( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_e_fire( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_se_fire( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_s_fire( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_sw_fire( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_w_fire( int xpos, int ypos, int xchar, int ychar );   
static void cb_joy_nw_fire( int xpos, int ypos, int xchar, int ychar );  
static void cb_joy_fire( int xpos, int ypos, int xchar, int ychar );  
static void cb_configure_keyboard( int xpos, int ypos, int xchar, int ychar );
static void cb_load_keyboard( int xpos, int ypos, int xchar, int ychar );
static void cb_tape_play( int xpos, int ypos, int xchar, int ychar );
static void cb_tape_stop( int xpos, int ypos, int xchar, int ychar );
static void cb_tape_rewind( int xpos, int ypos, int xchar, int ychar );
static void cb_mute( int xpos, int ypos, int xchar, int ychar );

// ----------------------------------------------------------------------------
//  Area definitions (these are defined after the local functions because
//  they contain pointers to the callback functions).
// ----------------------------------------------------------------------------

// Sub configuration mode (when the user has selected a configuration item in
// configuration mode). 
static area_t sub_configuration_areas[1] = {
	{ 0, 0, 0, 0, cb_sub_config_screen, CLICK_TYPE, 0, 0, 0, 0 }
};

// Load snapshot screen
static area_t load_areas[] = { 
	{  10,  11, 217, 180, cb_load_screen  , CLICK_TYPE, 0, 0, 12, 10 }, 
	{ 221, 147, 237, 163, cb_config_up    , CLICK_TYPE, 0, 0, 0, 0 },
	{ 221, 166, 237, 182, cb_config_down  , CLICK_TYPE, 0, 0, 0, 0 },
	{   1, 286,  32, 319, cb_config_key_1 , CLICK_TYPE, 0, 0, 0, 0 },
	{       1, 201, 236, 277, cb_load_keyboard    , CLICK_TYPE, 0, 0, 0, 0 }, 
	{     145, 281, 236, 296, cb_load_keyboard    , CLICK_TYPE, 0, 0, 0, 0 }, 
};

// Load tape screen
static area_t tape_areas[] = { 
	{  10,  11, 217, 180, cb_tape_screen  , CLICK_TYPE, 0, 0, 12, 10 }, 
	{  221, 147, 237, 163, cb_config_up    , CLICK_TYPE, 0, 0, 0, 0 },
	{  221, 166, 237, 182, cb_config_down  , CLICK_TYPE, 0, 0, 0, 0 },
	{   97, 286, 128, 319, cb_config_key_4, CLICK_TYPE  , 0, 0, 0, 0 },
	{       1, 201, 236, 277, cb_load_keyboard    , CLICK_TYPE, 0, 0, 0, 0 }, 
	{     145, 281, 236, 296, cb_load_keyboard    , CLICK_TYPE, 0, 0, 0, 0 }, 
};

// Configure emulator screen
static area_t configuration_areas[] = { 
	{  10,  11, 217, 181, cb_config_screen, CLICK_TYPE, 0, 0, 12, 12 }, 
	{  33, 286,  64, 319, cb_config_key_2, CLICK_TYPE  , 0, 0, 0, 0 },
	{   65, 286,  96, 319, cb_config_key_3 , CLICK_TYPE, 0, 0, 0, 0 },
	{       1, 201, 236, 277, cb_configure_keyboard    , CLICK_TYPE, 0, 0, 0, 0 }, 
	{     145, 281, 236, 296, cb_configure_keyboard    , CLICK_TYPE, 0, 0, 0, 0 }, 
};

// Running emulation with keyboard screen
static area_t emul_keyboard_areas[] = { 
	{   1, 201, 236, 277, cb_keyboard    , PENDOWN_TYPE, 0, 0, 0, 0 }, 
	{ 145, 281, 236, 296, cb_keyboard    , PENDOWN_TYPE, 0, 0, 0, 0 }, 
	{			 80,   0, 160, 64,  cb_joy_n       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{	  160,   0, 240, 64,  cb_joy_ne      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{			160,  64, 240, 128, cb_joy_e       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{		160, 128, 240, 192, cb_joy_se      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{			 80, 128, 160, 192, cb_joy_s       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{		  0, 128,  80, 192, cb_joy_sw      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{			  0,  64,  80, 128, cb_joy_w       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{		  0,   0,  80, 64,  cb_joy_nw      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{   1, 286,  32, 319, cb_config_key_1, CLICK_TYPE  , 0, 0, 0, 0 },
	{  33, 286,  64, 319, cb_config_key_2, CLICK_TYPE  , 0, 0, 0, 0 },
	{  65, 286,  96, 319, cb_config_key_3, CLICK_TYPE  , 0, 0, 0, 0 },
	{  97, 286, 128, 319, cb_config_key_4, CLICK_TYPE  , 0, 0, 0, 0 },
	{ 166, 302, 182, 316, cb_tape_play, CLICK_TYPE  , 0, 0, 0, 0 },
	{ 183, 302, 196, 316, cb_tape_stop, CLICK_TYPE  , 0, 0, 0, 0 },
	{ 198, 302, 213, 316, cb_tape_rewind, CLICK_TYPE  , 0, 0, 0, 0 },
	{ 225, 302, 239, 314, cb_mute, CLICK_TYPE  , 0, 0, 0, 0 },
};

// Running emulation with joystick screen
static area_t emul_joystick_areas[] = { 
	{ 			150, 197, 189, 236, cb_joy_n       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 	  189, 197, 228, 236, cb_joy_ne      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 			189, 236, 228, 275, cb_joy_e       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 		189, 275, 228, 314, cb_joy_se      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 			150, 275, 189, 314, cb_joy_s       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 		111, 275, 150, 314, cb_joy_sw      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 			111, 236, 150, 275, cb_joy_w       , PENDOWN_TYPE, 0, 0, 0, 0 },
	{ 		111, 197, 150, 236, cb_joy_nw      , PENDOWN_TYPE, 0, 0, 0, 0 },
	{  150, 236, 189, 275, cb_joy_fire    , PENDOWN_TYPE, 0, 0, 0, 0 },
	{    1, 286,  32, 319, cb_config_key_1, CLICK_TYPE  , 0, 0, 0, 0 },
	{   33, 286,  64, 319, cb_config_key_2, CLICK_TYPE  , 0, 0, 0, 0 },
	{   65, 286,  96, 319, cb_config_key_3, CLICK_TYPE  , 0, 0, 0, 0 },
	{ 225, 302, 239, 314, cb_mute, CLICK_TYPE  , 0, 0, 0, 0 },
};


/* --- Implementation --------------------------------------------------------- */

void __cdecl main()
{
	int game_nr = 0;
	int sub_menu = -1;
	int elapsed_time = 0;
	int nr_of_frames = 1;

	int error;

	int nr_bytes;

#if 0
	surf = getsecondary();
#endif

	/* Store paths for different directories so that the PocketClive executable and
	   other directories can be moved around (and to support PocketPC of non english
	   versions */
	nr_bytes = GetModuleFileName( NULL, wModuleFileName, MAX_PATH + 1 );
	
	/* Remove \PocketClive.exe - a bit dirty solution which makes it impossible to
	   rename the executable (or at least the name has to be the same length as 
	   the original) */
	*( wModuleFileName + nr_bytes - 16 ) = '\0';

	/* Since FUSE is no-unicode some variables has to be converted to char instead of
	   wchar_t */
	WideCharToMultiByte(CP_ACP, 0, wModuleFileName, wcslen( wModuleFileName ) + 1, ROMSDIR, MAX_PATH - 1, NULL, NULL);
	
	/* Load the images of the speakers */
	tgazload( "Images.tga.gz", 128, 128, &image_buffer[0] );

	/* Now follows the initialisation that is copied more or less unchanged from
	   FUSE */	
	if( settings_init() ) 
		return;

	settings_current.joy_kempston = 1;
	settings_current.tape_traps = 1;

	if( tape_init() ) 
		return;

	if(display_init(NULL,NULL)) 
		return;
  
	if(event_init()) 
		return;

	sound_init();

	fuse_keyboard_init();

	z80_init();

	error = machine_init_machines();
	if( error ) 
		return;
  
	error = machine_select_first();
	if( error ) 
		return;

	load_preferences();
	set_root_path();
	show_copyright();
	display_refresh_all();
	set_mode( EMULATION_MODE, KEYBOARD_EMULATION );

	update_keyboard_image();
	show_tape_player( 0 );

	update( TRUE, 320 );
	resettimer();

	while( !Terminated )
	{
		if( !paused )
		{
			if( mode == EMULATION_MODE )
			{
				z80_do_opcodes();
				event_do_events();
			}
			else if( msecs() >= 20 )
			{
				write_silent_sound();
				resettimer();
				update(true, 200);
			} 

			check_keyboard();
		}
		else
		{
			update( false, 0 );
		}
	}

	machine_end();
	sound_end();
	event_end();
	ui_end();
}

/* --- show_copyright ---
 *
 * Show the PocketClive copyright screen
 *
 * ---
 */
void show_copyright( void )
{
	int i;

	/* Show copyright screen */
	clear( 0x0000 );

	unsigned short * p = (unsigned short*)getbuffer();
	p += 40*240;
	tgazload( LOGO, 240, 85, p );

	for( i=0; i<5; i++ )
		print( copyright[ i ], 0, i * 25 + 90, 0xffff );

	print( copyright[ 5 ], 0, 125 + 70, 0xffff );
	print( copyright[ 6 ], 0, 135  + 70, 0xffff );

	/* Wait for user click */
	while ( clicked() ) 
		update( TRUE, 320  );
	while ( !clicked() ) 
		update( TRUE, 320 );

	/* Load keyboard image */
	clear( 0 );

	p = (unsigned short*)getbuffer();
	p += 192*240;
	tgazload( KEYBOARD, 240, 128, p );
	update_keyboard_image();
}

/* --- lbuild_absolute_filename ---
 *
 * build an abosulte filename (with path) by 
 * adding the path to the PocketClive executable
 *
 * This is the unicode (long) version
 *
 * ---
 */
void lbuild_absolute_filename( WORD *output, WORD *filename )
{
	lstrcpy( output, mod_path );
	lstrcat( output, filename );
}

/* --- build_absolute_filename ---
 *
 * build an abosulte filename (with path) by 
 * adding the path to the PocketClive executable
 *
 * This is the char version
 *
 * ---
 */
void build_absolute_filename( char *output, char *filename )
{
	WideCharToMultiByte(CP_ACP, 0, 
		mod_path, wcslen(mod_path) + 1, 
		output, MAX_PATH - 1, 
		NULL, NULL);
	strcat( output, filename );
}

/* --- set_root_path ---
 * 
 * Sets to root of the Roms directory
 *
 * ---
 */
void set_root_path( void )
{
	DWORD number_bytes;

	number_bytes = GetModuleFileName( NULL, mod_path, MAX_PATH + 1 );
	/* Remove PocketClive.exe */
	*( mod_path + number_bytes - 15 ) = 0;
}

/* --- dir_snapshot ---
 *
 * Fill the dir_element linked list with all *.z80 and *.sna files 
 * in the \\Program Files\\PocketClive\\Roms\\ directory.
 *
 * ---
 */
void dir_snapshot( void )
{
	WIN32_FIND_DATA FileData;  
	HANDLE					hSearch;            
	char						temp_name[ 32 ];
	WORD						file_name[ MAX_PATH ];

	BOOL bFinished = FALSE;

	/* Free old entries in the list */
	while( first_dir_element != NULL )
		delete_first_dir_element();

	/* Now find *.sna files */
	lstrcpy( file_name, mod_path );
	lstrcat( file_name, TEXT("Roms\\*.sna" ) );
	
	hSearch = FindFirstFile( file_name, &FileData );
	if( hSearch != INVALID_HANDLE_VALUE )
	{
 		WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, 32, NULL, NULL);
		add_dir_element( temp_name );

		while( !bFinished )
		{
			if (!FindNextFile (hSearch, &FileData))
			{
				bFinished = TRUE;
			}
			else
			{
				WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, 32, NULL, NULL);
				add_dir_element( temp_name );
			}
		}

		FindClose( hSearch );
	}

	/* Now find *.z80 */
	bFinished = FALSE;
		
	lstrcpy( file_name, mod_path );
	lstrcat( file_name, TEXT("Roms\\*.z80" ) );

	hSearch = FindFirstFile( file_name, &FileData );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		return;
	}

 	WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, 32, NULL, NULL);
	add_dir_element( temp_name );

	while( !bFinished )
	{
		if (!FindNextFile (hSearch, &FileData))
		{
			bFinished = TRUE;
		}
		else
		{
			WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, 32, NULL, NULL);
			add_dir_element( temp_name );
		}
	}

	if (!FindClose (hSearch))
	{
		return;
	}
} 

/* --- dir_tape ---
 *
 * Fill the dir_element linked list with all *.tap files 
 * in the \\Program Files\\PocketClive\\Tapes\\ directory.
 *
 * ---
 */
void dir_tape( void )
{
	WIN32_FIND_DATA FileData;  
	HANDLE					hSearch;            
	char						temp_name[ MAX_PATH ];
	WORD						file_name[ MAX_PATH ];

	BOOL bFinished = FALSE;

	/* Free old entries in the list */
	while( first_dir_element != NULL )
		delete_first_dir_element();

	/* Now find *.tap files */
	lstrcpy( file_name, mod_path );
	lstrcat( file_name, TEXT("Tapes\\*.tap" ) );
	
	hSearch = FindFirstFile( file_name, &FileData );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		return;
	}

 	WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, MAX_PATH, NULL, NULL);

	add_dir_element( temp_name );

	while( !bFinished )
	{
		if (!FindNextFile (hSearch, &FileData))
		{
			bFinished = TRUE;
		}
		else
		{
			WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, MAX_PATH, NULL, NULL);
			add_dir_element( temp_name );
		}
	}

	/* Now find *.tzx files */
	bFinished = FALSE;  

	lstrcpy( file_name, mod_path );
	lstrcat( file_name, TEXT("Tapes\\*.tzx" ) );
	
	hSearch = FindFirstFile( file_name, &FileData );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		return;
	}

 	WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, MAX_PATH, NULL, NULL);
	add_dir_element( temp_name );

	while( !bFinished )
	{
		if (!FindNextFile (hSearch, &FileData))
		{
			bFinished = TRUE;
		}
		else
		{
			WideCharToMultiByte(CP_ACP, 0, FileData.cFileName, -1, temp_name, MAX_PATH, NULL, NULL);
			add_dir_element( temp_name );
		}
	}

	if (!FindClose (hSearch))
	{
		return;
	}
} 

/* --- Linked list handling routines ---
 *
 * add_dir_element          - add an element at the end of the list
 * get_first_dir_element    - get the first element in the list (also set up
 *                            global variable to be used by get_next... function)
 * get_next_dir_element     - get the "next" element in the list
 * get_sel_dir_element      - get the specified element in the list
 * nr_dir_elements          - how many elements are there in the list
 * delete_first_dir_element - delete the first element in the list
 * 
 * ---
 */
void add_dir_element( char *name_ptr )
{
	dir_element_ptr dir_element;
	dir_element_ptr left_sort_dir_element, right_sort_dir_element;

	dir_element = (dir_element_ptr)malloc( sizeof( dir_element_t ) );
	strncpy( dir_element->name, name_ptr, DIR_ELEMENT_NAME_SIZE );

	if( strlen( name_ptr ) >= DIR_ELEMENT_NAME_SIZE )
	{
		/* Truncate string */
		dir_element->name[ DIR_ELEMENT_NAME_SIZE - 1 ] = '\0';
		dir_element->complete_name = 0;
	}
	else
	{
		dir_element->complete_name = 1;
	}

	_strupr( dir_element->name );

	if( first_dir_element == NULL )
	{
		first_dir_element       = dir_element;
		first_dir_element->next = NULL;
		last_dir_element        = dir_element;
	}
	else
	{
		/* Special case - first in list */
		if( strcmp( first_dir_element->name, dir_element->name ) > 0 )
		{
			dir_element->next = first_dir_element;
			first_dir_element = dir_element;
		}
		else
		{
			left_sort_dir_element  = first_dir_element;
			right_sort_dir_element = first_dir_element->next;
				
			for( ;; )
			{
				if( right_sort_dir_element == NULL )
				{
					dir_element->next      = NULL;
					last_dir_element->next = dir_element;
					last_dir_element       = dir_element;
					break;
				}
				else if( strcmp( right_sort_dir_element->name, dir_element->name ) > 0 )
				{
					dir_element->next           = right_sort_dir_element;
					left_sort_dir_element->next = dir_element;
					break;
				}

				left_sort_dir_element  = right_sort_dir_element;
				right_sort_dir_element = right_sort_dir_element->next;
			}
		}
	}
}

dir_element_ptr get_first_dir_element( void )
{
	current_dir_element = first_dir_element;

	return first_dir_element;
}

dir_element_ptr get_next_dir_element( void )
{
	if( current_dir_element != NULL )
		current_dir_element = current_dir_element->next;

	return current_dir_element;
}

void delete_first_dir_element( void )
{
	dir_element_ptr temp_dir_element;

	if( first_dir_element != NULL )
	{
		temp_dir_element  = first_dir_element;
		first_dir_element = first_dir_element->next;
		free( temp_dir_element );
	}
}

dir_element_ptr get_sel_dir_element( int index )
{
	dir_element_ptr dir_element;
	int i;

	dir_element = get_first_dir_element();

	if( index == 0 )
		return dir_element;

	for( i=0; i<index; i++ )
	{
		dir_element = get_next_dir_element();
		if( dir_element == NULL )
			return NULL;
	}

	return dir_element;

}

int nr_dir_elements( void )
{
	dir_element_ptr dir_element;
	int nr = 0;

	dir_element = get_first_dir_element();

	while( dir_element != NULL )
	{
		dir_element = dir_element->next;
		nr++;
	}

	return nr;
}

/* --- setup_sub_screen ---
 *
 * Setup the sub menu screens used during configuration...
 *
 * ---
 */
void setup_sub_screen( char *title, char items[][32], int nr_items, int ypos )
{
	int i, xpos, col;

	bar( 95, ypos - 10, 228, ypos - 1 + nr_items * 12 , 0xffff );
	box( 95, ypos - 10, 228, ypos - 1 + nr_items * 12 , 0x0000 );

	for( i=0; i<nr_items; i++ )
	  print( items[i], 104, ypos + i * 12, 0x0000 );

	ypos -= 16;

	box( 95, ypos, 228, ypos + 11, BLACK );
	bar( 96, ypos + 1, 227, ypos + 11, color[ 1 ] );
	print( title, 100, ypos + 3, 0xffff );

	xpos = 195; col  = 2;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 200; col  = 6;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 205; col  = 4;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 210; col  = 5;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );
	

	sub_configuration_areas[0].x1       = 95;
	sub_configuration_areas[0].y1       = ypos - 10;
	sub_configuration_areas[0].x2       = 228;
	sub_configuration_areas[0].y2       = ypos + 5 + nr_items * 10;
	sub_configuration_areas[0].y_mult   = 10;
	sub_configuration_areas[0].y_offset = 10;
}

/* --- setup_save_snapshot_screen ---
 *
 * ---
 */
void setup_save_snapshot_screen( int ypos )
{
	int col, xpos, i;

	bar( 95, ypos, 228, ypos + 60, 0xffff );
	box( 95, ypos, 228, ypos + 60, BLACK );
	box( 95, ypos, 228, ypos + 11, BLACK );
	bar( 96, ypos + 1, 227, ypos + 11, color[ 1 ] );
	print( "ENTER NAME", 100, ypos + 3, 0xffff );

	xpos = 195; col  = 2;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 200; col  = 6;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 205; col  = 4;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 210; col  = 5;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	box( 100, ypos + 20, 187, ypos + 31, BLACK );
//	print( "ABCDEFGHIJ", 105, ypos + 23, BLACK );

	box( 108, ypos + 40, 148, ypos + 51, BLACK );
	print( "SAVE", 113, ypos + 43, BLACK );

	box( 160, ypos + 40, 215, ypos + 51, BLACK );
	print( "CANCEL", 165, ypos + 43, BLACK );

}

void setup_configure_joystick_screen( int ypos )
{
	int col, xpos, i;

	bar( 95, ypos, 228, ypos + 80, 0xffff );
	box( 95, ypos, 228, ypos + 80, BLACK );
	box( 95, ypos, 228, ypos + 11, BLACK );
	bar( 96, ypos + 1, 227, ypos + 11, color[ 1 ] );
	print( "CONFIG JOYSTICK", 100, ypos + 3, 0xffff );

	xpos = 195; col  = 2;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 200; col  = 6;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 205; col  = 4;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	xpos = 210; col  = 5;
	for( i=0; i<5; i++ )
		line( xpos + 10 + i, ypos + 1, xpos + i, ypos + 11, color[ col ] );

	print( "UP    : ", 105, ypos + 23, BLACK );
	print( "DOWN  : ", 105, ypos + 33, BLACK );
	print( "LEFT  : ", 105, ypos + 43, BLACK );
	print( "RIGHT : ", 105, ypos + 53, BLACK );
	print( "FIRE  : ", 105, ypos + 63, BLACK );
}


/* --- check_keyboard ---
 *
 * Check if the user has pressed the screen, call the correct
 * callback function if so is the case. 
 *
 * ---
 */
void check_keyboard( void )
{
	area_ptr area;
	int      xchar, ychar;

	joystick_reset_movement();
	joystick_reset_fire();

	if( clicked() )
	{
		area = get_area( clickxpos(), clickypos() );
		if( area != NULL )
		{
			if( area->clicktype == CLICK_TYPE )
			{
				if( area->x_mult != 0 )
					xchar = ( clickxpos() - area->x1 - area->x_offset ) / area->x_mult;
				else
					xchar = 0;

				if( area->y_mult != 0 )
					ychar = ( clickypos() - area->y1 - area->y_offset ) / area->y_mult;
				else
					ychar = 0;

				area->area_clicked( clickxpos(), clickypos(), xchar, ychar );
			}
		}
	}
	
	if( pendown() )
	{
		area = get_area( clickxpos(), clickypos() );
		if( area != NULL )
		{
			if( area->clicktype == PENDOWN_TYPE )
			{

				if( area->x_mult != 0 )
					xchar = ( clickxpos() - area->x1 - area->x_offset ) / area->x_mult;
				else
					xchar = 0;

				if( area->y_mult != 0 )
					ychar = ( clickypos() - area->y1 - area->y_offset ) / area->y_mult;
				else
					ychar = 0;

				area->area_clicked( clickxpos(), clickypos(), xchar, ychar );
			}
		}
	}
	else
	{
		/* default handling when no pen is pressed */

		keyboard_up();

		if( input_type == KEYBOARD_EMULATION )
		{
			if( symb_activated )
				bar( 230, 194, 235, 199, 0xffff );
			else
				bar( 230, 194, 235, 199, BLACK );


			if( caps_activated )
				bar( 220, 194, 225, 199, 0xffff );
			else
				bar( 220, 194, 225, 199, BLACK );
		}
	}

	if( buttonleft() )
	{
		cb_joy_w(0,0,0,0);
	}

	if( buttonup() )
	{
		cb_joy_n(0,0,0,0);
	}

	if( buttonright() )
	{
		cb_joy_e(0,0,0,0);
	}

	if( buttondown() )
	{
		cb_joy_s(0,0,0,0);
	}

	if( buttonA() || buttonB() || buttonD() )
//	if( buttonA() || buttonB() || buttonC() || buttonD() )
	{
		cb_joy_fire(0,0,0,0);
	}

	if( current_settings.joystick_setting == joystick_user_defined )
		joystick = 0;

}

/* --- Callback Functions --- 
 *
 * Callback functions that are called by the check_keyboard
 * function when the user has pressed the "correct" area
 *
 * ---
 */
void cb_save_snap( int xpos, int ypos, int xchar, int ychar )
{
	char snapshot_name[ MAX_PATH ];

	build_absolute_filename( snapshot_name, "Roms\\" );
	strcat( snapshot_name, "test.z80" );
//	write_snapshot( snapshot_name );
}

 
void cb_tape_screen( int xpos, int ypos, int xchar, int ychar )
{
	dir_element_ptr dir_element;
	char tape_name[ MAX_PATH ];

	dir_element = get_sel_dir_element( top_dir_index + ychar );

	if( dir_element != NULL )
	{
		/* Is this a .tap or a .tzx file? */	
		if( dir_element->name[ strlen( dir_element->name ) - 1 ] == 'X' )
			settings_current.tape_traps = 0;
		else
			settings_current.tape_traps = 1;		

		update_keyboard_image();

		build_absolute_filename( tape_name, "Tapes\\" );
		strcat( tape_name, dir_element->name );
		tape_open( tape_name );

	}

	top_dir_tape_index = top_dir_index;

	display_refresh_all();
	update( true, 320 );
	set_mode( EMULATION_MODE, UNCHANGED );
}

void cb_load_screen( int xpos, int ypos, int xchar, int ychar )
{
	dir_element_ptr dir_element;
	char snapshot_name[ MAX_PATH ];

	dir_element = get_sel_dir_element( top_dir_index + ychar );

	if( dir_element != NULL )
	{
		build_absolute_filename( snapshot_name, "Roms\\" );
		strcat( snapshot_name, dir_element->name );
		snapshot_read( snapshot_name );
	}

	top_dir_rom_index = top_dir_index;

	display_refresh_all();
	set_mode( EMULATION_MODE, UNCHANGED );
}

void cb_config_up( int xpos, int ypos, int xchar, int ychar )
{
	int i;
	dir_element_ptr dir_element;

	if( top_dir_index < 15 )
		top_dir_index = 0;
	else
	{
		top_dir_index -= 15;
	}

	bar( 10, 16, 217, 176, 0xffff );
	
	dir_element = get_sel_dir_element( top_dir_index );

	if( dir_element == NULL )
		return;

	for( i=0; i<15; i++ )
	{
		printn( dir_element->name, 20, i * 10 + 24, BLACK, 25 );
		dir_element = get_next_dir_element();
		if( dir_element == NULL )
			break;
	}
}

void cb_config_down( int xpos, int ypos, int xchar, int ychar )
{
	int i;
	dir_element_ptr dir_element;

	if( top_dir_index < ( nr_dir_elements() - 15 ) )
		top_dir_index += 15;

	bar( 10, 16, 217, 176, 0xffff );
	
	dir_element = get_sel_dir_element( top_dir_index );

	if( dir_element == NULL )
		return;

	for( i=0; i<15; i++ )
	{
		printn( dir_element->name, 20, i * 10 + 24, BLACK, 25 );
		dir_element = get_next_dir_element();
		if( dir_element == NULL )
			break;
	}
}

void cb_sub_config_screen( int xpos, int ypos, int xchar, int ychar )
{
	if( sub_screen == TARGET_SUB_SCREEN )
	{
		if( ychar == 0 )
		{
			current_settings.machine_setting = SPECTRUM_MACHINE_48;
			machine_select( SPECTRUM_MACHINE_48 );
		}
		else if( ychar == 1 )
		{
			current_settings.machine_setting = SPECTRUM_MACHINE_128;
			machine_select( SPECTRUM_MACHINE_128 );
		}
		else if( ychar == 2 )
		{
			current_settings.machine_setting = SPECTRUM_MACHINE_PLUS2;
			machine_select( SPECTRUM_MACHINE_PLUS2 );
		}
		else if( ychar == 3 )
		{
			current_settings.machine_setting = SPECTRUM_MACHINE_PLUS3;
			machine_select( SPECTRUM_MACHINE_PLUS3 );
		}

		z80_reset();
	}

	/* uugly programing - 
	 * I just want to go back to the configuration screen 
	 */
	set_mode( EMULATION_MODE, UNCHANGED );
	cb_config_key_3(0,0,0,0);
}

void cb_config_screen( int xpos, int ypos, int xchar, int ychar )
{
	char buffer[ MAX_PATH ];

	if( sub_screen == NO_SUB_SCREEN )
	{
		if( ychar == 0 )
		{
			sub_screen = TARGET_SUB_SCREEN;
			setup_sub_screen( "TARGET", target_text, 4, 24 ); 
		}
		else if( ychar == 1 )
		{
			sub_screen = SCREEN_SUB_SCREEN;
			setup_sub_screen( "SCREEN", screen_text, 4, 36 );
		}
		else if( ychar == 2 )
		{
			sub_screen = SPEED_SUB_SCREEN;
			setup_sub_screen( "SPEED", speed_text, 4, 48 ); 
		}
		else if( ychar == 3 )
		{
			sub_screen = SOUND_SUB_SCREEN;
			setup_sub_screen( "SOUND", sound_text, 5, 60 ); 		
		}
		else if( ychar == 4 )
		{
			sub_screen = JOYSTICK_SUB_SCREEN;
			setup_sub_screen( "JOYSTICK", joystick_text, 2, 72 ); 		
		}
		else if( ychar == 7 )
		{
			sub_screen = SAVE_SNAPSHOT_SUB_SCREEN;
			setup_save_snapshot_screen( 80 );
			snapshot_name_index = 0;
		}
		else if( ychar == 6 )
		{
			sub_screen = CONFIGURE_JOYSTICK_SUB_SCREEN;
			setup_configure_joystick_screen( 40 );
	
			config_joystick_index = 0;	
		}
		else if( ychar == 8 ) 	
		{
			machine_select( current_settings.machine_setting );
			z80_reset();	
			display_refresh_all();
			set_mode( EMULATION_MODE, UNCHANGED );
		}
		else if( ychar == 9 )
		{
			Terminated = TRUE;
		}
	}
	else 
	{
		if( sub_screen == TARGET_SUB_SCREEN )
		{
			if( ychar == 0 )
			{
				current_settings.machine_setting = SPECTRUM_MACHINE_48;
				machine_select( SPECTRUM_MACHINE_48 );
			}
			else if( ychar == 1 )
			{
				current_settings.machine_setting = SPECTRUM_MACHINE_128;
				machine_select( SPECTRUM_MACHINE_128 );
			}
			else if( ychar == 2 )
			{
				current_settings.machine_setting = SPECTRUM_MACHINE_PLUS2;
				machine_select( SPECTRUM_MACHINE_PLUS2 );
			}
			else if( ychar == 3 )
			{
				current_settings.machine_setting = SPECTRUM_MACHINE_PLUS3;
				machine_select( SPECTRUM_MACHINE_PLUS3 );
			}
			z80_reset();
		}
		else if( sub_screen == SCREEN_SUB_SCREEN )
		{
			ychar--;

			if( ychar == 0 )
				current_settings.screen_mode_setting = screen_zoom;
			else if( ychar == 1 )
				current_settings.screen_mode_setting = screen_middle_align;
			else if( ychar == 2 )
				current_settings.screen_mode_setting = screen_left_align;
			else if( ychar == 3 )
				current_settings.screen_mode_setting = screen_right_align;

			display_refresh_all();
		}
		else if( sub_screen == SPEED_SUB_SCREEN )
		{
			ychar -= 2;

			if( ychar == 0 )
				current_settings.speed_setting = speed_full;
			else if( ychar == 1 )
				current_settings.speed_setting = speed_1_skip;
			else if( ychar == 2 )
				current_settings.speed_setting = speed_2_skip;
			else if( ychar == 3 )
				current_settings.speed_setting = speed_3_skip;
		}
		else if( sub_screen == SOUND_SUB_SCREEN )
		{
			ychar -= 3;

			if( ychar == 0 )
				current_settings.sound_mode_setting = sound_auto;	
			else if( ychar == 1 )
				current_settings.sound_mode_setting = sound_max;
			else if( ychar == 2 )
				current_settings.sound_mode_setting = sound_medium;
			else if( ychar == 3 )
				current_settings.sound_mode_setting = sound_minimum;
			else if( ychar == 4 )
				current_settings.sound_mode_setting = sound_off;

			pc_set_sound_volume( current_settings.sound_mode_setting );

			update_keyboard_image();
		}		
		else if( sub_screen == SAVE_SNAPSHOT_SUB_SCREEN )
		{
			if( ( ( xpos >= 108 ) && ( xpos <= 148 ) ) || ( ( ypos >= 120 ) && ( ypos <= 131 ) ) )
			{
				snapshot_name[ snapshot_name_index++ ] = '.';
				snapshot_name[ snapshot_name_index++ ] = 'z';
				snapshot_name[ snapshot_name_index++ ] = '8';
				snapshot_name[ snapshot_name_index++ ] = '0';
				snapshot_name[ snapshot_name_index++ ] = '\0';

				strcpy( buffer, ROMSDIR );
				strcat( buffer, "\\Roms\\" );
				strcat( buffer, snapshot_name );
				
				snapshot_write( buffer );
			}			
		}
		else if( sub_screen == JOYSTICK_SUB_SCREEN )
		{
			ychar -= 4;

			if( ychar == 0 )
			{
				current_settings.joystick_setting = joystick_kempston;				
			}
			else if( ychar == 1 )
			{
				current_settings.joystick_setting = joystick_user_defined;				
			}
		
		}

		/* uugly programing - 
		 * I just want to go back to the configuration screen 
		 */ 
		set_mode( EMULATION_MODE, UNCHANGED );
		cb_config_key_3(0,0,0,0);
		sub_screen = NO_SUB_SCREEN;
	
		save_preferences();

	}
}

void cb_keyboard( int xpos, int ypos, int xchar, int ychar )
{
  keyboard_down( xpos, ypos );
}

void cb_load_keyboard( int xpos, int ypos, int xchar, int ychar )
{

	keyboard_key_name key;
	char key_char;
	dir_element_ptr dir_element;
	int i, element_index;
	
	key = keyboard_loc[ ( ypos - 202 ) / 20 ][ ( xpos - 1 ) / 24 ];
	
	dir_element = get_first_dir_element();

	if( dir_element == NULL )
		return;

	element_index = 0;

	/* Ok, lets find the snapshot that starts with the character key_char */
	if( ( ( key >= KEYBOARD_0 ) && ( key <= KEYBOARD_9 ) ) || ( ( key >= KEYBOARD_a ) && ( key <= KEYBOARD_z ) ) )
	{
		key_char = (char)key;

		if( key >= KEYBOARD_a )
			key_char -= 32;

		while( dir_element != NULL )
		{
			if( dir_element->name[ 0 ] >= key_char )
			{
				top_dir_index = element_index;
				break;
			}

			element_index++;
			dir_element = get_next_dir_element();
		}

	}

	bar( 10, 16, 217, 176, 0xffff );
	
	dir_element = get_sel_dir_element( top_dir_index );

	if( dir_element == NULL )
		return;

	for( i=0; i<15; i++ )
	{
		printn( dir_element->name, 20, i * 10 + 24, BLACK, 25 );
		dir_element = get_next_dir_element();
		if( dir_element == NULL )
			break;
	}


}


void cb_configure_keyboard( int xpos, int ypos, int xchar, int ychar )
{
	keyboard_key_name key;
	char key_char[2];
	key = keyboard_loc[ ( ypos - 202 ) / 20 ][ ( xpos - 1 ) / 24 ];

	key_char[0] = '\0';
	key_char[1] = '\0';

	if( (input_type == KEYBOARD_EMULATION) && ( snapshot_name_index < 10 ) && ( sub_screen == SAVE_SNAPSHOT_SUB_SCREEN ) )
	{
		if( ( ( key >= KEYBOARD_0 ) && ( key <= KEYBOARD_9 ) ) || ( ( key >= KEYBOARD_a ) && ( key <= KEYBOARD_z ) ) )
		{
			key_char[0] = (char)key;
			print( key_char, 105 + snapshot_name_index * 8, 103, BLACK );
			snapshot_name[ snapshot_name_index ] = key_char[0];
			snapshot_name_index++;
		}
	}
	else if( (input_type == KEYBOARD_EMULATION) && ( sub_screen == CONFIGURE_JOYSTICK_SUB_SCREEN ) )
	{
		if( ( key != KEYBOARD_Caps ) && ( key!= KEYBOARD_Symbol ) && ( key!= KEYBOARD_End ) )
		{
			key_char[0] = (char)key;

			config_joystick_char[ config_joystick_index ] = key_char[0];
			if( key == KEYBOARD_space )
				print( "SPACE", 180, 63 + 10 * config_joystick_index, BLACK );
			else if( key == KEYBOARD_Enter )
				print( "ENTER", 180, 63 + 10 * config_joystick_index, BLACK );
			else
				print( key_char, 180, 63 + 10 * config_joystick_index, BLACK );

			config_joystick_index++;

			if( config_joystick_index >= 5 )
			{
				set_mode( EMULATION_MODE, UNCHANGED );
				cb_config_key_3(0,0,0,0);
				sub_screen = NO_SUB_SCREEN;
			
				save_preferences();
			
			}
		}
	}

}

void cb_joy_n( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 8 );
}
   
void cb_joy_ne( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 9 );
}
  
void cb_joy_e( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 1 );
}   

void cb_joy_se( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 5 );
}
  
void cb_joy_s( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 4 );
}
   
void cb_joy_sw( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 6 );
}  

void cb_joy_w( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 2 );
}
   
void cb_joy_nw( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 10 );
}  

void cb_joy_n_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 24 );
}
   
void cb_joy_ne_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 25 );
}
  
void cb_joy_e_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 17 );
}   

void cb_joy_se_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 21 );
}
  
void cb_joy_s_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 20 );
}
   
void cb_joy_sw_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 22 );
}  

void cb_joy_w_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 18 );
}
   
void cb_joy_nw_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 26 );
}  

void cb_joy_fire( int xpos, int ypos, int xchar, int ychar )
{
	joystick_set( 16 );
}  

void cb_config_key_1( int xpos, int ypos, int xchar, int ychar )
{
	unsigned short *p;
	dir_element_ptr dir_element;
	int i;

	if( mode == EMULATION_MODE )	
	{
		top_dir_index = top_dir_rom_index;
		
		set_mode( LOAD_MODE, UNCHANGED );
		
		p = (unsigned short*)getbuffer();
		tgazload( LOAD, 240, 192, p );

		dir_snapshot();

		if( top_dir_index > nr_dir_elements() )
			top_dir_index = 0;

		dir_element = get_sel_dir_element( top_dir_index );

		if( dir_element != NULL )
		{	
			for( i=0; i<15; i++ )
			{
				print( dir_element->name, 20, i * 10 + 24, BLACK );
				dir_element = get_next_dir_element();
				if( dir_element == NULL )
					break;
			}
		}
	}
	else if( mode == LOAD_MODE )
	{
		top_dir_rom_index = top_dir_index;

		display_refresh_all();
		set_mode( EMULATION_MODE, UNCHANGED );
	}
}

void cb_config_key_2( int xpos, int ypos, int xchar, int ychar )
{
	unsigned short *p;

	if( input_type == JOYSTICK_EMULATION )
	{
		p = (unsigned short*)getbuffer();
		p += 192*240;
		tgazload( KEYBOARD, 240, 128, p );
		update_keyboard_image();

		set_mode( UNCHANGED, KEYBOARD_EMULATION );
		input_type = KEYBOARD_EMULATION;
	}
	else
	{
		p = (unsigned short*)getbuffer();
		p += 192 * 240;
		tgazload( JOYSTICK, 240, 128, p );
		if( pc_sound_enabled == SOUND_ON )
			show_speaker( 1 );
		else
			show_speaker( 0 );

		set_mode( UNCHANGED, JOYSTICK_EMULATION );
		input_type = JOYSTICK_EMULATION;
	}

	update( TRUE, 320 );
	
}

void cb_config_key_3( int xpos, int ypos, int xchar, int ychar )
{
	unsigned short *p;
	int i;

	if( mode == EMULATION_MODE )
	{
		set_mode( CONFIGURATION_MODE, UNCHANGED );

		p = (unsigned short*)getbuffer();
		tgazload( CONFIGURE, 240, 192, p );

		if( current_settings.machine_setting == SPECTRUM_MACHINE_48 )
			strcpy( configure[0], "TARGET   SPECTRUM 48K" );
		else if( current_settings.machine_setting == SPECTRUM_MACHINE_128 )
			strcpy( configure[0], "TARGET   SPECTRUM 128K" );
		else if( current_settings.machine_setting == SPECTRUM_MACHINE_PLUS2 )
			strcpy( configure[0], "TARGET   SPECTRUM PLUS2" );
		else if( current_settings.machine_setting == SPECTRUM_MACHINE_PLUS3 )
			strcpy( configure[0], "TARGET   SPECTRUM PLUS3" );

		if( current_settings.screen_mode_setting == screen_zoom )
			strcpy( configure[1], "SCREEN   SHRINK" );
		else if( current_settings.screen_mode_setting == screen_middle_align )
			strcpy( configure[1], "SCREEN   ALIGN MIDDLE" );
		else if( current_settings.screen_mode_setting == screen_left_align )
			strcpy( configure[1], "SCREEN   ALIGN LEFT" );
		else if( current_settings.screen_mode_setting == screen_right_align )
			strcpy( configure[1], "SCREEN   ALIGN RIGHT" );

		if( current_settings.speed_setting == speed_full )
			strcpy( configure[2], "SPEED    FULL" );
		else if( current_settings.speed_setting == speed_1_skip )
			strcpy( configure[2], "SPEED    1 FRAME SKIP" );
		else if( current_settings.speed_setting == speed_2_skip )
			strcpy( configure[2], "SPEED    2 FRAME SKIPS" );
		else if( current_settings.speed_setting == speed_3_skip )
			strcpy( configure[2], "SPEED    3 FRAME SKIPS" );
			
		if( current_settings.sound_mode_setting == sound_auto )
			strcpy( configure[3], "SOUND    AUTO" );
		else if( current_settings.sound_mode_setting == sound_max )
			strcpy( configure[3], "SOUND    MAXIMUM" );
		else if( current_settings.sound_mode_setting == sound_medium )
			strcpy( configure[3], "SOUND    MEDIUM" );
		else if( current_settings.sound_mode_setting == sound_minimum )
			strcpy( configure[3], "SOUND    MINIMUM" );
		else if( current_settings.sound_mode_setting == sound_off )
			strcpy( configure[3], "SOUND    OFF" );

		if( current_settings.joystick_setting == joystick_kempston )
			strcpy( configure[4], "JOYSTICK KEMPSTON" );
		else if( current_settings.joystick_setting == joystick_user_defined )
		{
			strcpy( configure[4], "JOYSTICK USER (" );
			strcat( configure[4], config_joystick_char );
			strcat( configure[4], ")" );
		}

		for( i=0; i<13; i++ )
			print( configure[ i ], 20, i * 12 + 24, BLACK );
	}
	else if( CONFIGURATION_MODE )	
	{
		display_refresh_all();
		set_mode( EMULATION_MODE, UNCHANGED );
	}

}

void cb_config_key_4( int xpos, int ypos, int xchar, int ychar )
{
	unsigned short *p;
	dir_element_ptr dir_element;
	int i;

	if( mode == EMULATION_MODE )	
	{
		top_dir_index = top_dir_tape_index;

		set_mode( TAPE_MODE, UNCHANGED );
		
		p = (unsigned short*)getbuffer();
		tgazload( TAPE, 240, 192, p );

		dir_tape();

		if( top_dir_index > nr_dir_elements() )
			top_dir_index = 0;

		dir_element = get_sel_dir_element( top_dir_index );

		if( dir_element != NULL )
		{	
			for( i=0; i<15; i++ )
			{
				printn( dir_element->name, 20, i * 10 + 24, BLACK, 25 );
				dir_element = get_next_dir_element();
				if( dir_element == NULL )
					break;
			}
		}
	}
	else if( mode == TAPE_MODE )
	{
		top_dir_tape_index = top_dir_index;

		display_refresh_all();
		set_mode( EMULATION_MODE, UNCHANGED );
	}
}

void cb_tape_play( int xpos, int ypos, int xchar, int ychar )
{
	tape_play();
}

void cb_tape_stop( int xpos, int ypos, int xchar, int ychar )
{
	tape_stop();
}

void cb_tape_rewind( int xpos, int ypos, int xchar, int ychar )
{
	tape_rewind();
}

void cb_mute( int xpos, int ypos, int xchar, int ychar )
{
	if( pc_sound_enabled == SOUND_ON )
	{
		pc_sound_enabled = SOUND_MUTE;
		show_speaker( 0 );
		update( true, 320 );
	}
	else if( pc_sound_enabled == SOUND_MUTE )
	{
		pc_sound_enabled = SOUND_ON;
		show_speaker( 1 );
		update( true, 320 );
	}
}

/* --- set_mode ----
 *
 * Sets the current operation mode of the entire application
 * ---
 */
void set_mode( int new_mode, int new_input )
{
	if( new_mode != UNCHANGED )
		mode = new_mode;

	if( new_input != UNCHANGED )
		input_type = new_input;

	if( mode == LOAD_MODE )
		set_area( load_areas, sizeof( load_areas ) / sizeof( load_areas[ 0 ] ) );
	else if( mode == TAPE_MODE )
		set_area( tape_areas, sizeof( tape_areas ) / sizeof( tape_areas[ 0 ] ) );
	else if( mode == CONFIGURATION_MODE )
		set_area( configuration_areas, sizeof( configuration_areas ) / sizeof( configuration_areas[ 0 ] ) );
	else if( ( mode == EMULATION_MODE ) && ( input_type == KEYBOARD_EMULATION ) )
		set_area( emul_keyboard_areas, sizeof( emul_keyboard_areas ) / sizeof( emul_keyboard_areas[ 0 ] ) );
	else if( ( mode == EMULATION_MODE ) && ( input_type == JOYSTICK_EMULATION ) )
		set_area( emul_joystick_areas, sizeof( emul_joystick_areas ) / sizeof( emul_joystick_areas[ 0 ] ) );
}

/* --- keyboard functions ----
 *
 * keyboard_down
 * keyboard_up
 * ---
 */
void keyboard_down( int xpos, int ypos )
{
	keyboard_key_name key;

	key = keyboard_loc[ ( ypos - 202 ) / 20 ][ ( xpos - 1 ) / 24 ];

	/* The special key Extended renamed End (I do not want to change any
	   FUSE code) */
	if( key == KEYBOARD_End )	
	{
		keyboard_press( KEYBOARD_Caps );
		keyboard_press( KEYBOARD_Symbol );
		last_key = KEYBOARD_End;
	}

	else if( key == KEYBOARD_Home )
	{
		keyboard_press( KEYBOARD_Caps );
		keyboard_press( KEYBOARD_0 );
		keyboard_release( KEYBOARD_Symbol );
		last_key = KEYBOARD_Home;
	}

	/* Press the key! */
	else if( key != KEYBOARD_NONE )
	{
	  keyboard_press( key );
	  last_key = key;
	}
}

void keyboard_up( void )
{
	/* Special case for Extended Mode (renamed End because
	   I do not want to change the FUSE source code)*/
	if( last_key == KEYBOARD_End )
	{
		keyboard_release( KEYBOARD_Symbol );
		keyboard_release( KEYBOARD_Caps );

		last_key = KEYBOARD_NONE;

		return;
	}

	if( last_key == KEYBOARD_Home )
	{
		keyboard_release( KEYBOARD_Caps );
		keyboard_release( KEYBOARD_0 );

		last_key = KEYBOARD_NONE;

		return;
	}

	/* Special case - Caps shift is sticky */
	if( ( last_key == KEYBOARD_Caps  ) && ( !caps_activated ) )
	{
		caps_activated = ACTIVATED;
		last_key = KEYBOARD_NONE;
	}
	else if( ( last_key == KEYBOARD_Symbol ) && ( !symb_activated ) )
	{
		symb_activated = ACTIVATED;
		last_key = KEYBOARD_NONE;
	}
	/* Release the last pressed key */
	else if( last_key != KEYBOARD_NONE )
	{
 		if( caps_activated )
		{
			caps_activated = NOT_ACTIVATED;
			keyboard_release( KEYBOARD_Caps );
		}
		if( symb_activated )
		{
			symb_activated = NOT_ACTIVATED;
			keyboard_release( KEYBOARD_Symbol );
		}

 		keyboard_release( last_key );
		last_key = KEYBOARD_NONE;
	}
}

/* --- joystick functions ----
 *
 * joystick_set
 * joystick_reset_fire
 * joystick_reset_fire
 * ---
 */
void joystick_set( BYTE value )
{
	BYTE temp;	
	
	temp = ( joystick | value ) & 0x0f;

	if( (!(temp^0x05)) || (!(temp^0x06)) || (!(temp^0x09)) || (!(temp^0x0A)) )
	{
		joystick |= value;
	}
	else if( value == 0x10 )
	{
		joystick |= value;	
	}
	else
	{
		joystick = value | ( joystick & 0x10 );
	}

	if( current_settings.joystick_setting == joystick_user_defined )
	{
		if( joystick & 0x01 )
			keyboard_press((enum keyboard_key_name)config_joystick_char[ 3 ] );
		if( joystick & 0x02 )
			keyboard_press( (enum keyboard_key_name)config_joystick_char[ 2 ] );
		if( joystick & 0x04 )
			keyboard_press( (enum keyboard_key_name)config_joystick_char[ 1 ] );
		if( joystick & 0x08 )
			keyboard_press( (enum keyboard_key_name)config_joystick_char[ 0 ] );
		if( joystick & 0x10 )
			keyboard_press( (enum keyboard_key_name)config_joystick_char[ 4 ] );	
	}
}

void joystick_reset_movement( void )
{
	if( current_settings.joystick_setting == joystick_kempston )
	{
		joystick &= 0xf0;
	}
	else
	{
		keyboard_release( (enum keyboard_key_name)config_joystick_char[ 0 ] );	
		keyboard_release( (enum keyboard_key_name)config_joystick_char[ 1 ] );	
		keyboard_release( (enum keyboard_key_name)config_joystick_char[ 2 ] );
		keyboard_release( (enum keyboard_key_name)config_joystick_char[ 3 ] );	
	}
}

void joystick_reset_fire( void )
{
	if( current_settings.joystick_setting == joystick_kempston )
	{
		joystick &= 0xef;
	}
	else
	{
		keyboard_release( (enum keyboard_key_name)config_joystick_char[ 4 ] );	
	}
}

/* --- preferences ---
 *
 * save and load settings to registry
 * ---
 */
void load_preferences( void )
{
	HKEY	hkey;
	DWORD	dwDisposition;
	DWORD	dwType, dwSize;
	DWORD	temp_machine, temp_screen, temp_sound, temp_speed, temp_joystick;
	DWORD   temp_joystick_0, temp_joystick_1, temp_joystick_2, temp_joystick_3, temp_joystick_4; 

	/* Set default values */
	current_settings.machine_setting		= 0; /* 48k */
	current_settings.screen_mode_setting	= screen_zoom;
	current_settings.sound_mode_setting		= sound_auto;
	current_settings.speed_setting			= speed_full;

	if(RegCreateKeyEx(	HKEY_LOCAL_MACHINE, TEXT("Software\\Anders Holmberg\\PocketClive"), 
						0, NULL, 0, 0, NULL, &hkey, &dwDisposition ) == ERROR_SUCCESS )
	{
		dwType = REG_DWORD;
		dwSize = sizeof(DWORD);
		if( RegQueryValueEx( hkey, TEXT("Machine"), NULL, &dwType, (PBYTE)&temp_machine, &dwSize ) == ERROR_SUCCESS )
			current_settings.machine_setting = (int)temp_machine; /* 48k */
		else
			current_settings.machine_setting = 0; /* 48k */
	
		if( RegQueryValueEx( hkey, TEXT("Screen"), NULL, &dwType, (PBYTE)&temp_screen, &dwSize ) == ERROR_SUCCESS )
			current_settings.screen_mode_setting = (enum screen_mode_t)temp_screen;
		else
			current_settings.screen_mode_setting = screen_zoom;
		
		if( RegQueryValueEx( hkey, TEXT("Sound"), NULL, &dwType, (PBYTE)&temp_sound, &dwSize ) == ERROR_SUCCESS )
			current_settings.sound_mode_setting	= (enum sound_mode_t)temp_sound;
		else
			current_settings.sound_mode_setting	= sound_auto;

		if( RegQueryValueEx( hkey, TEXT("Speed"), NULL, &dwType, (PBYTE)&temp_speed, &dwSize ) == ERROR_SUCCESS )
			current_settings.speed_setting	= (enum speed_t)temp_speed;
		else
			current_settings.speed_setting	= speed_full;

		if( RegQueryValueEx( hkey, TEXT("Joystick"), NULL, &dwType, (PBYTE)&temp_joystick, &dwSize ) == ERROR_SUCCESS )
			current_settings.joystick_setting	= (enum joystick_t)temp_joystick;
		else
			current_settings.joystick_setting	= joystick_kempston;

		if( RegQueryValueEx( hkey, TEXT("Joystick_0"), NULL, &dwType, (PBYTE)&temp_joystick_0, &dwSize ) == ERROR_SUCCESS )
			config_joystick_char[0]	= (char)temp_joystick_0;
		else
			config_joystick_char[0]	= (char)KEYBOARD_q;

		if( RegQueryValueEx( hkey, TEXT("Joystick_1"), NULL, &dwType, (PBYTE)&temp_joystick_1, &dwSize ) == ERROR_SUCCESS )
			config_joystick_char[1]	= (char)temp_joystick_1;
		else
			config_joystick_char[1]	= (char)KEYBOARD_a;

		if( RegQueryValueEx( hkey, TEXT("Joystick_2"), NULL, &dwType, (PBYTE)&temp_joystick_2, &dwSize ) == ERROR_SUCCESS )
			config_joystick_char[2]	= (char)temp_joystick_2;
		else
			config_joystick_char[2]	= (char)KEYBOARD_o;

		if( RegQueryValueEx( hkey, TEXT("Joystick_3"), NULL, &dwType, (PBYTE)&temp_joystick_3, &dwSize ) == ERROR_SUCCESS )
			config_joystick_char[3]	= (char)temp_joystick_3;
		else
			config_joystick_char[3]	= (char)KEYBOARD_p;

		if( RegQueryValueEx( hkey, TEXT("Joystick_4"), NULL, &dwType, (PBYTE)&temp_joystick_4, &dwSize ) == ERROR_SUCCESS )
			config_joystick_char[4]	= (char)temp_joystick_4;
		else
			config_joystick_char[4]	= (char)KEYBOARD_space;

		RegCloseKey(hkey);
	}

	pc_set_sound_volume( current_settings.sound_mode_setting );
	machine_select( current_settings.machine_setting );
	z80_reset();

}

void save_preferences( void )
{
	HKEY	hkey;
	DWORD	dwDisposition;
	DWORD	dwType, dwSize;
	DWORD	temp_machine, temp_screen, temp_sound, temp_speed, temp_joystick;
	DWORD   temp_joystick_0, temp_joystick_1, temp_joystick_2, temp_joystick_3, temp_joystick_4; 

	if( RegCreateKeyEx(	HKEY_LOCAL_MACHINE, TEXT("Software\\Anders Holmberg\\PocketClive"), 
						0, NULL, 0, 0, NULL, &hkey, &dwDisposition ) == ERROR_SUCCESS )
	{
		dwType = REG_DWORD;
		dwSize = sizeof(DWORD);

		temp_machine   = (DWORD)current_settings.machine_setting;
		temp_screen    = (DWORD)current_settings.screen_mode_setting;
		temp_sound     = (DWORD)current_settings.sound_mode_setting;
		temp_speed     = (DWORD)current_settings.speed_setting;
		temp_joystick  = (DWORD)current_settings.joystick_setting;

		temp_joystick_0 = (DWORD)config_joystick_char[0];
		temp_joystick_1 = (DWORD)config_joystick_char[1];
		temp_joystick_2 = (DWORD)config_joystick_char[2];
		temp_joystick_3 = (DWORD)config_joystick_char[3];
		temp_joystick_4 = (DWORD)config_joystick_char[4];
			
		RegSetValueEx( hkey, TEXT("Machine"), 0, dwType, (PBYTE)&temp_machine, dwSize);
		RegSetValueEx( hkey, TEXT("Screen"), 0, dwType, (PBYTE)&temp_screen, dwSize);
		RegSetValueEx( hkey, TEXT("Sound"), 0, dwType, (PBYTE)&temp_sound, dwSize);
		RegSetValueEx( hkey, TEXT("Speed"), 0, dwType, (PBYTE)&temp_speed, dwSize);
		RegSetValueEx( hkey, TEXT("Joystick"), 0, dwType, (PBYTE)&temp_joystick, dwSize);

		RegSetValueEx( hkey, TEXT("Joystick_0"), 0, dwType, (PBYTE)&temp_joystick_0, dwSize );
		RegSetValueEx( hkey, TEXT("Joystick_1"), 0, dwType, (PBYTE)&temp_joystick_1, dwSize );
		RegSetValueEx( hkey, TEXT("Joystick_2"), 0, dwType, (PBYTE)&temp_joystick_2, dwSize );
		RegSetValueEx( hkey, TEXT("Joystick_3"), 0, dwType, (PBYTE)&temp_joystick_3, dwSize );
		RegSetValueEx( hkey, TEXT("Joystick_4"), 0, dwType, (PBYTE)&temp_joystick_4, dwSize );

		RegCloseKey(hkey);
	}
}

/* --- preferences ---
 *
 * Pause and restart all screen updating!
 * ---
 */
void pause_emulation( void )
{
#ifndef _X86_
	paused = 1;
#endif
}

void restart_emulation( void )
{
#ifndef _X86_
	paused = 0;
	update( true, 320 );
#endif
}

void show_speaker( int activated )
{
	int xdelta, xpos, ypos;

	if( activated )
	{
		xdelta = 14;
	}
	else
	{
		xdelta = 0;
	}

	for( xpos = 0; xpos < 14; xpos++ )
	{
		for( ypos = 0; ypos < 12; ypos ++ )
		{
			plot( xpos + 225, ypos + 302, image_buffer[ xpos + ypos * 128 + xdelta ] );
		}
	}

}

void show_tape_player( int activated )
{
	int ydelta, xpos, ypos;

	if( activated )
	{
		ydelta = 12;
	}
	else
	{
		ydelta = 28;
	}

	for( xpos = 0; xpos < 48; xpos++ )
	{
		for( ypos = 0; ypos < 15; ypos ++ )
		{
			plot( xpos + 166, ypos + 301, image_buffer[ xpos + ( ypos + ydelta ) * 128 ] );
		}
	}

}

void update_keyboard_image( void )
{
	if( pc_sound_enabled == SOUND_ON )
	{
		show_speaker( 1 );
	}
	else
	{
		show_speaker( 0 );
	}

	if( settings_current.tape_traps )
	{
		show_tape_player( 0 );
	}
	else
	{
		show_tape_player( 1 );
	}

	update( true, 320 );
}
