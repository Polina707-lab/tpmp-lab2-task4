# Tests (Task 4, Variant 19)

## Test 1 — Control Calculation

### Input File (`data/input.txt`)

```
[COMPANIES]
BigCorp;big.com;220000;Belarus;Minsk;Leninsky;Minsk;Lenina;10;15;50;15000;12
SmallCo;small.com;10001;USA;CA;LA;LosAngeles;Main;1;1;5;9000;2
```

---

## Expected Result

The program must output only companies whose number of employees exceeds 10000.

From the input:

- BigCorp → 15000 employees → must appear
- SmallCo → 9000 employees → must NOT appear

Therefore, only BigCorp should be written to the output file.

---

## Actual Program Output (`data/output.txt`)

```
=== TASK 4 REPORT (Variant 19) ===
Input file: data/input.txt
Companies read: 2

=== INTERNATIONAL COMPANIES WITH EMPLOYEES > 10000 ===
[0] BigCorp | site: big.com | employees: 15000 | branches(EU): 12 | market years: 50
    HQ: 220000, Belarus, Minsk, Leninsky, Minsk, Lenina, house 10, apt 15
```

---

## Conclusion

The actual output matches the expected result.

The program correctly:
- reads company data from a text file,
- filters companies with employees > 10000,
- writes the filtered result to a new file.

Modules `file_input` and `company_service` operate correctly.
