"""
Romberg数值积分算法实现
用于计算方法第六章实践题目第二题
"""

import math

def romberg(f, a, b, n=10, tol=1e-12):
    """
    使用Romberg算法计算定积分 ∫_a^b f(x) dx
    
    参数:
        f: 被积函数，接受一个浮点数参数
        a, b: 积分下限和上限
        n: 最大迭代次数（默认10）
        tol: 收敛容差（默认1e-12）
    
    返回:
        integral: 积分近似值
        table: Romberg表（二维列表）
    """
    # 初始化Romberg表，table[i][j] 对应 R(i,j)
    table = [[0.0 for _ in range(n+1)] for _ in range(n+1)]
    
    # 第一步：计算R(0,0) = (b-a)/2 * [f(a) + f(b)]
    h = b - a
    table[0][0] = h * 0.5 * (f(a) + f(b))
    
    # 迭代计算
    for i in range(1, n+1):
        # 步长减半
        h *= 0.5
        
        # 计算复化梯形公式 R(i,0)
        # 首先计算新增节点的函数值之和
        total = 0.0
        for k in range(1, 2**(i-1) + 1):
            x = a + (2*k - 1) * h
            total += f(x)
        
        # R(i,0) = 0.5 * R(i-1,0) + h * total
        table[i][0] = 0.5 * table[i-1][0] + h * total
        
        # Richardson外推
        for j in range(1, i+1):
            factor = 4.0**j
            table[i][j] = (factor * table[i][j-1] - table[i-1][j-1]) / (factor - 1.0)
        
        # 检查收敛性：如果 |R(i,i) - R(i-1,i-1)| < tol，则提前终止
        if i > 0 and abs(table[i][i] - table[i-1][i-1]) < tol:
            # 只保留前i+1行
            table = [row[:i+1] for row in table[:i+1]]
            break
    
    return table[-1][-1], table


def test_romberg():
    """测试Romberg算法"""
    
    # 测试函数1: f(x) = x^2 * exp(-x^2), 积分区间 [0, 2]
    def f1(x):
        return x**2 * math.exp(-x**2)
    
    # 测试函数2: f(x) = cot(x) = cos(x)/sin(x), 积分区间 [pi/2, 3*pi/4]
    def f2(x):
        return math.cos(x) / math.sin(x)
    
    print("Romberg算法测试")
    print("=" * 50)
    
    # 计算第一个积分
    a1, b1 = 0.0, 2.0
    integral1, table1 = romberg(f1, a1, b1)
    print(f"积分1: ∫_0^2 x^2 * exp(-x^2) dx")
    print(f"近似值: {integral1:.15f}")
    print(f"迭代次数: {len(table1)}")
    
    # 显示Romberg表的前几行
    print("\nRomberg表前几行:")
    for i in range(min(4, len(table1))):
        row_str = "  ".join(f"{table1[i][j]:.10f}" for j in range(i+1))
        print(f"R({i},0:{i}): {row_str}")
    
    print("\n" + "=" * 50)
    
    # 计算第二个积分
    a2, b2 = math.pi/2, 3*math.pi/4
    integral2, table2 = romberg(f2, a2, b2)
    print(f"积分2: ∫_π/2^{3*math.pi/4:.4f} cot(x) dx")
    print(f"近似值: {integral2:.15f}")
    print(f"迭代次数: {len(table2)}")
    
    # 显示Romberg表的前几行
    print("\nRomberg表前几行:")
    for i in range(min(4, len(table2))):
        row_str = "  ".join(f"{table2[i][j]:.10f}" for j in range(i+1))
        print(f"R({i},0:{i}): {row_str}")
    
    print("\n" + "=" * 50)
    
    # 与精确值比较（如果已知）
    # 第一个积分的精确值可以通过数值方法或特殊函数获得
    # 这里使用高精度的Romberg结果作为参考
    integral1_high, _ = romberg(f1, a1, b1, n=15)
    error1 = abs(integral1 - integral1_high)
    print(f"积分1误差估计: {error1:.2e}")
    
    integral2_high, _ = romberg(f2, a2, b2, n=15)
    error2 = abs(integral2 - integral2_high)
    print(f"积分2误差估计: {error2:.2e}")
    
    # 收敛性分析
    print("\n收敛性分析:")
    print("对于积分1，Romberg表对角线元素:")
    for i in range(min(5, len(table1))):
        print(f"  R({i},{i}) = {table1[i][i]:.15f}")
    
    print("\n对于积分2，Romberg表对角线元素:")
    for i in range(min(5, len(table2))):
        print(f"  R({i},{i}) = {table2[i][i]:.15f}")


def detailed_analysis():
    """
    详细分析Romberg算法的计算结果
    包括与精确值的比较、误差分析和收敛性讨论
    """
    print("\n\n详细分析报告")
    print("=" * 60)
    
    # 定义被积函数
    def f1(x):
        return x**2 * math.exp(-x**2)
    
    def f2(x):
        return math.cos(x) / math.sin(x)
    
    # 积分区间
    a1, b1 = 0.0, 2.0
    a2, b2 = math.pi/2, 3*math.pi/4
    
    # 使用高精度Romberg计算参考值
    print("计算高精度参考值...")
    ref1, table1 = romberg(f1, a1, b1, n=20)
    ref2, table2 = romberg(f2, a2, b2, n=20)
    
    # 第二个积分的精确解析解
    exact2 = math.log(math.sin(b2)) - math.log(math.sin(a2))
    
    print(f"\n积分1: ∫_0^2 x^2 * exp(-x^2) dx")
    print(f"  Romberg近似值: {ref1:.15f}")
    print(f"  高精度参考值: {ref1:.15f}")
    
    print(f"\n积分2: ∫_π/2^{3*math.pi/4:.4f} cot(x) dx")
    print(f"  Romberg近似值: {ref2:.15f}")
    print(f"  解析精确值:   {exact2:.15f}")
    print(f"  绝对误差:     {abs(ref2 - exact2):.2e}")
    
    # 收敛速度分析
    print("\n收敛速度分析:")
    print("积分1的对角线元素收敛情况:")
    for i in range(min(6, len(table1))):
        if i > 0:
            error = abs(table1[i][i] - ref1)
            ratio = abs(table1[i-1][i-1] - ref1) / error if error > 0 else 0
            print(f"  R({i},{i}) = {table1[i][i]:.15f}, 误差: {error:.2e}, 误差比: {ratio:.2f}")
        else:
            print(f"  R({i},{i}) = {table1[i][i]:.15f}")
    
    print("\n积分2的对角线元素收敛情况:")
    for i in range(min(6, len(table2))):
        if i > 0:
            error = abs(table2[i][i] - exact2)
            ratio = abs(table2[i-1][i-1] - exact2) / error if error > 0 else 0
            print(f"  R({i},{i}) = {table2[i][i]:.15f}, 误差: {error:.2e}, 误差比: {ratio:.2f}")
        else:
            print(f"  R({i},{i}) = {table2[i][i]:.15f}")
    

if __name__ == "__main__":
    test_romberg()
    detailed_analysis()
    