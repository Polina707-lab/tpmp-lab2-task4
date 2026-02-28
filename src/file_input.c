#include "file_input.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void rtrim(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || isspace((unsigned char)s[n - 1]))) {
        s[n - 1] = '\0';
        n--;
    }
}

static char *ltrim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

static char *skip_utf8_bom(char *s) {
    unsigned char *p = (unsigned char *)s;
    if (p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) return s + 3;
    return s;
}

static int is_comment_or_empty(const char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return (*s == '\0' || *s == '#');
}

static int split_semicolon(char *line, char *fields[], int max_fields) {
    int count = 0;
    char *p = line;

    while (count < max_fields) {
        fields[count++] = p;
        char *sep = strchr(p, ';');
        if (!sep) break;
        *sep = '\0';
        p = sep + 1;
    }
    return count;
}

static void safe_copy(char *dst, size_t cap, const char *src) {
    if (cap == 0) return;
    strncpy(dst, src, cap - 1);
    dst[cap - 1] = '\0';
}

int fin_read_companies(FILE *in,
                       InternationalCompany *companies,
                       size_t cap,
                       size_t *out_n) {
    if (!in || !companies || !out_n) return 0;

    *out_n = 0;

    enum { NONE, COMPANIES_SEC } sec = NONE;

    char buf[1024];
    size_t line_no = 0;

    while (fgets(buf, sizeof(buf), in)) {
        line_no++;

        rtrim(buf);
        char *line = ltrim(buf);
        line = skip_utf8_bom(line);

        if (is_comment_or_empty(line)) continue;

        if (line[0] == '[') {
            if (strcmp(line, "[COMPANIES]") == 0) sec = COMPANIES_SEC;
            else sec = NONE;
            continue;
        }

        if (sec != COMPANIES_SEC) continue; // игнорируем всё вне секции

        if (*out_n >= cap) {
            // переполнение — просто игнорируем лишние строки
            continue;
        }

        // name;website;postal;country;region;district;city;street;house;apartment;market_years;employees;branches_europe
        char tmp[1024];
        safe_copy(tmp, sizeof(tmp), line);

        char *f[16] = {0};
        int k = split_semicolon(tmp, f, 16);
        if (k < 13) {
            fprintf(stderr, "Bad COMPANY fields=%d at line %zu: '%s'\n", k, line_no, line);
            return 0;
        }

        InternationalCompany c;
        memset(&c, 0, sizeof(c));

        safe_copy(c.name, sizeof(c.name), f[0]);
        safe_copy(c.website, sizeof(c.website), f[1]);

        safe_copy(c.hq_address.postal_index, sizeof(c.hq_address.postal_index), f[2]);
        safe_copy(c.hq_address.country, sizeof(c.hq_address.country), f[3]);
        safe_copy(c.hq_address.region, sizeof(c.hq_address.region), f[4]);
        safe_copy(c.hq_address.district, sizeof(c.hq_address.district), f[5]);
        safe_copy(c.hq_address.city, sizeof(c.hq_address.city), f[6]);
        safe_copy(c.hq_address.street, sizeof(c.hq_address.street), f[7]);
        safe_copy(c.hq_address.house, sizeof(c.hq_address.house), f[8]);
        safe_copy(c.hq_address.apartment, sizeof(c.hq_address.apartment), f[9]);

        c.market_years = atoi(f[10]);
        c.employees = atoi(f[11]);
        c.branches_europe = atoi(f[12]);

        companies[*out_n] = c;
        (*out_n)++;
    }

    return 1;
}