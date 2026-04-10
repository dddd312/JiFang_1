/*
 * 计算方法实践作业 - 第三章第三题
 * 使用Jacobi迭代法和Gauss-Seidel迭代法求解线性方程组
 * 方程组：
 *   10*x1 - 2*x2 -  x3 = 3
 *  -2*x1 + 10*x2 -  x3 = 15
 *  - x1 - 2*x2 + 5*x3 = 10
 * 初始值：x0 = [0, 0, 0]
 * 误差容限：1e-5
 * 比较两种方法的迭代次数
 */
#include <stdio.h>
#include <math.h>

#define N 3
#define MAX_ITER 1000
#define TOL 1e-5

/* 打印向量 */
void print_vector(double x[N], const char* name) {
    printf("%s = [", name);
    for (int i = 0; i < N; i++) {
        printf("%.10f", x[i]);
        if (i < N - 1) printf(", ");
    }
    printf("]\n");
}

/* Jacobi迭代法求解线性方程组
 * A: 系数矩阵, b: 右端向量, x: 初始猜测/输出解
 * 返回迭代次数 */
int jacobi(double A[N][N], double b[N], double x[N]) {
    double x_new[N];
    int iter = 0;
    
    while (iter < MAX_ITER) {
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    sum += A[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / A[i][i];
        }
        
        double max_diff = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = fabs(x_new[i] - x[i]);
            if (diff > max_diff) max_diff = diff;
            x[i] = x_new[i];
        }
        
        iter++;
        if (max_diff < TOL) {
            break;
        }
    }
    
    return iter;
}

/* Gauss-Seidel迭代法求解线性方程组
 * A: 系数矩阵, b: 右端向量, x: 初始猜测/输出解
 * 返回迭代次数 */
int gauss_seidel(double A[N][N], double b[N], double x[N]) {
    int iter = 0;
    
    while (iter < MAX_ITER) {
        double max_diff = 0.0;
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                if (i != j) {
                    sum += A[i][j] * x[j];
                }
            }
            double x_new = (b[i] - sum) / A[i][i];
            double diff = fabs(x_new - x[i]);
            if (diff > max_diff) max_diff = diff;
            x[i] = x_new;
        }
        
        iter++;
        if (max_diff < TOL) {
            break;
        }
    }
    
    return iter;
}

/* 主函数：初始化并调用两种迭代方法 */
int main() {

    double A[N][N] = {
        {10.0, -2.0, -1.0},
        {-2.0, 10.0, -1.0},
        {-1.0, -2.0, 5.0}
    };
    double b[N] = {3.0, 15.0, 10.0};
    double x_jacobi[N] = {0.0, 0.0, 0.0};
    double x_gs[N] = {0.0, 0.0, 0.0};
    
    printf("迭代法解线性方程组:\n");
    printf("系数矩阵A = [[10, -2, -1], [-2, 10, -1], [-1, -2, 5]]\n");
    printf("b = [3, 15, 10]\n");
    printf("初始向量: x0 = [0, 0, 0]\n");
    printf("容差: %.0e\n", TOL);
    printf("\n");
    

    int iter_jacobi = jacobi(A, b, x_jacobi);
    printf("Jacobi迭代法:\n");
    printf("迭代次数: %d\n", iter_jacobi);
    print_vector(x_jacobi, "解为：");
    printf("\n");
    
    int iter_gs = gauss_seidel(A, b, x_gs);
    printf("Gauss-Seidel迭代法:\n");
    printf("迭代次数: %d\n", iter_gs);
    print_vector(x_gs, "解为");
    printf("\n");
    
    return 0;
}