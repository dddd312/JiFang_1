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
            printf("Warning: 导数过小\n");
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
            printf("Warning: 函数值过于接近\n");
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
    printf("精度: ε = %.6f\n\n", EPSILON);
    printf("初始值: x0 = π/4 = %.6f\n", x0);
    printf("       x1 = π/2 = %.6f\n\n", x1);
    
    printf("========================================\n");
    printf("1. 简单迭代法（原始）\n");
    printf("========================================\n");
    printf("迭代函数: φ(x) = ln(4cos(x))\n");
    printf("导数: φ'(x) = -tan(x)\n");
    printf("在 x0=π/4: |φ'(π/4)| = |tan(π/4)| = 1\n");
    printf("收敛条件: |φ'(x)| < 1 (不满足)\n");
    start = clock();
    double root1 = simple_iteration_original(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root1);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root1, f(root1));
    
    printf("========================================\n");
    printf("2. 简单迭代法 (收敛)\n");
    printf("========================================\n");
    printf("迭代函数: ψ(x) = arccos(e^x / 4)\n");
    printf("导数: ψ'(x) = -(e^x/4) / sqrt(1 - (e^x/4)^2)\n");
    printf("在 x0=π/4: |ψ'(π/4)| ≈ 0.6556 < 1 (满足)\n");
    start = clock();
    double root2 = simple_iteration_convergent(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root2);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root2, f(root2));
    
    printf("========================================\n");
    printf("3. 斯蒂芬森迭代法\n");
    printf("========================================\n");
    start = clock();
    double root3 = steffensen_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root3);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root3, f(root3));
    
    printf("========================================\n");
    printf("4. Newton 迭代法\n");
    printf("========================================\n");
    start = clock();
    double root4 = newton_iteration(x0);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root4);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root4, f(root4));
    
    printf("========================================\n");
    printf("5. 双点弦截法\n");
    printf("========================================\n");
    start = clock();
    double root5 = secant_method(x0, x1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("根: %.8f\n", root5);
    printf("迭代次数: %d\n", iterations);
    printf("计算时间: %.6f 秒\n", cpu_time_used);
    printf("验证: f(%.8f) = %.8f\n\n", root5, f(root5));
    
    return 0;
}