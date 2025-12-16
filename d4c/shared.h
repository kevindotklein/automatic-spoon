#ifndef CONSTS_H
#define CONSTS_H

#include <stdlib.h>

#define CROSS_RATE 1.f
#define MU_RATE 0.05f

static inline double frand(double min, double max) {
  return min + (max - min) * ((double)rand() / RAND_MAX);
}

#endif
