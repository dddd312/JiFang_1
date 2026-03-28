#include <stdio.h>  // 标准输入输出库
#include <math.h>   // 数学函数库

/**
 * 正序计算S_n = sum_{j=2}^n 1/(j^2-1)
 * @param n 求和的上限（长整型）
 * @return 计算结果（双精度浮点数）
 */
double calculate_forward(long long n) {
    double sum = 0.0;  // 初始化累加和为0
    for (long long j = 2; j <= n; j++) {
        sum += 1.0 / (j * j - 1);  // 累加每一项 1/(j^2-1)
    }
    return sum;  // 返回计算结果
}

/**
 * 倒序计算S_n = sum_{j=2}^n 1/(j^2-1)
 * @param n 求和的上限（长整型）
 * @return 计算结果（双精度浮点数）
 */
double calculate_backward(long long n) {
    double sum = 0.0;  // 初始化累加和为0
    for (long long j = n; j >= 2; j--) {
        sum += 1.0 / (j * j - 1);  // 从大到小累加每一项 1/(j^2-1)
    }
    return sum;  // 返回计算结果
}

/**
 * 计算S_n的精确值
 * 使用公式：S_n = 1/2 * (3/2 - 1/n - 1/(n+1))
 * @param n 求和的上限（长整型）
 * @return 精确值（双精度浮点数）
 */
double exact_value(long long n) {
    return 0.5 * (1.5 - 1.0 / n - 1.0 / (n + 1));
}

/**
 * 计算有效位数
 * @param computed 计算值
 * @param exact 精确值
 * @return 有效位数
 */
int count_significant_digits(double computed, double exact) {
    if (exact == 0.0) return 0;  // 精确值为0时返回0
    double relative_error = fabs((computed - exact) / exact);  // 计算相对误差
    if (relative_error == 0.0) return 16;  // 无误差时返回双精度浮点数的有效位数
    int digits = -floor(log10(relative_error));  // 计算有效位数
    return digits;  // 返回有效位数
}

/**
 * 主函数
 */
int main() {
    // 打印标题
    printf("题目1：计算S_n = sum_{j=2}^n 1/(j^2-1) (double精度)\n\n");
    
    // 打印表头
    printf("n\t正序结果\t倒序结果\t精确值\t\t正序误差\t倒序误差\t正序有效位数\t倒序有效位数\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    
    // 计算n=10^k (k=1到10)时的结果
    for (int k = 1; k <= 10; k++) {
        long long n = (long long)pow(10, k);  // 计算n=10^k
        double forward = calculate_forward(n);  // 正序计算
        double backward = calculate_backward(n);  // 倒序计算
        double exact = exact_value(n);  // 计算精确值
        
        // 计算误差
        double forward_error = fabs(forward - exact);
        double backward_error = fabs(backward - exact);
        
        // 计算有效位数
        int forward_digits = count_significant_digits(forward, exact);
        int backward_digits = count_significant_digits(backward, exact);
        
        // 打印结果
        printf("%lld\t%.8lf\t%.8lf\t%.8lf\t%.2e\t\t%.2e\t\t%d\t\t%d\n", 
               n, forward, backward, exact, forward_error, backward_error, forward_digits, backward_digits);
    }
    
    return 0;  // 程序正常结束
}
