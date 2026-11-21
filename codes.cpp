#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

// Structure to hold metrics for each sorting run
struct SortStats
{
  long long comparisons = 0;
  long long swaps = 0;
};

// 1. Selection Sort (In-Place, O(1) Auxiliary Space)
SortStats selectionSort(vector<int> &arr)
{
  SortStats stats;
  int n = static_cast<int>(arr.size());

  for (int i = 0; i < n - 1; ++i)
  {
    int minIdx = i;
    for (int j = i + 1; j < n; ++j)
    {
      stats.comparisons++;
      if (arr[j] < arr[minIdx])
      {
        minIdx = j;
      }
    }
    if (minIdx != i)
    {
      int temp = arr[i];
      arr[i] = arr[minIdx];
      arr[minIdx] = temp;
      stats.swaps++;
    }
  }
  return stats;
}

// 2. Bubble Sort with Early-Exit Optimization (In-Place, O(1) Auxiliary Space)
SortStats bubbleSort(vector<int> &arr)
{
  SortStats stats;
  int n = static_cast<int>(arr.size());

  for (int i = 0; i < n - 1; ++i)
  {
    bool swapped = false;
    for (int j = 0; j < n - i - 1; ++j)
    {
      stats.comparisons++;
      if (arr[j] > arr[j + 1])
      {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
        stats.swaps++;
        swapped = true;
      }
    }
    // Terminate early if array is already sorted
    if (!swapped)
      break;
  }
  return stats;
}

// Helper partition function for Quick Sort (middle-element pivot)
int partition(vector<int> &arr, int low, int high, SortStats &stats)
{
  int mid = low + (high - low) / 2;
  int pivot = arr[mid];

  // Move pivot to end temporarily
  int temp = arr[mid];
  arr[mid] = arr[high];
  arr[high] = temp;
  stats.swaps++;

  int i = low - 1;
  for (int j = low; j < high; ++j)
  {
    stats.comparisons++;
    if (arr[j] <= pivot)
    {
      ++i;
      if (i != j)
      {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
        stats.swaps++;
      }
    }
  }
  if (i + 1 != high)
  {
    int t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    stats.swaps++;
  }
  return i + 1;
}

void quickSortHelper(vector<int> &arr, int low, int high, SortStats &stats)
{
  if (low < high)
  {
    int pi = partition(arr, low, high, stats);
    quickSortHelper(arr, low, pi - 1, stats);
    quickSortHelper(arr, pi + 1, high, stats);
  }
}

// 3. Quick Sort (In-Place Partitioning, O(log n) avg / O(n) worst Stack Space)
SortStats quickSort(vector<int> &arr)
{
  SortStats stats;
  if (!arr.empty())
  {
    quickSortHelper(arr, 0, static_cast<int>(arr.size()) - 1, stats);
  }
  return stats;
}

// Helper merge function for Merge Sort
void merge(vector<int> &arr, int left, int mid, int right, SortStats &stats)
{
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // Temporary auxiliary arrays -> O(n) extra space
  vector<int> L(n1), R(n2);
  for (int i = 0; i < n1; ++i)
    L[i] = arr[left + i];
  for (int j = 0; j < n2; ++j)
    R[j] = arr[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2)
  {
    stats.comparisons++;
    if (L[i] <= R[j])
    {
      arr[k++] = L[i++];
    }
    else
    {
      arr[k++] = R[j++];
    }
    stats.swaps++; // Counts array write/copy operations
  }
  while (i < n1)
  {
    arr[k++] = L[i++];
    stats.swaps++;
  }
  while (j < n2)
  {
    arr[k++] = R[j++];
    stats.swaps++;
  }
}

void mergeSortHelper(vector<int> &arr, int left, int right, SortStats &stats)
{
  if (left < right)
  {
    int mid = left + (right - left) / 2;
    mergeSortHelper(arr, left, mid, stats);
    mergeSortHelper(arr, mid + 1, right, stats);
    merge(arr, left, mid, right, stats);
  }
}

// 4. Merge Sort (Not In-Place, O(n) Auxiliary Space)
SortStats mergeSort(vector<int> &arr)
{
  SortStats stats;
  if (!arr.empty())
  {
    mergeSortHelper(arr, 0, static_cast<int>(arr.size()) - 1, stats);
  }
  return stats;
}

// Generate test arrays
vector<int> generateArray(int size, bool reverseOrder)
{
  vector<int> arr(size);
  for (int i = 0; i < size; ++i)
  {
    arr[i] = reverseOrder ? (size - i) : (i + 1);
  }
  return arr;
}

// Benchmark runner (3 runs per case)
void runBenchmark(const string &algoName, SortStats (*sortFunc)(vector<int> &))
{
  struct TestCase
  {
    string label;
    int size;
    bool reverseOrder;
  };

  vector<TestCase> cases = {
      {"Size 5   | Sorted", 5, false},
      {"Size 5   | Reverse Sorted", 5, true},
      {"Size 100 | Sorted", 100, false},
      {"Size 100 | Reverse Sorted", 100, true}};

  for (const auto &tc : cases)
  {
    vector<int> base = generateArray(tc.size, tc.reverseOrder);
    double timesUs[3];
    SortStats lastStats;

    // Warmup once to stabilize CPU frequency/cache
    {
      vector<int> warm = base;
      sortFunc(warm);
    }

    for (int r = 0; r < 3; ++r)
    {
      vector<int> copyArr = base;
      auto start = high_resolution_clock::now();
      lastStats = sortFunc(copyArr);
      auto end = high_resolution_clock::now();
      timesUs[r] = duration<double, micro>(end - start).count();
    }

    double avgUs = (timesUs[0] + timesUs[1] + timesUs[2]) / 3.0;
    cout << algoName << "|" << tc.label
         << "|" << fixed << setprecision(3) << timesUs[0]
         << "|" << timesUs[1]
         << "|" << timesUs[2]
         << "|" << avgUs
         << "|" << lastStats.comparisons
         << "|" << lastStats.swaps << "\n";
  }
}

int main()
{
  runBenchmark("Selection Sort", selectionSort);
  runBenchmark("Bubble Sort", bubbleSort);
  runBenchmark("Quick Sort", quickSort);
  runBenchmark("Merge Sort", mergeSort);
  return 0;
}