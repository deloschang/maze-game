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

// Use dynamically allocated array of pointers to dynamically
// allocated arrays because the array bounds are not yet known 
// during compilation
/*void func(int** array, int rows, int cols){*/
  /*int i, j;*/

  /*for (i = 0; i<rows; i++){*/
    /*for (j = 0; j<cols; j++){*/
      /*array[i][j] = i*j;*/
    /*}*/
  /*}*/
/*}*/

// Takes a 2D array and renders it graphically
void render_2D_array(int row, int column, char array[row][column]){
  // Print the level 0 array
  for (int i = 0; i < row; i++){
    for (int j = 0; j < column; j++){
      printf("%c ", array[i][j]);
    }
    printf("\n");
  }

}

static gboolean cb_expose (GtkWidget *area, GdkEventExpose *event, gpointer *data){
  int       i, j, width, height;
  double    value, step_x, step_y;
  cairo_t  *cr;

  cr = gdk_cairo_create (event->window);
  width = gdk_window_get_width (event->window);
  height = gdk_window_get_height (event->window);

  step_x = (double)width / MAP_WIDTH;
  step_y = (double)height / MAP_HEIGHT;

  // Iterate through the matrix
  for (i = 0; i < MAP_WIDTH; i++){
    for (j = 0; j < MAP_HEIGHT; j++){
      value = g_random_double ();
      cairo_set_source_rgb (cr, value, value, value);

      cairo_move_to(cr, i * step_x, j * step_y);
      cairo_line_to(cr, i * step_x, (j * step_y) + step_y);

      /*cairo_rectangle (cr, i * step_x, j * step_y, step_x, step_y);*/
      /*cairo_fill (cr);*/
      cairo_stroke(cr);
    }
  }

  cairo_destroy (cr);

  return TRUE;
}

int main(int argc,  char **argv){
  GtkWidget *main_window,
            *vbox,
            *button,
            *area;

  gtk_init (&argc, &argv);

  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (main_window), 600, 600);
  g_signal_connect (main_window, "destroy", gtk_main_quit, NULL);

  vbox = gtk_vbox_new (FALSE, 5);
  gtk_container_add (GTK_CONTAINER (main_window), vbox);

  button = gtk_button_new_from_stock (GTK_STOCK_REFRESH);
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

  area = gtk_drawing_area_new ();
  g_signal_connect (area, "expose-event", G_CALLBACK (cb_expose), NULL);
  gtk_box_pack_start (GTK_BOX (vbox), area, TRUE, TRUE, 0);

  /* Connect refresh button (we need pointer to drawing area, this is
   * why connection is delayed until here). */
  g_signal_connect_swapped (button, "clicked",
                            G_CALLBACK (gtk_widget_queue_draw), area);

  gtk_widget_show_all (main_window);

  gtk_main ();


  /* TESTING WITH LEVEL 0 ARRAY */
  static char A[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {
    { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
    { '0', 'Z', '0', 'Z', '0', 'Z', '1', 'Z', '0' } ,
    { 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,
    { '1', 'Z', '1', 'Z', '0', 'Z', '0', 'Z', '1' } ,
    { 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,
    { '0', 'Z', '1', 'Z', '1', 'Z', '1', 'Z', '0' } ,
    { 'E', '0', 'E', '1', 'E', '0', 'E', '0', 'E' } ,
    { '0', 'Z', '0', 'Z', '0', 'Z', '1', 'Z', '1' } ,
    { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
  };

  // Print the level 0 array
  render_2D_array(LEVEL_0_ROWS, LEVEL_0_COLUMNS, A);

  gtk_main ();
  /*for (i = 0; i < LEVEL_0_ROWS; i++){*/
    /*for (int j = 0; j < LEVEL_0_COLUMNS; j++){*/
      /*printf("%c ", A[i][j]);*/
    /*}*/
    /*printf("\n");*/
  /*}*/

  // Send array to graphics module to render


  /* DYNAMIC MEMORY ALLOCATION FOR THE 2D ARRAY */
  // Allocate memory for the the array 
  /*x = malloc(rows * sizeof *x);*/
  /*for (i = 0; i<rows; i++){*/
    /*x[i] = malloc(cols * sizeof *x[i]);*/
  /*}*/

  // Use the array 
  /*func(x, rows, cols);*/


  // Deallocate the array for cleanup
  /*for (i=0; i<rows; i++) {*/
    /*free(x[i]);*/
  /*}*/

  /*free(x);*/
  return 0;
}


