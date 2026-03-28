import numpy as np
import matplotlib.pyplot as plt
import math

plt.switch_backend('Agg')


def f(x):
    return 4 * np.cos(x) - np.exp(x)


x_start = -2.0
x_end = 2.0
num_points = 1000
x = np.linspace(x_start, x_end, num_points)
y = f(x)

root = 0.904788  
x0 = math.pi / 4  
x1 = math.pi / 2

plt.figure(figsize=(12, 8))

plt.plot(x, y, 'b-', linewidth=2.5, label=r'$f(x) = 4\cos(x) - e^x$')

plt.axhline(y=0, color='k', linestyle='-', linewidth=1, alpha=0.5)

root_y = f(root)
plt.plot(root, root_y, 'ro', markersize=10, label=f'Root: x ≈ {root:.6f}')
plt.annotate(f'Root ≈ {root:.6f}', 
             xy=(root, root_y), 
             xytext=(root+0.1, root_y+0.5),
             arrowprops=dict(facecolor='red', shrink=0.05, width=1.5, headwidth=8),
             fontsize=12, color='red')

x0_y = f(x0)
plt.plot(x0, x0_y, 'gs', markersize=10, label=r'Initial $x_0 = \pi/4$')
plt.annotate(r'$x_0 = \pi/4 \approx' + f'{x0:.6f}$', 
             xy=(x0, x0_y), 
             xytext=(x0-0.8, x0_y+0.8),
             arrowprops=dict(facecolor='green', shrink=0.05, width=1.5, headwidth=8),
             fontsize=12, color='green')

x1_y = f(x1)
plt.plot(x1, x1_y, 'ms', markersize=10, label=r'Initial $x_1 = \pi/2$')
plt.annotate(r'$x_1 = \pi/2 \approx' + f'{x1:.6f}$', 
             xy=(x1, x1_y), 
             xytext=(x1-0.8, x1_y+0.8),
             arrowprops=dict(facecolor='magenta', shrink=0.05, width=1.5, headwidth=8),
             fontsize=12, color='magenta')

plt.axvline(x=root, color='r', linestyle='--', linewidth=1, alpha=0.7)
plt.axvline(x=x0, color='g', linestyle='--', linewidth=1, alpha=0.7)
plt.axvline(x=x1, color='m', linestyle='--', linewidth=1, alpha=0.7)

plt.title(r'Function Plot: $f(x) = 4\cos(x) - e^x$', fontsize=16, fontweight='bold')
plt.xlabel('x', fontsize=14)
plt.ylabel('f(x)', fontsize=14)
plt.grid(True, which='both', linestyle='--', linewidth=0.5, alpha=0.7)
plt.legend(loc='upper right', fontsize=12)

plt.xlim(x_start, x_end)
plt.ylim(-5, 5)  

plt.text(-1.9, 4.5, r'Equation: $4\cos(x) = e^x$', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightyellow"))
plt.text(-1.9, 4.0, f'Root: x ≈ {root:.6f}', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue"))
plt.text(-1.9, 3.5, r'Initial: $x_0 = \pi/4 \approx' + f'{x0:.6f}$', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgreen"))
plt.text(-1.9, 3.0, r'Initial: $x_1 = \pi/2 \approx' + f'{x1:.6f}$', fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightpink"))

plt.tight_layout()
plt.savefig('function_plot_english.png', dpi=300, bbox_inches='tight')
print("Image saved as 'function_plot_english.png'")
