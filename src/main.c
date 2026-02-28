#include <stdio.h>
#include <string.h>

#include "models.h"
#include "date_service.h"
#include "worker_service.h"
#include "company_service.h"
#include "file_input.h"

static void print_date(FILE *out, const char *label, Date d) {
    fprintf(out, "%s %04d-%02d-%02d\n", label, d.year, d.month, d.day);
}

int main(int argc, char **argv) {
    const char *input_path = "data/input.txt";
    const char *output_path = "data/output.txt";

    if (argc == 3) {
        input_path = argv[1];
        output_path = argv[2];
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [input.txt output.txt]\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(input_path, "r");
    if (!in) {
        fprintf(stderr, "Error: cannot open input file: %s\n", input_path);
        return 2;
    }

    Date d = {0}, today = {0};
    WORKER2 workers[12];
    size_t wn = 0;

    InternationalCompany companies[20];
    size_t cn = 0;

    char position[POSITION_MAX];

    if (!fin_read_all(in, &d, workers, 12, &wn, &today, position, sizeof(position), companies, 20, &cn)) {
        fclose(in);
        fprintf(stderr, "Error: invalid input format or data in %s\n", input_path);
        return 3;
    }
    fclose(in);

    FILE *out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Error: cannot open output file: %s\n", output_path);
        return 4;
    }

    fprintf(out, "=== TASK 4 REPORT (Variant 19) ===\n");
    fprintf(out, "Input file: %s\n\n", input_path);

    // Date tasks
    fprintf(out, "=== DATE TASKS ===\n");
    print_date(out, "Given date:     ", d);
    fprintf(out, "Days in month: %d\n", date_days_in_month(d.year, d.month));
    print_date(out, "Previous day:   ", date_prev_day(d));
    print_date(out, "Next day:       ", date_next_day(d));

    // Worker tasks
    fprintf(out, "\n=== WORKER2 TASKS ===\n");
    fprintf(out, "Workers count: %zu\n", wn);
    print_date(out, "Today for check:", today);

    worker_write_signed_less_than_year_ago(out, workers, wn, today);
    worker_write_double_contracts(out, workers, wn);

    fprintf(out, "\n=== AVERAGE CONTRACT TERM FOR POSITION ===\n");
    fprintf(out, "Position: %s\n", position);
    double avg_months = 0.0;
    if (worker_avg_term_for_position(workers, wn, position, &avg_months)) {
        fprintf(out, "Average term: %.2f months\n", avg_months);
    } else {
        fprintf(out, "No workers with this position.\n");
    }

    int male = 0, female = 0;
    worker_count_gender(workers, wn, &male, &female);
    fprintf(out, "\n=== GENDER COUNT ===\n");
    fprintf(out, "M=%d, F=%d\n", male, female);

    // Company tasks
    fprintf(out, "\n=== INTERNATIONAL COMPANY TASKS ===\n");
    fprintf(out, "Companies count: %zu\n", cn);
    company_write_with_employees_gt(out, companies, cn, 10000);

    fclose(out);

    printf("Done. Result saved to: %s\n", output_path);
    return 0;
}
