#ifndef HEKTOR_SPAN_H
#define HEKTOR_SPAN_H

#include <stdbool.h>
#include <time.h>

#include "hektor-snapshot.h"
#include "hektor-plan.h"

typedef struct {
  time_t elapsed;

  int uploaded;
  int downloaded;

  int total_transferred;
  double refilled;

  bool during_fap_free;
} span_t;

// Calculate the difference between two snapshots.
void span_calculate_between(const snapshot_t *begin, const snapshot_t *end,
                            const plan_t *plan, span_t *span);

// Apply a snapshot's usage information to a remaining value and return the
// result.
double span_calculate_next(const double remaining, const plan_t *plan,
                           const span_t *span);

#endif
