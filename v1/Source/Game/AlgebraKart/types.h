#pragma once

#include <future>
#include <unordered_map>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include <limits.h>
#include <ctype.h>
#include <stdint.h>
#include <Urho3D/Math/Vector3.h>


/// The number of time units per second of the game clock.
#define GAME_TICKS_PER_SEC 1000
/// The number of game state updates per second of the game clock.
#define GAME_UPDATES_PER_SEC 10
/// The number of time units per game tick.
#define GAME_TICKS_PER_UPDATE (GAME_TICKS_PER_SEC/GAME_UPDATES_PER_SEC)


static uint32_t selectedPlayer = 0;  /**< Current player */

/* Basic numeric types */
typedef uint8_t  UBYTE;
typedef int8_t   SBYTE;
typedef uint16_t UWORD;
typedef int16_t  SWORD;
typedef uint32_t UDWORD;
typedef int32_t  SDWORD;

/* Numeric size defines */
#define UBYTE_MAX	UINT8_MAX
#define SBYTE_MIN	INT8_MIN
#define SBYTE_MAX	INT8_MAX
#define UWORD_MAX	UINT16_MAX
#define SWORD_MIN	INT16_MIN
#define SWORD_MAX	INT16_MAX
#define UDWORD_MAX	UINT32_MAX
#define SDWORD_MIN	INT32_MIN
#define SDWORD_MAX	INT32_MAX

const float CAMERA_DISTANCE = 300.0f;
const float CLIENT_CAMERA_DISTANCE = 58.0f;

#define MAX_PLAYERS         11                 ///< Maximum number of players in the game.

#if MAX_PLAYERS <= 8
typedef uint8_t PlayerMask;
#elif MAX_PLAYERS <= 16
typedef uint16_t PlayerMask;
#endif

const int CTRL_UP = 1;
const int CTRL_DOWN = 2;
const int CTRL_LEFT = 4;
const int CTRL_RIGHT = 8;

const int CTRL_SPACE = 16;
const int CTRL_Q = 32;
const int CTRL_W = 64;
const int CTRL_E = 128;
const int CTRL_R = 256;

const int CTRL_FORWARD = 1;
const int CTRL_BACK = 2;


const float mapSize = 3000.0f;
const float miniMapWidth = 256.0f;
const float miniMapHeight = 256.0f;
const float WaitTimeNextFire = 0.2f;

/***************************************************************************/
/*
 *	Global Definitions (CONSTANTS)
 */
/***************************************************************************/

#define LONG_WAY			(1<<15)
#define BUTTON_DEPTH		2000 // will be stretched to 16000

#define OLD_TEXTURE_SIZE_FIX 256.0f

//Render style flags for all pie draw functions
#define pie_ECM                 0x1
#define pie_TRANSLUCENT         0x2
#define pie_ADDITIVE            0x4
#define pie_FORCE_FOG           0x8
#define pie_HEIGHT_SCALED       0x10
#define pie_RAISE               0x20
#define pie_BUTTON              0x40
#define pie_SHADOW              0x80
#define pie_STATIC_SHADOW       0x100
#define pie_PREMULTIPLIED       0x200

#define pie_RAISE_SCALE			256



/*#define BASE_COORDS_X	(640)
#define BASE_COORDS_Y	(460)
#define E_W (pie_GetVideoBufferWidth() - BASE_COORDS_X)
#define E_H (pie_GetVideoBufferHeight() - BASE_COORDS_Y)
#define D_W	((pie_GetVideoBufferWidth() - BASE_COORDS_X)/2)
#define D_H ((pie_GetVideoBufferHeight() - BASE_COORDS_Y)/2)
*/
// Reticule position.
#define E_H                  0
#define RET_X				6
#define RET_Y				(324+E_H)
#define RET_FORMWIDTH		132
#define RET_FORMHEIGHT		132


enum LIGHTING_TYPE
{
    LIGHT_EMISSIVE,
    LIGHT_AMBIENT,
    LIGHT_DIFFUSE,
    LIGHT_SPECULAR,
    LIGHT_MAX
};

enum REND_MODE
{
    REND_ALPHA,
    REND_ADDITIVE,
    REND_OPAQUE,
    REND_MULTIPLICATIVE,
    REND_PREMULTIPLIED,
    REND_TEXT,
};

enum DEPTH_MODE
{
    DEPTH_CMP_LEQ_WRT_ON,
    DEPTH_CMP_ALWAYS_WRT_ON,
    DEPTH_CMP_ALWAYS_WRT_OFF
};

enum TEXPAGE_TYPE
{
    TEXPAGE_NONE = -1,
    TEXPAGE_EXTERN = -2
};

enum SHADER_MODE
{
    SHADER_NONE,
    SHADER_COMPONENT,
    SHADER_BUTTON,
    SHADER_NOLIGHT,
    SHADER_TERRAIN,
    SHADER_TERRAIN_DEPTH,
    SHADER_DECALS,
    SHADER_WATER,
    SHADER_RECT,
    SHADER_TEXRECT,
    SHADER_GFX_COLOUR,
    SHADER_GFX_TEXT,
    SHADER_GENERIC_COLOR,
    SHADER_LINE,
    SHADER_TEXT,
    SHADER_MAX
};

//*************************************************************************
//
// Simple derived types
//
//*************************************************************************

struct iV_Image
{
    unsigned int width, height, depth;
    unsigned char *bmp;
};

struct PIELIGHTBYTES
{
    uint8_t r, g, b, a;
};

/** Our basic colour type. Use whenever you want to define a colour.
 *  Set bytes separetely, and do not assume a byte order between the components. */
union PIELIGHT
{
    PIELIGHTBYTES byte;
    UDWORD rgba;
    UBYTE vector[4];
};

#define CLIP(val, min, max) do                                                \
	{                                                                             \
		if ((val) < (min)) (val) = (min);                                         \
		else if ((val) > (max)) (val) = (max);                                    \
	} while(0)


// the size of the file loading buffer
#define FILE_LOAD_BUFFER_SIZE (1024*1024*4)

// these constants are adapted for fitting 256 textures of size 128x128 into a 2048x2048
// texture page; if such large texture pages are not available, just scaled everything down
#define TILES_IN_PAGE_COLUMN	16
#define TILES_IN_PAGE_ROW	16
#define TILES_IN_PAGE		(TILES_IN_PAGE_COLUMN * TILES_IN_PAGE_ROW)
#define MAX_TILES		TILES_IN_PAGE