"""
算法竞赛 Python 通用模板
比赛时直接复制粘贴此文件内容，在此基础上写题解

注意：Python 在 CPL 系统中可能会遇到递归深度限制和时间限制问题。
      如果题目数据量较大 (n > 10^5 的复杂操作)，建议使用 C++。
      但对于输入规模较小、或字符串处理为主的题目，Python 效率更高。
"""

import sys
import math
import itertools
import collections
import heapq
import bisect
import functools
import random
from typing import List, Tuple, Set, Dict, Optional

# ============ 输入输出优化 ============
# 使用 sys.stdin.read() 一次性读入所有内容，自己分割
# 这是 Python 在算法竞赛中最快的输入方式

def input():
    return sys.stdin.readline().rstrip('\n')

def ints() -> List[int]:
    """读取一行整数，返回列表"""
    return list(map(int, input().split()))

# 如果需要更快（万级以上数据），使用 sys.stdin.buffer.read()
# data = sys.stdin.buffer.read().split()
# n = int(data[0]); arr = list(map(int, data[1:]))

# ============ 常用函数别名 ============
gcd = math.gcd       # Python 3.5+ 自带 math.gcd
lcm = math.lcm       # Python 3.9+ 自带 math.lcm
isqrt = math.isqrt   # 整数平方根

# ============ 数学工具函数 ============

def lcm_custom(a: int, b: int) -> int:
    """最小公倍数 (Python < 3.9 的回退)"""
    return a // math.gcd(a, b) * b

def qpow(a: int, b: int, m: int = 10**9 + 7) -> int:
    """快速幂 a^b % m"""
    res = 1
    a %= m
    while b:
        if b & 1:
            res = res * a % m
        a = a * a % m
        b >>= 1
    return res

def ceil_div(a: int, b: int) -> int:
    """向上取整除法"""
    return (a + b - 1) // b

def is_prime(n: int) -> bool:
    """判断质数 O(sqrt(n))"""
    if n < 2:
        return False
    i = 2
    while i * i <= n:
        if n % i == 0:
            return False
        i += 1
    return True

def sieve(n: int) -> List[int]:
    """埃氏筛法：返回 [2, n] 内所有质数"""
    is_p = [True] * (n + 1)
    is_p[0] = is_p[1] = False
    for i in range(2, int(n ** 0.5) + 1):
        if is_p[i]:
            for j in range(i * i, n + 1, i):
                is_p[j] = False
    return [i for i in range(2, n + 1) if is_p[i]]

# ============ 常用数据结构速查 ============
# 以下列出 Python 内置数据结构的核心用法，详见 day1 文件

# --- list (动态数组，对应 C++ vector) ---
# arr = []
# arr.append(x)          # 末尾添加
# arr.pop()              # 末尾删除
# arr.sort()             # 升序排序
# arr.sort(reverse=True) # 降序排序
# arr.sort(key=lambda x: (x[0], -x[1]))  # 自定义排序

# --- dict (哈希表，对应 C++ unordered_map) ---
# d = {}
# d[key] = value
# d.get(key, default)    # 安全取值
# collections.Counter(iterable)   # 计数专用

# --- set (哈希集合，对应 C++ unordered_set) ---
# s = set()
# s.add(x); s.remove(x); x in s

# --- 有序字典/集合 (对应 C++ map/set) ---
# Python 标准库无内置平衡树，需要排序时转为 list 再 sort

# --- 堆 (对应 C++ priority_queue) ---
# import heapq
# pq = []
# heapq.heappush(pq, x)       # 小顶堆
# heapq.heappop(pq)
# # 大顶堆：存负数 heapq.heappush(pq, -x); y = -heapq.heappop(pq)

# --- 双端队列 (对应 C++ deque) ---
# from collections import deque
# dq = deque()
# dq.append(x); dq.popleft(); dq.appendleft(x)

# --- 二分查找 (对应 C++ lower_bound) ---
# import bisect
# pos = bisect.bisect_left(arr, x)   # 第一个 >= x 的位置
# pos = bisect.bisect_right(arr, x)  # 第一个 > x 的位置

# ============ 递归深度设置 ============
# 如果使用 DFS 递归，务必设置较大的递归深度
sys.setrecursionlimit(1000000)

# ============ 模数相关常量 ============
MOD = 10**9 + 7
INF = 10**18

# ============ 核心：每题的 solve 函数 ============
def solve():
    """在此编写每道题的解法"""
    pass

# ============ 主程序 ============
if __name__ == '__main__':
    # 单组数据
    # solve()

    # 多组数据：第一行是测试组数 T
    t = int(input())
    for _ in range(t):
        solve()
