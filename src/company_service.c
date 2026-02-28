#include "company_service.h"

static void print_company(FILE *out, const InternationalCompany *c, size_t index) {
    fprintf(out,
            "[%zu] %s | site: %s | employees: %d | branches(EU): %d | market years: %d\n",
            index,
            c->name,
            c->website,
            c->employees,
            c->branches_europe,
            c->market_years);

    fprintf(out,
            "    HQ: %s, %s, %s, %s, %s, %s, house %s, apt %s\n",
            c->hq_address.postal_index,
            c->hq_address.country,
            c->hq_address.region,
            c->hq_address.district,
            c->hq_address.city,
            c->hq_address.street,
            c->hq_address.house,
            c->hq_address.apartment);
}

void company_write_with_employees_gt(FILE *out,
                                     const InternationalCompany *arr,
                                     size_t n,
                                     int threshold) {
    fprintf(out, "=== INTERNATIONAL COMPANIES WITH EMPLOYEES > %d ===\n", threshold);

    int any = 0;
    for (size_t i = 0; i < n; i++) {
        if (arr[i].employees > threshold) {
            print_company(out, &arr[i], i);
            any = 1;
        }
    }

    if (!any) {
        fprintf(out, "No companies match the condition.\n");
    }
}