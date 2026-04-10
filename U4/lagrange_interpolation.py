import numpy as np
import matplotlib.pyplot as plt

# 设置中文字体，确保图片中中文正常显示
try:
    plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'DejaVu Sans']
    plt.rcParams['axes.unicode_minus'] = False
except:
    pass

"""
第四章实践题目1: 拉格朗日插值多项式对比

函数 f(x) = 1/(1+x^2) 在区间 [-5,5] 上进行等距节点拉格朗日插值。
取 n=2,4,6,8,10 分别计算 n 次插值多项式，并绘制对比图。
"""

def lagrange_interpolation(x_nodes, y_nodes, x_eval):
    """
    计算拉格朗日插值在x_eval点的值
    
    参数:
    x_nodes: 插值节点数组 (长度 n+1)
    y_nodes: 节点处函数值数组 (长度 n+1)
    x_eval: 需要计算插值的点 (标量或数组)
    
    返回:
    插值多项式在x_eval处的值
    """
    x_eval = np.asarray(x_eval)
    n = len(x_nodes) - 1
    result = np.zeros_like(x_eval, dtype=float)
    
    for i in range(n + 1):
        # 计算拉格朗日基函数 l_i(x)
        li = np.ones_like(x_eval, dtype=float)
        for j in range(n + 1):
            if i != j:
                li *= (x_eval - x_nodes[j]) / (x_nodes[i] - x_nodes[j])
        result += y_nodes[i] * li
    return result

def generate_nodes(n):
    """
    在[-5, 5]区间上生成n+1个等距节点和对应的函数值 f(x) = 1/(1+x^2)
    
    参数:
    n: 等分数
    
    返回:
    x_nodes: 节点数组
    y_nodes: 函数值数组
    """
    a, b = -5, 5
    h = (b - a) / n
    x_nodes = np.array([a + i * h for i in range(n + 1)])
    y_nodes = 1 / (1 + x_nodes**2)
    return x_nodes, y_nodes

def plot_comparison():
    """
    绘制原始函数和不同n值的拉格朗日插值多项式对比图
    """
    # 原始函数采样点
    x_fine = np.linspace(-5, 5, 1000)
    y_true = 1 / (1 + x_fine**2)
    
    # 不同的n值
    n_values = [2, 4, 6, 8, 10]
    colors = ['red', 'green', 'blue', 'orange', 'purple']
    
    plt.figure(figsize=(12, 8))
    
    # 绘制原始函数
    plt.plot(x_fine, y_true, 'k-', linewidth=3, label='原始函数 $f(x) = 1/(1+x^2)$')
    
    # 对每个n计算并绘制插值多项式
    for n, color in zip(n_values, colors):
        x_nodes, y_nodes = generate_nodes(n)
        y_interp = lagrange_interpolation(x_nodes, y_nodes, x_fine)
        plt.plot(x_fine, y_interp, '--', linewidth=1.5, color=color, label=f'n={n} 次插值')
        plt.plot(x_nodes, y_nodes, 'o', markersize=6, color=color)
    
    plt.xlabel('x', fontsize=14)
    plt.ylabel('f(x)', fontsize=14)
    plt.title('拉格朗日插值多项式对比', fontsize=16)
    plt.legend(loc='best', fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.xlim(-5.5, 5.5)
    plt.ylim(-0.5, 1.5)
    plt.tight_layout()
    plt.savefig('lagrange_interpolation_comparison.png', dpi=300)

    
    # 打印节点处误差
    print("节点处插值误差检查 (应接近0):")
    for n in n_values:
        x_nodes, y_nodes = generate_nodes(n)
        y_interp_nodes = lagrange_interpolation(x_nodes, y_nodes, x_nodes)
        error = np.max(np.abs(y_interp_nodes - y_nodes))
        print(f"  n={n}: 最大绝对误差 = {error:.2e}")
    
    # 区间内误差分析（在1000个采样点上）
    print("\n区间内最大绝对误差分析（1000个采样点）:")
    for n in n_values:
        x_nodes, y_nodes = generate_nodes(n)
        y_interp = lagrange_interpolation(x_nodes, y_nodes, x_fine)
        error = np.max(np.abs(y_interp - y_true))
        print(f"  n={n}: 最大绝对误差 ≈ {error:.3f}")

if __name__ == "__main__":
    print("开始计算拉格朗日插值")
    plot_comparison()
    print("结果图像已保存到 'lagrange_interpolation_comparison.png'")