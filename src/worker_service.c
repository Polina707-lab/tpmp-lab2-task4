#include "worker_service.h"
#include "date_service.h"
#include <string.h>

static void print_worker(FILE *out, const WORKER2 *w, size_t index) {
    fprintf(out,
            "[%zu] code=%d, surname=%s, position=%s, gender=%c, sign=%04d-%02d-%02d, term=%d months, salary=%.2f\n",
            index,
            w->code,
            w->surname,
            w->position,
            w->gender,
            w->sign_date.year, w->sign_date.month, w->sign_date.day,
            w->term_months,
            w->salary);
}

void worker_write_signed_less_than_year_ago(FILE *out, const WORKER2 *arr, size_t n, Date today) {
    fprintf(out, "\n=== WORKERS WHO SIGNED CONTRACT < 1 YEAR AGO ===\n");
    int any = 0;
    for (size_t i = 0; i < n; i++) {
        int diff = date_days_between(arr[i].sign_date, today);
        if (diff >= 0 && diff < 365) {
            print_worker(out, &arr[i], i);
            any = 1;
        }
    }
    if (!any) fprintf(out, "No workers match the condition.\n");
}

void worker_write_double_contracts(FILE *out, const WORKER2 *arr, size_t n) {
    fprintf(out, "\n=== WORKERS WITH DOUBLE CONTRACTS (same surname appears >= 2) ===\n");
    int any = 0;

    for (size_t i = 0; i < n; i++) {
        int count = 0;
        for (size_t j = 0; j < n; j++) {
            if (strcmp(arr[j].surname, arr[i].surname) == 0) count++;
        }
        if (count >= 2) {
            int first = 1;
            for (size_t k = 0; k < i; k++) {
                if (strcmp(arr[k].surname, arr[i].surname) == 0) {
                    first = 0;
                    break;
                }
            }
            if (first) {
                print_worker(out, &arr[i], i);
                any = 1;
            }
        }
    }

    if (!any) fprintf(out, "No workers with repeated surnames.\n");
}

int worker_avg_term_for_position(const WORKER2 *arr, size_t n, const char *position, double *out_avg_months) {
    long sum = 0;
    long cnt = 0;

    for (size_t i = 0; i < n; i++) {
        if (strcmp(arr[i].position, position) == 0) {
            sum += arr[i].term_months;
            cnt++;
        }
    }

    if (cnt == 0) return 0;
    *out_avg_months = (double)sum / (double)cnt;
    return 1;
}

void worker_count_gender(const WORKER2 *arr, size_t n, int *out_male, int *out_female) {
    int m = 0, f = 0;
    for (size_t i = 0; i < n; i++) {
        if (arr[i].gender == 'M') m++;
        else if (arr[i].gender == 'F') f++;
    }
    *out_male = m;
    *out_female = f;
}
