#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <windows.h>

using namespace std;
using namespace std::chrono;

void treeSort(int arr[], int n) {
    multiset<int> tree;
    for (int i = 0; i < n; i++) tree.insert(arr[i]);
    int i = 0;
    for (int val : tree) arr[i++] = val;
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIdx]) minIdx = j;
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
}

void countingSort(int arr[], int n) {
    if (n <= 0) return;
    int minVal = *min_element(arr, arr + n);
    int maxVal = *max_element(arr, arr + n);
    int range = maxVal - minVal + 1;
    vector<int> count(range, 0);
    for (int i = 0; i < n; i++) count[arr[i] - minVal]++;
    for (int i = 1; i < range; i++) count[i] += count[i - 1];
    vector<int> output(n);
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
}

// Функция для замера времени сортировки
double measureTime(void (*sortFunc)(int[], int), int arr[], int n) {
    // Создаем копию массива, чтобы не портить оригинал
    int* testArr = new int[n];
    copy(arr, arr + n, testArr);
    
    auto start = high_resolution_clock::now();
    sortFunc(testArr, n);
    auto end = high_resolution_clock::now();
    
    delete[] testArr;
    
    duration<double> duration = end - start;
    return duration.count();
}

// Генерация случайного массива
int* generateArray(int n, int minVal = 0, int maxVal = 10000) {
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = minVal + rand() % (maxVal - minVal + 1);
    }
    return arr;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    const int START_SIZE = 1000;
    const int END_SIZE = 1000000;
    const int STEP = 1000;
    srand(42);
    
    ofstream csvFile("sorting_performance.csv");
    if (!csvFile) {
        cerr << "Ошибка: не удалось создать файл sorting_performance.csv\n";
        return 1;
    }
    csvFile << "РазмерМассива,СортировкаВыбором,СортировкаДеревом,СортировкаПодсчётом\n";

    cout << "Диапазон размеров: от " << START_SIZE << " до " << END_SIZE
         << " с шагом " << STEP << "\n";

    for (int size = START_SIZE; size <= END_SIZE; size += STEP) {
        int* arr = generateArray(size, 0, 10000);

        double timeSelection = measureTime(selectionSort, arr, size);
        double timeTree = measureTime(treeSort, arr, size);
        double timeCounting = measureTime(countingSort, arr, size);

        csvFile << size << ","
                << timeSelection << ","
                << timeTree << ","
                << timeCounting << "\n";

        if (size % 10000 == 0) {
            cout << "Обработан размер: " << size
                 << " (выбором: " << timeSelection << " c, "
                 << "деревом: " << timeTree << " c, "
                 << "подсчётом: " << timeCounting << " c)\n";
        }

        delete[] arr;
    }

    csvFile.close();

    cout << "\nДанные сохранены в файл: sorting_performance.csv\n";
    cout << "Всего точек данных: " << ((END_SIZE - START_SIZE) / STEP + 1) << "\n";

    return 0;
}