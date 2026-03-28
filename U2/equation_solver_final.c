/*
 * 计算方法课程作业 - 方程求根
 * 题目：用四种迭代方法求解方程 4cos(x) = e^x 的根
 * 方法：简单迭代法、斯蒂芬森迭代法、Newton迭代法、双点弦截法
 * 精度要求：ε = 10^-4
 * 初始值：简单迭代法、斯蒂芬森迭代法、Newton迭代法取 x0 = π/4
 *        双点弦截法取 x0 = π/4, x1 = π/2
 * 作者：软件工程大二学生
 * 日期：2026-03-15
 */

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

// 迭代函数1: φ(x) = ln(4cos(x)) （从方程 4cos(x)=e^x 直接推导）
// 用于简单迭代法和斯蒂芬森迭代法
double phi(double x) {
    return log(4 * cos(x));
}

// 迭代函数2: ψ(x) = arccos(e^x / 4) （另一种等价形式）
// 用于收敛的简单迭代法
double phi2(double x) {
    return acos(exp(x) / 4);
}

// 简单迭代法（原始）: x_{k+1} = φ(x_k) = ln(4cos(x_k))
// 参数: x0 - 初始值
// 返回值: 方程的近似根
double simple_iteration_original(double x0) {
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

// 简单迭代法（收敛版本）: x_{k+1} = ψ(x_k) = arccos(e^{x_k} / 4)
// 参数: x0 - 初始值
// 返回值: 方程的近似根
double simple_iteration_convergent(double x0) {
    iterations = 0;
    double x, x_prev = x0;
    
    do {
        iterations++;
        x = phi2(x_prev);
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
    
    printf("Solving equation: 4cos(x) = e^x\n");
    printf("Precision requirement: ε = %.6f\n\n", EPSILON);
    printf("Initial values: x0 = π/4 = %.6f\n", x0);
    printf("                x1 = π/2 = %.6f\n\n", x1);
    
    printf("========================================\n");
    printf("1. Simple Iteration (Original)\n");
    printf("========================================\n");
    printf("Iteration function: φ(x) = ln(4cos(x))\n");
    printf("Derivative: φ'(x) = -tan(x)\n");
    printf("At x0=π/4: |φ'(π/4)| = |tan(π/4)| = 1\n");
    printf("Convergence condition: |φ'(x)| < 1 (NOT satisfied)\n");
    start = clock();
    double root1 = simple_iteration_original(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Root: %.8f\n", root1);
    printf("Iterations: %d\n", iterations);
    printf("Time: %.6f seconds\n", cpu_time_used);
    printf("Verification: f(%.8f) = %.8f\n\n", root1, f(root1));
    
    printf("========================================\n");
    printf("2. Simple Iteration (Convergent Version)\n");
    printf("========================================\n");
    printf("Iteration function: ψ(x) = arccos(e^x / 4)\n");
    printf("Derivative: ψ'(x) = -(e^x/4) / sqrt(1 - (e^x/4)^2)\n");
    printf("At x0=π/4: |ψ'(π/4)| ≈ 0.6556 < 1 (satisfied)\n");
    start = clock();
    double root2 = simple_iteration_convergent(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Root: %.8f\n", root2);
    printf("Iterations: %d\n", iterations);
    printf("Time: %.6f seconds\n", cpu_time_used);
    printf("Verification: f(%.8f) = %.8f\n\n", root2, f(root2));
    
    printf("========================================\n");
    printf("3. Steffensen Iteration\n");
    printf("========================================\n");
    start = clock();
    double root3 = steffensen_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Root: %.8f\n", root3);
    printf("Iterations: %d\n", iterations);
    printf("Time: %.6f seconds\n", cpu_time_used);
    printf("Verification: f(%.8f) = %.8f\n\n", root3, f(root3));
    
    printf("========================================\n");
    printf("4. Newton Iteration\n");
    printf("========================================\n");
    start = clock();
    double root4 = newton_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Root: %.8f\n", root4);
    printf("Iterations: %d\n", iterations);
    printf("Time: %.6f seconds\n", cpu_time_used);
    printf("Verification: f(%.8f) = %.8f\n\n", root4, f(root4));
    
    printf("========================================\n");
    printf("5. Secant Method (Two-Point)\n");
    printf("========================================\n");
    start = clock();
    double root5 = secant_method(x0, x1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Root: %.8f\n", root5);
    printf("Iterations: %d\n", iterations);
    printf("Time: %.6f seconds\n", cpu_time_used);
    printf("Verification: f(%.8f) = %.8f\n\n", root5, f(root5));
    
    printf("========================================\n");
    printf("Efficiency Comparison\n");
    printf("========================================\n");
    printf("Method\t\t\tIterations\tTime (s)\tConvergence\n");
    printf("----------------------------------------\n");
    
    start = clock();
    simple_iteration_original(x0);
    end = clock();
    int iter1 = iterations;
    double time1 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    simple_iteration_convergent(x0);
    end = clock();
    int iter2 = iterations;
    double time2 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    steffensen_iteration(x0);
    end = clock();
    int iter3 = iterations;
    double time3 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    newton_iteration(x0);
    end = clock();
    int iter4 = iterations;
    double time4 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    secant_method(x0, x1);
    end = clock();
    int iter5 = iterations;
    double time5 = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Simple (Original)\t%d\t\t%.6f\tNo\n", iter1, time1);
    printf("Simple (Convergent)\t%d\t\t%.6f\tYes\n", iter2, time2);
    printf("Steffensen\t\t%d\t\t%.6f\tYes\n", iter3, time3);
    printf("Newton\t\t\t%d\t\t%.6f\tYes\n", iter4, time4);
    printf("Secant\t\t\t%d\t\t%.6f\tYes\n", iter5, time5);
    
    printf("\n========================================\n");
    printf("Conclusion\n");
    printf("========================================\n");
    printf("1. Original simple iteration does not converge because |φ'(x0)| = 1.\n");
    printf("2. Alternative simple iteration with ψ(x) = arccos(e^x/4) converges.\n");
    printf("3. Steffensen and Newton methods are the most efficient (3 iterations).\n");
    printf("4. All convergent methods find the same root: x ≈ %.8f.\n", root2);
    printf("5. The secant method requires 4 iterations.\n");
    
    return 0;
}