#ifndef COMPANY_SERVICE_H
#define COMPANY_SERVICE_H

#include "models.h"
#include <stddef.h>
#include <stdio.h>

void company_write_with_employees_gt(FILE *out, const InternationalCompany *arr, size_t n, int threshold);

#endif
