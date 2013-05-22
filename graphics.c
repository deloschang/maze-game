/* 
* Filename: graphics.c
* This file will render the maze graphically given the 2D int array
*/

#include <stdlib.h>                          // for calloc
#include <string.h>                          // for strlen
#include <gtk/gtk.h>                         // for gtk functionality

//! Zero out the 2D array
#define BZEROARRAY(array,n,m)  memset(array, 0, sizeof(array[0][0]) * m * n)

#define WINDOW_WIDTH 600  
#define WINDOW_HEIGHT 600  

#define LEVEL_0_ROWS 9  // (2n+1) adjustment 
#define LEVEL_0_COLUMNS 9

#define MAP_WIDTH  50
#define MAP_HEIGHT 50

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
  printf("sent");
  return array[chooseRow][chooseColumn];

}

static gboolean cb_expose (GtkWidget *area, GdkEventExpose *event, gpointer *data){
  int       i, j, width, height, maze_width, maze_column;
  double    value, step_x, step_y;
  char      field;
  cairo_t  *cr;

  cr = gdk_cairo_create (event->window);
  width = gdk_window_get_width (event->window);
  height = gdk_window_get_height (event->window);

  // Localize the data
  maze_width = ((matrix*)data)->row;
  maze_column = ((matrix*)data)->column;

  step_x = (double)width / maze_width;
  step_y = (double)height / maze_column;

  /** EXAMPLE **/
  // Print the 2D Array onto the console to check
  /*render_2D_array(((matrix*)data)->row, ((matrix*)data)->column, ((matrix*)data)->matrix);*/
  /** EXAMPLE **/

  // Iterate through the matrix
  for (i = 0; i < maze_width; i++){
    for (j = 0; j < maze_column; j++){
      // Setting color
      /*value = g_random_double ();*/
      /*cairo_set_source_rgb (cr, value, value, value);*/

      // Parse the 2D Array 
      /*printf("matrix[%c][%c]: %c", i, j, ((matrix*)data)->matrix[i][j]);*/
      /*if (((matrix*)data)->matrix[i][j] == '1'){*/
      field = retrieve_field(maze_width, maze_column, ((matrix*)data)->matrix, i, j);
      if ( field == '1'){
        cairo_move_to(cr, i * step_x, j * step_y);
        cairo_line_to(cr, i * step_x, (j * step_y) + step_y);
      } else if ( field == '_'){
        cairo_move_to(cr, i * step_x, j * step_y);
        cairo_line_to(cr, (i * step_x) + step_x, j * step_y);
      }
      /*}*/

      /*cairo_rectangle (cr, i * step_x, j * step_y, step_x, step_y);*/
      /*cairo_fill (cr);*/
      cairo_stroke(cr);
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

  g_signal_connect (area, "expose-event", G_CALLBACK (cb_expose), data);
  gtk_box_pack_start (GTK_BOX (vbox), area, TRUE, TRUE, 0);

  /* Connect refresh button (we need pointer to drawing area, this is
   * why connection is delayed until here). */
  /*g_signal_connect_swapped (button, "clicked",*/
                            /*G_CALLBACK (gtk_widget_queue_draw), area);*/
  gtk_widget_show_all (main_window);

  /*gtk_main ();*/


  /* TESTING WITH LEVEL 0 ARRAY */
  /*render_2D_array(LEVEL_0_ROWS, LEVEL_0_COLUMNS, A);*/
  /* END TESTING LEVEL 0 ARRAY */

  gtk_main();

  // Clean up 
  free(data);
  return 0;
}


