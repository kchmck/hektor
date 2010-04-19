#include "hektor-plan.h"

bool plan_load(plan_t *plan, const plan_id_t plan_id) {
  plan->threshold = plan_threshold(plan_id);
  if (!plan->threshold) return false;

  plan->refill_rate = plan_refill_rate(plan->threshold);
  return true;
}
