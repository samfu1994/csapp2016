#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
long block_bit, asso, index_bit;
long hit, miss, evic;
long get_addr(long addr, long * tag){
  *tag = addr >> (block_bit + index_bit);
  addr >>= block_bit;
  long tmp = 1 << index_bit;
  addr %= tmp;
  return addr;
}
typedef struct node{
   long addr, tag;
   struct node * next;
} Node;
void initNode(Node * root){
  root -> tag = -1;
  root -> addr = -1;
  root -> next = (Node * )malloc(sizeof(Node));
  root = root -> next;
  root -> addr = -1;
  root -> tag = -1;
  root -> next = NULL;
}
long judge(long addr, long tag, Node * root){
  long res;
  Node * tmp = root -> next, * last = NULL, * pre_last = NULL;
  Node * pre = root;
  long found = 0;
  long count = 0;
  while(tmp -> next != NULL && !found && count < asso){
    if(tmp -> addr == addr){
      count++;
      if(tmp -> tag == tag){
        pre -> next = tmp -> next;
        tmp -> next = root -> next;
        root -> next = tmp;
        found = 1;
        break;
      }
      else{
        last = tmp;
        pre_last = pre;
      }
    }
    tmp = tmp -> next;
    pre = pre -> next;
  }
  if(!found){
    if(count == asso){
      pre_last -> next = last -> next;
      free(last);
      res = -1;
    }
    else{
      res = 0;
    }
    Node * newNode = (Node *) malloc(sizeof(Node));
    newNode -> next = root -> next;
    newNode -> tag = tag;
    newNode -> addr = addr;
    root -> next = newNode;
  }
  else{
    res = 1;
  }
  return res;
}
void simulation(FILE * fp, long mode_h, long mode_v){
  long block_size, set_size, cache_size;

  // const long SIZE = 20;
  size_t len = 20;
  size_t read;
  block_size = pow(2, block_bit);
  set_size = block_size * asso;
  cache_size = set_size * pow(2, index_bit);
  if (fp == NULL)
       exit(EXIT_FAILURE);
  char op;
  char * line = (char*)malloc(sizeof(char) * len);
  char * pch;
  long addr, res;
  long * tag = (long*)malloc(sizeof(long));
  Node * root = (Node * )malloc(sizeof(Node));
  initNode(root);
  while ((read = getline(&line, &len, fp)) != -1) {
       if(line[0] != ' ')
          continue;
       op = line[1];
       pch = strtok(line, " ,");
       pch = strtok(NULL, " ,");
       addr = strtol(pch, NULL, 16);
       addr = get_addr(addr, tag);
      //  printf("addr: %x   ", addr );

       res = judge(addr, *tag, root);
       if(res == 1){
        //  printf("hit ");
        hit++;
      }
       else if(res == 0){
        //  printf("miss ");
        miss++;
      }
       else if(res == -1){
        //  printf("miss eviction ");
         miss++; evic++;
       }
       if(op == 'M') {
        //  printf("hit");
         hit++;
       }
      //  printf("\n");

      //  printf("%d\n", addr);
  }


}
int main(int argc, char * argv[])
{
    // long current = 1;
    char c1[] = "-h";
    long mode_h = 0, mode_v = 0;
    long off = 0;
    if(argc == 9){
    }
    else if(argc == 10){
      if(strcmp(argv[1], c1) == 0)
        mode_h = 1;
      else
        mode_v = 1;
      off = 1;
    }
    else if(argc == 11){
      mode_h = 1;
      mode_v = 1;
      off = 2;
    }
    else{
    }
    if(mode_h == 0 && mode_v == 0){

    }
    block_bit = atoi(argv[6 + off]);
    asso = atoi(argv[4 + off]);
    index_bit = atoi(argv[2 + off]);


    FILE * fp = fopen(argv[off + 8], "r");

    simulation(fp, mode_h, mode_v);
    printSummary(hit,miss,evic);
    // printf("hit %d, miss %d, evictions %d\n", hit, miss, evic);
    return 0;
}
