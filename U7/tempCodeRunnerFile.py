import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams
import warnings
import logging
import sys
import os

class StderrFilter:
    def __init__(self, original_stderr):
        self.original_stderr = original_stderr
        self.buffer = ""

    def write(self, text):
        if "Glyph" not in text and "does not have a glyph" not in text:
            self.original_stderr.write(text)

    def flush(self):
        self.original_stderr.flush()

original_stderr = sys.stderr
sys.stderr = StderrFilter(original_stderr)

logging.getLogger('matplotlib.font_manager').disabled = True
warnings.filterwarnings('ignore', category=UserWarning, module='matplotlib')
warnings.filterwarnings('ignore', message='.*Glyph.*missing from font.*')
warnings.filterwarnings('ignore', message='.*does not have a glyph.*')

plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['mathtext.fontset'] = 'dejavusans'
plt.rcParams['axes.formatter.limits'] = (-5, 5)

def f(x, y):
    return -x * y**2

def exact_solution(x):
    return 2 / (x**2 + 1)

def euler_predictor_corrector(x0, y0, xf, h):
    n = int((xf - x0) / h)
    x_vals = [x0]
    y_vals = [y0]
    x, y = x0, y0
    for i in range(n):
        f_xy = f(x, y)
        y_pred = y + h * f_xy
        x_new = x + h
        f_new = f(x_new, y_pred)
        y_new = y + (h / 2) * (f_xy + f_new)
        x_vals.append(x_new)
        y_vals.append(y_new)
        x, y = x_new, y_new
    return np.array(x_vals), np.array(y_vals)

def runge_kutta_4(x0, y0, xf, h):
    n = int((xf - x0) / h)
    x_vals = [x0]
    y_vals = [y0]
    x, y = x0, y0
    for i in range(n):
        k1 = f(x, y)
        k2 = f(x + h / 2, y + h * k1 / 2)
        k3 = f(x + h / 2, y + h * k2 / 2)
        k4 = f(x + h, y + h * k3)
        y_new = y + (h / 6) * (k1 + 2 * k2 + 2 * k3 + k4)
        x_new = x + h
        x_vals.append(x_new)
        y_vals.append(y_new)
        x, y = x_new, y_new
    return np.array(x_vals), np.array(y_vals)

def adams_pc_4th(x0, y0, xf, h):
    x_rk4, y_rk4 = runge_kutta_4(x0, y0, x0 + 3 * h, h)
    x_vals = list(x_rk4[:4])
    y_vals = list(y_rk4[:4])
    f_vals = [f(x_vals[i], y_vals[i]) for i in range(4)]
    n = int((xf - x0) / h) - 3
    for i in range(n):
        x = x_vals[-1]
        y_pred = y_vals[-1] + (h / 24) * (55 * f_vals[-1] - 59 * f_vals[-2] +
                                          37 * f_vals[-3] - 9 * f_vals[-4])
        x_new = x + h
        f_pred = f(x_new, y_pred)
        y_new = y_vals[-1] + (h / 24) * (9 * f_pred + 19 * f_vals[-1] -
                                           5 * f_vals[-2] + f_vals[-3])
        x_vals.append(x_new)
        y_vals.append(y_new)
        f_vals.pop(0)
        f_vals.append(f(x_new, y_new))
    return np.array(x_vals), np.array(y_vals)

def compute_errors(x_num, y_num, x_exact, y_exact):
    y_interp = np.interp(x_num, x_exact, y_exact)
    abs_error = np.abs(y_num - y_interp)
    rel_error = abs_error / np.abs(y_interp) * 100
    max_abs_error = np.max(abs_error)
    mean_abs_error = np.mean(abs_error)
    rms_error = np.sqrt(np.mean(abs_error**2))
    return {
        'abs_errors': abs_error,
        'rel_errors': rel_error,
        'max_abs_error': max_abs_error,
        'mean_abs_error': mean_abs_error,
        'rms_error': rms_error
    }

def main():
    print("=" * 80)
    print("第七章实践题目2：常微分方程初值问题数值解法对比分析")
    print("=" * 80)
    print("\n微分方程: y' = -xy²,  0 ≤ x ≤ 5,  y(0) = 2")
    print("精确解: y(x) = 2/(x² + 1)")
    print("-" * 80)

    x0, y0, xf = 0.0, 2.0, 5.0
    step_sizes = [0.5, 0.2, 0.1, 0.05]
    methods = [
        ('Euler Predictor-Corrector', euler_predictor_corrector),
        ('Classical 4th-Order RK', runge_kutta_4),
        ('4th-Order Adams PC', adams_pc_4th)
    ]

    x_fine = np.linspace(x0, xf, 1000)
    y_exact_fine = exact_solution(x_fine)

    results = {}
    for method_name, method_func in methods:
        results[method_name] = {}
        print(f"\n【{method_name}】")
        print("-" * 60)
        for h in step_sizes:
            x_num, y_num = method_func(x0, y0, xf, h)
            errors = compute_errors(x_num, y_num, x_fine, y_exact_fine)
            results[method_name][h] = {
                'x': x_num,
                'y': y_num,
                'errors': errors
            }
            print(f"  步长 h = {h:.2f}: 最大绝对误差 = {errors['max_abs_error']:.6e}, "
                  f"均方根误差 = {errors['rms_error']:.6e}")

    fig, axes = plt.subplots(2, 2, figsize=(14, 10))

    colors = ['blue', 'red', 'green']
    markers = ['o', 's', '^']

    ax1 = axes[0, 0]
    ax1.plot(x_fine, y_exact_fine, 'k-', linewidth=2, label='Exact Solution')
    for idx, (method_name, method_func) in enumerate(methods):
        x_num, y_num = method_func(x0, y0, xf, 0.1)
        ax1.plot(x_num, y_num, color=colors[idx], marker=markers[idx],
                markersize=5, linewidth=1.5, linestyle='--',
                label=f'{method_name} (h=0.1)', markevery=5)
    ax1.set_xlabel('x', fontsize=12)
    ax1.set_ylabel('y', fontsize=12)
    ax1.set_title('Numerical Solutions vs Exact Solution (h=0.1)', fontsize=13)
    ax1.legend(loc='upper right', fontsize=9)
    ax1.grid(True, alpha=0.3)

    ax2 = axes[0, 1]
    for idx, (method_name, method_func) in enumerate(methods):
        x_num, y_num = method_func(x0, y0, xf, 0.1)
        errors = compute_errors(x_num, y_num, x_fine, y_exact_fine)
        ax2.semilogy(x_num, errors['abs_errors'], color=colors[idx],
                    marker=markers[idx], markersize=4, linewidth=1.5,
                    label=f'{method_name}', markevery=3)
    ax2.set_xlabel('x', fontsize=12)
    ax2.set_ylabel('Absolute Error (log scale)', fontsize=12)
    ax2.set_title('Absolute Error Distribution (h=0.1)', fontsize=13)
    ax2.legend(loc='upper left', fontsize=9)
    ax2.grid(True, alpha=0.3, which='both')

    ax3 = axes[1, 0]
    h_values = list(step_sizes)
    for idx, method_name in enumerate([m[0] for m in methods]):
        max_errors = [results[method_name][h]['errors']['max_abs_error']
                     for h in h_values]
        ax3.loglog(h_values, max_errors, color=colors[idx],
                  marker=markers[idx], markersize=8, linewidth=2,
                  label=f'{method_name}')
    ax3.set_xlabel('Step Size h (log scale)', fontsize=12)
    ax3.set_ylabel('Max Absolute Error (log scale)', fontsize=12)
    ax3.set_title('Error Convergence Analysis', fontsize=13)
    ax3.legend(loc='upper left', fontsize=9)
    ax3.grid(True, alpha=0.3, which='both')

    reference_h = np.array(step_sizes)
    ref_order2 = 0.01 * reference_h**2
    ref_order4 = 0.001 * reference_h**4
    ax3.loglog(reference_h, ref_order2, 'k--', linewidth=1, alpha=0.7,
              label=r'$O(h^2)$ Reference')
    ax3.loglog(reference_h, ref_order4, 'k:', linewidth=1, alpha=0.7,
              label=r'$O(h^4)$ Reference')
    ax3.legend(loc='upper left', fontsize=8)

    ax4 = axes[1, 1]
    x_data = np.linspace(x0, xf, 200)
    bar_width = 0.25
    index = np.arange(len(step_sizes))
    for idx, method_name in enumerate([m[0] for m in methods]):
        rms_values = [results[method_name][h]['errors']['rms_error']
                     for h in step_sizes]
        bars = ax4.bar(index + idx * bar_width, rms_values, bar_width,
                      label=method_name, color=colors[idx], alpha=0.8)
        for bar, val in zip(bars, rms_values):
            height = bar.get_height()
            ax4.text(bar.get_x() + bar.get_width() / 2., height,
                    f'{val:.2e}', ha='center', va='bottom', fontsize=7)
    ax4.set_xlabel('Step Size h', fontsize=12)
    ax4.set_ylabel('RMSE (log scale)', fontsize=12)
    ax4.set_title('RMSE Comparison at Different Step Sizes', fontsize=13)
    ax4.set_xticks(index + bar_width)
    ax4.set_xticklabels([str(h) for h in step_sizes])
    ax4.legend(loc='upper right', fontsize=9)
    ax4.set_yscale('log')
    ax4.grid(True, alpha=0.3, axis='y')

    plt.tight_layout()
    plt.savefig('ode_comparison_results.png', dpi=150, bbox_inches='tight')
    print("\n\n图表已保存为: ode_comparison_results.png")

    print("\n" + "=" * 80)
    print("详细结果数据表")
    print("=" * 80)
    print(f"\n{'方法':<20} {'步长':<8} {'最大绝对误差':<18} {'均方根误差':<18}")
    print("-" * 70)
    for method_name in [m[0] for m in methods]:
        for h in step_sizes:
            err = results[method_name][h]['errors']
            print(f"{method_name:<20} {h:<8.2f} {err['max_abs_error']:<18.6e} "
                  f"{err['rms_error']:<18.6e}")

    print("\n" + "=" * 80)
    print("关键点数值对比 (x = 1, 2, 3, 4, 5)")
    print("=" * 80)
    key_points = [1, 2, 3, 4, 5]
    print(f"\n{'x值':<6}", end="")
    for method_name in [m[0] for m in methods]:
        print(f"{method_name:<22}", end="")
    print(f"{'精确解':<12}")
    print("-" * 100)
    for x_val in key_points:
        print(f"{x_val:<6.1f}", end="")
        for method_name, method_func in methods:
            x_num, y_num = method_func(x0, y0, xf, 0.1)
            idx = np.argmin(np.abs(x_num - x_val))
            print(f"{y_num[idx]:<22.10f}", end="")
        print(f"{exact_solution(x_val):<12.10f}")

    plt.show()

if __name__ == "__main__":
    main()
