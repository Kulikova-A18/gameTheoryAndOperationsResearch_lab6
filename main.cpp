#include <string>

#include <iostream>
#include <math.h>
#include <array>
#include <vector>
#include <iomanip>
#include <algorithm>

const int N = 10;

namespace gameTheoryAndOperationsResearch
{

// Умножение матрицы на вектор
// Принимает матрицу размерности NxN и вектор размерности N, возвращает результат умножения - вектор размерности N
std::array<double, N> matrix_vector_multiplication(std::array<std::array<double, N>, N> &Matr, std::array<double, N> &Vec)
{
    std::array<double, N> Vecout; // Результирующий вектор
    for (int i = 0; i < N; i++)
    {
        Vecout[i] = 0; // Инициализация элемента результирующего вектора
        for (int j = 0; j < N; j++)
            Vecout[i] += Matr[i][j] * Vec[j]; // Умножение и суммирование элементов для получения нового значения
    }
    return Vecout; // Возвращение результирующего вектора
}


// Умножение матрицы на матрицу
// Принимает две матрицы размерности NxN и возвращает произведение этих матриц
std::array<std::array<double, N>, N> matrix_matrix_multiplication(std::array<std::array<double, N>, N> &Matr1, std::array<std::array<double, N>, N> &Matr2)
{
    std::array<std::array<double, N>, N> MatrOut; // Результирующая матрица
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            MatrOut[i][j] = 0; // Инициализация элемента результирующей матрицы
            for (int k = 0; k < N; k++)
                MatrOut[i][j] += Matr1[i][k] * Matr2[k][j]; // Умножение и суммирование элементов для получения нового значения
        }
    }
    return MatrOut; // Возвращение результирующей матрицы
}


// Печать матрицы
void print(std::array<std::array<double, N>, N> &Matr)
{
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    for (auto &pos : Matr)
    {
        for (auto pos2 : pos)
            std::cout << std::setw(6) << pos2;
        std::cout << std::endl;

    }
}

// Печать вектора
void print(std::array<double, N> &Vec)
{
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    for (auto pos : Vec)
        std::cout << std::setw(8) << pos;
    std::cout << std::endl;
}

// Функция для сравнения двух векторов с заданной точностью
// Принимает два вектора, и значение точности e
bool comparison_of_accuracy_vectors(std::array<double, N>& Vec1, std::array<double, N>& Vec2, double e)
{
    for (int i = 0; i < N; i++)
        if (fabs(Vec1[i] - Vec2[i]) > e)
            return false; // Если разница между элементами больше e, точность не достигнута
    return true; // Если все элементы удовлетворяют условию, точность достигнута
}


double rand_A_B(double a, double b) // Генерация псевдослучайного числа в интервале [a, b)
{
    return ((double)rand() / RAND_MAX)*(b - a) + a;
}

// Функция для моделирования игры с агентами влияния
// Принимает матрицу взаимодействий, вектор мнений, индексы агентов влияния первого и второго игроков
std::array<double, N> influencer_games(std::array<std::array<double, N>, N>& Matr, std::array<double, N>& Vec,
    std::vector<size_t>& index1, std::vector<size_t>& index2)
{
    std::array<double, N> Vecout; // Выходной вектор мнений
    std::vector<size_t> index0; // Индексы пользователей не являющихся агентами

    // Получение индексов пользователей не являющихся агентами
    for (int i = 0; i < N; i++) {
        if (find(index1.begin(), index1.end(), i) != index1.end()) continue;
        if (find(index2.begin(), index2.end(), i) != index2.end()) continue;
        index0.push_back(i);
    }

    // Инициализация выходного вектора нулями
    for (int i = 0; i < N; i++)
        Vecout[i] = 0;

    // Вычисление влияния агентов первого игрока на мнения
    for (auto pos : index1)
        for (int i = 0; i < N; i++)
            Vecout[pos] += Matr[pos][i] * Vec[i];

    // Вычисление влияния агентов второго игрока на мнения
    for (auto pos : index2)
        for (int i = 0; i < N; i++)
            Vecout[pos] += Matr[pos][i] * Vec[i];

    // Вычисление влияния пользователей не являющихся агентами на мнения
    for (auto pos : index0)
        for (int i = 0; i < N; i++)
            Vecout[pos] += Matr[pos][i] * Vec[i];

    return Vecout; // Возврат результирующего вектора мнений
}


int _main(int argc, char* argv[])
{
    std::array<std::array<double, N>, N> A; // Матрица А

    srand(43); // Устанавливаем начальное значение для генератора случайных чисел

    std::cout << "[Trust Matrix]\n"; // Выводим заголовок матрицы

    for (int i = 0; i < N; i++)
    {
        double ost = 1, step = 0.1; // Инициализация остатка и шага для распределения вероятностей
        for (int j = 0; j < N - 1; j++)
        {
            A[i][j] = rand_A_B(0.001, step * 2 - 0.001); // Заполняем элементы матрицы случайными значениями
            ost -= A[i][j]; // Вычитаем текущее значение из остатка
            step = ost / (N - 1 - j); // Рассчитываем новый шаг
        }
        A[i][N - 1] = ost; // Заполняем последний элемент строки для обеспечения суммы равной 1
    }

    print(A);

    std::array<std::array<double, N>, N> ItogMatr = A; // Итоговая матрица, инициализируется начальной матрицей A
    std::array<double, N> X0; // Начальный вектор

    for (int i = 0; i < N; i++)
        X0[i] = rand_A_B(1, 20); // Заполняем начальный вектор случайными значениями от 1 до 20

    std::cout << "[Initial opinions (X0)]\n"; // Вывод заголовка для начального вектора
    print(X0); // Вывод начального вектора

    std::array<double, N> X = X0, XOld; // Вектора для хранения текущего и предыдущего состояний
    int n = 0; // Счетчик итераций

    do // Цикл перехода из состояния в состояние
    {
        XOld = X; // Сохраняем текущий вектор
        X = matrix_vector_multiplication(A, X); // Вычисляем новый вектор
        ItogMatr = matrix_matrix_multiplication(ItogMatr, A); // Умножаем итоговую матрицу на матрицу A
        n++; // Увеличиваем счетчик итераций
    } while (!comparison_of_accuracy_vectors(X, XOld, 1e-6)); // Проверяем точность совпадения векторов

    std::cout << "Number of iterations n:   " << n << std::endl; // Вывод количества итераций
    std::cout << "Final opinions after n iterations of X(n):    " << std::endl; // Вывод финального вектора после n итераций
    print(X); // Вывод финального вектора X

    std::cout << "[Final matrix (all rows must be the same)]\n"; // Вывод заголовка для финальной матрицы
    print(ItogMatr); // Вывод финальной матрицы

    std::cout<<std::endl; // Пустая строка для отделения вывода

    // Далее игра с информационным влиянием
    std::cout<<"[Game with information influence]\n"; // Вывод заголовка для игры с информационным влиянием

    double u = rand_A_B(10, 100); // Управление для агентов 1-го игрока
    double v = -rand_A_B(10, 100); // Управление для агентов 2-го игрока

    std::cout << "Management for agents of influence of the first player u: " << u << std::endl; // Вывод управления для агентов 1-го игрока
    std::cout << "Control for agents of influence of the second player v:   " << v << std::endl; // Вывод управления для агентов 2-го игрока

    // Назначаем агентов влияния
    std::vector<size_t> index1 = { 1, 2 };  // Индексы агентов влияния 1-го игрока
    std::vector<size_t> index2 = { 3, 4 }; // Индексы агентов влияния 2-го игрока


    // Вывод индексов агентов влияния первого игрока
    std::cout << "Indices of agents of influence of the first player:   ";
    for (auto pos : index1)
        std::cout << pos << ' ';
    std::cout << std::endl;

    // Вывод индексов агентов влияния второго игрока
    std::cout << "Indices of agents of influence of the second player:  ";
    for (auto pos : index2)
        std::cout << pos << ' ';
    std::cout << std::endl;

    // Присвоение управлений агентам влияния
    for (auto pos : index1)
        X0[pos] = u;
    for (auto pos : index2)
        X0[pos] = v;

    std::cout << "[Initial opinions (X0)]\n";
    print(X0); // Вывод начальных мнений

    X = X0; // Присвоение начальных мнений
    n = 0; // Счетчик итераций
    do
    {
        XOld = X; // Сохранение предыдущего состояния мнений
        X = influencer_games(A, X, index1, index2); // Выполнение игры с информационным влиянием
        n++; // Увеличение счетчика итераций
    } while (!comparison_of_accuracy_vectors(X, XOld, 1e-6)); // Проверка на точность сходимости

    std::cout << "Number of iterations n:   " << n << std::endl; // Вывод количества итераций
    std::cout << "[Final opinions after n iterations X(n)]\n"; // Вывод финальных мнений после n итераций
    print(X); // Вывод финальных мнений

    // Определение победителя
    (X[0] >= 0) ?
        std::cout << "The first player wins" :
        std::cout << "The second player wins";

    return 0;
}

}


int main(int argc, char* argv[])
{
    gameTheoryAndOperationsResearch::_main(argc, argv);
    return 0;
}
