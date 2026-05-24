# Day 6：并查集、字符串哈希、单调栈与比赛策略

> **学习目标**：补齐最后几个高频技巧，同时制定可操作的比赛策略。
>

---

## 1. 并查集（Disjoint Set Union, DSU）

### 1.1 核心思想

并查集解决**动态连通性**问题：初始 n 个集合各含一个元素，支持两种操作：
- **合并（Union）**：把两个集合合并
- **查询（Find）**：判断两个元素是否在同一集合

**典型场景**：
- 朋友圈 / 社交网络 → 两人是否在同一朋友圈
- 岛屿数量 → 陆地合并后统计集合数
- 网络连通性 → 两台机器是否互通
- Kruskal 最小生成树 → 判断加边是否会形成环

### 1.2 基础实现 + 路径压缩

```cpp
struct DSU {
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i)
            parent[i] = i;  // 初始每个节点的父节点是自己
    }

    // Find：找根节点，同时路径压缩
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // 递归，路径压缩
        }
        return parent[x];
    }

    // Union：合并两个集合
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx != ry) {
            parent[rx] = ry;  // 简单合并（把 rx 的根指向 ry）
        }
    }

    // 判断是否连通
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
```

### 1.3 进阶优化：按秩合并

加入 size/rank 数组，总是把小的树接到大的树下，进一步优化。

```cpp
struct DSU {
    vector<int> parent, sz;  // sz 记录集合大小

    DSU(int n) {
        parent.resize(n);
        sz.resize(n, 1);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;

        // 小集合接到大集合下面
        if (sz[rx] < sz[ry]) swap(rx, ry);
        parent[ry] = rx;
        sz[rx] += sz[ry];  // 维护集合大小
    }

    int size(int x) {
        return sz[find(x)];
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
```

### 1.4 带权并查集（维护额外信息）

有时需要维护节点到父节点的关系（如 "x 和 y 的关系类型"）。

```cpp
// 例题：食物链（POJ 1182）
// 维护 d[x] 表示 x 到 parent[x] 的关系：0=同类, 1=吃, 2=被吃
struct DSU {
    vector<int> parent, d;

    DSU(int n) {
        parent.resize(n);
        d.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) {
            int root = find(parent[x]);
            d[x] = (d[x] + d[parent[x]]) % 3;  // 更新到根的关系
            parent[x] = root;
        }
        return parent[x];
    }

    // relation: 0=同类, 1=x吃y
    bool unite(int x, int y, int relation) {
        int rx = find(x), ry = find(y);
        if (rx == ry) {
            // 已经连通，验证关系
            return (d[x] - d[y] + 3) % 3 == relation;
        }
        parent[rx] = ry;
        d[rx] = (relation + d[y] - d[x] + 3) % 3;
        return true;
    }
};
```

### 1.5 并查集的代码极简版（比赛用）

```cpp
// 最简版，比赛直接粘贴
vector<int> p;
int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
void unite(int x, int y) { p[find(x)] = find(y); }
// 初始化：p.resize(n); iota(p.begin(), p.end(), 0);
```

#### Python

```python
class DSU:
    def __init__(self, n):
        self.p = list(range(n))
        self.sz = [1] * n

    def find(self, x):
        if self.p[x] != x:
            self.p[x] = self.find(self.p[x])
        return self.p[x]

    def unite(self, x, y):
        rx, ry = self.find(x), self.find(y)
        if rx == ry:
            return False
        if self.sz[rx] < self.sz[ry]:
            rx, ry = ry, rx
        self.p[ry] = rx
        self.sz[rx] += self.sz[ry]
        return True

    def connected(self, x, y):
        return self.find(x) == self.find(y)
```

---

## 2. 字符串哈希

### 2.1 核心思想

把一个字符串映射成一个整数，使得：
- 相同的字符串 → 相同的哈希值
- 不同的字符串 → 大概率不同的哈希值（有一定碰撞概率）

**用途**：快速比较两个子串是否相等（O(1) 查询，预处理 O(n)）。
在比赛中，字符串哈希通常比 KMP 更灵活，能处理回文、最长公共前缀等问题。

### 2.2 单哈希实现

```cpp
using ull = unsigned long long;
const ull BASE = 131;  // 或 13331，经验值

vector<ull> ha, pw;  // ha: 前缀哈希, pw: BASE 的幂

void init_hash(const string &s) {
    int n = s.size();
    ha.resize(n + 1);
    pw.resize(n + 1);
    pw[0] = 1;
    for (int i = 0; i < n; ++i) {
        ha[i + 1] = ha[i] * BASE + (ull)s[i];
        pw[i + 1] = pw[i] * BASE;
    }
}

// 获取子串 s[l..r] 的哈希值（0-indexed，闭区间）
ull get_hash(int l, int r) {
    return ha[r + 1] - ha[l] * pw[r - l + 1];
}

// 比较两个子串 s[l1..r1] 和 s[l2..r2] 是否相等
bool is_equal(int l1, int r1, int l2, int r2) {
    return get_hash(l1, r1) == get_hash(l2, r2);
}
```

**哈希公式理解**：
```
s = "abc"
ha[0] = 0
ha[1] = 'a'
ha[2] = 'a' * BASE + 'b'
ha[3] = ('a' * BASE + 'b') * BASE + 'c'
      = 'a' * BASE² + 'b' * BASE + 'c'

子串 "bc" (l=1, r=2)：
ha[3] - ha[1] * BASE²
= ('a' * BASE² + 'b' * BASE + 'c') - ('a' * BASE²)
= 'b' * BASE + 'c'   ← 这正是 "bc" 的编码
```

### 2.3 双哈希（防碰撞）

比赛中偶尔会有人故意卡单哈希（出数据使哈希碰撞）。双哈希几乎不可能被卡。

```cpp
using ull = unsigned long long;
const ull BASE1 = 131, BASE2 = 13331;
const ull MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;  // 或用 ULL 自然溢出

vector<ull> ha1, ha2, pw1, pw2;

void init_hash(const string &s) {
    int n = s.size();
    ha1.resize(n + 1); ha2.resize(n + 1);
    pw1.resize(n + 1); pw2.resize(n + 1);
    pw1[0] = pw2[0] = 1;
    for (int i = 0; i < n; ++i) {
        ha1[i + 1] = (ha1[i] * BASE1 + s[i]) % MOD1;
        ha2[i + 1] = (ha2[i] * BASE2 + s[i]) % MOD2;
        pw1[i + 1] = pw1[i] * BASE1 % MOD1;
        pw2[i + 1] = pw2[i] * BASE2 % MOD2;
    }
}

// 返回 (hash1, hash2) 对
pair<ull, ull> get_hash(int l, int r) {
    ull h1 = (ha1[r + 1] - ha1[l] * pw1[r - l + 1] % MOD1 + MOD1) % MOD1;
    ull h2 = (ha2[r + 1] - ha2[l] * pw2[r - l + 1] % MOD2 + MOD2) % MOD2;
    return {h1, h2};
}
```

### 2.4 Python 字符串哈希

```python
BASE = 131
MOD = 10 ** 9 + 7

def init_hash(s: str):
    n = len(s)
    ha = [0] * (n + 1)
    pw = [1] * (n + 1)
    for i, ch in enumerate(s):
        ha[i + 1] = (ha[i] * BASE + ord(ch)) % MOD
        pw[i + 1] = pw[i] * BASE % MOD
    return ha, pw

def get_hash(ha, pw, l, r):
    return (ha[r + 1] - ha[l] * pw[r - l + 1]) % MOD
```

### 2.5 字符串哈希常见应用

| 应用 | 做法 |
|------|------|
| 判断子串相等 | `get_hash(l1, r1) == get_hash(l2, r2)` |
| 最长公共前缀（LCP） | 二分 + 哈希 |
| 最长回文子串 | 正序哈希 + 倒序哈希，枚举中心，二分半径 |
| 字符串匹配 | 预处理模式串哈希，扫描文本串 |
| 去重子串数目 | 枚举长度 + 枚举起点 + 哈希去重 |

**经典例子：最长回文子串（O(n log n)）**

```cpp
// 思路：预处理正序和倒序哈希，然后枚举每个位置作为中心，二分最大半径
string s;
vector<ull> ha_fwd, pw_fwd;
vector<ull> ha_rev, pw_rev;

void init() {
    // 正序哈希
    init_hash_fwd(s);
    // 倒序哈希（对 reversed string 做）
    string rev = s;
    reverse(rev.begin(), rev.end());
    init_hash_rev(rev);
}

// 检查 s[l..r] 是否回文
bool is_pal(int l, int r) {
    int n = s.size();
    return get_hash_fwd(l, r) == get_hash_rev(n - 1 - r, n - 1 - l);
}

// 二分最大回文半径
int longest_pal() {
    int ans = 1;
    for (int i = 0; i < s.size(); ++i) {
        // 奇数长度回文
        int l = 1, r = min(i + 1, (int)s.size() - i);
        while (l < r) {
            int mid = (l + r + 1) / 2;
            if (is_pal(i - mid + 1, i + mid - 1)) l = mid;
            else r = mid - 1;
        }
        ans = max(ans, 2 * l - 1);

        // 偶数长度回文
        // ... 类似处理 ...
    }
    return ans;
}
```

---

## 3. 单调栈

### 3.1 核心思想

单调栈维护一个**单调递增（或递减）的序列**。当新元素破坏单调性时，弹出栈顶。

**识别信号**：找"下一个/上一个更大的/更小的元素"、"柱状图最大矩形"

### 3.2 经典模型：下一个更大元素

> 给定数组 a，对于每个位置 i，找到它右边第一个比 a[i] 大的元素。

```cpp
vector<int> nextGreaterElement(vector<int>& nums) {
    int n = nums.size();
    vector<int> ans(n, -1);  // -1 表示右边没有更大的
    stack<int> st;  // 存储下标，栈内元素对应值单调递减

    for (int i = 0; i < n; ++i) {
        // 当前元素比栈顶元素大 → 对栈顶来说这就是"下一个更大"
        while (!st.empty() && nums[i] > nums[st.top()]) {
            int idx = st.top();
            st.pop();
            ans[idx] = nums[i];
        }
        st.push(i);
    }
    return ans;
}
```

**直觉**：当新来一个"大"元素时，之前所有比它小的元素都被它"撑破"了——它们的"下一个更大"就是这个新元素。

### 3.3 经典模型：柱状图中最大矩形

> 给定 heights 数组表示每个柱子的高度，宽均为 1，求这些柱子能形成的最大矩形面积。

```cpp
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    stack<int> st;  // 存储下标，对应高度单调递增
    int ans = 0;

    // 遍历，末尾加哨兵 0
    for (int i = 0; i <= n; ++i) {
        int h = (i == n) ? 0 : heights[i];
        // 当前高度 < 栈顶高度 → 以栈顶高度为高的矩形不能再延伸了
        while (!st.empty() && h < heights[st.top()]) {
            int height = heights[st.top()];
            st.pop();
            int width = st.empty() ? i : i - st.top() - 1;
            ans = max(ans, height * width);
        }
        st.push(i);
    }
    return ans;
}
```

**直觉**：
- 栈中高度单调递增
- 当遇到更低的高度时，以栈顶高度为高的矩形"到头了"
- 它的宽度可以向左延伸到"栈中前一个位置"的右边一格

#### Python

```python
def largest_rectangle_area(heights):
    n = len(heights)
    st = []
    ans = 0
    for i in range(n + 1):
        h = heights[i] if i < n else 0
        while st and h < heights[st[-1]]:
            height = heights[st.pop()]
            width = i if not st else i - st[-1] - 1
            ans = max(ans, height * width)
        st.append(i)
    return ans
```

---

## 4. 单调队列

### 4.1 核心思想

单调队列维护**滑动窗口的最值**。比优先队列好在：可以 O(1) 删除窗口左侧离开的元素。

### 4.2 滑动窗口最大值

> 给定数组 nums 和窗口大小 k，求每个窗口的最大值。

```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    int n = nums.size();
    deque<int> dq;  // 存储下标，对应值单调递减
    vector<int> ans;

    for (int i = 0; i < n; ++i) {
        // 1. 移除窗口外（左边）的元素
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }

        // 2. 维护单调性：新元素进来，比它小的都弹出
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }

        dq.push_back(i);

        // 3. 窗口形成后，队首就是当前窗口最大值
        if (i >= k - 1) {
            ans.push_back(nums[dq.front()]);
        }
    }
    return ans;
}
```

#### Python

```python
from collections import deque

def max_sliding_window(nums, k):
    n = len(nums)
    dq = deque()
    ans = []
    for i in range(n):
        if dq and dq[0] <= i - k:
            dq.popleft()
        while dq and nums[dq[-1]] <= nums[i]:
            dq.pop()
        dq.append(i)
        if i >= k - 1:
            ans.append(nums[dq[0]])
    return ans
```

---

## 5. Trie（字典树）

### 5.1 核心思想

Trie 是用于**字符串集合的多路查找树**。每个节点代表一个前缀，从根到某节点的路径就是一个字符串。

**用途**：自动补全、前缀匹配、敏感词过滤、异或最大值。

### 5.2 基础实现

```cpp
struct TrieNode {
    TrieNode* children[26] = {};  // 假设只有小写字母
    bool is_end = false;          // 是否是某个单词的结尾
    int cnt = 0;                  // 经过此节点的单词数（用于前缀统计）
};

struct Trie {
    TrieNode* root;

    Trie() { root = new TrieNode(); }

    void insert(const string &word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
            node->cnt++;
        }
        node->is_end = true;
    }

    bool search(const string &word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        return node->is_end;
    }

    bool startsWith(const string &prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) return false;
            node = node->children[idx];
        }
        return true;
    }
};
```

---

## 6. 比赛策略

### 6.1 时间分配（2 小时 = 120 分钟）

| 阶段 | 时间 | 任务 |
|------|------|------|
| **快速浏览** | 0-10 min | 浏览所有题目，标记难度（签到/中等/难题） |
| **签到题** | 10-30 min | 先 AC 最简单的 1-2 道题，建立信心 |
| **中等题** | 30-80 min | 主攻 2-3 道中等题，争取每题 AC 或至少部分分 |
| **难题** | 80-110 min | 选一道最有把握的题冲，其余写暴力拿部分分 |
| **检查** | 110-120 min | 检查边界、文件名、输出格式 |

### 6.2 IOI 赛制下的骗分策略

IOI 赛制：每道题的每个测试点独立计分，**做对几个就给几个的分**，没有罚时。

1. **读数据范围**：如果题目有 20% 的数据是 n≤10，先写个 O(2^n) 的暴力稳拿这 20 分。
2. **特判**：有些题目 n=1 或特殊情况可以直接输出，白送的分。
3. **部分分有提示**：观察题目的"子任务"描述，往往说明了部分分的条件。
4. **只输出 0 或固定值**：实在不会做，输出一个可能的合理值，有时能撞到一两个点。

### 6.3 考试前准备清单

- [ ] 将模板文件（template.cpp / template.py）放入可访问的位置
- [ ] 确认 CPL 系统的提交操作流程（热身赛应该已经熟悉了）
- [ ] 准备好草稿纸和笔（机房提供草稿纸，自备笔）
- [ ] 每道题在纸上先画清楚思路再写代码
- [ ] 准备好常用算法的打印版或本地文件（机房是否允许本地文件？确认一下）

### 6.4 卡题了怎么办

1. **5 分钟原则**：一道题卡了 5 分钟没思路，换下一题。不要让一道题吃掉所有时间。
2. **先写暴力**：不会正解就先写暴力，跑通了小数据后可以用于验证正解。
3. **重新读题**：很多时候卡住是因为漏看了条件或数据范围。
4. **打印调试**：CPL 系统看本地运行的输出；用 `cerr`/`stderr` 打印调试信息（调试信息不计入答案）。

### 6.5 最终建议

考虑到你只有 6 天准备时间，前两周比赛的策略是：

**首场（5/30）目标**：AC 签到题 + 中等题拿部分分。不要追求每题满分。
**第二场（5/31）**：复盘第一场的错误，调整策略。
**后续场次**：逐步提高期望，争取 AC 中等题。

---

## Day 6 配套练习

| 题号 | 题目 | 考察点 |
|------|------|--------|
| 1 | 朋友圈/省份数量 | 并查集基础 |
| 2 | 滑动窗口最大值 | 单调队列 |
| 3 | 下一个更大元素 | 单调栈 |
| 4 | 柱状图中最大矩形 | 单调栈（进阶） |
| 5 | 最长回文子串 | 字符串哈希 + 二分 |
| 6 | 实现 Trie | Trie 树 |

---

## 快速参考卡片

```
┌──────────────────────────────────────────────────────────┐
│  并查集 (DSU)                                              │
│  vector<int> p(n); iota(p.begin(), p.end(), 0);           │
│  int find(x) { return p[x]==x ? x : p[x]=find(p[x]); }   │
│  void unite(x,y) { p[find(x)] = find(y); }               │
│                                                             │
│  字符串哈希                                                 │
│  ha[i+1] = ha[i] * BASE + s[i]                            │
│  get(l,r) = ha[r+1] - ha[l] * pw[r-l+1]                  │
│  双哈希防碰撞: 用两个 BASE 和 MOD                          │
│                                                             │
│  单调栈 (下一个更大)                                        │
│  for i in range(n):                                       │
│      while st and a[i] > a[st.top()]:                     │
│          ans[st.top()] = a[i]; st.pop()                   │
│      st.push(i)                                            │
│                                                             │
│  单调队列 (滑动窗口最大值)                                   │
│  deque: pop过期(左边) → 维护递减(右边) → push → 取队首    │
│                                                             │
│  比赛口诀: 浏览→签到→中等冲分→暴力骗分→检查                 │
└──────────────────────────────────────────────────────────┘
```
