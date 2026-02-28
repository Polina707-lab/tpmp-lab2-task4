#ifndef DATE_SERVICE_H
#define DATE_SERVICE_H

#include "models.h"

int  date_is_leap_year(int year);
int  date_days_in_month(int year, int month);
int  date_is_valid(Date d);

Date date_prev_day(Date d);
Date date_next_day(Date d);

int  date_days_between(Date a, Date b); // b - a in days

#endif
