import numpy as np
import warnings
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm

warnings.filterwarnings('ignore', category=UserWarning, module='matplotlib')
warnings.filterwarnings('ignore', message=".*glyph.*")

plt.rcParams['font.sans-serif'] = ['DejaVu Sans', 'Arial', 'sans-serif']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['mathtext.fontset'] = 'dejavusans'
plt.rcParams['font.family'] = 'sans-serif'

A = np.array([[-12, 3, 3],
              [3, 1, -2],
              [3, -2, 7]], dtype=float)

def inverse_power_method(A, p, tol=1e-10, max_iter=1000):
    n = A.shape[0]
    I = np.eye(n)
    B = A - p * I
    
    v = np.ones(n)
    v = v / np.linalg.norm(v)
    eigenvalues = []
    errors = []
    
    lambda_old = 0
    for k in range(max_iter):
        try:
            w = np.linalg.solve(B, v)
        except np.linalg.LinAlgError:
            return None, None, [], []
        
        w_norm = np.linalg.norm(w)
        v_new = w / w_norm
        
        rayleigh = (v_new.T @ A @ v_new) / (v_new.T @ v_new)
        eigenvalues.append(rayleigh)
        
        error = abs(rayleigh - lambda_old) if k > 0 else abs(rayleigh)
        errors.append(error)
        
        if error < tol and k > 0:
            return rayleigh, v_new, eigenvalues, errors
        
        v = v_new
        lambda_old = rayleigh
    
    return rayleigh, v, eigenvalues, errors

def power_method(A, tol=1e-10, max_iter=1000):
    n = A.shape[0]
    v = np.ones(n)
    v = v / np.linalg.norm(v)
    eigenvalues = []
    errors = []
    
    lambda_old = 0
    for k in range(max_iter):
        w = A @ v
        rayleigh = (v.T @ w) / (v.T @ v)
        v_new = w / np.linalg.norm(w)
        
        eigenvalues.append(rayleigh)
        
        error = abs(rayleigh - lambda_old) if k > 0 else abs(rayleigh)
        errors.append(error)
        
        if error < tol and k > 0:
            return rayleigh, v_new, eigenvalues, errors
        
        v = v_new
        lambda_old = rayleigh
    
    return rayleigh, v, eigenvalues, errors

print("=" * 70)
print("第八章实践题目：矩阵特征值计算")
print("矩阵 A:")
print(A)
print("=" * 70)

print("\n【第一部分】反幂法求解最接近 p = -13 的特征值")
print("-" * 50)
p = -13
lambda_inv, v_inv, eig_history_inv, err_history_inv = inverse_power_method(A, p)

if lambda_inv is not None:
    print(f"位移量 p = {p}")
    print(f"收敛后的特征值 λ ≈ {lambda_inv:.12f}")
    print(f"对应的特征向量 (归一化):")
    print(f"  [{v_inv[0]:.10f}, {v_inv[1]:.10f}, {v_inv[2]:.10f}]")
    print(f"迭代次数: {len(eig_history_inv)}")
    print(f"最终误差: {err_history_inv[-1]:.2e}")
else:
    print("反幂法计算失败！")

print("\n【第二部分】幂乘法求解主特征值")
print("-" * 50)
lambda_pow, v_pow, eig_history_pow, err_history_pow = power_method(A)

if lambda_pow is not None:
    print(f"主特征值 (按模最大) λ ≈ {lambda_pow:.12f}")
    print(f"对应的特征向量 (归一化):")
    print(f"  [{v_pow[0]:.10f}, {v_pow[1]:.10f}, {v_pow[2]:.10f}]")
    print(f"迭代次数: {len(eig_history_pow)}")
    print(f"最终误差: {err_history_pow[-1]:.2e}")

print("\n【第三部分】使用 numpy 验证（精确解）")
print("-" * 50)
eigenvalues_exact, eigenvectors_exact = np.linalg.eig(A)
print("所有特征值 (精确值):")
for i, ev in enumerate(eigenvalues_exact):
    print(f"  λ{i+1} = {ev:.12f}")

idx_closest = np.argmin(np.abs(eigenvalues_exact - (-13)))
print(f"\n最接近 -13 的特征值: {eigenvalues_exact[idx_closest]:.12f}")
print(f"对应的精确特征向量:")
exact_vec = eigenvectors_exact[:, idx_closest]
exact_vec_normalized = exact_vec / np.linalg.norm(exact_vec)
print(f"  [{exact_vec_normalized[0]:.10f}, {exact_vec_normalized[1]:.10f}, {exact_vec_normalized[2]:.10f}]")

idx_dominant = np.argmax(np.abs(eigenvalues_exact))
print(f"\n主特征值 (按模最大): {eigenvalues_exact[idx_dominant]:.12f}")

print("\n【第四部分】精度对比分析")
print("-" * 50)
if lambda_inv is not None:
    error_inv = abs(lambda_inv - eigenvalues_exact[idx_closest])
    rel_error_inv = error_inv / abs(eigenvalues_exact[idx_closest]) * 100
    print(f"反幂法结果 (目标: 最接近-13的特征值):")
    print(f"  计算值: {lambda_inv:.12f}")
    print(f"  精确值: {eigenvalues_exact[idx_closest]:.12f}")
    print(f"  绝对误差: {error_inv:.2e}")
    print(f"  相对误差: {rel_error_inv:.8f}%")

if lambda_pow is not None:
    error_pow = abs(lambda_pow - eigenvalues_exact[idx_dominant])
    rel_error_pow = error_pow / abs(eigenvalues_exact[idx_dominant]) * 100
    print(f"\n幂乘法结果 (目标: 主特征值):")
    print(f"  计算值: {lambda_pow:.12f}")
    print(f"  精确值: {eigenvalues_exact[idx_dominant]:.12f}")
    print(f"  绝对误差: {error_pow:.2e}")
    print(f"  相对误差: {rel_error_pow:.8f}%")

print("\n【第五部分】收敛速度分析")
print("-" * 50)
if len(err_history_inv) > 0 and len(err_history_pow) > 0:
    print(f"反幂法迭代次数: {len(err_history_inv)}")
    print(f"幂乘法迭代次数: {len(err_history_pow)}")
    
    print(f"\n反幂法收敛过程 (前10次迭代):")
    for i, (eig, err) in enumerate(zip(eig_history_inv[:10], err_history_inv[:10])):
        print(f"  迭代 {i+1:2d}: λ = {eig:.12f}, 误差 = {err:.2e}")
    
    print(f"\n幂乘法收敛过程 (前15次迭代):")
    for i, (eig, err) in enumerate(zip(eig_history_pow[:15], err_history_pow[:15])):
        print(f"  迭代 {i+1:2d}: λ = {eig:.12f}, 误差 = {err:.2e}")
    
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    
    ax1 = axes[0, 0]
    if len(eig_history_inv) > 0:
        ax1.plot(range(1, len(eig_history_inv)+1), eig_history_inv, 'b-o', markersize=4, linewidth=1.5, label='calculated value')
        ax1.axhline(y=eigenvalues_exact[idx_closest], color='r', linestyle='--', linewidth=2, label=f'precise value: {eigenvalues_exact[idx_closest]:.4f}')
    ax1.set_xlabel('Iteration')
    ax1.set_ylabel('Eigenvalue')
    ax1.set_title('Inverse Power Method Convergence')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    ax2 = axes[0, 1]
    if len(eig_history_pow) > 0:
        ax2.plot(range(1, len(eig_history_pow)+1), eig_history_pow, 'g-s', markersize=4, linewidth=1.5, label='calculated value')
        ax2.axhline(y=eigenvalues_exact[idx_dominant], color='r', linestyle='--', linewidth=2, label=f'precise value: {eigenvalues_exact[idx_dominant]:.4f}')
    ax2.set_xlabel('Iteration')
    ax2.set_ylabel('Eigenvalue')
    ax2.set_title('Power Method Convergence')
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    ax3 = axes[1, 0]
    if len(err_history_inv) > 0:
        ax3.semilogy(range(1, len(err_history_inv)+1), err_history_inv, 'b-o', markersize=4, linewidth=1.5)
    ax3.set_xlabel('Iteration')
    ax3.set_ylabel('Error (log scale)')
    ax3.set_title('Inverse Power Method Error Convergence')
    ax3.grid(True, alpha=0.3)
    
    ax4 = axes[1, 1]
    if len(err_history_pow) > 0:
        ax4.semilogy(range(1, len(err_history_pow)+1), err_history_pow, 'g-s', markersize=4, linewidth=1.5)
    ax4.set_xlabel('Iteration')
    ax4.set_ylabel('Error (log scale)')
    ax4.set_title('Power Method Error Convergence')
    ax4.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('eigenvalue_comparison.png', dpi=150, bbox_inches='tight')
    plt.close()
    print("\n收敛曲线图已保存为 'eigenvalue_comparison.png'")

