/* 	amazing.h cs50 project master include file

      Copyright 2012

      Project name:   amazing
      Component name: amazing.h

      This file contains constants, macros, function prototypes, etc., 
      for the amazing project.
	
      Primary Author:	C. C. Palmer
      Date Created:	   02Jan2012
	
      ======================================================================*/
/* do not remove any of these sections, even if they are empty */

#ifndef amazing_H
#define amazing_H

/* ---------------- Prerequisites e.g., Requires "math.h"  */

/* ---------------- Constants and limits */
/* Server parameters */
#define AM_SERVER_PORT   17235      /* ( ASCII "CS" ) Server management port */
#define AM_MAX_MESSAGE     128      /* max message length in bytes */
#define AM_MAX_AVATAR       10      /* max number of avatars for any given MazePort */
#define AM_MAX_MOVES      2000      /* maximum # of moves for any given MazePort (sum across all avatars) */
#define AM_WAIT_TIME       600      /* how long in seconds the server will wait for a message before giving up. */

/* Avatar constants */
#define M_WEST  0
#define M_NORTH 1
#define M_SOUTH 2
#define M_EAST  3
#define M_NUM_DIRECTIONS 4
#define M_NULL_MOVE 8
    
/*
 * client-server protocol message type values
 */
#define AM_INITIALIZE              0x00000001
#define AM_INITIALIZE_OK           0x00000002
#define AM_INITIALIZE_FAILED      (0x00000004 | AM_ERROR_MASK)
#define AM_AVATAR_READY            0x00000008
#define AM_AVATAR_TURN             0x00000010
#define AM_AVATAR_MOVE             0x00000020
#define AM_NO_SUCH_AVATARID       (0x00000040 | AM_ERROR_MASK)
#define AM_TOO_MANY_MOVES         (0x00000100 | AM_ERROR_MASK)
#define AM_MAZE_SOLVED             0x00010000
#define AM_UNKNOWN_MSG_TYPE       (0x01000000 | AM_ERROR_MASK)
#define AM_SERVER_TIMEOUT         (0x02000000 | AM_ERROR_MASK)
#define AM_UNEXPECTED_MSG_TYPE    (0x04000000 | AM_ERROR_MASK)

#define AM_ERROR_MASK              0x80000000

#define IS_AM_ERROR(code)  ((code) & AM_ERROR_MASK)

/* AM_INITIALIZE failure errnums */
#define AM_INIT_ERROR_MASK        0xC0000000
#define AM_INIT_TOO_MANY_AVATARS  0xC0000001
#define AM_INIT_BAD_DIFFICULTY    0xC0000002

/* ---------------- Structures/Types */
typedef struct _xypos 
{
     unsigned int xPos;
     unsigned int yPos;
} XYPOS;

typedef struct _avatar
{
    int fd;
    XYPOS pos;
} avatar;
     
/* general AM Message description */
typedef struct _AM_Message 
{
     unsigned int        message_type;

     /* define a union for all the message types that have parameters.
      * messages that are just a message_type with no parameters 
      * don't need to be part of this union. */
     union _ma_prot  
     {
          struct _initialize 
          {
               unsigned int  nAvatars;
               unsigned int  Difficulty;
          } initialize;
          
          struct _initialize_ok
          {
               unsigned int  MazePort;
               unsigned int  MazeWidth;   /* ccp 26Feb12 */
               unsigned int  MazeHeight;  /* ccp 26Feb12 */
          } initialize_ok;
          
          struct _initialize_failed 
          {
               unsigned int  errnum;
          } initialize_failed;
          
          struct _avatar_ready
          {
               unsigned int  AvatarId;
          } avatar_ready;
          
          struct _avatar_turn
          {
               unsigned int  TurnId;
               XYPOS         Pos[AM_MAX_AVATAR];
          } avatar_turn;
          
          struct _avatar_move
          {
               unsigned int  AvatarId;
               unsigned int  Direction;
          } avatar_move;
          
          struct _maze_solved
          {
               unsigned int  Difficulty;
               unsigned int  nAvatars;
               unsigned int  nMoves;
               unsigned int  Hash;  /* to hold a secret hash receipt */
          } maze_solved;

          struct _unknown_message_type 
          {
               unsigned int  badType;
          } unknown_message_type;
          
     } msg;

} AM_MESSAGE;

/* ---------------- Public Variables */

/* ---------------- Prototypes/Macros */

#endif /* amazing_H */

