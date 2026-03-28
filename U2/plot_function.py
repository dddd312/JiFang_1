"""
绘制函数 f(x) = 4cos(x) - e^x 的图像
标记初始值 x0 = π/4 和方程的根
计算方法课程作业 - 方程求根可视化
"""

import numpy as np
import matplotlib.pyplot as plt
import math

# 定义函数 f(x) = 4cos(x) - e^x
def f(x):
    return 4 * np.cos(x) - np.exp(x)

# 设置绘图范围
x_start = -2.0
x_end = 2.0
num_points = 1000
x = np.linspace(x_start, x_end, num_points)
y = f(x)

# 已知的根和初始值
root = 0.904788  # 从迭代法计算得到的根
x0 = math.pi / 4  # 初始值 π/4

# 创建图形
plt.figure(figsize=(12, 8))

# 绘制函数曲线
plt.plot(x, y, 'b-', linewidth=2.5, label=r'$f(x) = 4\cos(x) - e^x$')

# 绘制x轴（y=0的参考线）
plt.axhline(y=0, color='k', linestyle='-', linewidth=1, alpha=0.5)

# 标记根的位置（函数零点）
root_y = f(root)
plt.plot(root, root_y, 'ro', markersize=10, label=f'根: x ≈ {root:.6f}')
plt.annotate(f'根 ≈ {root:.6f}', 
             xy=(root, root_y), 
             xytext=(root+0.1, root_y+0.5),
             arrowprops=dict(facecolor='red', shrink=0.05, width=1.5, headwidth=8),
             fontsize=12, color='red')

# 标记初始值 x0
x0_y = f(x0)
plt.plot(x0, x0_y, 'gs', markersize=10, label=r'初始值 $x_0 = \pi/4$')
plt.annotate(r'$x_0 = \pi/4 \approx' + f'{x0:.6f}$', 
             xy=(x0, x0_y), 
             xytext=(x0-0.8, x0_y+0.8),
             arrowprops=dict(facecolor='green', shrink=0.05, width=1.5, headwidth=8),
             fontsize=12, color='green')

# 添加特殊点的垂直线
plt.axvline(x=root, color='r', linestyle='--', linewidth=1, alpha=0.7)
plt.axvline(x=x0, color='g', linestyle='--', linewidth=1, alpha=0.7)

# 设置图形属性
plt.title(r'函数图像: $f(x) = 4\cos(x) - e^x$', fontsize=16, fontweight='bold')
plt.xlabel('x', fontsize=14)
plt.ylabel('f(x)', fontsize=14)
plt.grid(True, which='both', linestyle='--', linewidth=0.5, alpha=0.7)
plt.legend(loc='upper right', fontsize=12)

# 设置坐标轴范围
plt.xlim(x_start, x_end)
plt.ylim(-5, 5)  # 根据函数值调整y轴范围

# 添加文本说明
plt.text(-1.9, 4.5, r'方程: $4\cos(x) = e^x$', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow"))
plt.text(-1.9, 4.0, f'根: x ≈ {root:.6f}', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue"))
plt.text(-1.9, 3.5, r'初始值: $x_0 = \pi/4 \approx' + f'{x0:.6f}$', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgreen"))

# 保存图像
plt.tight_layout()
plt.savefig('function_plot.png', dpi=300, bbox_inches='tight')
print("图像已保存为 'function_plot.png'")

# 显示图像
plt.show()

# 输出关键点的函数值
print("\n关键点信息:")
print(f"初始值 x0 = π/4 ≈ {x0:.6f}")
print(f"f(x0) = {x0_y:.6f}")
print(f"\n根 x* ≈ {root:.6f}")
print(f"f(x*) = {root_y:.6f}")
print(f"验证: |f(x*)| = {abs(root_y):.6e} {'<' if abs(root_y) < 1e-4 else '>='} 10^-4")

# 绘制函数 4cos(x) 和 e^x 的对比图（可选）
plt.figure(figsize=(12, 8))
y1 = 4 * np.cos(x)
y2 = np.exp(x)

plt.plot(x, y1, 'b-', linewidth=2.5, label=r'$y = 4\cos(x)$')
plt.plot(x, y2, 'r-', linewidth=2.5, label=r'$y = e^x$')
plt.plot(root, 4*np.cos(root), 'ko', markersize=10, label=f'交点: x ≈ {root:.6f}')

plt.axhline(y=0, color='k', linestyle='-', linewidth=1, alpha=0.5)
plt.axvline(x=root, color='k', linestyle='--', linewidth=1, alpha=0.7)

plt.title(r'函数对比: $4\cos(x)$ 和 $e^x$', fontsize=16, fontweight='bold')
plt.xlabel('x', fontsize=14)
plt.ylabel('y', fontsize=14)
plt.grid(True, which='both', linestyle='--', linewidth=0.5, alpha=0.7)
plt.legend(loc='upper right', fontsize=12)
plt.xlim(x_start, x_end)
plt.ylim(-5, 8)

plt.tight_layout()
plt.savefig('functions_comparison.png', dpi=300, bbox_inches='tight')
print("对比图像已保存为 'functions_comparison.png'")

# 显示对比图像
plt.show()