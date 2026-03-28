/*
 * 计算方法课程作业 - 方程求根 (EasyX图形版)
 * 题目：用四种迭代方法求解方程 4cos(x) = e^x 的根
 * 方法：简单迭代法、斯蒂芬森迭代法、Newton迭代法、双点弦截法
 * 精度要求：ε = 10^-4
 * 初始值：简单迭代法、斯蒂芬森迭代法、Newton迭代法取 x0 = π/4
 *        双点弦截法取 x0 = π/4, x1 = π/2
 * 
 * 新增功能：EasyX图形绘制
 * 1. 绘制坐标系和网格
 * 2. 绘制 4cos(x) 和 e^x 函数曲线
 * 3. 标记方程根（交点）
 * 4. 显示图例和函数信息
 * 
 * 编译说明：
 * 1. 需要安装 EasyX 图形库
 * 2. 使用 Visual Studio 或 Dev-C++ 等支持 EasyX 的编译器
 * 3. 或使用 MinGW 配合 EasyX for MinGW
 * 
 * 作者：软件工程大二学生
 * 日期：2026-03-15
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

// EasyX 图形库头文件
#include <graphics.h>
#include <conio.h>

// 圆周率常量
#define PI 3.14159265358979323846
// 精度要求
#define EPSILON 1e-4

// 全局变量：记录迭代次数
int iterations;

// ========================================
// 函数定义部分（与原始程序相同）
// ========================================

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

// ========================================
// EasyX 图形绘制函数
// ========================================

// 绘制坐标系和网格
void draw_coordinate_system(int width, int height, 
                           double x_min, double x_max, 
                           double y_min, double y_max) {
    // 设置背景色为白色
    setbkcolor(WHITE);
    cleardevice();
    
    // 计算坐标轴位置
    int x_axis_y = height - (int)((0 - y_min) / (y_max - y_min) * height);
    int y_axis_x = (int)((0 - x_min) / (x_max - x_min) * width);
    
    // 绘制坐标轴
    setcolor(BLACK);
    setlinestyle(PS_SOLID, 2);
    
    // x轴
    if (x_axis_y >= 0 && x_axis_y <= height) {
        line(0, x_axis_y, width, x_axis_y);
    }
    
    // y轴
    if (y_axis_x >= 0 && y_axis_x <= width) {
        line(y_axis_x, 0, y_axis_x, height);
    }
    
    // 绘制网格
    setlinestyle(PS_DASH, 1);
    setcolor(LIGHTGRAY);
    
    // 垂直网格线
    for (double x = x_min; x <= x_max; x += (x_max - x_min) / 10) {
        int grid_x = (int)((x - x_min) / (x_max - x_min) * width);
        if (grid_x > 0 && grid_x < width) {
            line(grid_x, 0, grid_x, height);
        }
    }
    
    // 水平网格线
    for (double y = y_min; y <= y_max; y += (y_max - y_min) / 10) {
        int grid_y = height - (int)((y - y_min) / (y_max - y_min) * height);
        if (grid_y > 0 && grid_y < height) {
            line(0, grid_y, width, grid_y);
        }
    }
    
    // 恢复实线样式
    setlinestyle(PS_SOLID, 2);
}

// 绘制函数曲线
void draw_function_curves(int width, int height,
                         double x_min, double x_max,
                         double y_min, double y_max,
                         double root) {
    // 计算坐标转换参数
    double x_scale = width / (x_max - x_min);
    double y_scale = height / (y_max - y_min);
    
    // 绘制 4cos(x) 曲线（蓝色）
    setcolor(BLUE);
    setlinestyle(PS_SOLID, 3);
    
    double step = (x_max - x_min) / 500;  // 500个采样点
    double prev_x = x_min;
    double prev_y1 = 4 * cos(prev_x);
    int prev_screen_x = (int)((prev_x - x_min) * x_scale);
    int prev_screen_y1 = height - (int)((prev_y1 - y_min) * y_scale);
    
    for (double x = x_min + step; x <= x_max; x += step) {
        double y1 = 4 * cos(x);
        int screen_x = (int)((x - x_min) * x_scale);
        int screen_y1 = height - (int)((y1 - y_min) * y_scale);
        
        // 检查坐标是否在屏幕范围内
        if (prev_screen_y1 >= 0 && prev_screen_y1 <= height &&
            screen_y1 >= 0 && screen_y1 <= height) {
            line(prev_screen_x, prev_screen_y1, screen_x, screen_y1);
        }
        
        prev_screen_x = screen_x;
        prev_screen_y1 = screen_y1;
    }
    
    // 绘制 e^x 曲线（红色）
    setcolor(RED);
    setlinestyle(PS_SOLID, 3);
    
    prev_x = x_min;
    double prev_y2 = exp(prev_x);
    int prev_screen_y2 = height - (int)((prev_y2 - y_min) * y_scale);
    
    for (double x = x_min + step; x <= x_max; x += step) {
        double y2 = exp(x);
        int screen_x = (int)((x - x_min) * x_scale);
        int screen_y2 = height - (int)((y2 - y_min) * y_scale);
        
        if (prev_screen_y2 >= 0 && prev_screen_y2 <= height &&
            screen_y2 >= 0 && screen_y2 <= height) {
            line(prev_screen_x, prev_screen_y2, screen_x, screen_y2);
        }
        
        prev_screen_x = screen_x;
        prev_screen_y2 = screen_y2;
    }
    
    // 标记根的位置（交点）
    if (root >= x_min && root <= x_max) {
        double root_y = 4 * cos(root);  // 或 exp(root)，两者相等
        int root_screen_x = (int)((root - x_min) * x_scale);
        int root_screen_y = height - (int)((root_y - y_min) * y_scale);
        
        setcolor(GREEN);
        setfillcolor(GREEN);
        setlinestyle(PS_SOLID, 1);
        
        // 绘制交点标记
        circle(root_screen_x, root_screen_y, 8);
        floodfill(root_screen_x, root_screen_y, GREEN);
        
        // 显示根的值
        char root_text[50];
        sprintf(root_text, "Root: x = %.6f", root);
        setcolor(BLACK);
        settextstyle(14, 0, _T("宋体"));
        outtextxy(root_screen_x + 10, root_screen_y - 20, root_text);
    }
    
    // 绘制坐标轴刻度标签
    setcolor(BLACK);
    settextstyle(12, 0, _T("宋体"));
    
    // x轴刻度
    for (double x = x_min; x <= x_max; x += (x_max - x_min) / 10) {
        int tick_x = (int)((x - x_min) * x_scale);
        int tick_y = height - (int)((0 - y_min) * y_scale);
        
        char label[20];
        sprintf(label, "%.1f", x);
        outtextxy(tick_x - 10, tick_y + 5, label);
        
        // 绘制刻度线
        line(tick_x, tick_y - 5, tick_x, tick_y + 5);
    }
    
    // y轴刻度
    for (double y = y_min; y <= y_max; y += (y_max - y_min) / 10) {
        int tick_x = (int)((0 - x_min) * x_scale);
        int tick_y = height - (int)((y - y_min) * y_scale);
        
        char label[20];
        sprintf(label, "%.1f", y);
        outtextxy(tick_x + 5, tick_y - 8, label);
        
        // 绘制刻度线
        line(tick_x - 5, tick_y, tick_x + 5, tick_y);
    }
    
    // 添加坐标轴标签
    outtextxy(width - 50, height - 30, "x");
    outtextxy(10, 10, "y");
}

// 显示图例和标题
void draw_legend_and_title(int width, int height, double root) {
    // 绘制标题
    setcolor(BLACK);
    settextstyle(20, 0, _T("宋体"));
    outtextxy(width/2 - 200, 20, "函数图像: 4cos(x) 和 e^x");
    
    settextstyle(16, 0, _T("宋体"));
    outtextxy(width/2 - 150, 50, "方程: 4cos(x) = e^x");
    
    // 绘制图例
    int legend_x = width - 250;
    int legend_y = 100;
    
    // 4cos(x) 图例
    setcolor(BLUE);
    setlinestyle(PS_SOLID, 3);
    line(legend_x, legend_y, legend_x + 50, legend_y);
    setcolor(BLACK);
    settextstyle(14, 0, _T("宋体"));
    outtextxy(legend_x + 60, legend_y - 10, "y = 4cos(x)");
    
    // e^x 图例
    setcolor(RED);
    line(legend_x, legend_y + 30, legend_x + 50, legend_y + 30);
    setcolor(BLACK);
    outtextxy(legend_x + 60, legend_y + 20, "y = e^x");
    
    // 根标记图例
    setcolor(GREEN);
    setfillcolor(GREEN);
    circle(legend_x + 25, legend_y + 60, 5);
    floodfill(legend_x + 25, legend_y + 60, GREEN);
    setcolor(BLACK);
    outtextxy(legend_x + 60, legend_y + 55, "方程根 (交点)");
    
    // 显示根的值
    char root_info[100];
    sprintf(root_info, "计算得到的根: x = %.8f", root);
    outtextxy(legend_x, legend_y + 100, root_info);
    
    sprintf(root_info, "验证: f(%.8f) = %.8f", root, f(root));
    outtextxy(legend_x, legend_y + 120, root_info);
    
    // 显示操作提示
    settextstyle(12, 0, _T("宋体"));
    outtextxy(50, height - 50, "按任意键返回控制台...");
}

// EasyX 主绘图函数
void easyx_plot_functions(double root) {
    // 初始化图形窗口
    int width = 1000;
    int height = 700;
    initgraph(width, height);
    
    // 设置坐标系范围
    double x_min = -3.0;
    double x_max = 3.0;
    double y_min = -5.0;
    double y_max = 10.0;
    
    // 绘制坐标系和网格
    draw_coordinate_system(width, height, x_min, x_max, y_min, y_max);
    
    // 绘制函数曲线
    draw_function_curves(width, height, x_min, x_max, y_min, y_max, root);
    
    // 显示图例和标题
    draw_legend_and_title(width, height, root);
    
    // 等待用户按键
    _getch();
    
    // 关闭图形窗口
    closegraph();
}

// ========================================
// 控制台输出函数（与原程序相同）
// ========================================

void console_output() {
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
}

// ========================================
// 主函数
// ========================================

int main() {
    // 控制台输出
    console_output();
    
    // 获取收敛的根（使用收敛的简单迭代法结果）
    double root = simple_iteration_convergent(PI / 4);
    
    printf("\n========================================\n");
    printf("EasyX 图形绘制\n");
    printf("========================================\n");
    printf("正在启动图形窗口...\n");
    printf("请确保已安装 EasyX 图形库。\n");
    printf("如果未安装，图形功能将无法使用。\n\n");
    
    // 启动 EasyX 图形绘制
    printf("按 Enter 键启动图形窗口...");
    getchar();
    
    // 尝试启动图形窗口
    easyx_plot_functions(root);
    
    printf("图形窗口已关闭。\n");
    printf("程序结束。\n");
    
    return 0;
}