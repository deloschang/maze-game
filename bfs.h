//bfs.h
//

// DATA STRUCTURES 

// Cell. This is a data structure that holds the coordinates of a position on the map, a pointer to the 
// parent cell (the previous position), the distance between the cell and the goal (start?), and pointers
// to the next and previous cells in the queue.
typedef struct _cell{
     int x;
     int y;
     struct _cell* parent;
     struct _cell* next;
     struct _cell* prev;
     int dist;
}cell;


// The queue is a FIFO data structure with a pointer to the cell at the front of the queue.
typedef struct _queue{
     cell head;
}queue;

// PROTOTYPES

// Enqueues (adds) the cell to the end of the queue
void enqueue(queue* q,cell c);

// Dequeues (removes) the cell at front of queue and returns a pointer to it or NULL if the queue is
// empty. 
cell* dequeue(queue* q);


// remove?
int* find path();
