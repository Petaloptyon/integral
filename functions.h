#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

#define EPSILON 0.000001
typedef double (*func)(double);
extern func funcs[];
double idx(double x);

#ifdef TEST_MODE
double test_f1(double x);
double test_f2(double x);
double test_f3(double x);
double test_df1(double x);
double test_df2(double x);
double test_df3(double x);
double test_f4(double x);
double test_df4(double x);

#else
extern double f1(double x);
extern double f2(double x);
extern double f3(double x);
extern double df1(double x);
extern double df2(double x);
extern double df3(double x);
#endif

double root(func f, func g, func df, func dg, double a, double b, double eps);
double integral(func f, double a, double b, double eps);
void calc_intersection(double *x1, double *x2, double *x3);
void test_root(int f1_num, int f2_num, double a, double b, double eps, double expected);
void test_integral(int f_num, double a, double b, double eps, double expected);
extern int root_iterations;

#endif
