#include "expr.h"

expr_t* new_expr(int degree, double value) {
  expr_t* e = malloc(sizeof(expr_t));
  e->degree = degree;
  e->value = value;
  e->next = NULL;
  return e;
}

expr_t* copy_expr(expr_t* e) {
  expr_t* c = new_expr(e->degree, e->value);
  c->next = e->next;
  return c;
}

expr_t* sum_expr(expr_t* a, expr_t* b) {
    expr_t* head = NULL;
    expr_t* tail = NULL;

    while (a && b) {
        expr_t* r;

        if (a->degree < b->degree) {
            r = new_expr(a->degree, a->value);
            a = a->next;
        } else if (a->degree == b->degree) {
            r = new_expr(a->degree, a->value + b->value);
            a = a->next;
            b = b->next;
        } else {
            r = new_expr(b->degree, b->value);
            b = b->next;
        }

        if (!head) {
            head = tail = r;
        } else {
            tail->next = r;
            tail = r;
        }
    }

    while (a) {
        expr_t* r = new_expr(a->degree, a->value);
        tail->next = r;
        tail = r;
        a = a->next;
    }

    while (b) {
        expr_t* r = new_expr(b->degree, b->value);
        tail->next = r;
        tail = r;
        b = b->next;
    }

    return head;
}

expr_t* mul_expr(expr_t* a, expr_t* b) {

}

expr_t* eval_expr(node_t* n) {
  if(n->type == CONST) {
    expr_t* e = new_expr(0, n->value);
    return e;
  }

  if(n->type == VAR) {
    expr_t* e = new_expr(1, 1);
    return e;
  }

  expr_t* a = eval_expr(n->left);
  expr_t* b = eval_expr(n->right);

  switch(n->op) {
    case '+': return sum_expr(a, b);
  }
}

node_t* expr_to_node(expr_t* e) {
  node_t* n = NULL;

  if(e->degree == 0) {
    n = new_node(CONST);
    n->value = e->value;
  }

  if(e->degree == 1) {
    if(e->value != 1) {
      n = new_node(OP);
      n->op = '*';
      n->right = new_node(VAR);
      n->left = new_node(CONST);
      n->left->value = e->value;
    } else {
      n = new_node(VAR);
    }
  }

  if(e->degree > 1) {
    if(e->value != 1) {
      n = new_node(OP);
      n->op = '*';
      n->left = new_node(CONST);
      n->left->value = e->value;
      n->right = new_node(OP);
      n->right->op = '^';
      n->right->left = new_node(VAR);
      n->right->right = new_node(CONST);
      n->right->right->value = e->degree;
    } else {
      n = new_node(OP);
      n->op = '^';
      n->right = new_node(VAR);
      n->left = new_node(CONST);
      n->left->value = e->degree;
    }
  }

  if(e->next) {
    //add + plug what i did right hand side and lhs is the recursive call
    node_t *n2 = new_node(OP);
    n2->op = '+';
    n2->right = n;
    n2->left = expr_to_node(e->next);
    return n2;
  }
  return n;

//  if(e->degree == 0 && !e->next) {
//    node_t* n = new_node(CONST);
//    n->value = e->value;
//    return n;
//  }
//
//  if(e->next) {
//    node_t* n = new_node(OP);
//    n->op = '+';
//
//    if(e->degree == 0) {
//      n->right = new_node(CONST);
//      n->right->value = e->value;
//      goto tail;
//    }
//
//    n->right = new_node(OP);
//    n->right->op = '*';
//
//    n->right->right = new_node(CONST);
//    n->right->right->value = e->value;
//
//    n->right->left = new_node(OP);
//    n->right->left->op = '^';
//
//    n->right->left->left = new_node(VAR);
//    n->right->left->right = new_node(CONST);
//    n->right->left->right->value = e->degree;
//
//tail:
//    n->left = expr_to_node(e->next);
//    return n;
//  }
//
//  node_t* n = new_node(OP);
//  n->op = '*';
//
//  n->right = new_node(CONST);
//  n->right->value = e->value;
//
//  n->left = new_node(OP);
//  n->left->op = '^';
//
//  n->left->left = new_node(VAR);
//  n->left->right = new_node(CONST);
//  n->left->right->value = e->degree;
//  return n;
}

void print_expr(expr_t* e) {
  printf("[%d %.2f]\n", e->degree, e->value);
  if(e->next) print_expr(e->next);
}
