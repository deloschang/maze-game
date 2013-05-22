/* 
* Filename: graphics.c
* This file will render the maze graphically given the 2D int array
*/

#include <stdlib.h>                          // for calloc
#include <string.h>                          // for strlen
#include <gtk/gtk.h>                         // for gtk functionality

//! Zero out the 2D array
#define BZEROARRAY(array,n,m)  memset(array, 0, sizeof(array[0][0]) * m * n)
#define convertBack(n)  (n+1)/2
#define convertForward(n)  (2*n)+1

#define WINDOW_WIDTH 600  
#define WINDOW_HEIGHT 600  

#define LEVEL_0_ROWS 9  // (2n+1) adjustment 
#define LEVEL_0_COLUMNS 9

/* --- Structures / Types --- */
// Example struct to pass into the graphics function.
typedef struct _matrix {
  int row;
  int column;
  char** matrix;
} matrix;

  static char A[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {
    { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
    { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,
    { 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,
    { '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,
    { 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,
    { '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,
    { 'E', '0', 'E', '1', 'E', '0', 'E', '0', 'E' } ,
    { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,
    { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
  };


/**
 * Takes the 2D Array and prints it onto the terminal
 */
void render_2D_array(int row, int column, char array[row][column]){
  // Print the level 0 array
  for (int i = 0; i < row; i++){
    for (int j = 0; j < column; j++){
      printf("%c ", array[i][j]);
    }
    printf("\n");
  }
}

/**
 *  Retrieves the char at the given row and column 
 */
char retrieve_field(int row, int column, char array[row][column], 
  int chooseRow, int chooseColumn){
  return array[chooseRow][chooseColumn];
}

static gboolean cb_expose (GtkWidget *area, GdkEventExpose *event, gpointer *data){
  int       i, j, width, height, maze_width, maze_column, convert_step_x, convert_step_y, x, y, flag;
  double    value, step_x, step_y;
  char      field;
  cairo_t  *cr;

  cr = gdk_cairo_create (event->window);
  width = gdk_window_get_width (event->window);
  height = gdk_window_get_height (event->window);

  // Localize the data
  maze_width = ((matrix*)data)->row;
  maze_column = ((matrix*)data)->column;

  // converting to 2D array width / height (e.g. Level 0 would be 9 rows)
  step_x = (double)width / maze_width;
  step_y = (double)height / maze_column;

  // converting back to maze width / height (e.g. Level 0 maze would be 5 rows)
  convert_step_x = (double)width / (convertBack(maze_width));
  convert_step_y = (double)height / (convertBack(maze_column));

  /** EXAMPLE **/
  // Print the 2D Array onto the console to check
  /*render_2D_array(((matrix*)data)->row, ((matrix*)data)->column, ((matrix*)data)->matrix);*/
  /** EXAMPLE **/

  x = 0;    // x coordinate to draw at
  y = 0 ;   // y coordinate to draw at

  flag = 0; // checks if it is an odd or even row in 2D array

  // Iterate through the matrix
  for (i = 0; i < maze_width; i++){
    for (j = 0; j < maze_column; j++){
      // Setting color
      /*value = g_random_double ();*/
      /*cairo_set_source_rgb (cr, value, value, value);*/

      // Parse the 2D Array 
      field = retrieve_field(maze_width, maze_column, ((matrix*)data)->matrix, i, j);
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
      }

      /*cairo_rectangle (cr, i * step_x, j * step_y, step_x, step_y);*/
      /*cairo_fill (cr);*/
      cairo_stroke(cr);
    }

    // reset the x back to 0  (start a new row)
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
 * Function that will take in a 2D Array and update the graphics
 * Refactoring required
 **/
int main(int argc,  char **argv){
  GtkWidget *main_window,
            *vbox,
            /**button,*/
            *area;

  gtk_init (&argc, &argv);

  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (main_window), 600, 600);
  g_signal_connect (main_window, "destroy", gtk_main_quit, NULL);

  vbox = gtk_vbox_new (FALSE, 5);
  gtk_container_add (GTK_CONTAINER (main_window), vbox);

  /*button = gtk_button_new_from_stock (GTK_STOCK_REFRESH);*/
  /*gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);*/

  area = gtk_drawing_area_new ();

  /* Start drawing the 2D array here */
  // Example use of the Matrix Structure to pass into g_signal_connect
  matrix *data = malloc(sizeof(matrix));
  data->row = LEVEL_0_ROWS;
  data->column = LEVEL_0_COLUMNS;
  data->matrix = (char**)A;

  /* END SAMPLE */

  g_signal_connect (area, "expose-event", G_CALLBACK (cb_expose), data);
  gtk_box_pack_start (GTK_BOX (vbox), area, TRUE, TRUE, 0);

  /* Connect refresh button (we need pointer to drawing area, this is
   * why connection is delayed until here). */
  /*g_signal_connect_swapped (button, "clicked",*/
                            /*G_CALLBACK (gtk_widget_queue_draw), area);*/
  gtk_widget_show_all (main_window);


  gtk_main();

  // Clean up 
  free(data);
  return 0;
}


