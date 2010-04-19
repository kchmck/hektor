#include "hektor-usage.h"

double usage_calculate(const snapshots_t *snapshots, const plan_t *plan) {
  // At least two snapshots are required for a calculation.
  if (snapshots->length < 2) return plan->threshold;

  double remaining = plan->threshold;

  // Loop through snapshot pairs.
  for (int i = 1; i < snapshots->length; ++i) {
    const snapshot_t *begin = &snapshots->list[i - 1];
    const snapshot_t *end = &snapshots->list[i];

    span_t span = {0};
    span_calculate_between(begin, end, plan, &span);

    remaining = span_calculate_next(remaining, plan, &span);
  }

  return remaining;
}
