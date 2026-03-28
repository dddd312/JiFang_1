#include <stdio.h>  // 标准输入输出库
#include <math.h>   // 数学函数库

/**
 * 正向递推计算定积分y_n
 * 使用递推公式：y_n = 1 - n*y_{n-1}
 * @param y 存储结果的数组
 */
void forward_recurrence(float y[]) {
    y[0] = logf(2.0f);  // 初始值y_0 = ln2
    for (int n = 1; n <= 20; n++) {
        y[n] = 1.0f - n * y[n - 1];  // 正向递推计算
    }
}

/**
 * 反向递推计算定积分y_n
 * 使用递推公式：y_{n-1} = (1 - y_n)/n
 * @param y 存储结果的数组
 */
void backward_recurrence(float y[]) {
    y[20] = 0.0f;  // 初始值y_20 = 0（近似值）
    for (int n = 20; n >= 1; n--) {
        y[n - 1] = (1.0f - y[n]) / n;  // 反向递推计算
    }
}

/**
 * 主函数
 */
int main() {
    // 打印标题
    printf("题目4：定积分y_n的递推计算\n\n");
    
    float y_forward[21];  // 正向递推结果
    float y_backward[21];  // 反向递推结果
    
    // 计算正向递推结果
    forward_recurrence(y_forward);
    // 计算反向递推结果
    backward_recurrence(y_backward);
    
    // 打印正向递推结果
    printf("(1) 正向递推：y_n = 1 - n*y_{n-1}\n");
    printf("n\ty_n\n");
    printf("----------------\n");
    for (int n = 0; n <= 20; n++) {
        printf("%d\t%.8f\n", n, y_forward[n]);
    }
    
    // 打印反向递推结果
    printf("\n(2) 反向递推：y_{n-1} = (1-y_n)/n, y_20=0\n");
    printf("n\ty_n\n");
    printf("----------------\n");
    for (int n = 0; n <= 20; n++) {
        printf("%d\t%.8f\n", n, y_backward[n]);
    }
    
    // 打印两种方法的对比
    printf("\n(3) 两种方法的对比：\n");
    printf("n\t正向递推\t反向递推\t绝对差值\n");
    printf("--------------------------------------------\n");
    for (int n = 0; n <= 20; n++) {
        float diff = fabsf(y_forward[n] - y_backward[n]);  // 计算绝对差值
        printf("%d\t%.8f\t%.8f\t%.2e\n", n, y_forward[n], y_backward[n], diff);
    }
    
    return 0;  // 程序正常结束
}
