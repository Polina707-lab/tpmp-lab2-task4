#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include "models.h"
#include <stddef.h>
#include <stdio.h>

int fin_read_all(
    FILE *in,
    Date *out_date,
    WORKER2 *workers, size_t workers_cap, size_t *out_workers_n,
    Date *out_today,
    char *out_position, size_t position_cap,
    InternationalCompany *companies, size_t companies_cap, size_t *out_companies_n
);

#endif
