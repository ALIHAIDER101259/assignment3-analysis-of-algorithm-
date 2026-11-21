# Empirical Analysis of Sorting Algorithms

From-scratch C++ implementation and empirical time/space comparison of **Selection Sort**, **Bubble Sort**, **Quick Sort**, and **Merge Sort** across different array sizes (`n = 5`, `n = 100`) and input orders (Sorted & Reverse Sorted), averaged over 3 runs per case.

## Algorithms Implemented
- **Selection Sort** — In-place, `O(1)` auxiliary space
- **Bubble Sort** (with early-exit flag) — In-place, `O(1)` auxiliary space
- **Quick Sort** (middle-element pivot) — In-place partitioning, `O(log n)` average call-stack space
- **Merge Sort** — Not in-place, `O(n)` auxiliary space for temporary sub-arrays

## Test Cases (16 Total Runs)
- `n = 5`, Sorted `[1, 2, 3, 4, 5]` (3 runs)
- `n = 5`, Reverse Sorted `[5, 4, 3, 2, 1]` (3 runs)
- `n = 100`, Sorted `[1, 2, ..., 100]` (3 runs)
- `n = 100`, Reverse Sorted `[100, 99, ..., 1]` (3 runs)

