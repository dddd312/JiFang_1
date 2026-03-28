#include <stdio.h>  // 标准输入输出库
#include <math.h>   // 数学函数库

/**
 * 计算阶乘
 * @param n 非负整数
 * @return n!（单精度浮点数）
 */
float factorial(int n) {
    float result = 1.0f;  // 初始化结果为1
    for (int i = 1; i <= n; i++) {
        result *= i;  // 累乘计算阶乘
    }
    return result;  // 返回阶乘值
}

/**
 * 用无穷级数计算e^x
 * 使用公式：e^x = 1 + x + x^2/2! + x^3/3! + ...
 * @param x 指数
 * @param max_terms 最大项数
 * @return e^x的近似值（单精度浮点数）
 */
float exp_series(float x, int max_terms) {
    float sum = 0.0f;  // 初始化累加和为0
    float term = 1.0f;  // 第一项为1
    
    for (int n = 0; n < max_terms; n++) {
        sum += term;  // 累加当前项
        term *= x / (n + 1);  // 计算下一项
    }
    
    return sum;  // 返回计算结果
}

/**
 * 用倒数公式计算e^x（x<0时）
 * 使用公式：e^x = 1/e^(-x)
 * @param x 负指数
 * @param max_terms 最大项数
 * @return e^x的近似值（单精度浮点数）
 */
float exp_series_negative(float x, int max_terms) {
    float abs_x = -x;  // 取x的绝对值
    float sum = exp_series(abs_x, max_terms);  // 计算e^(-x)
    return 1.0f / sum;  // 返回1/e^(-x)
}

/**
 * 主函数
 */
int main() {
    // 打印标题
    printf("题目3：用无穷级数计算e^x\n\n");
    
    int max_terms = 100;  // 最大项数
    
    // 测试x>0的情况
    printf("(1) x > 0的情况：\n");
    printf("x\t级数计算值\t库函数exp(x)\t绝对误差\n");
    printf("----------------------------------------\n");
    
    float positive_x[] = {1.0f, 5.0f, 10.0f, 15.0f, 20.0f};
    for (int i = 0; i < 5; i++) {
        float x = positive_x[i];
        float series_result = exp_series(x, max_terms);  // 级数计算
        float library_result = expf(x);  // 库函数计算
        float error = fabsf(series_result - library_result);  // 计算绝对误差
        
        printf("%.0f\t%.8e\t%.8e\t%.2e\n", x, series_result, library_result, error);
    }
    
    // 测试x<0的情况（直接用级数）
    printf("\n(2) x < 0的情况（直接用级数）：\n");
    printf("x\t级数计算值\t库函数exp(x)\t绝对误差\n");
    printf("----------------------------------------\n");
    
    float negative_x[] = {-1.0f, -5.0f, -10.0f, -15.0f, -20.0f};
    for (int i = 0; i < 5; i++) {
        float x = negative_x[i];
        float series_result = exp_series(x, max_terms);  // 直接用级数计算
        float library_result = expf(x);  // 库函数计算
        float error = fabsf(series_result - library_result);  // 计算绝对误差
        
        printf("%.0f\t%.8e\t%.8e\t%.2e\n", x, series_result, library_result, error);
    }
    
    // 测试x<0的情况（用倒数公式）
    printf("\n(3) x < 0的情况（用倒数公式）：\n");
    printf("x\t级数计算值\t库函数exp(x)\t绝对误差\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < 5; i++) {
        float x = negative_x[i];
        float series_result = exp_series_negative(x, max_terms);  // 用倒数公式计算
        float library_result = expf(x);  // 库函数计算
        float error = fabsf(series_result - library_result);  // 计算绝对误差
        
        printf("%.0f\t%.8e\t%.8e\t%.2e\n", x, series_result, library_result, error);
    }
    
    return 0;  // 程序正常结束
}
