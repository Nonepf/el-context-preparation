# Day 3：贪心、优先队列、双指针与滑动窗口

> **学习目标**：掌握贪心思想的几种经典模型，能用优先队列实现反悔贪心，熟练写出双指针和滑动窗口代码。
>
> **预计时间**：3 小时阅读 + 1 小时练习

---

## 1. 贪心算法

### 1.1 什么是贪心

**核心**：每一步都做"当前看起来最好"的选择，不回头。

**贪心 vs DP**：
- 贪心：局部最优 → 全局最优（需要证明，比赛中更多靠经验和直觉）
- DP：需要考虑所有可能的状态转移，保证全局最优

**识别信号**：
- "最多"、"最少"、"最大化"、"最小化"
- 选择之间"不重叠"、"不冲突"
- 可以按某个标准排序后依次处理

### 1.2 经典模型一：活动选择 / 区间调度

> 给定 n 个活动，每个有开始时间 s[i] 和结束时间 e[i]。同一时间只能参加一个活动（不重叠）。问最多能参加几个？

**贪心策略**：按**结束时间**升序排序，每次选择最早结束的、且与已选活动不冲突的活动。

**为什么对**：越早结束的活动，留给后面的时间越多。选最早结束的永远不会比选更晚结束的更差——这是可证明的最优策略。

```cpp
struct Activity {
    int start, end;
};

int maxActivities(vector<Activity>& acts) {
    // 按结束时间升序
    sort(acts.begin(), acts.end(),
         [](const Activity &a, const Activity &b) {
             return a.end < b.end;
         });

    int count = 0;
    int last_end = -INF;  // 上一个选中活动的结束时间

    for (auto &act : acts) {
        if (act.start >= last_end) {  // 不冲突
            count++;
            last_end = act.end;
        }
    }
    return count;
}
```

**变体 1：需要安排所有活动，求最少需要几个教室？**
→ 按开始时间排序，用优先队列维护各教室的结束时间。

**变体 2：每个活动有收益，选不重叠的活动使总收益最大？**
→ 这是 DP 问题（按结束时间排序 + `dp[i] = max(dp[i-1], dp[prev(i)] + value[i])`），贪心不行。

### 1.3 经典模型二：任务调度 / 最小化完成时间

> 有 n 个任务，每个任务需要处理时间 t[i]。同一时间只能处理一个任务。问如何排序使所有任务的平均等待时间最小？

**贪心策略**：按处理时间**升序**排序（短任务优先）。

这正是著名的 **SJF（最短作业优先）** 调度算法。

### 1.4 经典模型三：区间覆盖

> 数轴上有 n 个区间 [l, r]。选择最少的区间，使它们覆盖整个 [0, L]。

**贪心策略**：
1. 按**左端点**升序排序
2. 维护"当前已覆盖到的最远位置"max_right
3. 每次在左端点 ≤ max_right 的区间中，选择右端点最大的那个
4. 更新 max_right，计数 +1

```cpp
int minIntervals(vector<pair<int, int>>& intervals, int L) {
    sort(intervals.begin(), intervals.end());

    int count = 0;
    int i = 0, n = intervals.size();
    int max_right = 0;  // 当前覆盖到的最远位置

    while (max_right < L) {
        int best = max_right;
        // 在所有左端点 ≤ max_right 的区间中找右端点最大的
        while (i < n && intervals[i].first <= max_right) {
            best = max(best, intervals[i].second);
            i++;
        }
        if (best == max_right) {
            // 无法继续拓展，说明无法覆盖
            return -1;
        }
        max_right = best;
        count++;
    }
    return count;
}
```

### 1.5 经典模型四：字典序最小

> 从长度为 n 的字符串中删掉 k 个字符，使得剩下的字符串字典序最小。

**贪心策略**：维护一个单调递增栈。遍历每个字符，当栈顶字符 > 当前字符且还能删除时，弹出栈顶。

```cpp
string removeKdigits(string num, int k) {
    string st;  // 用 string 模拟栈
    for (char c : num) {
        while (!st.empty() && st.back() > c && k > 0) {
            st.pop_back();
            k--;
        }
        st.push_back(c);
    }
    // 如果还需要删，从末尾删
    while (k--) st.pop_back();
    // 去掉前导零
    int start = st.find_first_not_of('0');
    return start == string::npos ? "0" : st.substr(start);
}
```

#### Python 版本

```python
def remove_k_digits(num: str, k: int) -> str:
    st = []
    for c in num:
        while st and st[-1] > c and k > 0:
            st.pop()
            k -= 1
        st.append(c)
    while k:
        st.pop()
        k -= 1
    # 去掉前导零
    res = ''.join(st).lstrip('0')
    return res if res else '0'
```

本题也展示了**单调栈**的思想（Day 6 会详细展开）。

---

## 2. 优先队列 + 贪心（反悔贪心）

这是中等偏难题的常客。当普通的贪心顺序不够时，用优先队列动态维护"最优选择"，并支持"反悔"。

### 2.1 经典问题：有限时间内的最大收益

> 有 n 个任务，第 i 个任务需要 1 单位时间，截止时间 d[i]，完成后收益为 p[i]。每个时间单位只能做一个任务。问最大总收益。

**贪心策略**（时间线扫描）：
1. 按截止时间排序
2. 用优先队列维护**已选中的任务集合**（小顶堆，按收益排序）
3. 遍历每个任务：
   - 如果当前时间 ≤ 截止时间：直接做，加入堆
   - 如果当前时间 > 截止时间：比较当前收益与堆中最小收益。若更大，**反悔**——放弃堆中收益最小的任务，换当前任务

```cpp
struct Task {
    int deadline, profit;
};

int maxProfit(vector<Task>& tasks) {
    // 按截止时间升序
    sort(tasks.begin(), tasks.end(),
         [](const Task &a, const Task &b) {
             return a.deadline < b.deadline;
         });

    // 小顶堆：存储已选中任务的收益
    priority_queue<int, vector<int>, greater<int>> pq;
    int time = 0;

    for (auto &task : tasks) {
        if (time < task.deadline) {
            // 有时间，直接做
            pq.push(task.profit);
            time++;
        } else if (!pq.empty() && pq.top() < task.profit) {
            // 反悔：放弃收益最小的任务
            pq.pop();
            pq.push(task.profit);
        }
    }

    int ans = 0;
    while (!pq.empty()) {
        ans += pq.top();
        pq.pop();
    }
    return ans;
}
```

#### Python 版本

```python
import heapq

def max_profit(tasks):
    tasks.sort(key=lambda t: t[0])  # 按截止时间排序
    pq = []  # 小顶堆
    time = 0
    for deadline, profit in tasks:
        if time < deadline:
            heapq.heappush(pq, profit)
            time += 1
        elif pq and pq[0] < profit:
            heapq.heapreplace(pq, profit)  # 弹出最小，插入新值（比 heappop+heappush 快）
    return sum(pq)
```

### 2.2 反悔贪心的通用框架

1. 按某个维度排序（时间、位置等）
2. 用堆维护"当前已选择集合"
3. 遇到新元素时，检查是否替换堆中"最差"元素更优
4. 如果是，弹出最差元素，加入新元素

### 2.3 另一个经典模型：合并果子（哈夫曼编码）

> 有 n 堆果子，每次合并任意两堆，代价为两堆果子数之和。问将所有堆合并成一堆的最小总代价。

```cpp
// 贪心：每次合并最小的两堆
// 用优先队列（小顶堆）维护当前各堆大小

int minCost(vector<int>& piles) {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int x : piles) pq.push(x);

    int total = 0;
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        int cost = a + b;
        total += cost;
        pq.push(cost);  // 合并后的新堆
    }
    return total;
}
```

---

## 3. 双指针

### 3.1 核心思想

用两个指针在数组上移动，将 O(n²) 的暴力枚举降到 O(n)。

**关键**：两个指针的移动方向有**单调性**——某个条件满足/不满足时，指针只往一个方向移动，不回退。

### 3.2 经典模型：有序数组的两数之和

> 给定升序数组 a，找两个不同位置的数之和等于 target。

```cpp
// 首尾双指针
int l = 0, r = n - 1;
while (l < r) {
    int sum = a[l] + a[r];
    if (sum == target) {
        // 找到了！a[l] + a[r] == target
        break;
    } else if (sum < target) {
        l++;  // 和太小，左指针右移增大
    } else {
        r--;  // 和太大，右指针左移减小
    }
}
```

**单调性在哪？** 当 `l` 右移时 `sum` 只增不减（因为数组有序），当 `r` 左移时 `sum` 只减不增。所以不会错过任何答案。

### 3.3 经典模型：三数之和

> 升序数组 a，找所有不重复的三元组使和为 0。

```cpp
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> ans;
    sort(nums.begin(), nums.end());
    int n = nums.size();

    for (int i = 0; i < n; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;  // 跳过重复

        int l = i + 1, r = n - 1;
        int target = -nums[i];

        while (l < r) {
            int sum = nums[l] + nums[r];
            if (sum == target) {
                ans.push_back({nums[i], nums[l], nums[r]});
                // 跳过重复
                while (l < r && nums[l] == nums[l + 1]) l++;
                while (l < r && nums[r] == nums[r - 1]) r--;
                l++; r--;
            } else if (sum < target) {
                l++;
            } else {
                r--;
            }
        }
    }
    return ans;
}
```

**模式**：固定一个数，剩下两个用双指针 → O(n²)

### 3.4 快慢指针（Floyd 判圈）

> 链表 / 数组中找环。

```cpp
// 检测数组中的重复数字（数组值在 [1, n] 范围内，有 n+1 个元素）
int findDuplicate(vector<int>& nums) {
    int slow = nums[0], fast = nums[0];
    do {
        slow = nums[slow];
        fast = nums[nums[fast]];
    } while (slow != fast);

    // 找环的入口
    slow = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}
```

---

## 4. 滑动窗口

### 4.1 核心思想

维护一个**连续子数组/子串**，当窗口向右移动时，左边可能需要收缩，右边拓展出去。

**固定套路**：
```
left = 0
for right in range(n):
    窗口加入 a[right]
    while 窗口不满足条件:
        窗口移出 a[left]
        left++
    更新答案（此时窗口满足条件）
```

### 4.2 经典模型：最长无重复字符子串

> 给定字符串 s，找出其中不含有重复字符的最长子串的长度。

```cpp
int lengthOfLongestSubstring(string s) {
    unordered_map<char, int> window;  // 字符 → 在当前窗口中的出现次数
    int l = 0, ans = 0;

    for (int r = 0; r < s.size(); ++r) {
        window[s[r]]++;  // 右边界扩展

        // 如果有重复，收缩左边界直到重复消除
        while (window[s[r]] > 1) {
            window[s[l]]--;
            l++;
        }

        // 此时窗口内无重复字符
        ans = max(ans, r - l + 1);
    }
    return ans;
}
```

#### Python

```python
def length_of_longest_substring(s: str) -> int:
    window = {}
    l = ans = 0
    for r, ch in enumerate(s):
        window[ch] = window.get(ch, 0) + 1
        while window[ch] > 1:
            window[s[l]] -= 1
            l += 1
        ans = max(ans, r - l + 1)
    return ans
```

### 4.3 经典模型：最小覆盖子串

> 字符串 s 和 t，找 s 中包含 t 所有字符的最短子串。

```cpp
string minWindow(string s, string t) {
    unordered_map<char, int> need;  // t 中每个字符需要的个数
    for (char c : t) need[c]++;

    unordered_map<char, int> window;  // 当前窗口中字符的计数
    int l = 0, match = 0;   // match = 已满足的字符种类数
    int need_match = need.size();  // 总共需要满足的字符种类数

    int min_len = s.size() + 1, start = 0;

    for (int r = 0; r < s.size(); ++r) {
        char c = s[r];
        window[c]++;

        // 如果这个字符的数量刚好满足需求，match++
        if (need.count(c) && window[c] == need[c]) {
            match++;
        }

        // 当所有字符都满足，尝试收缩左边界
        while (match == need_match) {
            // 更新答案
            if (r - l + 1 < min_len) {
                min_len = r - l + 1;
                start = l;
            }

            // 移除左边界字符
            char left_char = s[l];
            window[left_char]--;
            if (need.count(left_char) && window[left_char] < need[left_char]) {
                match--;
            }
            l++;
        }
    }

    return min_len > s.size() ? "" : s.substr(start, min_len);
}
```

### 4.4 滑动窗口的扩展形式

```cpp
// ===== 定长窗口：窗口长度为 k =====
for (int r = 0; r < n; ++r) {
    // 加入 a[r]
    if (r >= k - 1) {    // 窗口形成了
        // 处理/更新答案
        // 移出 a[r - k + 1]
    }
}

// ===== 不定长窗口，满足"恰好"条件 =====
// 通常转化为两个"不超过"的差
// count(恰好 K) = count(≤ K) - count(≤ K-1)
// 例如：含 K 个不同整数的子数组数目
```

---

## 5. 双指针与滑动窗口的区别

很多新手会混淆两者。区别在于：

| | 双指针 | 滑动窗口 |
|------|--------|------------|
| 典型场景 | 两端向中间走（有序数组） | 维护一个区间，同向移动 |
| 窗口 | 不一定连续 | 必须是**连续子数组/子串** |
| 指针移动 | 两个指针在数组两端，相向而行 | left 和 right 都从左到右，同向 |
| 常见题目 | 两数之和、三数之和、接雨水 | 最长无重复子串、最小覆盖子串、最大连续和 |

---

## Day 3 配套练习

| 题号 | 题目 | 考察点 | 推荐时间 |
|------|------|--------|---------|
| 1 | 活动安排：N 个区间选最多不重叠 | 贪心基础 | 15 min |
| 2 | 区间覆盖：最少区间数覆盖 [0, L] | 贪心 | 20 min |
| 3 | 有限时间内最大收益（反悔贪心） | 优先队列 + 贪心 | 25 min |
| 4 | 三数之和 | 双指针 | 20 min |
| 5 | 最长无重复字符子串 | 滑动窗口 | 15 min |
| 6 | 最小覆盖子串 | 滑动窗口（进阶） | 25 min |

---

## 快速参考卡片

```
┌──────────────────────────────────────────────────────────┐
│  贪心常见模型                                               │
│  1. 活动选择 → 按结束时间排序，选不冲突的                    │
│  2. 区间覆盖 → 按左端点排序，每次选右端点最远的              │
│  3. 反悔贪心 → 排序 + 优先队列，遇到更好的替换堆中最差的      │
│                                                             │
│  双指针                                                     │
│  有序数组两数之和: l=0, r=n-1, sum<target→l++, else r--    │
│                                                             │
│  滑动窗口模板                                               │
│  l = 0                                                     │
│  for r in range(n):                                        │
│      窗口加入 a[r]                                          │
│      while 窗口不满足条件:                                   │
│          移出 a[l]; l++                                     │
│      更新答案                                               │
│                                                             │
│  小顶堆: priority_queue<int, vector<int>, greater<int>>    │
│  大顶堆: priority_queue<int>  (默认)                       │
└──────────────────────────────────────────────────────────┘
```
