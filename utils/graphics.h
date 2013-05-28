#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

// Filename: graphics.h
// Description: General macros and definitions for the graphics rendering program
// This includes DEFINES constants and common macros
//

#define UPDATE_INTERVAL 1  // how often graphics checks for map
#define WINDOW_WIDTH 600  
#define WINDOW_HEIGHT 600  
#define convertBack(n)  (n+1)/2

// Prototypes

static gboolean cb_expose (GtkWidget *area, GdkEventExpose *event, gpointer *data);
gboolean timer_tic(gpointer data);
void render_maze();

#endif
