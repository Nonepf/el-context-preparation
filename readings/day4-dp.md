# Day 4：动态规划（DP）

> **学习目标**：掌握 DP 的核心思维——状态定义和状态转移方程。重点攻克 01 背包、完全背包、最长上升子序列、最大子数组和。
>

---

## 0. 前置理解：DP 的本质

### 一句话定义

**DP = 记忆化搜索 + 最优子结构**

- **状态**：用一个（或多个）变量描述"当前做到哪了"、"当前是什么条件"
- **状态转移**：当前状态的最优解，可以由之前某些状态推导出来
- **最优子结构**：大问题的最优解包含子问题的最优解

### DP vs 贪心

| | 贪心 | DP |
|------|------|-----|
| 决策 | 只看局部，不回退 | 考虑所有可能，取最优 |
| 适合 | 局部最优 = 全局最优 | 需要全局比较 |
| 代码量 | 少 | 中等 |
| 证明 | 需要直觉或证明 | 不需要（保证正确） |

**判断方法**：如果"选 or 不选"的决策会影响后续选项，通常是 DP。如果排序后按顺序贪心即可，就是贪心。

---

## 1. 背包 DP

### 1.1 01 背包（每个物品最多选一次）

**问题**：有 n 个物品，第 i 个重量为 w[i]，价值为 v[i]。背包容量为 C。选若干物品使总价值最大，总体积不超过 C。

**状态**：`dp[i][j]` = 考虑前 i 个物品，背包容量为 j 时的最大价值。

**转移**：对第 i 个物品，要么不选，要么选：
```
dp[i][j] = max(
    dp[i-1][j],                  // 不选第 i 个
    dp[i-1][j - w[i]] + v[i]     // 选第 i 个（前提 j ≥ w[i]）
)
```

#### 二维版本（理解用）

```cpp
int knapsack(int n, int C, vector<int>& w, vector<int>& v) {
    vector<vector<int>> dp(n + 1, vector<int>(C + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= C; ++j) {
            dp[i][j] = dp[i - 1][j];  // 不选
            if (j >= w[i - 1]) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i - 1]] + v[i - 1]);
            }
        }
    }
    return dp[n][C];
}
```

#### 一维优化版本（比赛必用）

观察：`dp[i][*]` 只取决于 `dp[i-1][*]`，可以滚动。

**关键**：容量 j 必须**倒序遍历**！正序会导致物品被多次选择（变成完全背包）。

```cpp
int knapsack(int n, int C, vector<int>& w, vector<int>& v) {
    vector<int> dp(C + 1, 0);

    for (int i = 0; i < n; ++i) {          // 遍历每个物品
        for (int j = C; j >= w[i]; --j) {  // 倒序遍历容量！
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
        }
    }
    return dp[C];
}
```

**为什么倒序？** 画个图理解：
- 正序：更新 `dp[j]` 时，`dp[j - w[i]]` 可能已经用了第 i 个物品（即被覆盖了）
- 倒序：更新 `dp[j]` 时，`dp[j - w[i]]` 还是上一轮的（第 i 个物品未用过），保证每个物品只用一次

#### Python

```python
def knapsack(n, C, w, v):
    dp = [0] * (C + 1)
    for i in range(n):
        for j in range(C, w[i] - 1, -1):  # 倒序！
            dp[j] = max(dp[j], dp[j - w[i]] + v[i])
    return dp[C]
```

**记忆模板**：
```
for 物品:
    for j from C down to w[i]:   // 倒序！
        dp[j] = max(dp[j], dp[j - w[i]] + v[i])
```

### 1.2 完全背包（每个物品可以选无限次）

**唯一变化**：容量 j 改为**正序**遍历。

```cpp
int unboundedKnapsack(int n, int C, vector<int>& w, vector<int>& v) {
    vector<int> dp(C + 1, 0);

    for (int i = 0; i < n; ++i) {
        for (int j = w[i]; j <= C; ++j) {  // 正序遍历！
            dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
        }
    }
    return dp[C];
}
```

**为什么正序？** 更新 `dp[j]` 时，`dp[j - w[i]]` 已经是考虑过第 i 个物品的状态，所以物品可以被多次使用。

**记忆**：
- 01 背包 = 倒序
- 完全背包 = 正序

### 1.3 背包问题的识别

题目中出现以下模式，考虑背包：

- "给定一组物品，每个物品有 XXX，在不超过 YYY 的条件下最大化 ZZZ"
- "容量"、"限制"、"两种选择"
- 多重背包（物品有数量限制）可以二进制优化转化为 01 背包

### 1.4 背包模型的扩展

#### 恰好装满 vs 不超过

```cpp
// 恰好装满容量 C：初始化 dp[0] = 0, dp[1..C] = -INF
vector<int> dp(C + 1, -INF);
dp[0] = 0;

// 不超过容量 C：初始化全 0（上面的做法）
vector<int> dp(C + 1, 0);
```

#### 求方案数（计数型背包）

```cpp
// 01 背包求恰好装满的方案数
vector<int> dp(C + 1, 0);
dp[0] = 1;
for (int i = 0; i < n; ++i) {
    for (int j = C; j >= w[i]; --j) {
        dp[j] = (dp[j] + dp[j - w[i]]) % MOD;
    }
}
// dp[C] 就是方案数
```

#### 二维费用背包

```cpp
// 每个物品有重量 w[i] 和体积 b[i]，背包容量 C，容积 D
// dp[j][k] = 重量 j、体积 k 下的最大价值
vector<vector<int>> dp(C + 1, vector<int>(D + 1, 0));
for (int i = 0; i < n; ++i)
    for (int j = C; j >= w[i]; --j)
        for (int k = D; k >= b[i]; --k)
            dp[j][k] = max(dp[j][k], dp[j - w[i]][k - b[i]] + v[i]);
```

---

## 2. 线性 DP

### 2.1 最大子数组和（Kadane 算法）

**问题**：给定整数数组，找和最大的连续子数组。

**状态定义**：`dp[i]` = 以 a[i] 结尾的最大子数组和。

**转移**：要么接在前面的子数组后面，要么另起炉灶。
```
dp[i] = max(dp[i-1] + a[i], a[i])
```

**空间优化**：`dp[i]` 只依赖 `dp[i-1]`，用一个变量即可。

```cpp
int maxSubArray(vector<int>& nums) {
    int cur = 0, ans = nums[0];  // 或者 INT_MIN
    for (int x : nums) {
        cur = max(cur + x, x);
        ans = max(ans, cur);
    }
    return ans;
}
```

```python
def max_sub_array(nums):
    cur = ans = nums[0]
    for x in nums[1:]:
        cur = max(cur + x, x)
        ans = max(ans, cur)
    return ans
```

### 2.2 最长上升子序列（LIS）

**问题**：给定数组，找最长的严格递增子序列（不要求连续）。

#### O(n²) 做法——直观易懂

```cpp
// dp[i] = 以 a[i] 结尾的最长上升子序列长度
int lengthOfLIS(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1);
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}
```

#### O(n log n) 做法——贪心 + 二分（比赛推荐）

```cpp
// 维护数组 tails，tails[k] 表示长度为 k+1 的上升子序列的最小末尾值
int lengthOfLIS(vector<int>& nums) {
    vector<int> tails;
    for (int x : nums) {
        // 在 tails 中找第一个 >= x 的位置
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);  // x 比所有都大，LIS 长度 +1
        } else {
            *it = x;  // 用更小的值替换，给后面留更多空间
        }
    }
    return tails.size();
}
```

#### Python

```python
import bisect

def length_of_LIS(nums):
    tails = []
    for x in nums:
        i = bisect.bisect_left(tails, x)
        if i == len(tails):
            tails.append(x)
        else:
            tails[i] = x
    return len(tails)
```

**这个算法的直觉**：tails 数组维护的是"各种长度下，LIS 末尾的最小可能值"。因为更小的末尾值更容易被后面的数接上。

### 2.3 最长公共子序列（LCS）

**问题**：两个字符串 s 和 t，找它们的最长公共子序列（不要求连续）。

```cpp
// dp[i][j] = s[0..i-1] 和 t[0..j-1] 的 LCS 长度
int longestCommonSubsequence(string s, string t) {
    int n = s.size(), m = t.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s[i - 1] == t[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][m];
}
```

**理解**：当字符相同时，LCS 可以增长；不同时，取"扔掉 s 的最后一个字符"和"扔掉 t 的最后一个字符"的较大者。

---

## 3. 状态机 DP

**思想**：每一天/每一步有"多个状态"，状态之间可以转移。常用于买卖股票系列。

### 3.1 经典问题：买卖股票（只能买卖一次）

> 数组 prices 表示每天股价。只能买入一次、卖出一次。求最大收益。

```cpp
// dp[i][0] = 第 i 天结束、不持有股票的最大收益
// dp[i][1] = 第 i 天结束、持有股票的最大收益

int maxProfit(vector<int>& prices) {
    int n = prices.size();
    // 空间优化：只维护当天两个状态
    int hold = -prices[0];   // 第一天买入
    int cash = 0;             // 第一天不买

    for (int i = 1; i < n; ++i) {
        hold = max(hold, -prices[i]);          // 保持持有，或今天买入
        cash = max(cash, hold + prices[i]);    // 保持空仓，或今天卖出
    }
    return cash;
}
```

### 3.2 买卖股票（可以买卖多次，有冷冻期/手续费）

状态机的通用框架：

```
        买入
  持有 ←────── 空仓
   │              ↑
   └── 卖出 ──────┘

有时加上 "冷冻" 状态（卖出后不能立刻买入）
```

通用做法：**枚举所有状态，枚举所有转移方式，取 max**。

---

## 4. DP 的通用解题步骤

### 四步法（遇到 DP 题按这个来）

1. **状态定义**：`dp[i]` 或 `dp[i][j]` 表示什么？
   - 通常是"前 i 个元素中，满足某条件的最优值"
   - 有时需要加额外维度（是否选了第 i 个、当前容量等）

2. **状态转移方程**：`dp[i]` 可以由哪些之前的什么状态转移过来？
   - 常见模式："选 or 不选" / "接在前面 or 另起炉灶" / "从哪里来"

3. **初始化和边界**：
   - `dp[0]` 通常需要一个巧妙的值
   - 取 max 时初始化为 -INF，取 min 时初始化为 INF
   - "恰好"的约束通常通过初始化表达

4. **答案位置**：答案可能不是 `dp[n]`，而是 `max(dp[0..n])` 或 `dp[某条件]`

### 例题：打家劫舍（House Robber）

> 一排房子，每个有金额。不能偷相邻的。问最大收益。

**状态**：`dp[i]` = 考虑前 i 个房子的最大收益。
**转移**：`dp[i] = max(dp[i-1], dp[i-2] + a[i-1])`（不偷 vs 偷）
**初始化**：`dp[0] = 0, dp[1] = a[0]`
**答案**：`dp[n]`

```cpp
int rob(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    // 空间优化：只用两个变量
    int prev2 = 0;          // dp[i-2]
    int prev1 = nums[0];    // dp[i-1]
    for (int i = 1; i < n; ++i) {
        int cur = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
```

---

## 5. 区间 DP（了解即可）

**特征**：问题涉及一个区间，大区间的最优解依赖于小区间。

**典型问题**：石子合并——每次合并相邻两堆，代价为两堆之和，求最小总代价。

```cpp
// dp[i][j] = 合并区间 [i, j] 的最小代价
// 枚举中间切分点 k：
// dp[i][j] = min(dp[i][j], dp[i][k] + dp[k+1][j] + sum(i, j))

for (int len = 2; len <= n; ++len) {           // 区间长度从小到大
    for (int i = 0; i + len - 1 < n; ++i) {    // 区间起点
        int j = i + len - 1;                   // 区间终点
        dp[i][j] = INF;
        for (int k = i; k < j; ++k) {          // 枚举切分点
            dp[i][j] = min(dp[i][j],
                dp[i][k] + dp[k + 1][j] + pre[j + 1] - pre[i]);
        }
    }
}
```

**区间 DP 的模板特征**：三重循环 —— 长度 → 起点 → 切分点。

---

## 6. DP 常见坑

### 6.1 初始化的差别

```cpp
// "恰好"约束：dp[0] = 0, 其余 = -INF（求 max 时）
// "不超过"约束：全部 = 0
// "至少"约束：dp[0] = 0, 其余 = INF，转移时 max(0, j - w)

// 例子：零钱兑换（最少硬币数凑出 amount）
vector<int> dp(amount + 1, INF);
dp[0] = 0;
for (int coin : coins)
    for (int j = coin; j <= amount; ++j)
        dp[j] = min(dp[j], dp[j - coin] + 1);
// dp[amount] == INF ? -1 : dp[amount]
```

### 6.2 大数取模

```cpp
// 计数型 DP 通常需要取模
const int MOD = 1e9 + 7;
dp[j] = (dp[j] + dp[j - w[i]]) % MOD;

// 注意：负数取模
int x = -5;
int mod_x = (x % MOD + MOD) % MOD;  // 确保结果在 [0, MOD-1]
```

### 6.3 递归 + 记忆化 vs 递推

比赛中**优先用递推**（for 循环）。递归写法容易爆栈或超时（函数调用开销大）。

但如果状态数很多且大部分用不到，记忆化搜索更省（如博弈 DP）。

---

## Day 4 配套练习

| 题号 | 题目 | 考察点 |
|------|------|--------|---------|
| 1 | 经典 01 背包 | 背包模板 |
| 2 | 完全背包（每种无限个） | 正序 vs 倒序 |
| 3 | 最大子数组和 | Kadane |
| 4 | 最长上升子序列 | LIS O(n log n) |
| 5 | 打家劫舍 | 线性 DP |
| 6 | 买卖股票最佳时机 | 状态机 DP |
| 7 | 最长公共子序列 | 二维 DP |
| 8 | 零钱兑换（最少硬币凑金额） | 完全背包变体 |

---

## 快速参考卡片

```
┌──────────────────────────────────────────────────────────┐
│  01 背包 (每物一次)                                        │
│  for 物品:                                                │
│      for j = C down to w[i]:   // 倒序！                 │
│          dp[j] = max(dp[j], dp[j-w[i]] + v[i])           │
│                                                             │
│  完全背包 (每物无限次)                                      │
│  for 物品:                                                │
│      for j = w[i] to C:        // 正序！                 │
│          dp[j] = max(dp[j], dp[j-w[i]] + v[i])           │
│                                                             │
│  LIS O(n log n)                                            │
│  tails = []                                               │
│  for x in nums:                                           │
│      pos = lower_bound(tails, x)                          │
│      if pos == end: tails.pb(x) else tails[pos] = x      │
│                                                             │
│  最大子数组和                                               │
│  cur = max(cur + x, x)                                    │
│  ans = max(ans, cur)                                      │
│                                                             │
│  DP 四步: 状态→转移→初始化→答案                             │
└──────────────────────────────────────────────────────────┘
```
