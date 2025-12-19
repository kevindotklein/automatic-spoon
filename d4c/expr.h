#ifndef EXPR_H
#define EXPR_H

#include "node.h"

typedef struct expr_t {
  union {
    int degree;
    struct expr_t* e_degree;
  };
  double value;
  struct expr_t* next;
} expr_t;

expr_t* new_expr(int degree, double value);
expr_t* copy_expr(expr_t* e);
expr_t* sum_expr(expr_t* a, expr_t* b);
expr_t* mul_expr(expr_t* a, expr_t* b);
expr_t* eval_expr(node_t* n);
node_t* expr_to_node(expr_t* e);
void print_expr(expr_t* e);

#endif
