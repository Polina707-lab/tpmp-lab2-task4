#include <stdio.h>

#include "company_service.h"
#include "file_input.h"
#include "models.h"

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

    InternationalCompany companies[20];
    size_t n = 0;

    if (!fin_read_companies(in, companies, 20, &n)) {
        fclose(in);
        fprintf(stderr, "Error: invalid input format in %s\n", input_path);
        return 3;
    }
    fclose(in);

    FILE *out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Error: cannot open output file: %s\n", output_path);
        return 4;
    }

    fprintf(out, "=== TASK 4 REPORT (Variant 19) ===\n");
    fprintf(out, "Input file: %s\n", input_path);
    fprintf(out, "Companies read: %zu\n\n", n);

    company_write_with_employees_gt(out, companies, n, 10000);

    fclose(out);

    printf("Done. Result saved to: %s\n", output_path);
    return 0;
}