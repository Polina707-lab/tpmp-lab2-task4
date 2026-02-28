# tpmp-lab2-task4

## Overview

This repository contains Task 4 of Laboratory Work 2.

Variant 19 – "International Company".

The goal of this task is to implement a C program for processing text files.

The program reads information about international companies from a text file and outputs only those companies whose number of employees exceeds 10000.

Each company includes:
- company name,
- website,
- headquarters address (postal index, country, region, district, city, street, house, apartment),
- years on the global market,
- number of employees,
- number of branches in Europe.

The program:
- reads company data from a text file,
- processes the data,
- writes the filtered results to a new text file.

The project is organized according to the KIS model and built using the `make` utility.
Continuous Integration is configured using GitHub Actions.

## Author

Shamruk Polina  
Course: 2  
Group: 11  

## Usage

Clone the repository:

    git clone git@github.com:Polina707-lab/tpmp-lab2-task4.git
    cd tpmp-lab2-task4

Build the project:

    make

Run the program:

    ./bin/task4 data/input.txt data/output.txt

Clean build files:

    make clean

## Project Structure

    tpmp-lab2-task4/
    ├── src/        # Source files (.c)
    ├── include/    # Header files (.h)
    ├── data/       # Input and output text files
    ├── docs/       # Test results and report
    ├── build/      # Object files
    ├── bin/        # Compiled executable
    ├── Makefile
    └── README.md

## Additional Notes

- The project follows the KIS project structure model.
- The program consists of multiple source files.
- Compilation is performed using `gcc` with flags:
  - `-Wall`
  - `-Wextra`
  - `-Wpedantic`
  - `-std=c11`
- Continuous Integration is configured in `.github/workflows/`.
- GitHub Actions automatically builds the project on push and pull request.
- Repl.it is not used for this assignment, as required.
- The test results (control calculations) are included in the `docs/` directory.
