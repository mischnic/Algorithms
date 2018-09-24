#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

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

double timespecToSeconds(struct timespec *ts) {
	return (double)ts->tv_sec + (double)ts->tv_nsec / 1000000000.0;
}

constexpr int myabs(int i) {
	return i >= 0 ? i : -i;
}

// ------------------------------------------------

constexpr bool AllePermutationen_checkDiagonals(int n, board_t *solutionColumns) {
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (myabs(solutionColumns[j] - solutionColumns[i]) == j - i) {
				return false;
			}
		}
	}
	return true;
}

constexpr const int AllePermutationen(int n, board_t *solutionColumns, board_t missingColumns,
									  char rowToPlace) {
	int numOfSolutions = 0;
	for (int c = 0; c < n; c++) {		 // for every row ...
		if (getBit(missingColumns, c)) { // ... that doesn't have a queen
			solutionColumns[rowToPlace] = c + 1;
			if (rowToPlace < n - 1) {									 // not done yet
				board_t nextMissingColumns = missingColumns & ~(1 << c); // we've used this column
				numOfSolutions +=
					AllePermutationen(n, solutionColumns, nextMissingColumns, rowToPlace + 1);
			} else {
				if (AllePermutationen_checkDiagonals(n, solutionColumns)) { // done, but valid?
					return numOfSolutions + 1;
				} else {
					return numOfSolutions;
				}
			}
		}
	}
	return numOfSolutions;
}

// ------------------------------------------------

constexpr bool BegrenzteEnumeration_checkDiagonals(int rowToPlace, board_t *solutionColumns) {
	for (int j = 0; j < rowToPlace; j++) {
		if (myabs(solutionColumns[j] - solutionColumns[rowToPlace]) == rowToPlace - j) {
			return false;
		}
	}
	return true;
}

constexpr int BegrenzteEnumeration(int n, board_t *solutionColumns, board_t missingColumns,
						  char rowToPlace) {
	int numOfSolutions = 0;
	for (int c = 0; c < n; c++) {		 // for every row ...
		if (getBit(missingColumns, c)) { // ... that doesn't have a queen
			solutionColumns[rowToPlace] = c + 1;

			// are there diagonals already?
			const bool diagonalsOk = BegrenzteEnumeration_checkDiagonals(rowToPlace, solutionColumns);

			if (diagonalsOk) {
				if (rowToPlace < n - 1) {								 // not done yet
					const board_t nextMissingColumns = missingColumns & ~(1 << c); // we've used this column
					numOfSolutions += BegrenzteEnumeration(n, solutionColumns, nextMissingColumns, rowToPlace + 1);
				} else {
					return numOfSolutions + 1;
				}
			}
		}
	}
	return numOfSolutions;
}

// ------------------------------------------------

constexpr int run() {
	constexpr int n = 8;
	board_t solutionColumns[n] = {};
	constexpr board_t missingColumns = BOARD_MAX;

	return BegrenzteEnumeration(n, solutionColumns, missingColumns, 0);
	// return AllePermutationen(n, solutionColumns, missingColumns, 0);
}

// ------------------------------------------------

int main(int argv, char **args) {
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	constexpr int numOfSolutions = run();
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("Took %fms for %d solutions\n",
		   1000 * (timespecToSeconds(&end) - timespecToSeconds(&start)), numOfSolutions);

	return 0;
}