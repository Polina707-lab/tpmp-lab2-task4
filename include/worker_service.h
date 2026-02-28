#ifndef WORKER_SERVICE_H
#define WORKER_SERVICE_H

#include "models.h"
#include <stddef.h>
#include <stdio.h>

void worker_write_signed_less_than_year_ago(FILE *out, const WORKER2 *arr, size_t n, Date today);
void worker_write_double_contracts(FILE *out, const WORKER2 *arr, size_t n);

int  worker_avg_term_for_position(const WORKER2 *arr, size_t n, const char *position, double *out_avg_months);
void worker_count_gender(const WORKER2 *arr, size_t n, int *out_male, int *out_female);

#endif
