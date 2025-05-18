#include "functions.h"

// ----------------------------------------------------------------------------------
//                              Тестовые функции
// ----------------------------------------------------------------------------------
double idx(double x)  {return 4;}
double test_f1(double x)  {return x*x + 5*x + 6;}
double test_df1(double x) {return 2*x + 5;}
double test_f2(double x)  {return sin(x);}
double test_df2(double x) {return cos(x);}
double test_f3(double x)  {return x*x*x - 1;}
double test_df3(double x) {return 3*x*x;}
double test_f4(double x)  {return 0;}
double test_df4(double x) {return 0;}
func funcs[] = {idx, test_f1, test_f2, test_f3, test_f4, test_df1, test_df2, test_df3, test_df4};

// ----------------------------------------------------------------------------------
//                     Основная функция тестового режима
// ----------------------------------------------------------------------------------
int main(void) {
    printf("Test1\n");
    test_root(1, 4, -2.5, -1.5, EPSILON, -2);
    test_root(1, 4, -3.5, -2.5, EPSILON, -3);
    test_integral(1, -4, -2, EPSILON, 0.666666);
    printf("Test2\n");
    test_root(2, 4, 3, 4, EPSILON, M_PI);
    test_integral(2, 0, M_PI, EPSILON, 2);
    printf("Test3\n");
    test_root(3, 4, 0, 1, EPSILON, 1);
    test_integral(3, 0, 2, EPSILON, 2);
    return 0;
}
