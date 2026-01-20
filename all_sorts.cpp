#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <windows.h>

using namespace std;

/*
Временная сложность:
- Вставка n элементов в multiset: n * O(log n) = O(n log n)
- Обход и копирование обратно: O(n)
Итого:
- Лучший случай:  O(n log n)
- Средний случай: O(n log n)
- Худший случай:  O(n log n)

Пространственная сложность:
- Хранит n элементов, память = O(n)
*/

void treeSort(int arr[], int n) {
    multiset<int> tree; // O(1) - создание структуры

    for (int i = 0; i < n; i++) // n итераций
        tree.insert(arr[i]); // O(n log n)
    int i = 0; // O(1)
    for (int val: tree) // обход дерева = O(n)
        arr[i++] = val; // O(1) на итерацию
}

/*
Временная сложность:
- Два вложенных цикла
- Число сравнений: n(n-1)/2

Итого:
- Лучший случай:  O(n²)
- Средний случай: O(n²)
- Худший случай:  O(n²)

Пространственная сложность:
- Используются только локальные переменные
- Дополнительная память: O(1)
*/
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        // n итераций
        int minIdx = i; // O(1)
        for (int j = i + 1; j < n; j++) // до n итераций
            if (arr[j] < arr[minIdx]) // O(1) сравнение
                minIdx = j; // O(1)
        if (minIdx != i)
            swap(arr[i], arr[minIdx]); // O(1)
    }
}

/*

n - размер массива
k - диапазон значений (maxVal - minVal + 1)

Временная сложность:
- Поиск min и max: O(n)
- Подсчёт элементов: O(n)
- Префиксные суммы: O(k)
- Формирование выходного массива: O(n)
- Копирование обратно: O(n)

Итого:
- Лучший случай:  O(n + k)
- Средний случай: O(n + k)
- Худший случай:  O(n + k)

Пространственная сложность:
- Массив count размером = O(k)
- Выходной массив output размером = O(n)
Итого по памяти: O(n + k)
*/
void countingSort(int arr[], int n) {
    if (n <= 0) return; // O(1)
    int minVal = *min_element(arr, arr + n); // O(n)
    int maxVal = *max_element(arr, arr + n); // O(n)

    int range = maxVal - minVal + 1; // O(1)

    vector<int> count(range, 0);  // O(k) — выделение памяти

    for (int i = 0; i < n; i++) // n итераций
        count[arr[i] - minVal]++;  // O(1)

    for (int i = 1; i < range; i++) // k итераций
        count[i] += count[i - 1]; // O(1)

    vector<int> output(n); // O(n) — выделение памяти

    for (int i = n - 1; i >= 0; i--) { // n итераций
        output[count[arr[i] - minVal] - 1] = arr[i]; // O(1)
        count[arr[i] - minVal]--; // O(1)
    }
    for (int i = 0; i < n; i++) // n итераций
        arr[i] = output[i]; // O(1)
}

/*
Временная сложность: O(n)
Пространственная сложность: O(1)
Проверка отсортированности
*/
bool isSorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)  // n итераций
        if (arr[i] > arr[i + 1]) return false; // O(1)
    return true;
}

// Генерация тестовых данных
int *generateBestCase(int n, int algorithm) {
    int *arr = new int[n];

    if (algorithm == 1) {
        // Selection Sort
        // Лучший, если отсортированный
        for (int i = 0; i < n; i++) arr[i] = i;
    } else if (algorithm == 2) {
        // Tree Sort
        // Лучший, если самосбалансируется
        for (int i = 0; i < n; i++) arr[i] = i;
    } else if (algorithm == 3) {
        // Counting Sort
        // Лучший, если малый диапазон значений
        for (int i = 0; i < n; i++) arr[i] = i % 100;
    }

    return arr;
}

int *generateAverageCase(int n) {
    int *arr = new int[n];
    srand(42);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
    return arr;
}

int *generateWorstCase(int n, int algorithm) {
    int *arr = new int[n];

    if (algorithm == 1) {
        // Худший, если отсортированный в обратном порядке
        for (int i = 0; i < n; i++) arr[i] = n - i - 1;
    } else if (algorithm == 2) {
        // Худший, если вырожденное дерево
        for (int i = 0; i < n; i++) arr[i] = i; // или обратный порядок
    } else if (algorithm == 3) {
        // Counting Sort
        // Худший, если огромный диапазон значений
        for (int i = 0; i < n; i++) {
            arr[i] = 1000000 + rand() % 1000000;
        }
    }

    return arr;
}

void testAlgorithm(void (*sortFunc)(int [], int), const char *name, int algorithm) {
    const int n = 100;
    bool allPassed = true;

    cout << "\nТестирование: " << name << "\n";

    // Лучший случай
    int *bestCase = generateBestCase(n, algorithm);
    sortFunc(bestCase, n);
    bool bestOk = isSorted(bestCase, n);
    cout << "Лучший случай:   " << (bestOk ? "Пройден" : "Не пройден") << endl;
    delete[] bestCase;

    // Средний случай
    int *avgCase = generateAverageCase(n);
    sortFunc(avgCase, n);
    bool avgOk = isSorted(avgCase, n);
    cout << "Средний случай: " << (avgOk ? "Пройден" : "Не пройден") << endl;
    delete[] avgCase;

    // 3. Худший случай
    int *worstCase = generateWorstCase(n, algorithm);
    sortFunc(worstCase, n);
    bool worstOk = isSorted(worstCase, n);
    cout << "Худший случай:  " << (worstOk ? "Пройден" : "Не пройден") << endl;
    delete[] worstCase;

    allPassed = bestOk && avgOk && worstOk;
    cout << "Итог: " << (allPassed ? "Все тесты пройдены" : "Есть проваленные тесты") << endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // Тестируем все три алгоритма
    testAlgorithm(selectionSort, "Selection Sort", 1);
    testAlgorithm(treeSort, "Tree Sort", 2);
    testAlgorithm(countingSort, "Counting Sort", 3);

    return 0;
}

/*
Selection Sort:
- Время: O(n²) во всех случаях
- Память: O(1)

Tree Sort:
- Время: O(n log n) во всех случаях
- Память: O(n)

Counting Sort:
- Время: O(n + k)
- Память: O(n + k),
  где k - диапазон значений элементов массива
*/
