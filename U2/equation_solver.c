#include <stdio.h>
#include <math.h>
#include <time.h>

// 圆周率常量
#define PI 3.14159265358979323846
// 精度要求
#define EPSILON 1e-4

// 全局变量：记录迭代次数
int iterations;

// 目标函数: f(x) = 4cos(x) - e^x
double f(double x) {
    return 4 * cos(x) - exp(x);
}

// 目标函数的导数: f'(x) = -4sin(x) - e^x
double df(double x) {
    return -4 * sin(x) - exp(x);
}

// 迭代函数: φ(x) = ln(4cos(x))
// 用于简单迭代法和斯蒂芬森迭代法
double phi(double x) {
    return log(4 * cos(x));
}

// 简单迭代法: x_{k+1} = φ(x_k) = ln(4cos(x_k))
// 参数: x0 - 初始值
// 返回值: 方程的近似根
double simple_iteration(double x0) {
    iterations = 0;
    double x, x_prev = x0;
    
    do {
        iterations++;
        x = phi(x_prev);
        if (fabs(x - x_prev) < EPSILON) {
            return x;
        }
        x_prev = x;
    } while (iterations < 1000);
    
    return x;
}

// 斯蒂芬森迭代法: 对简单迭代法的加速
// 迭代公式: x_{k+1} = x_k - [φ(x_k) - x_k]^2 / [φ(φ(x_k)) - 2φ(x_k) + x_k]
// 参数: x0 - 初始值
// 返回值: 方程的近似根
double steffensen_iteration(double x0) {
    iterations = 0;
    double x, x_prev = x0;
    
    do {
        iterations++;
        double phi_x = phi(x_prev);
        double phi_phi_x = phi(phi_x);
        
        x = x_prev - pow(phi_x - x_prev, 2) / (phi_phi_x - 2 * phi_x + x_prev);
        
        if (fabs(x - x_prev) < EPSILON) {
            return x;
        }
        x_prev = x;
    } while (iterations < 1000);
    
    return x;
}

// Newton迭代法: x_{k+1} = x_k - f(x_k)/f'(x_k)
// 具有二次收敛性
// 参数: x0 - 初始值
// 返回值: 方程的近似根
double newton_iteration(double x0) {
    iterations = 0;
    double x, x_prev = x0;
    
    do {
        iterations++;
        double fx = f(x_prev);
        double dfx = df(x_prev);
        
        if (fabs(dfx) < 1e-10) {
            printf("Warning: Derivative too small\n");
            return x_prev;
        }
        
        x = x_prev - fx / dfx;
        
        if (fabs(x - x_prev) < EPSILON) {
            return x;
        }
        x_prev = x;
    } while (iterations < 1000);
    
    return x;
}

// 双点弦截法: 使用两点间的线性逼近
// 迭代公式: x_{k+1} = x_k - f(x_k) * (x_k - x_{k-1}) / (f(x_k) - f(x_{k-1}))
// 具有超线性收敛性
// 参数: x0, x1 - 两个初始值
// 返回值: 方程的近似根
double secant_method(double x0, double x1) {
    iterations = 0;
    double x, x_prev1 = x0, x_prev2 = x1;
    
    do {
        iterations++;
        double fx1 = f(x_prev1);
        double fx2 = f(x_prev2);
        
        if (fabs(fx2 - fx1) < 1e-10) {
            printf("Warning: Function values too close\n");
            return x_prev2;
        }
        
        x = x_prev2 - fx2 * (x_prev2 - x_prev1) / (fx2 - fx1);
        
        if (fabs(x - x_prev2) < EPSILON) {
            return x;
        }
        
        x_prev1 = x_prev2;
        x_prev2 = x;
    } while (iterations < 1000);
    
    return x;
}

// 主函数：测试四种迭代方法并比较效率
int main() {
    double x0 = PI / 4;
    double x1 = PI / 2;
    
    clock_t start, end;
    double cpu_time_used;
    
    printf("求解方程: 4cos(x) = e^x\n");
    printf("精度要求: ε = %.6f\n\n", EPSILON);
    printf("初始值: x0 = π/4 = %.6f\n", x0);
    printf("       x1 = π/2 = %.6f\n\n", x1);
    
    printf("========================================\n");
    printf("1. 简单迭代法\n");
    printf("========================================\n");
    start = clock();
    double root1 = simple_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root1);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root1, f(root1));
    
    printf("========================================\n");
    printf("2. 斯蒂芬森迭代法\n");
    printf("========================================\n");
    start = clock();
    double root2 = steffensen_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root2);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root2, f(root2));
    
    printf("========================================\n");
    printf("3. Newton迭代法\n");
    printf("========================================\n");
    start = clock();
    double root3 = newton_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root3);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root3, f(root3));
    
    printf("========================================\n");
    printf("4. 双点弦截法\n");
    printf("========================================\n");
    start = clock();
    double root4 = secant_method(x0, x1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root4);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root4, f(root4));
    
    printf("========================================\n");
    printf("方法效率比较\n");
    printf("========================================\n");
    printf("方法名称\t\t迭代次数\t计算时间\n");
    printf("----------------------------------------\n");
    
    start = clock();
    simple_iteration(x0);
    end = clock();
    int iter1 = iterations;
    double time1 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    steffensen_iteration(x0);
    end = clock();
    int iter2 = iterations;
    double time2 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    newton_iteration(x0);
    end = clock();
    int iter3 = iterations;
    double time3 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    secant_method(x0, x1);
    end = clock();
    int iter4 = iterations;
    double time4 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("简单迭代法\t\t%d\t\t%.6f\n", iter1, time1);
    printf("斯蒂芬森迭代法\t\t%d\t\t%.6f\n", iter2, time2);
    printf("Newton迭代法\t\t%d\t\t%.6f\n", iter3, time3);
    printf("双点弦截法\t\t%d\t\t%.6f\n", iter4, time4);
    
    return 0;
}
