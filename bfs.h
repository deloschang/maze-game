//bfs.h
//


typedef struct _cell{
     int x;
     int y;
     struct _cell* parent;
     struct _cell* next;
     struct _cell* prev;
     int dist;
}cell;

typedef struct _queue{
     cell head;
}queue;

void enqueue(queue* q,cell c);
cell* dequeue(queue* q);
int* find path();
