#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include "models.h"
#include <stddef.h>
#include <stdio.h>

int fin_read_companies(FILE *in,
                       InternationalCompany *companies,
                       size_t cap,
                       size_t *out_n);

#endif