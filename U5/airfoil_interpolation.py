import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import CubicSpline, lagrange
from scipy.interpolate import interp1d

# 设置中文字体，确保图片中中文正常显示
try:
    plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'DejaVu Sans']
    plt.rcParams['axes.unicode_minus'] = False
except:
    pass

"""
第五章实践题目1: 机翼上缘轮线插值方法比较

已知机翼上缘轮线的数据，试选用合适的插值方法画出机翼曲线，
比较那种方法画出的曲线更光滑。

数据点:
x: [0.0, 4.74, 9.50, 19.00, 38.00, 57.00, 76.00, 95.00, 114.00, 133.00, 152.00, 171.00, 190.00]
y: [0.00, 5.32, 8.10, 11.97, 16.15, 17.10, 16.34, 14.63, 12.16, 9.69, 7.03, 3.99, 0.00]

实现方法:
1. 分段线性插值 - 简单但不光滑
2. 拉格朗日插值 - 通过所有点但可能振荡
3. 三次样条插值 - 光滑且通过所有点
4. 多项式拟合 (3,5,7次) - 最小二乘法拟合

输出:
1. 四幅对比图展示不同方法
2. 光滑度指标和均方误差分析
3. 结论和建议
"""

# 机翼上缘轮线数据
x_data = np.array([0.0, 4.74, 9.50, 19.00, 38.00, 57.00, 76.00, 
                   95.00, 114.00, 133.00, 152.00, 171.00, 190.00])
y_data = np.array([0.00, 5.32, 8.10, 11.97, 16.15, 17.10, 16.34, 
                   14.63, 12.16, 9.69, 7.03, 3.99, 0.00])

def lagrange_interpolation(x_nodes, y_nodes, x_eval):
    """
    计算拉格朗日插值在x_eval点的值
    
    参数:
    x_nodes: 插值节点数组
    y_nodes: 节点处函数值数组
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

def linear_interpolation(x_nodes, y_nodes, x_eval):
    """
    分段线性插值
    """
    return np.interp(x_eval, x_nodes, y_nodes)

def cubic_spline_interpolation(x_nodes, y_nodes, x_eval):
    """
    三次样条插值 (自然边界条件)
    """
    cs = CubicSpline(x_nodes, y_nodes, bc_type='natural')
    return cs(x_eval)

def polynomial_fit(x_nodes, y_nodes, x_eval, degree=5):
    """
    多项式拟合 (最小二乘法)
    
    参数:
    degree: 多项式次数
    """
    coeff = np.polyfit(x_nodes, y_nodes, degree)
    poly = np.poly1d(coeff)
    return poly(x_eval)

def calculate_smoothness(x_fine, y_fine):
    """
    计算曲线的光滑度指标
    数值越小表示曲线越光滑
    """
    if len(y_fine) < 3:
        return float('inf')
    # 计算二阶差分 (近似二阶导数)
    second_diff = np.diff(y_fine, n=2)
    # 光滑度指标：二阶差分的平方和
    smoothness = np.sum(second_diff**2)
    return smoothness

def plot_comparison():
    """
    绘制不同插值方法的对比图
    """
    # 生成密集采样点
    x_fine = np.linspace(0, 190, 1000)
    
    # 计算各种插值方法的结果
    y_linear = linear_interpolation(x_data, y_data, x_fine)
    y_lagrange = lagrange_interpolation(x_data, y_data, x_fine)
    y_spline = cubic_spline_interpolation(x_data, y_data, x_fine)
    
    # 多项式拟合 (尝试不同次数)
    y_poly3 = polynomial_fit(x_data, y_data, x_fine, degree=3)
    y_poly5 = polynomial_fit(x_data, y_data, x_fine, degree=5)
    y_poly7 = polynomial_fit(x_data, y_data, x_fine, degree=7)
    
    # 计算光滑度指标
    smoothness_linear = calculate_smoothness(x_fine, y_linear)
    smoothness_lagrange = calculate_smoothness(x_fine, y_lagrange)
    smoothness_spline = calculate_smoothness(x_fine, y_spline)
    smoothness_poly3 = calculate_smoothness(x_fine, y_poly3)
    smoothness_poly5 = calculate_smoothness(x_fine, y_poly5)
    smoothness_poly7 = calculate_smoothness(x_fine, y_poly7)
    
    # 计算在数据点处的拟合误差 (均方误差 MSE)
    def calculate_mse(x_nodes, y_nodes, method_func):
        y_pred = method_func(x_nodes, y_nodes, x_nodes)
        mse = np.mean((y_pred - y_nodes) ** 2)
        return mse
    
    # 对于插值方法，在节点处误差应为0（除了多项式拟合）
    mse_linear = calculate_mse(x_data, y_data, linear_interpolation)
    mse_lagrange = calculate_mse(x_data, y_data, lagrange_interpolation)
    mse_spline = calculate_mse(x_data, y_data, cubic_spline_interpolation)
    
    # 多项式拟合的误差（拟合不强制通过节点）
    mse_poly3 = np.mean((polynomial_fit(x_data, y_data, x_data, degree=3) - y_data) ** 2)
    mse_poly5 = np.mean((polynomial_fit(x_data, y_data, x_data, degree=5) - y_data) ** 2)
    mse_poly7 = np.mean((polynomial_fit(x_data, y_data, x_data, degree=7) - y_data) ** 2)
    
    # 创建图形
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    
    # 图1: 所有方法对比
    ax1 = axes[0, 0]
    ax1.plot(x_data, y_data, 'ko', markersize=8, label='原始数据点')
    ax1.plot(x_fine, y_linear, 'b-', linewidth=1.5, label='分段线性插值')
    ax1.plot(x_fine, y_lagrange, 'g-', linewidth=1.5, label='拉格朗日插值')
    ax1.plot(x_fine, y_spline, 'r-', linewidth=2, label='三次样条插值')
    ax1.set_xlabel('x (长度)', fontsize=12)
    ax1.set_ylabel('y (高度)', fontsize=12)
    ax1.set_title('机翼上缘轮线 - 插值方法对比', fontsize=14)
    ax1.legend(loc='best', fontsize=10)
    ax1.grid(True, alpha=0.3)
    ax1.set_xlim(-5, 195)
    ax1.set_ylim(-2, 20)
    
    # 图2: 多项式拟合对比
    ax2 = axes[0, 1]
    ax2.plot(x_data, y_data, 'ko', markersize=8, label='原始数据点')
    ax2.plot(x_fine, y_poly3, 'c-', linewidth=2, label='3次多项式拟合')
    ax2.plot(x_fine, y_poly5, 'm-', linewidth=2, label='5次多项式拟合')
    ax2.plot(x_fine, y_poly7, 'y-', linewidth=2, label='7次多项式拟合')
    ax2.set_xlabel('x (长度)', fontsize=12)
    ax2.set_ylabel('y (高度)', fontsize=12)
    ax2.set_title('机翼上缘轮线 - 多项式拟合对比', fontsize=14)
    ax2.legend(loc='best', fontsize=10)
    ax2.grid(True, alpha=0.3)
    ax2.set_xlim(-5, 195)
    ax2.set_ylim(-2, 20)
    
    # 图3: 样条插值细节 (最光滑的方法)
    ax3 = axes[1, 0]
    ax3.plot(x_data, y_data, 'ko', markersize=8, label='原始数据点')
    ax3.plot(x_fine, y_spline, 'r-', linewidth=2.5, label='三次样条插值')
    # 绘制样条插值的一阶导数
    cs = CubicSpline(x_data, y_data, bc_type='natural')
    y_derivative = cs(x_fine, 1)  # 一阶导数
    ax3_twin = ax3.twinx()
    ax3_twin.plot(x_fine, y_derivative, 'b--', linewidth=1.5, alpha=0.7, label='一阶导数')
    ax3.set_xlabel('x (长度)', fontsize=12)
    ax3.set_ylabel('y (高度)', fontsize=12, color='red')
    ax3_twin.set_ylabel('dy/dx (斜率)', fontsize=12, color='blue')
    ax3.set_title('三次样条插值及其导数', fontsize=14)
    ax3.legend(loc='upper left', fontsize=10)
    ax3_twin.legend(loc='upper right', fontsize=10)
    ax3.grid(True, alpha=0.3)
    ax3.set_xlim(-5, 195)
    ax3.set_ylim(-2, 20)
    
    # 图4: 光滑度指标对比
    ax4 = axes[1, 1]
    methods = ['线性插值', '拉格朗日', '三次样条', '3次多项式', '5次多项式', '7次多项式']
    smoothness_values = [smoothness_linear, smoothness_lagrange, smoothness_spline,
                        smoothness_poly3, smoothness_poly5, smoothness_poly7]
    colors = ['blue', 'green', 'red', 'cyan', 'magenta', 'yellow']
    
    bars = ax4.bar(methods, smoothness_values, color=colors, alpha=0.7)
    ax4.set_xlabel('插值方法', fontsize=12)
    ax4.set_ylabel('光滑度指标 (越小越光滑)', fontsize=12)
    ax4.set_title('不同插值方法的光滑度对比', fontsize=14)
    ax4.grid(True, alpha=0.3, axis='y')
    
    # 在柱状图上添加数值标签
    for bar, value in zip(bars, smoothness_values):
        height = bar.get_height()
        ax4.text(bar.get_x() + bar.get_width()/2., height + max(smoothness_values)*0.01,
                f'{value:.2f}', ha='center', va='bottom', fontsize=9)
    
    plt.tight_layout()
    plt.savefig('airfoil_interpolation_comparison.png', dpi=300)
    # plt.show()  # 注释掉以避免阻塞，图像已保存
    
    # 输出分析结果
    print("=" * 80)
    print("机翼上缘轮线插值方法综合分析")
    print("=" * 80)
    print("方法说明:")
    print("  - 光滑度指标: 数值越小表示曲线越光滑（基于二阶导数的平方和）")
    print("  - 均方误差(MSE): 在原始数据点处的拟合误差，插值方法应为0")
    print("-" * 80)
    print(f"{'方法':15s} {'光滑度指标':15s} {'均方误差(MSE)':15s}")
    print("-" * 80)
    
    mse_values = [mse_linear, mse_lagrange, mse_spline, mse_poly3, mse_poly5, mse_poly7]
    
    for method, smooth_val, mse_val in zip(methods, smoothness_values, mse_values):
        print(f"{method:15s} {smooth_val:15.4f} {mse_val:15.6f}")
    print("-" * 80)
    
    # 结论
    min_smooth_index = np.argmin(smoothness_values)
    min_mse_index = np.argmin(mse_values)
    
    print(f"最光滑的方法: {methods[min_smooth_index]} (光滑度指标: {smoothness_values[min_smooth_index]:.4f})")
    print(f"拟合误差最小的方法: {methods[min_mse_index]} (MSE: {mse_values[min_mse_index]:.6f})")
    
    print("\n观察结论:")
    print("1. 三次样条插值和低次多项式拟合都能产生光滑曲线")
    print("2. 拉格朗日插值虽然通过所有点，但可能产生不必要的振荡")
    print("3. 分段线性插值最简单，但不光滑（导数不连续）")
    print("4. 对于机翼曲线，三次样条插值是平衡光滑度和精度的好选择")
    print("5. 3次多项式拟合误差很小且非常光滑，适合作为参数化模型")
    print("=" * 80)

if __name__ == "__main__":
    print("开始计算机翼上缘轮线插值方法比较...")
    plot_comparison()
    print("图像保存为 'airfoil_interpolation_comparison.png'")