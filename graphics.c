/* 
* Filename: graphics.c
* This file will render the maze graphically given the 2D int array
*/

#include <stdlib.h>                          // for calloc
#include <string.h>                          // for strlen
#include <gtk/gtk.h>                         // for gtk functionality

//! Zero out the 2D array
#define BZEROARRAY(array,n,m)  memset(array, 0, sizeof(array[0][0]) * m * n)

#define LEVEL_0_ROWS 9  // (2n+1) adjustment 
#define LEVEL_0_COLUMNS 9

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

int main(){
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
}


