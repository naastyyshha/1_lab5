#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <random>


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

// Замер времени: сортируем копию массива, исходный не трогаем
double measureTime(void (*sortFunc)(int[], int), const int* arr, int n) {
    vector<int> testArr(arr, arr + n);

    auto start = high_resolution_clock::now();
    sortFunc(testArr.data(), n);
    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;
    return elapsed.count();
}

// Генерация случайного массива
vector<int> generateRandomArray(int n, mt19937& gen, int minVal = 0, int maxVal = 10000) {
    uniform_int_distribution<int> dist(minVal, maxVal);
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = dist(gen);
    return arr;
}

void generateBoxPlotData(int n, const string& filename, int runs = 50) {
    ofstream csvFile(filename);
    if (!csvFile) {
        cerr << "Ошибка: не удалось открыть файл для записи: " << filename << "\n";
        return;
    }

    csvFile << "Запуск,Сортировка_выбором,Сортировка_деревом,Сортировка_подсчётом\n";

    cout << "Генерация данных для boxplot: n = " << n << ", запусков = " << runs << "\n";

    mt19937 gen(42); // фиксированный seed для повторяемости

    for (int run = 0; run < runs; run++) {
        vector<int> arr = generateRandomArray(n, gen, 0, 10000);

        double timeSelection = measureTime(selectionSort, arr.data(), n);
        double timeTree = measureTime(treeSort, arr.data(), n);
        double timeCounting = measureTime(countingSort, arr.data(), n);

        csvFile << (run + 1) << ","
                << timeSelection << ","
                << timeTree << ","
                << timeCounting << "\n";
    }

    cout << "Данные сохранены в файл: " << filename << "\n\n";
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    const int RUNS = 50;

    generateBoxPlotData(10000, "boxplot_10k.csv", RUNS);
    generateBoxPlotData(100000, "boxplot_100k.csv", RUNS);

    cout << "Все данные для boxplot успешно сгенерированы\n";

    return 0;
}
