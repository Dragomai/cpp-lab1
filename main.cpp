#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Структура для возврата результата минимального разреза
struct MinCut {
    int weight;
    vector<int> vertices;
};

// Функция для поиска глобального минимального разреза алгоритмом Шрёдера — Вагнера
MinCut stoerWagner(int n, vector<vector<int>>& weights) {
    vector<vector<int>> vertices(n);
    for (int i = 0; i < n; ++i) {
        vertices[i] = {i};
    }

    int best_weight = INF;
    vector<int> best_cut;

    for (int phase = 0; phase < n - 1; ++phase) {
        vector<int> w(n, 0);
        vector<bool> added(n, false);
        vector<bool> in_set(n, true);

        // Помечаем вершины, которые уже были объединены
        for (int i = 0; i < n; ++i) {
            if (vertices[i].empty()) {
                in_set[i] = false;
            }
        }

        int prev = -1, last = -1;
        
        // Находим порядок добавления вершин (Maximum Adjacency Search)
        for (int i = 0; i < n - phase; ++i) {
            int sel = -1;
            for (int j = 0; j < n; ++j) {
                if (in_set[j] && !added[j] && (sel == -1 || w[j] > w[sel])) {
                    sel = j;
                }
            }

            if (sel == -1) break;

            added[sel] = true;
            prev = last;
            last = sel;

            for (int j = 0; j < n; ++j) {
                if (in_set[j] && !added[j]) {
                    w[j] += weights[sel][j];
                }
            }
        }

        // Вес разреза последней добавленной вершины
        int cut_weight = w[last];

        if (cut_weight < best_weight) {
            best_weight = cut_weight;
            best_cut = vertices[last];
        }

        // Объединение вершин last и prev
        if (prev != -1) {
            for (int j = 0; j < n; ++j) {
                weights[prev][j] += weights[last][j];
                weights[j][prev] = weights[prev][j];
            }
            vertices[prev].insert(vertices[prev].end(), vertices[last].begin(), vertices[last].end());
            vertices[last].clear();
            weights[last][last] = 0;
        }
    }

    return {best_weight, best_cut};
}

int main() {
    // Установка локализации для корректного вывода кириллицы
    setlocale(LC_ALL, "Russian");

    cout << "==================================================" << endl;
    cout << "     АЛГОРИТМ ШТЁРА — ВАГНЕРА (ПОИСК МИН. РАЗРЕЗА)" << endl;
    cout << "==================================================" << endl << endl;

    // ШАГ 1: Ввод размеров матрицы
    int rows, cols;
    cout << "[ШАГ 1] Введите размер матрицы смежности." << endl;
    cout << "Введите количество строк и столбцов через пробел (например: 4 4): ";
    cin >> rows >> cols;

    // Проверка на корректность и квадратность матрицы
    if (rows <= 1 || cols <= 1) {
        cout << endl << "[ОШИБКА] Размер матрицы должен быть больше 1!" << endl;
        return 0;
    }

    if (rows != cols) {
        cout << endl << "[ОШИБКА] Матрица смежности должна быть квадратной (строк = столбцов)!" << endl;
        return 0;
    }

    int n = rows; // Размер матрицы равен количеству вершин

    // ШАГ 2: Ввод весов рёбер (матрицы смежности)
    cout << endl << "--------------------------------------------------" << endl;
    cout << "[ШАГ 2] Ввод весов рёбер (матрица " << n << "x" << n << ")." << endl;
    cout << "Введите " << n * n << " чисел(а) — по " << n << " чисел для каждой строки." << endl;
    cout << "(0 означает отсутствие ребра или петлю в вершине)" << endl;
    cout << "--------------------------------------------------" << endl;

    vector<vector<int>> weights(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        cout << "Введите строку " << (i + 1) << " (для вершины " << i << "): ";
        for (int j = 0; j < n; ++j) {
            cin >> weights[i][j];
        }
    }

    // РАСЧЁТ
    cout << endl << "Выполняется расчет минимального разреза..." << endl;
    MinCut result = stoerWagner(n, weights);

    // ВЫВОД РЕЗУЛЬТАТА
    cout << endl << "==================================================" << endl;
    cout << "                    РЕЗУЛЬТАТ                     " << endl;
    cout << "==================================================" << endl;
    cout << "Минимальный вес разреза: " << result.weight << endl;
    cout << "Вершины в первой части разреза: ";
    for (int v : result.vertices) {
        cout << v << " ";
    }
    cout << endl << "==================================================" << endl;

    return 0;
}