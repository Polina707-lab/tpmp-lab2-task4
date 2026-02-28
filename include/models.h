#ifndef MODELS_H
#define MODELS_H

#define NAME_MAX     128
#define SITE_MAX     128
#define COUNTRY_MAX   64
#define REGION_MAX    64
#define DISTRICT_MAX  64
#define CITY_MAX      64
#define STREET_MAX    64

typedef struct {
    char postal_index[16];
    char country[COUNTRY_MAX];
    char region[REGION_MAX];
    char district[DISTRICT_MAX];
    char city[CITY_MAX];
    char street[STREET_MAX];
    char house[16];
    char apartment[16];
} Address;

typedef struct {
    char name[NAME_MAX];
    char website[SITE_MAX];
    Address hq_address;
    int market_years;
    int employees;
    int branches_europe;
} InternationalCompany;

#endif
