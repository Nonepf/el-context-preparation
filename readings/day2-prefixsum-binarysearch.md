# Day 2：前缀和、差分与二分答案

> **学习目标**：掌握三种"会了就拿分"的最高性价比算法。前缀和和二分答案是比赛中出现频率最高的技巧之一。
>

---

## 1. 一维前缀和

### 1.1 核心思想

**问题**：给定一个长度为 n 的数组，有 q 次询问，每次问区间 [l, r] 的元素之和。

**暴力做法**：每次询问都遍历 [l, r] 求和 → O(nq)，n=10^5, q=10^5 时超时。

**前缀和做法**：

1. 预处理一个数组 `pre[i]`，表示 `a[0] + a[1] + ... + a[i-1]`（前 i 个数的和）
2. 区间 [l, r] 的和 = `pre[r+1] - pre[l]`
3. 预处理 O(n)，每次查询 O(1)

### 1.2 代码实现

#### C++

```cpp
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

int main() {
    int n, q;
    cin >> n >> q;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // 构建前缀和数组
    // pre[i] = a[0] + ... + a[i-1] （前 i 个元素的和）
    // pre[0] = 0
    vector<ll> pre(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pre[i + 1] = pre[i] + a[i];
    }

    // 处理查询
    while (q--) {
        int l, r;  // 注意：题目可能用 0-indexed 或 1-indexed
        cin >> l >> r;
        // 若 l, r 是 0-indexed：[l, r] 的和
        cout << pre[r + 1] - pre[l] << '\n';
        // 若 l, r 是 1-indexed：[l, r] 的和
        // cout << pre[r] - pre[l - 1] << '\n';
    }
    return 0;
}
```

**记忆口诀**：`pre[i]` 是前 i 个数的和，`[l, r]` 的和 = `pre[r+1] - pre[l]`（0-indexed）。

#### Python

```python
n, q = map(int, input().split())
a = list(map(int, input().split()))

pre = [0] * (n + 1)
for i in range(n):
    pre[i + 1] = pre[i] + a[i]

for _ in range(q):
    l, r = map(int, input().split())
    # 0-indexed 区间 [l, r]
    print(pre[r + 1] - pre[l])
```

### 1.3 前缀和不止于"求和"

前缀和的思想可以扩展到任何**可逆运算**：

| 运算 | 区间查询公式 |
|------|-------------|
| 求和 | `pre[r+1] - pre[l]` |
| 异或和 | `pre[r+1] ^ pre[l]`（因为 a⊕a=0） |
| 乘积 | `pre[r+1] / pre[l]`（注意 MOD 下用逆元） |

### 1.4 经典应用："和为 K 的子数组"

> 给定数组，求有多少个连续子数组的和恰好等于 K。

```cpp
// 思路：pre[r+1] - pre[l] = K  ⇔  pre[l] = pre[r+1] - K
// 遍历右端点，用哈希表记录之前出现过的前缀和值

int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> cnt;  // 前缀和数值 -> 出现次数
    cnt[0] = 1;  // 空前缀的和为 0，出现 1 次
    int pre = 0, ans = 0;
    for (int x : nums) {
        pre += x;
        // 需要找值为 pre - k 的前缀的个数
        ans += cnt[pre - k];
        cnt[pre]++;
    }
    return ans;
}
```

```python
def subarray_sum(nums, k):
    cnt = {0: 1}
    pre = ans = 0
    for x in nums:
        pre += x
        ans += cnt.get(pre - k, 0)
        cnt[pre] = cnt.get(pre, 0) + 1
    return ans
```

---

## 2. 二维前缀和

### 2.1 核心思想

**问题**：给一个 n×m 的矩阵，q 次询问子矩阵 (x1,y1) 到 (x2,y2) 的元素之和。

**前缀和定义**：`pre[i][j]` = 以 (0,0) 为左上角，(i-1, j-1) 为右下角的子矩阵所有元素之和。

**递推公式（容斥原理）**：
```
pre[i][j] = pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1] + a[i-1][j-1]
```

**查询子矩阵 (x1,y1) ∼ (x2,y2)（0-indexed）**：
```
sum = pre[x2+1][y2+1] - pre[x1][y2+1] - pre[x2+1][y1] + pre[x1][y1]
```

### 2.2 图解

```
pre[i-1][j-1] ────────── pre[i-1][j]
     │                       │
     │    重叠区域（加两次）   │
     │                       │
pre[i][j-1] ──────────── pre[i][j]

pre[i][j] = 上面一整块 + 左边一整块 - 左上角（被加了两次） + a[i-1][j-1]
```

### 2.3 代码实现

#### C++

```cpp
int n, m, q;
cin >> n >> m >> q;
vector<vector<int>> a(n, vector<int>(m));
for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
        cin >> a[i][j];

// 构建二维前缀和
vector<vector<ll>> pre(n + 1, vector<ll>(m + 1, 0));
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
        pre[i][j] = pre[i-1][j] + pre[i][j-1]
                  - pre[i-1][j-1] + a[i-1][j-1];
    }

// 查询 (x1,y1) 到 (x2,y2) 的和（0-indexed）
while (q--) {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    ll sum = pre[x2+1][y2+1] - pre[x1][y2+1]
           - pre[x2+1][y1] + pre[x1][y1];
    cout << sum << '\n';
}
```

#### Python

```python
n, m, q = map(int, input().split())
a = [list(map(int, input().split())) for _ in range(n)]

pre = [[0] * (m + 1) for _ in range(n + 1)]
for i in range(1, n + 1):
    for j in range(1, m + 1):
        pre[i][j] = (pre[i-1][j] + pre[i][j-1]
                     - pre[i-1][j-1] + a[i-1][j-1])

for _ in range(q):
    x1, y1, x2, y2 = map(int, input().split())
    ans = (pre[x2+1][y2+1] - pre[x1][y2+1]
           - pre[x2+1][y1] + pre[x1][y1])
    print(ans)
```

---

## 3. 差分数组

### 3.1 核心思想

**问题**：一个数组，进行 q 次操作，每次将区间 [l, r] 的每个数都加上 v，最后输出最终数组。

**暴力做法**：每次操作遍历 [l, r] → O(nq)，超时。

**差分法**：对"区间修改"做标记，最后一次性还原。

**核心**：构造差分数组 `d`，使得 `a` 是 `d` 的前缀和。

- `d[l] += v`
- `d[r+1] -= v`
- 最后对 `d` 做一次前缀和即得到最终结果

**直觉理解**：`d[l] += v` 意味着从位置 l 开始，后面的数都会被 +v（因为做前缀和时会累加这个 v）。`d[r+1] -= v` 在 r+1 处"消除"这个影响。

### 3.2 代码实现

#### 一维差分

```cpp
// 初始数组 a[0..n-1]
// q 次操作，每次 [l, r] += val

vector<int> d(n + 1, 0);  // 差分数组，多开一位

// 处理操作
while (q--) {
    int l, r, val;
    cin >> l >> r >> val;
    d[l] += val;
    d[r + 1] -= val;
}

// 还原：对 d 做前缀和
for (int i = 1; i < n; ++i) {
    d[i] += d[i - 1];
}

// 现在 d[i] 就是 a[i] 的变化量
// 最终答案：a[i] + d[i]
```

#### 二维差分

对子矩阵 (x1,y1)∼(x2,y2) 进行增减：

```cpp
// 二维差分，四个角操作：
d[x1][y1]       += val;   // 左上角：从这里开始生效
d[x2+1][y1]     -= val;   // 下方边界外：消除
d[x1][y2+1]     -= val;   // 右侧边界外：消除
d[x2+1][y2+1]   += val;   // 右下角：被消除了两次，加回一次

// 还原：对 d 做二维前缀和（公式同一维前缀和的递推）
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
        d[i][j] += d[i-1][j] + d[i][j-1] - d[i-1][j-1];
```

---

## 4. 二分查找

### 4.1 标准二分（在有序数组中找目标值）

STL 的 `lower_bound` 和 `upper_bound` 在 Day1 已经讲过了。这里补充**手写二分**的模板——当 STL 不够灵活时（比如在自定义函数上二分）需要用。

#### 手写二分的通用模板（C++）

```cpp
// 在有序数组 a 中找第一个 >= target 的位置（等价于 lower_bound）
int binary_search(vector<int> &a, int target) {
    int l = 0, r = a.size();  // r 是开区间
    while (l < r) {
        int mid = l + (r - l) / 2;  // 防溢出写法
        if (a[mid] >= target) {
            r = mid;      // mid 满足条件，可能是答案，往左找
        } else {
            l = mid + 1;  // mid 不满足，往右找
        }
    }
    // 循环结束时 l == r，l == a.size() 表示没找到
    return l;
}
```

**手写二分的三个关键点**：
1. **区间定义**：`[l, r)` 左闭右开，保持一致
2. **mid 计算**：`l + (r - l) / 2` 防止 int 溢出（虽然 `(l+r)>>1` 也行，但前者更安全）
3. **更新方向**：`r = mid` 或 `l = mid + 1` —— 确保区间缩小，不会死循环

### 4.2 常见变体

```cpp
// 找第一个 > target 的位置（等价于 upper_bound）
if (a[mid] > target) r = mid;
else l = mid + 1;

// 找最后一个 <= target 的位置
// 等价：找第一个 > target 的位置 - 1
int pos = upper_bound(a.begin(), a.end(), target) - a.begin() - 1;

// 找 target 第一次出现的位置
int pos = lower_bound(a.begin(), a.end(), target) - a.begin();

// 找 target 最后一次出现的位置
int pos = upper_bound(a.begin(), a.end(), target) - a.begin() - 1;
```

---

## 5. 二分答案（最重要的二分技巧）

### 5.1 核心思想

**问题特征**：答案具有**单调性**。即：如果 X 可行，那么所有 >= X（或 <= X）的值也都可行/不可行。

**做法**：对答案的值域做二分，而不是对数组做二分。

**模板**：
```
while (l < r) {
    mid = (l + r) / 2
    if (check(mid))  // mid 可行
        r = mid      // 尝试更小的（最小化）
        // 或 l = mid  // 尝试更大的（最大化）
    else
        l = mid + 1  // 或 r = mid
}
```

### 5.2 二分答案的识别信号

- "最大化最小值" 或 "最小化最大值"
- "在...条件下，最大/最小是多少"
- 资源分配问题、任务调度问题

### 5.3 经典问题：木材加工

> 有 n 根木头，长度分别为 a[i]。需要切出 k 根长度相同的木段，问每段最长可以是多少？

```cpp
// 答案单调性：如果每段长度 x 可行（能切出 ≥k 段），那么所有 < x 的长度也可行
// 我们要找最大的可行 x

int n, k;
vector<int> a;

// 检查：每段长度 x 时能否切出 ≥k 段
bool check(int x) {
    if (x == 0) return true;  // 边界
    ll cnt = 0;
    for (int len : a) {
        cnt += len / x;  // 这根木头能切几段
        if (cnt >= k) return true;  // 够了就提前返回
    }
    return cnt >= k;
}

int solve() {
    int l = 0, r = *max_element(a.begin(), a.end()) + 1;
    // 区间 [l, r)：l 一定可行（长度为 0），r 一定不可行
    while (l + 1 < r) {
        int mid = l + (r - l) / 2;
        if (check(mid)) {
            l = mid;      // mid 可行，尝试更大
        } else {
            r = mid;      // mid 不可行，缩小上限
        }
    }
    return l;
}
```

### 5.4 经典问题：最小化最大距离（"路标放置"型）

> n 个路标在数轴上排好，要在它们之间插入 k 个新路标，使得相邻路标的最大距离最小。

```cpp
// 单调性：如果最大间距 x 可行（能放 ≤k 个路标就使每段 ≤x），
//         那么所有 > x 的值也可行
// 我们要最小化 x

bool check(int x) {
    int need = 0;
    for (int i = 1; i < n; ++i) {
        int gap = pos[i] - pos[i - 1];
        if (gap > x) {
            // 需要在 gap 中插入多少个路标使每段 ≤ x
            // 一段长 gap，→ 分成 ceil(gap / x) 段 → 需要 ceil(gap / x) - 1 个插入
            need += (gap + x - 1) / x - 1;
        }
    }
    return need <= k;
}

int solve() {
    int l = 1, r = pos.back() - pos.front() + 1;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (check(mid)) {
            r = mid;      // mid 可行，尝试更小
        } else {
            l = mid + 1;  // mid 不可行，需要更大
        }
    }
    return l;
}
```

### 5.5 二分答案的两个模板

```cpp
// ====== 模板 A：最小化最大值（找最小的可行值） ======
int l = 最小值, r = 最大值 + 1;
while (l < r) {
    int mid = l + (r - l) / 2;
    if (check(mid))
        r = mid;
    else
        l = mid + 1;
}
// 答案 = l（= r）

// ====== 模板 B：最大化最小值（找最大的可行值） ======
int l = 最小值 - 1, r = 最大值;
while (l < r) {
    int mid = l + (r - l + 1) / 2;  // 注意这里要 +1，否则死循环！
    if (check(mid))
        l = mid;
    else
        r = mid - 1;
}
// 答案 = l（= r）
```

**怎么判断用哪个模板？**

- `check(mid) = true` 后你想往哪边走，就往哪边收。
- 如果 `check(mid) = true` → 答案可能是 mid → 用 `r = mid`（模板 A）或 `l = mid`（模板 B）
- 如果 `check(mid) = false` → 答案不可能是 mid → 用 `l = mid + 1`（模板 A）或 `r = mid - 1`（模板 B）
- 选 `l = mid` 时，int mid 必须用 `(l + r + 1) / 2` 向上取整（否则当 l, r 相邻时会死循环）

### 5.6 Python 版本

```python
# 模板 A：最小化最大值
def binary_search_minimize(check, lo, hi):
    """找最小的可行值"""
    l, r = lo, hi + 1
    while l < r:
        mid = (l + r) // 2
        if check(mid):
            r = mid
        else:
            l = mid + 1
    return l

# 模板 B：最大化最小值
def binary_search_maximize(check, lo, hi):
    """找最大的可行值"""
    l, r = lo - 1, hi
    while l < r:
        mid = (l + r + 1) // 2  # 向上取整
        if check(mid):
            l = mid
        else:
            r = mid - 1
    return l
```

---

## 6. 浮点二分

当答案是浮点数时：

```cpp
// 解方程 / 几何问题中的二分
double l = 0.0, r = 1e9;
for (int iter = 0; iter < 100; ++iter) {  // 固定 100 次精度就够了
    double mid = (l + r) / 2.0;
    if (check(mid))
        r = mid;
    else
        l = mid;
}
// 答案 = l 或 r
// 100 次迭代后精度约为 (r-l)/2^100，远超 double 精度
```

**为什么固定 100 次而不是 `while(r-l > EPS)`？** 避免精度问题导致的死循环。

---

## 7. 前缀和 + 二分的组合使用

笔试中常出现**"需要多次查询某区间是否满足条件"**的题目：

1. 前缀和维护信息
2. 对前缀和数组二分查找第一个满足条件的位置

**经典例子**：数组中找最短的连续子数组，其和 ≥ S。

```cpp
int minSubArrayLen(int s, vector<int>& nums) {
    int n = nums.size();
    vector<int> pre(n + 1, 0);
    for (int i = 0; i < n; ++i)
        pre[i + 1] = pre[i] + nums[i];

    int ans = n + 1;
    for (int i = 0; i < n; ++i) {
        // 需要 pre[j] - pre[i] >= s，即 pre[j] >= pre[i] + s
        int j = lower_bound(pre.begin(), pre.end(), pre[i] + s) - pre.begin();
        if (j <= n)
            ans = min(ans, j - i);
    }
    return ans <= n ? ans : 0;
}
```

---

## Day 2 配套练习

| 题号 | 题目 | 考察点 |
|------|------|--------|
| 1 | 二维前缀和模板题：子矩阵求和 | 二维前缀和 |
| 2 | 木材加工（切木段） | 二分答案 |
| 3 | 路标放置（最小化最大距离） | 二分答案 |
| 4 | 和为 K 的子数组 | 前缀和 + 哈希 |
| 5 | 差分模板题：区间加法后输出最终数组 | 一维差分 |

---

## 快速参考卡片

```
┌──────────────────────────────────────────────────────────┐
│  前缀和                                                     │
│  pre[i+1] = pre[i] + a[i]         // 构建                  │
│  sum[l..r] = pre[r+1] - pre[l]    // 查询 (0-indexed)      │
│                                                             │
│  二维前缀和                                                 │
│  pre[i][j] = pre[i-1][j] + pre[i][j-1]                    │
│            - pre[i-1][j-1] + a[i-1][j-1]   // 构建        │
│                                                             │
│  差分                                                       │
│  [l, r] += v  →  d[l] += v; d[r+1] -= v                   │
│  还原：对 d 做前缀和                                        │
│                                                             │
│  二分答案                                                   │
│  while (l < r) {                                           │
│      mid = l + (r - l) / 2;                                │
│      if (check(mid)) r = mid;  // 可行，尝试更小           │
│      else l = mid + 1;                                     │
│  }                                                          │
└──────────────────────────────────────────────────────────┘
```
