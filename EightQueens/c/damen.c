#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// SET TO 1 TO PRINT OUT SOLUTIONS AS THEY'RE FOUND
#define PRINTOUT 0

// Utilities

#define getBit(a, i) (((a) >> (i)) & 1)
#define clearBit(a, i) a &= ~(1 << (i));
#define setBit(a, i) a |= 1 << (i);
#define setBitTo(a, i, v)                                                                          \
	if (v)                                                                                         \
		setBit(a, i);                                                                              \
	else                                                                                           \
		clearBit(a, i);

typedef uint_fast16_t board_t;
#define BOARD_MAX UINT_FAST16_MAX;

const double timespecToSeconds(const struct timespec *ts) {
	return (double)ts->tv_sec + (double)ts->tv_nsec / 1000000000.0;
}

void print(const int n, const board_t *solutionColumns) {
	for (int j = 0; j < n; j++) {
		printf("%c", 'A' - 1 + solutionColumns[j]);
	}
	printf("\n");

	// printf("-----------\n");
	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		printf("%c", solutionColumns[i] - 1 == j ? 'X' : '+');
	// 	}
	// 	printf("\n");
	// }
	// printf("-----------\n");
}

int numOfSolutions = 0;

// ------------------------------------------------

const bool AllePermutationen_checkDiagonals(const int n, const board_t *solutionColumns) {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (abs(solutionColumns[j] - solutionColumns[i]) == j - i) {
				return false;
			}
		}
	}
	return true;
}

void AllePermutationen(const int n, board_t *solutionColumns, board_t missingColumns,
					   const char rowToPlace) {
	for (int c = 0; c < n; c++) {		 // for every row ...
		if (getBit(missingColumns, c)) { // ... that doesn't have a queen
			solutionColumns[rowToPlace] = c + 1;
			if (rowToPlace < n - 1) {									 // not done yet
				board_t nextMissingColumns = missingColumns & ~(1 << c); // we've used this column
				AllePermutationen(n, solutionColumns, nextMissingColumns, rowToPlace + 1);
			} else {
				if (AllePermutationen_checkDiagonals(n, solutionColumns)) { // done, but valid?
					numOfSolutions++;
#if PRINTOUT
					print(n, solutionColumns);
#endif
				}
			}
		}
	}
}

// ------------------------------------------------

const bool BegrenzteEnumeration_checkDiagonals(const int rowToPlace,
											   const board_t *solutionColumns) {
	for (int j = 0; j < rowToPlace; j++) {
		if (abs(solutionColumns[j] - solutionColumns[rowToPlace]) == rowToPlace - j) {
			return false;
		}
	}
	return true;
}

void BegrenzteEnumeration(const int n, board_t *solutionColumns, board_t missingColumns,
						  const char rowToPlace) {
	for (int c = 0; c < n; c++) {		 // for every row ...
		if (getBit(missingColumns, c)) { // ... that doesn't have a queen
			solutionColumns[rowToPlace] = c + 1;

			// have we already created diagonals?
			const bool diagonalsOk =
				BegrenzteEnumeration_checkDiagonals(rowToPlace, solutionColumns);

			if (diagonalsOk) {
				if (rowToPlace < n - 1) { // not done yet
					const board_t nextMissingColumns =
						missingColumns & ~(1 << c); // we've used this column
					BegrenzteEnumeration(n, solutionColumns, nextMissingColumns, rowToPlace + 1);
				} else {
					numOfSolutions++;
#if PRINTOUT
					print(n, solutionColumns);
#endif
				}
			}
		}
	}
}

// ------------------------------------------------

void printUsage(char *name) {
	printf("Usage: %s n [algorithm] [print]\n"
		   "\t n: board size\n"
		   "\t algorithm: either 'AllePermutationen' or 'BegrenzteEnumeration'\n"
		   "\t print: print solutions (slower)? (noprint/print)\n",
		   name);
}

int main(int argv, char **args) {
	if (argv < 2) {
		printUsage(args[0]);
		return 1;
	}

	const int n = strtol(args[1], NULL, 10);
	if (n >= 16 || n == 0) {
		printf("Board too large (maximum is 15) or invalid n!");
		return 1;
	}

	const bool useAllePermutationen =
		args[2] == NULL ? false : (strncmp("AllePermutationen", args[2], 20) == 0);

	printf("Solving for n=%d using %s\n", n,
		   useAllePermutationen ? "AllePermutationen" : "BegrenzteEnumeration");

	board_t *solutionColumns = calloc(n, sizeof(board_t));
	const board_t missingColumns = BOARD_MAX;

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	if (useAllePermutationen) {
		AllePermutationen(n, solutionColumns, missingColumns, 0);
	} else {
		BegrenzteEnumeration(n, solutionColumns, missingColumns, 0);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("Took %fms for %d solutions\n",
		   1000 * (timespecToSeconds(&end) - timespecToSeconds(&start)), numOfSolutions);

	free(solutionColumns);
	return 0;
}