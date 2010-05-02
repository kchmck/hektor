#ifndef HEKTOR_USAGE_H
#define HEKTOR_USAGE_H

#include "hektor-plan.h"
#include "hektor-snapshot.h"

// Calculate the remaining bytes before the fap is activated.
double usage_calculate_remaining(const snapshots_t *const snapshots,
                                 const plan_t *const plan);

#endif
