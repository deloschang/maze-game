/*
* Filename: graphics.c
* This file will render the maze graphically given the 2D int array
*/

#include <stdlib.h> // for calloc
#include <string.h> // for strlen
#include <gtk/gtk.h> // for gtk functionality
#include "amazing.h"
#include "amazing_client.h" // for gtk functionality
#include "graphics.h"

  // Global map struct should go here.
matrix* data;
  // Call render_maze() once and it will rerender the global
  // map struct (data) every 1 sec.
  static char Array[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {
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
  // Shared map (struct) between all the avatars
  GtkWidget *main_window,
            *vbox,
            *area;
    /**button,*/

  /******************************************/
  // FOR DEMOING AVATAR MOVEMENT TESTING-- REMOVE FOR DEPLOYMENT --
  // Example 2D array with avatar
  /*static char A_WITH_AVATAR[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {*/
    /*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,*/
    /*{ 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,*/
    /*{ '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,*/
    /*{ 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,*/
    /*{ 'E', '0', 'E', '1', 'A', '0', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,*/
    /*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
  /*};*/

  /*// Another simulation of avatar moving*/
  /*static char A_WITH_AVATAR_TWO[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {*/
    /*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,*/
    /*{ 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,*/
    /*{ '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,*/
    /*{ 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,*/
    /*{ 'E', '0', 'E', '1', 'E', '0', 'A', '0', 'E' } ,*/
    /*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,*/
    /*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
  /*};*/

  /*// Another simulation of avatar moving*/
  /*static char A_WITH_AVATAR_THREE[LEVEL_0_ROWS][LEVEL_0_COLUMNS] = {*/
    /*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,*/
    /*{ 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,*/
    /*{ '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,*/
    /*{ 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,*/
    /*{ '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,*/
    /*{ 'E', '0', 'E', '1', 'E', '0', 'E', '0', 'A' } ,*/
    /*{ '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,*/
    /*{ 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,*/
  /*};*/
  /*********************END*********************/


// Simulation Flag
int flag = 0; // used to simulate avatar moving in sep thread

/**
* Takes the 2D Array and prints it onto the terminal
*/
/*void render_2D_array(int row, int column, char array[row][column]){*/
  /*// Print the level 0 array*/
  /*for (int i = 0; i < row; i++){*/
    /*for (int j = 0; j < column; j++){*/
      /*printf("%c ", array[i][j]);*/
    /*}*/
    /*printf("\n");*/
  /*}*/
/*}*/




static gboolean cb_expose (GtkWidget *area, GdkEventExpose *event, gpointer *data){
  int i, j, width, height, maze_width, maze_column, x, y, flag;
  double value, convert_step_x, convert_step_y;
  char field;
  cairo_t *cr;

  cr = gdk_cairo_create (event->window);
  width = gdk_window_get_width (event->window);
  height = gdk_window_get_height (event->window);


  // Localize the data
  maze_width = ((matrix*)data)->row;
  maze_column = ((matrix*)data)->column;
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
      /*cairo_set_source_rgb (cr, value, value, value);*/

      // Parse the 2D Array
      field = ((matrix*)data)->matrix[i][j];
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
        // Temporary rectangle / change later

        cairo_rectangle (cr, x, y, convert_step_x, convert_step_y);
        cairo_fill (cr);

        x += convert_step_x;
      }

      /*cairo_rectangle (cr, i * step_x, j * step_y, step_x, step_y);*/
      /*cairo_fill (cr);*/
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

// 0 flag for new
// 1 flag for update
void render_maze(){
  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (main_window), WINDOW_WIDTH, WINDOW_HEIGHT);
  g_signal_connect (main_window, "destroy", gtk_main_quit, NULL);

  vbox = gtk_vbox_new (FALSE, 5);
  gtk_container_add (GTK_CONTAINER (main_window), vbox);

  /*button = gtk_button_new_from_stock (GTK_STOCK_REFRESH);*/
  /*gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);*/

  area = gtk_drawing_area_new ();

  g_signal_connect (area, "expose-event", G_CALLBACK (cb_expose), data);
  gtk_box_pack_start (GTK_BOX (vbox), area, TRUE, TRUE, 0);

  /* Connect refresh button (we need pointer to drawing area, this is
* why connection is delayed until here). */
  /*g_signal_connect_swapped (button, "clicked",*/
                            /*G_CALLBACK (gtk_widget_queue_draw), area);*/

  // start a timeout with interval
  gint timer = g_timeout_add_seconds(UPDATE_INTERVAL, timer_tic, NULL);

  gtk_widget_show_all (main_window);

  gtk_main();

  // Clean up
  g_source_remove(timer);
  free(data);
}

// Simulates the moving avatars
/*void* print_i(void *ptr){*/
  /*while (1){*/
    /*if (flag == 0){*/
      /*data->row = LEVEL_0_ROWS;*/
      /*data->column = LEVEL_0_COLUMNS;*/
      /*data->matrix = (char**)A_WITH_AVATAR;*/

      /*flag = 1;*/
    /*} else if ( flag == 1 ){*/
      /*data->row = LEVEL_0_ROWS;*/
      /*data->column = LEVEL_0_COLUMNS;*/
      /*data->matrix = (char**)A_WITH_AVATAR_TWO;*/

      /*flag = 2;*/
    /*} else if (flag == 2){*/
      /*data->row = LEVEL_0_ROWS;*/
      /*data->column = LEVEL_0_COLUMNS;*/
      /*data->matrix = (char**)A_WITH_AVATAR_THREE;*/


      /*flag = 0;*/
    /*}*/

  /*}*/
/*}*/

/**
* Function that will take in a 2D Array and update the graphics
**/
int main(int argc, char **argv){
   /*[>(1) Init the GTK with arguments<]*/
  gtk_init (&argc, &argv);

   /*[>Example use of the Matrix Structure<]*/
  data = malloc(sizeof(matrix));
  data->row = LEVEL_0_ROWS;
  data->column = LEVEL_0_COLUMNS;
  /*data->matrix = (char**)A;*/

  /*data->matrix = A;*/
    
    matrix* A = malloc(sizeof(matrix));
    A->row = LEVEL_0_ROWS;
    A->column = LEVEL_0_COLUMNS;

  for (int i = 0; i < A->row; i++){
      for (int j = 0; j < A->column; j++){
          A->matrix[i][j] = Array[i][j];
      }
  }
  
  for (int i = 0; i < data->row; i++){
      for (int j = 0; j < data->column; j++){
          data->matrix[i][j] = A->matrix[i][j];
      }
  }


   /*[>Simulates the avatars moving<]*/
  /*[>pthread_t t1;<]*/
  /*[>int iret1 = pthread_create(&t1, NULL, print_i, NULL);<]*/
  /*[>if (iret1) {<]*/
    /*[>fprintf(stderr,<]*/
        /*[>"pthread_create failed, rc=%d\n",iret1);<]*/
    /*[>exit(iret1);<]*/
  /*[>}<]*/

   /*[>(2) Start the maze that takes from the global MAZE<]*/
  render_maze();


  return 0;
}
