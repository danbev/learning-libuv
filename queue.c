#include <stdio.h>

#include "../libuv/src/queue.h"

//typedef void* QUEUE[2];
// QUEUE is now an alias for an array with 2 void pointer elements.

//#define QUEUE_NEXT(q) (*(QUEUE **) &((*(q))[0]))
//
struct loop {
  const char* name;
  void* wp[2];
};

int main(int argc, char** argv) {
  /*
  void* s[2];
  (*(QUEUE **) &((*(&s))[0])) = &s;
  (*(QUEUE **) s) = &s;

  printf("------------\n");
  auto first = &((*(&s))[0]);
  // first will be of type void**
  printf("first=%p\n", first);
  (*(QUEUE**) first) = &s;
  (*(void**) first) = &s;
  auto p = (QUEUE**) first;
  */


  QUEUE q = {NULL, NULL};
  //QUEUE_INIT(&q);
  // The following is the same as what the QUEUE_INIT macro does:
  //(*(&((*(&q))[0]))) = &q;
  //(*(&((*(&q))[1]))) = &q;
  (*(QUEUE **) &((*(&q))[0])) = &q;
  (*(QUEUE **) &((*(&q))[1])) = &q;

  struct loop* l;
  struct loop l1 = {"loop1"};
  struct loop l2 = {"loop2"};
  struct loop l3 = {"loop3"};

  QUEUE_INSERT_TAIL(&q, &l1.wp);
  QUEUE_INSERT_TAIL(&q, &l2.wp);
  QUEUE_INSERT_TAIL(&q, &l3.wp);

  QUEUE* head = QUEUE_HEAD(&q);
  l = QUEUE_DATA(head, struct loop, wp);
  printf("first entry: %s\n", l->name);

  QUEUE_FOREACH(head, &q) {
    l = QUEUE_DATA(head, struct loop, wp);
    printf("entry: %s\n", l->name);
  }
}
