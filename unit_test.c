// Filename: Test cases for bfs.h/.c
// Description: A unit test harness for the maze solver
// 
//
// Test Harness Spec:
// ------------------
//
//  It uses these files but they are not unit tested in this test harness:
//
//   
// 
//  It tests the following functions:
//
//  void find_path(matrix* mat,XYPOS* start,XYPOS* end,int path[]);
//  void construct_path(cell* c,int path[]);
// 
//
//  If any of the tests fail it prints status 
//  If all tests pass it prints status.
//
//  Test Cases:
//  -----------
//   
//  The test harness runs a number of test cases to test the code.
//  The approach is to first set up the environment for the test,
//  invoke the function to be tested, then validate the state of
//  the data structures using the SHOULD_BE macro. This is repeated
//  for each test case. 
//
//  The test harness isolates the functions under test and setting
//  up the environment the code expects when integrated in the real
//  system.
//
//
//  
//  The following test cases  (1-3) are for function:
//
//  void find_path(matrix* mat,XYPOS* start,XYPOS* end,int path[]);
//
//  Test case: TestBFS:1
// This test case calls find_path() and construct_path() under the condition
// where the start and goal coordinates are separated by 1 node with no
// two children added onto the queue at once (simple)
//
//  Test case: TestBFS:2
// This test case calls find_path() and construct_path() under the condition
// where the start and goal coordinates are separated by many nodes
// and thus many children are added to the queue at once
//
//  Test case: TestBFS:3
// This test case calls find_path() and construct_path() under the condition
// where the goal coordinate is unreachable from the start coordinate
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/amazing.h"
#include "utils/amazing_client.h"
#include "utils/header.h"

// Useful MACROS for controlling the unit tests.

// each test should start by setting the result count to zero
#define START_TEST_CASE  int rs=0

// check a condition and if false print the test condition failed
// e.g., SHOULD_BE(dict->start == NULL)
// note: the construct "#x" below is the sstringification preprocessor operator that
//       converts the argument x into a character string constant
#define SHOULD_BE(x) if (!(x))  {rs=rs+1; \
    printf("Line %d test [%s] Failed\n", __LINE__,#x); \
  }


// return the result count at the end of a test
#define END_TEST_CASE return rs

//
// general macro for running a best
// e.g., RUN_TEST(TestDAdd1, "DAdd Test case 1");
// translates to:
// if (!TestDAdd1()) {
//     printf("Test %s passed\n","DAdd Test case 1");
// } else { 
//     printf("Test %s failed\n", "DAdd Test case 1");
//     cnt = cnt +1;
// }
//

#define RUN_TEST(x, y) if (!x()) {              \
    printf("Test %s passed\n", y);              \
} else {                                        \
    printf("Test %s failed\n", y);              \
    cnt = cnt + 1;                              \
}
    

// Test case: TestBFS:1
// This test case calls find_path() and construct_path() under the condition
// where the start and goal coordinates are separated by 1 node with no
// two children added onto the queue at once (simple)
int TestBFS1(){
  START_TEST_CASE;

  static char Array[9][9] = {
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
  matrix* data = malloc(sizeof(matrix));
  data->row = 9;
  data->column = 9;

  for (int i = 0; i < data->row; i++){
      for (int j = 0; j < data->column; j++){
          data->matrix[i][j] = Array[i][j];
      }
  }

  XYPOS* cur_pos = malloc(sizeof(XYPOS));
  cur_pos->xPos = 0;
  cur_pos->yPos = 0;

  XYPOS* goal = malloc(sizeof(XYPOS));
  goal->xPos = 0;
  goal->yPos = 1;
  int path[100];

  find_path(data, cur_pos, goal, path);
  SHOULD_BE(path != NULL);
  SHOULD_BE(path[0] == 2);
  SHOULD_BE(path[1] == 0);

  free(cur_pos);
  free(goal);
  free(data);

  END_TEST_CASE;
}

// Test case: TestBFS:2
// This test case calls find_path() and construct_path() under the condition
// where the start and goal coordinates are separated by many nodes
// and thus many children are added to the queue at once
int TestBFS2(){
  START_TEST_CASE;

  static char Array[9][9] = {
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
  matrix* data = malloc(sizeof(matrix));
  data->row = 9;
  data->column = 9;

  for (int i = 0; i < data->row; i++){
      for (int j = 0; j < data->column; j++){
          data->matrix[i][j] = Array[i][j];
      }
  }

  XYPOS* cur_pos = malloc(sizeof(XYPOS));
  cur_pos->xPos = 0;
  cur_pos->yPos = 0;

  XYPOS* goal = malloc(sizeof(XYPOS));
  goal->xPos = 4;
  goal->yPos = 4;
  int path[100];

  find_path(data, cur_pos, goal, path);
  SHOULD_BE(path != NULL);
  SHOULD_BE(path[0] == 2);
  SHOULD_BE(path[1] == 3);
  SHOULD_BE(path[2] == 1);
  SHOULD_BE(path[3] == 3);
  SHOULD_BE(path[4] == 2);
  SHOULD_BE(path[5] == 2);
  SHOULD_BE(path[6] == 0);
  SHOULD_BE(path[7] == 0);

  free(cur_pos);
  free(goal);
  free(data);

  END_TEST_CASE;
}

// Test case: TestBFS:3
// This test case calls find_path() and construct_path() under the condition
// where the goal coordinate is unreachable from the start coordinate
int TestBFS3(){
  START_TEST_CASE;

  static char Array[9][9] = {
      { 'E', '1', 'E', '0', 'E', '0', 'E', '0', 'E' } ,
      { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '0' } ,
      { 'E', '0', 'E', '1', 'E', '1', 'E', '1', 'E' } ,
      { '_', 'Z', '_', 'Z', '0', 'Z', '0', 'Z', '_' } ,
      { 'E', '0', 'E', '0', 'E', '1', 'E', '0', 'E' } ,
      { '0', 'Z', '_', 'Z', '_', 'Z', '_', 'Z', '0' } ,
      { 'E', '0', 'E', '1', 'E', '0', '1', '1', '1' } ,
      { '0', 'Z', '0', 'Z', '0', 'Z', '_', 'Z', '_' } ,
      { 'E', '1', 'E', '0', 'E', '0', '1', '1', '1' } ,
  };
  matrix* data = malloc(sizeof(matrix));
  data->row = 9;
  data->column = 9;

  for (int i = 0; i < data->row; i++){
      for (int j = 0; j < data->column; j++){
          data->matrix[i][j] = Array[i][j];
      }
  }

  XYPOS* cur_pos = malloc(sizeof(XYPOS));
  cur_pos->xPos = 0;
  cur_pos->yPos = 0;

  XYPOS* goal = malloc(sizeof(XYPOS));
  goal->xPos = 4;
  goal->yPos = 5;
  int path[100];
  BZERO(path, 100);

  find_path(data, cur_pos, goal, path);
  SHOULD_BE(path[1] == 0);
  SHOULD_BE(path[2] == 0);
  SHOULD_BE(path[3] == 0);

  free(cur_pos);
  free(goal);
  free(data);

  END_TEST_CASE;
}

// This is the main test harness for the set of query engine functions. It tests all the code
// in querylogic.c:
//
//  It uses these files but they are not unit tested in this test harness:
//
//   
// 
//  It tests the following functions:
//
//  void find_path(matrix* mat,XYPOS* start,XYPOS* end,int path[]);
//  void construct_path(cell* c,int path[]);
//
//

int main(int argc, char** argv) {
  int cnt = 0;

  RUN_TEST(TestBFS1, "BFS Test Case 1");
  RUN_TEST(TestBFS2, "BFS Test Case 2");
  RUN_TEST(TestBFS3, "BFS Test Case 3");

  if (!cnt) {
    printf("All passed!\n Failed: %d \n", cnt); return 0;
  } else {
    printf("Some fails!\n Failed: %d \n", cnt); return 1;
  }
}

