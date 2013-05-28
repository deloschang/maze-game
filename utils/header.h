#ifndef _HEADER_H_
#define _HEADER_H_

// Filename: header.h
// Description: General macros and definitions for the maze-solving algorithm 
// This includes DEFINES constants and common macros
//

// DEFINES
#define min(x,y)   ((x)>(y))?(y):(x)

//! Zero out the 2D array
#define BZEROARRAY(array,n,m)  memset(array, 0, sizeof(array[0][0]) * m * n)

//! Print arg together with the source file name and the current line number.
#define LOG(s)  printf("[%s:%d]%s\n", __FILE__, __LINE__, s)

//! Check whether arg is NULL or not. Quit this program if it is NULL.
#define MYASSERT(s)  if (!(s))   {                                      \
    printf("General Assert Error at %s:line%d\n", __FILE__, __LINE__);  \
    exit(-1); \
  }

//! Check whether arg is NULL or not on a memory allocation. Quit this program if it is NULL.
#define MALLOC_CHECK(s)  if ((s) == NULL)   {                     \
    printf("No enough memory at %s:line%d ", __FILE__, __LINE__); \
    perror(":");                                                  \
    exit(-1); \
  }

//! Set memory space starts at pointer \a n of size \a m to zero. 
#define BZERO(n,m)  memset(n, 0, m)

#endif
