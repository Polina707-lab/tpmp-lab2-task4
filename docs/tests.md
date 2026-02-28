# Tests (Task 4, Variant 19)

## Test 1 — Control Calculation

### Input Data

File: `data/input.txt`

```
[DATE]
2024 2 29

[WORKERS]
1;Ivanov;Engineer;M;2024-10-10;12;1200.5
2;Petrova;Engineer;F;2023-12-01;24;1500
3;Ivanov;Manager;M;2022-05-20;12;2000

[TODAY]
2025 2 28

[POSITION]
Engineer

[COMPANIES]
BigCorp;big.com;220000;Belarus;Minsk;Leninsky;Minsk;Lenina;10;15;50;15000;12
SmallCo;small.com;10001;USA;CA;LA;LosAngeles;Main;1;1;5;9000;2
```

---

## Expected Results

### DATE TASKS

- Given date: 2024-02-29 (leap year)
- Days in month: 29
- Previous day: 2024-02-28
- Next day: 2024-03-01

### WORKER TASKS

- Worker signed less than 1 year ago (relative to 2025-02-28):
  - Ivanov (sign date 2024-10-10)
- Workers with double contracts:
  - Ivanov (surname appears twice)
- Average contract term for position "Engineer":
  - (12 + 24) / 2 = 18.00 months
- Gender count:
  - M = 2
  - F = 1

### COMPANY TASKS

- Only companies with employees > 10000 should appear:
  - BigCorp (15000 employees)
  - SmallCo (9000 employees) should NOT appear

---

## Actual Program Output (fragment)

```
=== DATE TASKS ===
Given date:      2024-02-29
Days in month: 29
Previous day:    2024-02-28
Next day:        2024-03-01

=== AVERAGE CONTRACT TERM FOR POSITION ===
Position: Engineer
Average term: 18.00 months

=== GENDER COUNT ===
M=2, F=1

=== INTERNATIONAL COMPANIES WITH EMPLOYEES > 10000 ===
[0] BigCorp | site: big.com | employees: 15000 | branches(EU): 12 | market years: 50
```

---

## Conclusion

The actual program output matches the expected results.

All implemented modules (date_service, worker_service, company_service, file_input)
work correctly for the control test.
