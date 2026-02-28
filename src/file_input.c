#include "file_input.h"
#include "date_service.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

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

static int parse_date_ymd(const char *s, Date *out) {
    if (!s || !out) return 0;

    int y = 0, m = 0, d = 0;

    if (sscanf(s, "%d-%d-%d", &y, &m, &d) == 3) {
        out->year = y;
        out->month = m;
        out->day = d;
        return date_is_valid(*out);
    }

    if (sscanf(s, "%d %d %d", &y, &m, &d) == 3) {
        out->year = y;
        out->month = m;
        out->day = d;
        return date_is_valid(*out);
    }

    return 0;
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

int fin_read_all(
    FILE *in,
    Date *out_date,
    WORKER2 *workers, size_t workers_cap, size_t *out_workers_n,
    Date *out_today,
    char *out_position, size_t position_cap,
    InternationalCompany *companies, size_t companies_cap, size_t *out_companies_n
) {
    if (!in || !out_date || !workers || !out_workers_n || !out_today || !out_position || !companies || !out_companies_n)
        return 0;

    *out_workers_n = 0;
    *out_companies_n = 0;
    out_position[0] = '\0';

    // Важно: инициализируем даты нулями, чтобы было проще отлавливать, что не заполнилось
    *out_date = (Date){0,0,0};
    *out_today = (Date){0,0,0};

    enum { NONE, DATE_SEC, WORKERS_SEC, TODAY_SEC, POSITION_SEC, COMPANIES_SEC } sec = NONE;

    char buf[1024];
    size_t line_no = 0;

    while (fgets(buf, sizeof(buf), in)) {
        line_no++;

        rtrim(buf);
        char *line = ltrim(buf);
        line = skip_utf8_bom(line);

        if (is_comment_or_empty(line)) continue;

        if (line[0] == '[') {
            if (strcmp(line, "[DATE]") == 0) sec = DATE_SEC;
            else if (strcmp(line, "[WORKERS]") == 0) sec = WORKERS_SEC;
            else if (strcmp(line, "[TODAY]") == 0) sec = TODAY_SEC;
            else if (strcmp(line, "[POSITION]") == 0) sec = POSITION_SEC;
            else if (strcmp(line, "[COMPANIES]") == 0) sec = COMPANIES_SEC;
            else {
                fprintf(stderr, "Unknown section at line %zu: '%s'\n", line_no, line);
                sec = NONE;
            }
            continue;
        }

        if (sec == NONE) {
            fprintf(stderr, "Data outside section at line %zu: '%s'\n", line_no, line);
            return 0;
        }

        if (sec == DATE_SEC) {
            if (!parse_date_ymd(line, out_date)) {
                fprintf(stderr, "Bad DATE at line %zu: '%s'\n", line_no, line);
                return 0;
            }
            sec = NONE;

        } else if (sec == TODAY_SEC) {
            if (!parse_date_ymd(line, out_today)) {
                fprintf(stderr, "Bad TODAY at line %zu: '%s'\n", line_no, line);
                return 0;
            }
            sec = NONE;

        } else if (sec == POSITION_SEC) {
            safe_copy(out_position, position_cap, line);
            if (out_position[0] == '\0') {
                fprintf(stderr, "Empty POSITION at line %zu\n", line_no);
                return 0;
            }
            sec = NONE;

        } else if (sec == WORKERS_SEC) {
            if (*out_workers_n >= workers_cap) {
             
                // fprintf(stderr, "Workers overflow, skipping line %zu\n", line_no);
                continue;
            }

            // code;surname;position;gender;YYYY-MM-DD;term;salary
            char tmp[1024];
            safe_copy(tmp, sizeof(tmp), line);

            char *f[8] = {0};
            int k = split_semicolon(tmp, f, 8);
            if (k < 7) {
                fprintf(stderr, "Bad WORKER fields=%d at line %zu: '%s'\n", k, line_no, line);
                return 0;
            }

            WORKER2 w;
            memset(&w, 0, sizeof(w));

            w.code = atoi(f[0]);
            safe_copy(w.surname, sizeof(w.surname), f[1]);
            safe_copy(w.position, sizeof(w.position), f[2]);
            w.gender = (char)toupper((unsigned char)f[3][0]);

            if (!parse_date_ymd(f[4], &w.sign_date)) {
                fprintf(stderr, "Bad WORKER sign_date at line %zu: '%s'\n", line_no, f[4]);
                return 0;
            }

            w.term_months = atoi(f[5]);
            w.salary = strtod(f[6], NULL);

            workers[*out_workers_n] = w;
            (*out_workers_n)++;

        } else if (sec == COMPANIES_SEC) {
            if (*out_companies_n >= companies_cap) {
                // fprintf(stderr, "Companies overflow, skipping line %zu\n", line_no);
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

            companies[*out_companies_n] = c;
            (*out_companies_n)++;
        }
    }


    // Basic required checks:
    if (!date_is_valid(*out_date)) {
        fprintf(stderr, "Final check failed: invalid DATE\n");
        return 0;
    }
    if (!date_is_valid(*out_today)) {
        fprintf(stderr, "Final check failed: invalid TODAY\n");
        return 0;
    }
    if (out_position[0] == '\0') {
        fprintf(stderr, "Final check failed: empty POSITION\n");
        return 0;
    }

    return 1;
}