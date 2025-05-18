#include "functions.h"

double idx(double x)  {return 3;}
func funcs[] = {idx, f1, f2, f3, df1, df2, df3};
// Функция для вычисления трёх точек пересечения функций f1,f2,f3 на заранее заданных интервалах
void calc_intersection(double *x1, double *x2, double *x3) {
    // Заранее известные интервалы, на которых расположены точки пересечения
    *x1 = root(f1, f3, df1, df3, -1.9, -0.2, EPSILON);
    *x2 = root(f2, f3, df2, df3, -1.0, -0.1, EPSILON);
    *x3 = root(f1, f2, df1, df2, 0.1,  1.0, EPSILON);
}


// ----------------------------------------------------------------------------------
//                                    main
// ----------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    int opt;
    struct option long_opts[] = {
        {"help", no_argument, 0, 'h'},
        {"root", no_argument, 0, 'r'},
        {"iterations", no_argument, 0, 'i'},
        {"test-root", required_argument, 0, 'R'},
        {"test-integral", required_argument, 0, 'I'},
        {0,0,0,0}
    };

    double x1, x2, x3;
    // Разбор ключей командной строки через getopt_long
    while ((opt = getopt_long(argc, argv, "hriR:I:", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                // Вывод справки по использованию
                printf("Usage:\n"
                       "  -h, --help              Show this help message\n"
                       "  -r, --root              Compute intersections of f1,f2,f3\n"
                       "  -i, --iterations        Display number of iterations used in last root search\n"
                       "  -R, --test-root=...     Test root (format f1:f2:a:b:eps:expected)\n"
                       "  -I, --test-integral=... Test integral (format f:a:b:eps:expected)\n");
                break;

            case 'r':
                // Вычисление трёх точек пересечения и их вывод
                calc_intersection(&x1, &x2, &x3);
                printf("    functions    \tx  \ty\n");
                printf("betwen f1 and f2 %.6lf, %.6lf\n", x3, f1(x3));
                printf("betwen f2 and f3 %.6lf, %.6lf\n", x2, f2(x2));
                printf("betwen f1 and f3 %.6lf, %.6lf\n", x1, f1(x1));
                break;

            case 'i':
                // Сначала вычисляем пересечения, затем показываем счётчик итераций
                calc_intersection(&x1, &x2, &x3);
                printf("root_iterations=%d\n", root_iterations);
                break;

            case 'R': {
                // Тестирование поиска корня по заданным параметрам
                int f1, f2;
                double a, b, eps, expval;
                sscanf(optarg, "%d:%d:%lf:%lf:%lf:%lf", &f1, &f2, &a, &b, &eps, &expval);
                test_root(f1, f2, a, b, eps, expval);
                break;
            }

            case 'I': {
                // Тестирование интеграла по заданным параметрам
                int f;
                double a, b, eps, expval;
                sscanf(optarg, "%d:%lf:%lf:%lf:%lf", &f, &a, &b, &eps, &expval);
                test_integral(f, a, b, eps, expval);
                break;
            }

            default:
                fprintf(stderr, "Unknown option\n");
                return 1;
        }
    }

    // Если программа запущена без аргументов, вычисляем площадь фигуры:
    if (argc == 1) {
        calc_intersection(&x1, &x2, &x3);
        double area = 0.0;
        // Площадь = интеграл (f1-f3) на [x1,x2] + интеграл (f2-f3) на [x2,x3]
        area += integral(f1, x1, x2, EPSILON) - integral(f3, x1, x2, EPSILON);
        area += integral(f2, x2, x3, EPSILON) - integral(f3, x2, x3, EPSILON);
        printf("area = %.6f\n", fabs(area));
    }

    return 0;
}
