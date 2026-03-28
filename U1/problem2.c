#include <stdio.h>  // 标准输入输出库
#include <math.h>   // 数学函数库

/**
 * 用Gauss消去法求解方程组1：
 * [ε  1][x1]   [1+ε]
 * [1  1][x2] = [  2  ]
 * @param epsilon 系数矩阵中的小参数
 * @param x1 解向量的第一个分量
 * @param x2 解向量的第二个分量
 */
void gauss_elimination_1(float epsilon, float *x1, float *x2) {
    // 初始化系数矩阵和右端项
    float a11 = epsilon;
    float a12 = 1.0f;
    float a21 = 1.0f;
    float a22 = 1.0f;
    float b1 = 1.0f + epsilon;
    float b2 = 2.0f;
    
    // 计算消元因子
    float m = a21 / a11;
    // 消去第二行第一列的元素
    a22 = a22 - m * a12;
    b2 = b2 - m * b1;
    
    // 回代求解
    *x2 = b2 / a22;
    *x1 = (b1 - a12 * (*x2)) / a11;
}

/**
 * 用Gauss消去法求解方程组2：
 * [1  1][x2]   [  2  ]
 * [ε  1][x1] = [1+ε]
 * @param epsilon 系数矩阵中的小参数
 * @param x1 解向量的第一个分量
 * @param x2 解向量的第二个分量
 */
void gauss_elimination_2(float epsilon, float *x1, float *x2) {
    // 初始化系数矩阵和右端项
    float a11 = 1.0f;
    float a12 = 1.0f;
    float a21 = epsilon;
    float a22 = 1.0f;
    float b1 = 2.0f;
    float b2 = 1.0f + epsilon;
    
    // 计算消元因子
    float m = a21 / a11;
    // 消去第二行第一列的元素
    a22 = a22 - m * a12;
    b2 = b2 - m * b1;
    
    // 回代求解
    *x2 = b2 / a22;
    *x1 = (b1 - a12 * (*x2)) / a11;
}

/**
 * 主函数
 */
int main() {
    // 打印标题
    printf("题目2：Gauss消去法求解方程组\n\n");
    
    // 打印表头
    printf("k\tepsilon\t\t方法1 x1\t方法1 x2\t方法2 x1\t方法2 x2\n");
    printf("--------------------------------------------------------------------------------\n");
    
    // 计算不同epsilon值的结果
    for (int k = 0; k <= 10; k++) {
        float epsilon = powf(10.0f, -2.0f * k);  // epsilon = 10^(-2k)
        
        // 方法1求解
        float x1_1, x2_1;
        gauss_elimination_1(epsilon, &x1_1, &x2_1);
        
        // 方法2求解
        float x1_2, x2_2;
        gauss_elimination_2(epsilon, &x1_2, &x2_2);
        
        // 打印结果
        printf("%d\t%.2e\t\t%.8f\t%.8f\t%.8f\t%.8f\n", 
               k, epsilon, x1_1, x2_1, x1_2, x2_2);
    }
    
    return 0;  // 程序正常结束
}
