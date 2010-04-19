#ifndef HEKTOR_PLAN_H
#define HEKTOR_PLAN_H

#include <stdbool.h>

typedef enum {
  PLAN_HOME,
  PLAN_PRO,
  PLAN_PRO_PLUS,
  PLAN_ELITE,
  PLAN_ELITE_PLUS,
  PLAN_ELITE_PREMIUM
} plan_id_t;

typedef struct {
  // The rolling, 24-hour bandwidth threshold of a plan in bytes
  int threshold;
  double refill_rate;
} plan_t;

static const int plans[] = {
  [PLAN_HOME]          = 200000000,
  [PLAN_PRO]           = 300000000,
  [PLAN_PRO_PLUS]      = 425000000,
  [PLAN_ELITE]         = 500000000,
  [PLAN_ELITE_PLUS]    = 500000000,
  [PLAN_ELITE_PREMIUM] = 500000000
};

// Get the bandwidth threshold of a plan.
static inline int plan_threshold(const plan_id_t plan_id) {
  return plans[plan_id] ? plans[plan_id] : 0;
}

// Get the refill rate of a plan in bytes per second.
static inline double plan_refill_rate(const int threshold) {
  return (double)threshold / 24 / 60 / 60;
}

// Load a new plan.
bool plan_load(plan_t *plan, const plan_id_t plan_id);

#endif
