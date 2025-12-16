#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include "shared.h"

typedef enum { OP, VAR, CONST } node_type_t;

typedef struct node_t {
  node_type_t type;
  char op;
  double value;
  struct node_t* left;
  struct node_t* right;
} node_t;

node_t* new_node(node_type_t type);
node_t* random_tree(int depth);
node_t* copy_tree(node_t* n);
double eval(node_t* n, double x);
void mutate(node_t* n);
void crossover(node_t* a, node_t* b);
void print_tree(node_t* n);

#endif
