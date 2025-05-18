#include "functions.h"

// ----------------------------------------------------------------------------------
//                              Функции для тестирования
// ----------------------------------------------------------------------------------
// Функция тестирования поиска корней: сравнивает результат с ожидаемым и выводит погрешность
void test_root(int f1_num, int f2_num, double a, double b, double eps, double expected) {
    int sdvg = funcs[0](0);
    double res = root(funcs[f1_num], funcs[f2_num], funcs[f1_num + sdvg], funcs[f2_num + sdvg], a, b, eps);
    double err = fabs(res - expected);
    double rel_err;
    if (expected - res < 0.000001 && expected - res > -0.000001) {
        rel_err = 0;
    } else {
        rel_err = err / fabs(expected) * 100.0;
    }
    printf("root=%.4f, error=%.4f (%.2f%%)\n", res, err, rel_err);
}
// Функция тестирования интеграла: аналогично, с вычислением относительной ошибки
void test_integral(int f_num, double a, double b, double eps, double expected) {
    double res = integral(funcs[f_num], a, b, eps);
    double err = fabs(res - expected);
    double rel_err;
    if (expected - res < 0.000001 && expected - res > -0.000001) {
        rel_err = 0;
    } else {
        rel_err = err / fabs(expected) * 100.0;
    }
    printf("integral=%.4f, error=%.4f (%.2f%%)\n", res, err, rel_err);
}

// ----------------------------------------------------------------------------------
//                             Поиск корня (root)
// ----------------------------------------------------------------------------------
int root_iterations = 0;  // Счётчик итераций (глобально, для опции --iterations)
/*
Комбинированный метод для поиска корня уравнения F(x)=f(x)-g(x)=0
Метод сочетает в себе метод секущих и метод Ньютона для эффективного поиска корня.
На каждом шаге:
1. Вычисляется середина интервала и производные в этой точке
2. В зависимости от знаков производных выбирается схема обновления границ
3. Применяется метод секущих для одной границы и метод Ньютона для другой
4. Процесс повторяется до достижения заданной точности
*/
double root(func f, func g, func df, func dg, double a, double b, double eps) {
    // Сбрасываем счётчик итераций при каждом вызове
    if (a > b) {
        double tmp = a;
        a = b;
        b = tmp;
    }

    double fa = f(a) - g(a);
    double fb = f(b) - g(b);

    // Проверка на наличие корня в граничных точках
    if (fa <= 0.000001 && fa > -0.000001) return a;
    if (fb <= 0.000001 && fb > -0.000001) return b;

    // Проверка условия существования корня на интервале
    if (fa * fb > 0) {
        printf("No root on interval [%.6f, %.6f]\n", a, b);
        exit(1);
    }

    // Переменные для середины, первой и второй производных F
    double mid, Fp, Fpp;
    int case_num;

    // Основной цикл: сужаем отрезок до тех пор, пока его длина > eps
    do {
        root_iterations++;

        // 1) Оценка кривизны: находим mid = (a+b)/2 и вычисляем F'(mid), F''(mid)
        mid = 0.5 * (a + b);
        Fp = df(mid) - dg(mid);
        
        // Вычисляем вторую производную численным методом
        double h = (b - a) * 0.01; // Малое смещение
        double Fp_left = df(mid - h) - dg(mid - h);
        double Fp_right = df(mid + h) - dg(mid + h);
        Fpp = (Fp_right - Fp_left) / (2 * h);

        // 2) Выбор схемы: если F'(mid) и F''(mid) одного знака → case 1, иначе case 2
        case_num = (Fp * Fpp > 0) ? 1 : 2;

        // 3) Шаг секущих (хорды) на текущем отрезке [a,b]
        //    формула секущей: x = (a*F(b) - b*F(a)) / (F(b) - F(a))
        double c_chord = (a * fb - b * fa) / (fb - fa);

        // 4) Шаг Ньютона (касательные) с точки d = (case==1 ? b : a)
        double d  = (case_num == 1 ? b : a);
        double Fd = f(d) - g(d);
        double Fpd = df(d) - dg(d);
        double c_newt = d - Fd / Fpd;

        // 5) Обновление границ:
        //    - case 1: [a,b] → [c_chord, c_newt]
        //    - case 2: [a,b] → [c_newt, c_chord]
        if (case_num == 1) {
            a = c_chord;
            b = c_newt;
        } else {
            a = c_newt;
            b = c_chord;
        }

        // 6) Вычисляем новые F(a), F(b) для следующей итерации
        fa = f(a) - g(a);
        fb = f(b) - g(b);

    } while (fabs(b - a) > eps);
    // Возвращаем середину последнего отрезка как приближение корня
    return 0.5 * (a + b);
}

// ----------------------------------------------------------------------------------
//                          Метод Симпсона (integral)
// ----------------------------------------------------------------------------------
/*
Формула Симпсона: I ≈ (h/3)[f(a) + 4f(a+h) + 2f(a+2h) + 4f(a+3h) + ... + f(b)]
*/
double integral(func f, double a, double b, double eps2) {
    const double p = 1.0 / 15.0; // коэффициент Рунге для Симпсона
    int n = 2; // начальное (минимальное) чётное число отрезков
    const int max_n = 5000000; // защита от бесконечного цикла

    double fa = f(a), fb = f(b);
    double h = (b - a) / n;

    // первоначальная «нечётная» точка и первое приближение
    double sum_odd = f(a + h);
    double I_n = (h / 3.0) * (fa + 4.0 * sum_odd + fb);

    while (1) {
        int n2 = n * 2;
        double h2 = (b - a) / n2;

        // суммируем f в новых «нечётных» точках сетки из 2n отрезков
        double sum_new_odd = 0.0;
        for (int i = 1; i < n2; i += 2) {
            // здесь i = 1,3,5,... — все нечётные узлы новой сетки
            sum_new_odd += f(a + i * h2);
        }

        // пересчитываем I для 2n отрезков:
        // старые «нечётные» узлы становятся «чётными» (коэффициент 2),
        // новые — остаются «нечётными» (коэффициент 4)
        double I_2n = (h2 / 3.0) * (fa + fb + 2.0 * sum_odd + 4.0 * sum_new_odd);

        // проверка по правилу Рунге
        if (p * fabs(I_n - I_2n) < eps2) {
            return I_2n;
        }

        // готовимся к следующей итерации
        sum_odd += sum_new_odd;  // все «нечётные» для 2n теперь в sum_odd
        I_n = I_2n;
        n = n2;

        if (n > max_n) {
            fprintf(stderr, "Error: maximum number of intervals exceeded (%d)\n", max_n);
            return I_2n;
        }
    }
}
