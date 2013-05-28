/*
 * Filename: graphics.c
 * Description: This file will render the maze graphically given the 2D char array
 * Pseudo Code: 
 *   Set up the maze via render_maze(). Every UPDATE_INTERVAL (1 sec by default), check the 2D char array again
 *   Then redraw the canvas onto the graphical update.
 */

#include <stdlib.h> // for calloc
#include <string.h> // for strlen
#include <gtk/gtk.h> // for gtk functionality
#include "amazing.h"
#include "amazing_client.h" // for gtk functionality
#include "graphics.h"


// Global shared 2D char array used by all the avatars.
// Everytime that this is updated, the cb_expose() function's timer
// will try to redraw it every second. 
// Thus, there will be no signal from the avatar to the drawing window
char data2[DIM][DIM];
int data_row_length =0;
int data_column_length =0;


// Sample Level 0 Maze 
/*static char Array[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {*/
/*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
/*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,*/
/*{ 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,*/
/*{ '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,*/
/*{ 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,*/
/*{ '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,*/
/*{ 'E', '0', 'E', '1', 'E', '0', 'E', '0', 'E' } ,*/
/*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,*/
/*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
/*};*/

// Shared map area between all the avatars
GtkWidget *main_window,
          *vbox,
          *area;

/**
 * cb_expose
 * Main function that will render the drawing canvas. 
 * Once this is set up, it will create a timer that redraws the maze every UPDATE_INTERVAL
 **/
static gboolean cb_expose (GtkWidget *area, GdkEventExpose *event, gpointer *data){
    int i, j, width, height, maze_width, maze_column, x, y, flag;
    double convert_step_x, convert_step_y;
    char field;
    cairo_t *cr;

    cr = gdk_cairo_create (event->window);
    width = gdk_window_get_width (event->window);
    height = gdk_window_get_height (event->window);


    // Localize the data
    /*maze_width = ((matrix*)data)->row;*/
    /*maze_column = ((matrix*)data)->column;*/

    maze_width = data_row_length;
    maze_column = data_column_length;

    // converting back to maze width / height (e.g. Level 0 maze would be 5 rows)
    convert_step_x = (double)width / (convertBack(maze_width));
    convert_step_y = (double)height / (convertBack(maze_column));

    x = 0; // x coordinate to draw at
    y = 0 ; // y coordinate to draw at

    flag = 0; // checks if it is an odd or even row in 2D array

    // Iterate through the matrix
    for (i = 0; i < maze_width; i++){
        for (j = 0; j < maze_column; j++){

            // Setting color
            /*value = g_random_double ();*/
            cairo_set_source_rgb (cr, 0.5, 0.5, 1);

            // Parse the 2D Array
            field = data2[i][j];

            if ( field == 'E'){
                x += convert_step_x;

            } else if ( field == 'Z'){
                x += convert_step_x;

            } else if ( field == '1'){
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, x, y + convert_step_y);

            } else if ( field == '_'){
                // Check to see if it is an odd or even row
                if ( flag == 0){
                    // Odd Row
                    cairo_move_to(cr, x, y + convert_step_y); // start from bottom
                    cairo_line_to(cr, x + convert_step_x, y + convert_step_y);
                } else {
                    // Even Row
                    cairo_move_to(cr, x, y); // start from top
                    cairo_line_to(cr, x + convert_step_x, y);
                }
            } else if ( field == 'A'){
                // Draw the avatar
                cairo_set_source_rgb (cr, 0, 0, 0);
                cairo_rectangle (cr, x + (convert_step_x / 4), 
                        y + (convert_step_y / 4), 
                        convert_step_x - (convert_step_x / 3), 
                        convert_step_y - (convert_step_y / 3));

                cairo_fill (cr);
                cairo_set_source_rgb (cr, 0.5, 0.5, 1);

                x += convert_step_x;
            }

            cairo_stroke(cr);
        }

        // reset the x back to 0 (start a new row)
        // Depending if odd or even row, horizontal walls are drawn differently
        if ( flag == 0 ){
            x = 0;
            y += convert_step_y;
            flag = 1;
        } else {
            x = 0;
            // skip moving y
            flag = 0;
        }

    }
    cairo_destroy (cr);

    return TRUE;
}


/**
 * timer_tic
 * redraws the canvas area every UPDATE_INTERVAL
 **/
gboolean timer_tic(gpointer data){
    gtk_widget_queue_draw(area);
    return TRUE;
}

/**
 * render_maze
 * Sets up the basic GTK canvas
 **/
void render_maze(){
    // Create the main graphical window with the specified dimensions
    main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (main_window), WINDOW_WIDTH, WINDOW_HEIGHT);
    g_signal_connect (main_window, "destroy", gtk_main_quit, NULL);

    vbox = gtk_vbox_new (FALSE, 5);
    gtk_container_add (GTK_CONTAINER (main_window), vbox);

    area = gtk_drawing_area_new ();

    // Main cb_expose function that will start when the graphics is rendered.
    // cb_expose function will call a timer that will redraw the canvas every 1 second
    g_signal_connect (area, "expose-event", G_CALLBACK (cb_expose), NULL);
    gtk_box_pack_start (GTK_BOX (vbox), area, TRUE, TRUE, 0);


    // start a timeout with interval
    gint timer = g_timeout_add_seconds(UPDATE_INTERVAL, timer_tic, NULL);

    gtk_widget_show_all (main_window);

    gtk_main();

    // Clean up
    g_source_remove(timer);
}
