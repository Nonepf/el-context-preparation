# Day 1：STL 基础、输入输出、暴力与模拟

> **学习目标**：能在 5 分钟内写出一个包含多组数据读入、vector 操作、map 计数、sort 排序、格式化输出的完整程序。
>

---

## 1. 输入输出：一切代码的起点

比赛中卡在输入输出上是最亏的。这部分必须形成肌肉记忆。

### 1.1 C++ 输入输出

#### 最常用模式（记这一个就够了）

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // === 情况1：已知有 T 组数据 ===
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        // ... 解题逻辑 ...
        cout << answer << '\n';  // 用 '\n' 而非 endl（endl 会刷新缓冲区，更慢）
    }

    // === 情况2：读到文件末尾 (EOF) ===
    int x;
    while (cin >> x) {
        // 每次读入一个 x 进行处理
    }

    // === 情况3：整行读取 ===
    string line;
    cin.ignore();  // 如果前面用过 cin >>，需要先吃掉残留的换行符
    while (getline(cin, line)) {
        // 处理每一行
    }
}
```

#### 关键细节

| 要点 | 说明 |
|------|------|
| `ios::sync_with_stdio(false)` | 关闭 C/C++ 输入输出流同步，**必须写**，否则大数据会超时 |
| `cin.tie(nullptr)` | 解除 cin 和 cout 的绑定，进一步加速 |
| `'\n'` vs `endl` | **永远用 `'\n'`**。`endl` 会刷新缓冲区，比 `'\n'` 慢 10 倍以上 |
| 混用问题 | 加速后**不要**混用 `scanf/printf` 和 `cin/cout`，会导致输出顺序错乱 |
| 格式化输出 | 需要保留小数位数等复杂格式时，用 `printf` 更方便：`printf("%.2f\n", ans);` |

#### 整行读取的经典坑

```cpp
int n;
cin >> n;              // 读入 n 后，缓冲区里还剩一个 '\n'
cin.ignore();          // 必须吃掉这个 '\n'，否则下面的 getline 会读到一个空行
string line;
getline(cin, line);    // 现在才能正确读到下一行
```

#### 一行中用空格/逗号分隔的数字如何读？

```cpp
// 输入格式：1 2 3 4 5（空格分隔）
int x;
while (cin >> x) { /* ... */ }  // cin 自动跳过空白字符

// 输入格式：1,2,3,4,5（逗号分隔）
char comma;
int a, b, c;
cin >> a >> comma >> b >> comma >> c;  // 手动吃掉逗号
```

### 1.2 Python 输入输出

#### 最常用模式

```python
import sys

# ---- 标准模式：逐行读取 ----
def input():
    return sys.stdin.readline().rstrip('\n')

# 读一行整数
def ints():
    return list(map(int, input().split()))

# 使用
T = int(input())
for _ in range(T):
    n = int(input())
    a = ints()
    # ... 解题逻辑 ...
    print(answer)

# ---- 极速模式：一次性读完（适合超大数据） ----
data = sys.stdin.buffer.read().split()
n = int(data[0])
arr = list(map(int, data[1:1 + n]))
# 然后逐个处理 data 的元素
```

#### Python 的输入性能陷阱

```python
# ❌ 慢：input() 默认会去掉换行符但有额外开销
# ❌ 慢：反复调用 int(input()) 读多行
# ✅ 快：sys.stdin.buffer.read().split() 一次读完
# ✅ 快：输出用 sys.stdout.write('\n'.join(map(str, results)))
```

对于大多数比赛题目（n ≤ 10^5），标准逐行读取就够用。如果遇到 10^6 级别的数据，再用极速模式。

---

## 2. vector / list：动态数组

这分别是你使用频率最高的数据结构，没有之一。

### 2.1 C++ vector 核心操作

```cpp
#include <vector>
using namespace std;

// ---- 创建 ----
vector<int> a;           // 空数组
vector<int> a(10);       // 10 个元素，初始化为 0
vector<int> a(10, -1);   // 10 个元素，初始化为 -1
vector<int> a = {1, 2, 3}; // 初始化列表
vector<vector<int>> mat(n, vector<int>(m, 0)); // n×m 二维数组

// ---- 添加/删除 ----
a.push_back(x);          // 末尾添加（最常用）
a.pop_back();            // 末尾删除
a.emplace_back(x);       // 同 push_back，但更高效（原地构造）
a.insert(a.begin() + i, x); // 在第 i 个位置前插入（O(n)，慎用）
a.erase(a.begin() + i);     // 删除第 i 个元素（O(n)，慎用）

// ---- 访问 ----
a[i];                    // 下标访问（不检查越界）
a.at(i);                 // 带越界检查的访问（慢，比赛不用）
a.front();               // 第一个元素
a.back();                // 最后一个元素

// ---- 遍历 ----
for (int i = 0; i < a.size(); ++i) { /* a[i] */ }
for (auto &x : a) { /* x */ }            // 修改元素
for (const auto &x : a) { /* x */ }      // 只读

// ---- 常用属性 ----
a.size();                // 元素个数（返回 size_t，与 int 比较时注意）
a.empty();               // 是否为空
a.clear();               // 清空

// ---- 注意 ----
// size() 返回无符号整数！若 size=0，则 size()-1 = 18446744073709551615
// 建议：for (int i = 0; i < (int)a.size(); ++i) —— 强制转 int
```

### 2.2 Python list 核心操作

```python
# ---- 创建 ----
arr = []              # 空列表
arr = [0] * 10        # 10 个 0（注意：元素是同一个对象的引用！）
arr = [0 for _ in range(10)]  # 安全的 10 个 0
arr = [1, 2, 3]
# 二维数组（安全写法）
mat = [[0] * m for _ in range(n)]

# ---- 添加/删除 ----
arr.append(x)         # 末尾添加
arr.pop()             # 末尾删除并返回
arr.pop(i)            # 删除第 i 个元素（O(n)）
arr.insert(i, x)      # 在第 i 个位置插入（O(n)）

# ---- 切片 (Python 独有优势) ----
arr[1:5]              # 下标 [1, 5) 的子列表（浅拷贝）
arr[::-1]             # 反转
arr[::2]              # 每隔一个取一个

# ---- 遍历 ----
for x in arr: ...
for i, x in enumerate(arr): ...  # 同时获取下标和值
```

#### Python list 的一个经典坑

```python
# ❌ 错：所有行是同一个 list 的引用
mat = [[0] * 5] * 3
mat[0][0] = 1
# mat 变成 [[1,0,0,0,0], [1,0,0,0,0], [1,0,0,0,0]]

# ✅ 对：每行是独立的 list
mat = [[0] * 5 for _ in range(3)]
mat[0][0] = 1
# mat 变成 [[1,0,0,0,0], [0,0,0,0,0], [0,0,0,0,0]]
```

---

## 3. sort / 排序

排序是使用频率最高的算法之一，STL 的 `sort()` 是 O(n log n) 的内省排序。

### 3.1 C++ sort

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> a = {3, 1, 4, 1, 5, 9};

// ---- 基本排序 ----
sort(a.begin(), a.end());                      // 升序：[1,1,3,4,5,9]
sort(a.begin(), a.end(), greater<int>());      // 降序：[9,5,4,3,1,1]

// ---- 部分排序 ----
sort(a.begin(), a.begin() + 3);                // 只排前 3 个：[1,3,4,1,5,9]

// ---- 结构体/自定义排序 ----
struct Person {
    string name;
    int age;
    int score;
};
vector<Person> p;

// 方式1：Lambda 表达式（推荐，最灵活）
sort(p.begin(), p.end(), [](const Person &a, const Person &b) {
    // 按分数降序；分数相同按年龄升序
    if (a.score != b.score) return a.score > b.score;
    return a.age < b.age;
});

// 方式2：重载 < 运算符
struct Person {
    string name;
    int age;
    bool operator<(const Person &other) const {
        return age < other.age;  // 按年龄升序
    }
};
sort(p.begin(), p.end());  // 自动使用 operator<

// ---- 对 pair 排序 ----
vector<pair<int, int>> v = {{3, 2}, {1, 5}, {3, 1}};
sort(v.begin(), v.end());
// 结果：[(1,5), (3,1), (3,2)]  -- 默认先比 first 再比 second，均升序
```

#### Lambda 排序速查

```cpp
// 按 first 升序，second 降序
sort(v.begin(), v.end(), [](auto &a, auto &b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second > b.second;
});

// 按绝对值升序
sort(a.begin(), a.end(), [](int x, int y) {
    return abs(x) < abs(y);
});
```

### 3.2 Python sort

```python
# ---- 基本排序 ----
arr = [3, 1, 4, 1, 5, 9]
arr.sort()                  # 原地升序：[1,1,3,4,5,9]
arr.sort(reverse=True)      # 原地降序：[9,5,4,3,1,1]
sorted_arr = sorted(arr)    # 返回新列表，原列表不变

# ---- 自定义排序 ----
# key 参数（最常用）
arr.sort(key=lambda x: abs(x))          # 按绝对值升序
arr.sort(key=lambda x: (x[0], -x[1]))   # 按 first 升序，second 降序

# 自定义比较函数（functools.cmp_to_key）
from functools import cmp_to_key
def cmp(a, b):
    if a[0] != b[0]:
        return -1 if a[0] > b[0] else 1  # first 降序
    return -1 if a[1] < b[1] else 1      # second 升序
arr.sort(key=cmp_to_key(cmp))
```

---

## 4. lower_bound / upper_bound（二分查找）

**重要性：⭐⭐⭐⭐⭐**。在有序数组中定位元素，是二分答案的基础。

### 4.1 C++

```cpp
#include <algorithm>
#include <vector>
using namespace std;

vector<int> a = {1, 2, 2, 2, 5, 7, 9};  // 必须已排序！

// lower_bound(begin, end, x)：第一个 >= x 的位置
int p1 = lower_bound(a.begin(), a.end(), 2) - a.begin();  // p1 = 1 (a[1]=2)

// upper_bound(begin, end, x)：第一个 > x 的位置
int p2 = upper_bound(a.begin(), a.end(), 2) - a.begin();  // p2 = 4 (a[4]=5)

// 所以数组中值为 2 的元素个数 = upper_bound - lower_bound = 3

// binary_search(begin, end, x)：x 是否存在
bool exist = binary_search(a.begin(), a.end(), 5);  // true
bool exist2 = binary_search(a.begin(), a.end(), 3); // false
```

**关键理解：返回的是迭代器，减去 `a.begin()` 得到下标。如果所有元素都小于 x，返回 `a.end()`。**

### 4.2 Python（bisect 模块）

```python
import bisect

arr = [1, 2, 2, 2, 5, 7, 9]

# bisect_left: 第一个 >= x 的位置（等价于 C++ lower_bound）
p1 = bisect.bisect_left(arr, 2)   # p1 = 1

# bisect_right: 第一个 > x 的位置（等价于 C++ upper_bound）
p2 = bisect.bisect_right(arr, 2)  # p2 = 4

# 带插入功能
bisect.insort_left(arr, 3)   # 插入 3 到合适位置，保持有序（O(n)）
```

---

## 5. map / set 与 unordered_map / unordered_set

这是数据结构题的核心。

### 5.1 区别一览

| 类型 | C++ | Python 等价 | 底层 | 操作复杂度 |
|------|-----|------------|------|-----------|
| 有序映射 | `map<k, v>` | 无内置* | 红黑树 | O(log n) |
| 哈希映射 | `unordered_map<k, v>` | `dict` | 哈希表 | O(1) 平均 |
| 有序集合 | `set<k>` | 无内置* | 红黑树 | O(log n) |
| 哈希集合 | `unordered_set<k>` | `set` | 哈希表 | O(1) 平均 |

\* Python 无内置有序映射/集合。需要时可用 `bisect` 维护有序 list，或用 `sortedcontainers` 库（比赛中不可用）。大多数情况 `dict`/`set` 就足够了。

### 5.2 C++ map / unordered_map（映射）

```cpp
#include <map>
#include <unordered_map>
using namespace std;

// ---- 创建 ----
map<string, int> mp;             // 有序映射，键是 string
unordered_map<int, int> cnt;     // 哈希映射（比赛中更常用）

// ---- 插入/修改 ----
cnt[5] = 10;             // 若 key=5 不存在则创建，存在则覆盖
cnt[5]++;                // 计数场景（键不存在时自动插入并初始化为 0 再 ++）

// ---- 查找 ----
if (cnt.count(5)) { }    // 判断 key 是否存在（返回 0 或 1）
auto it = cnt.find(5);   // 返回迭代器，不存在则返回 cnt.end()
if (it != cnt.end()) {
    int val = it->second;  // 键值对：it->first 是 key，it->second 是 value
}

// ---- 遍历 ----
for (auto &[key, val] : cnt) {   // C++17 结构化绑定
    cout << key << ": " << val << '\n';
}
for (auto &p : cnt) {            // 兼容旧标准
    cout << p.first << ": " << p.second << '\n';
}

// ---- 删除 ----
cnt.erase(5);             // 删除 key=5
cnt.clear();              // 清空
```

#### 字符串键用 unordered_map 的注意点

```cpp
// unordered_map<string, int> 每次查找需要计算字符串哈希，O(字符串长度)
// 如果字符串很长且查询次数多，可能超时
// 策略：如果不需要有序，且键是整数，优先用 unordered_map
// 如果键是字符串且需要大量查找，考虑用 map 或自己写字符串哈希
```

### 5.3 C++ set / unordered_set（集合）

```cpp
#include <set>
#include <unordered_set>
using namespace std;

// ---- 创建 ----
set<int> s;               // 有序集合（自动去重 + 升序）
unordered_set<int> us;    // 哈希集合（去重，无序，O(1)）

// ---- 插入 ----
s.insert(5);              // 插入，若已存在则忽略
us.insert(5);

// ---- 查找 ----
if (s.count(5)) { }       // 是否存在
auto it = s.find(5);      // 返回迭代器或 s.end()

// ---- 删除 ----
s.erase(5);
s.erase(it);              // 按迭代器删除（O(1) 均摊）

// ---- set 独有：有序操作 ----
auto it = s.lower_bound(5);   // 第一个 >= 5 的元素
auto it = s.upper_bound(5);   // 第一个 > 5 的元素
int min_val = *s.begin();     // 最小值
int max_val = *s.rbegin();    // 最大值

// ---- 遍历 set ----
for (int x : s) { /* 按升序遍历 */ }
```

### 5.4 Python dict / set

```python
# ---- dict（哈希映射） ----
cnt = {}
cnt[5] = cnt.get(5, 0) + 1   # 安全计数（key 不存在时返回默认值 0）

from collections import Counter
cnt = Counter(arr)             # 批量计数，一行搞定
most = cnt.most_common(3)      # 出现次数最多的前 3 个

# ---- set（哈希集合） ----
s = set()
s.add(5)
s.remove(5)                   # 删除，若不存在会报错
s.discard(5)                  # 安全删除，不存在也不报错

# 集合运算
a = {1, 2, 3}
b = {2, 3, 4}
a & b    # 交集 {2, 3}
a | b    # 并集 {1, 2, 3, 4}
a - b    # 差集 {1}
```

---

## 6. stack / queue / deque

### 6.1 C++

```cpp
#include <stack>
#include <queue>
#include <deque>
using namespace std;

// ---- stack：后进先出 ----
stack<int> st;
st.push(1);
st.push(2);
int top = st.top();  // top = 2
st.pop();            // 弹出栈顶（不返回值！必须先 top() 再 pop()）

// ---- queue：先进先出（BFS 用）----
queue<int> q;
q.push(1);
q.push(2);
int front = q.front();  // front = 1
int back = q.back();    // back = 2
q.pop();

// ---- deque：双端队列 ----
deque<int> dq;
dq.push_back(1);       // 尾部插入
dq.push_front(2);      // 头部插入
dq.pop_back();         // 尾部弹出
dq.pop_front();        // 头部弹出
dq.front();            // 访问头部
dq.back();             // 访问尾部
// deque 也支持随机访问：dq[i]
```

### 6.2 Python

```python
# ---- list 当作栈 ----
st = []
st.append(1)
st.append(2)
st.pop()     # 返回 2

# ---- collections.deque（双端队列，BFS 用）----
from collections import deque
q = deque()
q.append(1)       # 右边加
q.appendleft(2)   # 左边加
q.pop()           # 右边弹出
q.popleft()       # 左边弹出（BFS 最常用）

# 普通队列：每次用 pop(0) 从 list 左端弹出是 O(n)，必须用 deque！
```

---

## 7. priority_queue / heapq（优先队列）

**重要性：⭐⭐⭐⭐⭐**。贪心题、Dijkstra 的基石。

### 7.1 C++ priority_queue

```cpp
#include <queue>
#include <vector>
using namespace std;

// ---- 大顶堆（默认）：top() 返回最大值 ----
priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(5);
int max_val = pq.top();  // 5
pq.pop();

// ---- 小顶堆：top() 返回最小值 ----
priority_queue<int, vector<int>, greater<int>> min_pq;
min_pq.push(3);
min_pq.push(1);
int min_val = min_pq.top();  // 1

// ---- 自定义排序的优先队列 ----
// pair 的优先队列：默认按 first 降序，first 相同按 second 降序
priority_queue<pair<int, int>> pq_pair;

// 按 second 降序的小顶堆（自定义）
struct Node {
    int id, val;
};
// 重要：优先队列的比较方向与 sort 相反！
// priority_queue 默认是大顶堆，返回 true 表示"b 优先于 a"
auto cmp = [](const Node &a, const Node &b) {
    return a.val > b.val;  // val 小的在堆顶（小顶堆）
};
priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);
```

#### priority_queue 与 sort 比较函数的区别（重要！）

```cpp
// sort：true 表示 a 排在 b 前面
// priority_queue：true 表示 b 更优先（在堆顶方向更靠前）

// 例子：按年龄升序
// sort:  return a.age < b.age;
// pq:    return a.age > b.age;  // 反过来的！

// 实践中建议用 decltype + lambda 的方式，避免混乱
```

### 7.2 Python heapq

```python
import heapq

# ---- Python 只有小顶堆 ----
pq = []
heapq.heappush(pq, 3)
heapq.heappush(pq, 1)
heapq.heappush(pq, 5)
min_val = pq[0]         # 1（看堆顶，不弹出）
min_val = heapq.heappop(pq)  # 1（弹出堆顶）

# ---- 大顶堆技巧：存负数 ----
max_pq = []
heapq.heappush(max_pq, -3)
heapq.heappush(max_pq, -1)
max_val = -heapq.heappop(max_pq)  # -(-3) = 3

# ---- 堆中存元组：按第一个元素排序 ----
pq = []
heapq.heappush(pq, (val, idx))  # 先按 val 再按 idx 排序
```

---

## 8. string 常用操作

### 8.1 C++ string

```cpp
#include <string>
using namespace std;

// ---- 创建 ----
string s = "hello";
string s2(s);              // 拷贝
string s3(5, 'a');         // "aaaaa"

// ---- 拼接 ----
s += " world";             // "hello world"
s += '!';                  // "hello world!"
string combined = s + s2;  // + 运算符

// ---- 子串 ----
string sub = s.substr(0, 5);    // 从 0 开始长 5（"hello"）
string sub2 = s.substr(6);      // 从 6 到末尾（"world!"）

// ---- 查找 ----
size_t pos = s.find("wo");      // 返回第一次出现的位置，找不到返回 string::npos
if (s.find("xy") == string::npos) { }  // 判断是否找到

// ---- 类型转换 ----
int x = stoi("123");            // string → int
long long y = stoll("123456");  // string → long long
double d = stod("3.14");        // string → double
string num = to_string(123);    // 数字 → string

// ---- 其他 ----
reverse(s.begin(), s.end());    // 反转
sort(s.begin(), s.end());       // 按字符排序
s.push_back('!');               // 末尾加字符
s.pop_back();                   // 末尾删字符
for (char c : s) { }            // 遍历每个字符
```

### 8.2 Python string

```python
s = "hello"

# ---- 子串（切片） ----
s[0:5]        # "hello"
s[1:]         # "ello"
s[::-1]       # "olleh"（反转）

# ---- 查找 ----
s.find("ll")  # 2（返回第一次出现的位置，找不到返回 -1）
"ll" in s     # True

# ---- 类型转换 ----
str(123)       # 数字 → 字符串
int("123")     # 字符串 → 数字

# ---- 字符串的不可变性 ----
# Python 字符串是不可变的，修改需要转为 list
chars = list(s)
chars[0] = 'H'
s = ''.join(chars)  # "Hello"

# ---- 常用 ----
','.join(["a", "b", "c"])  # "a,b,c"
"a,b,c".split(',')          # ["a", "b", "c"]
s.upper()
s.lower()
s.isdigit()
s.isalpha()
```

---

## 9. 暴力与模拟

### 9.1 什么时候该写暴力

- **签到题**：通常直接模拟就能过
- **不会做的题**：写暴力拿部分分（IOI 赛制！）
- **数据范围小（n ≤ 100）**：可能是暴力 + 剪枝的复杂度
- **验证想法**：先用暴力跑小样例验证思路

### 9.2 枚举基本模式

#### 多重循环枚举

```cpp
// 枚举所有组合：从 n 个元素中选 3 个
int n;
vector<int> a(n);
for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j)
        for (int k = j + 1; k < n; ++k) {
            // 处理 (a[i], a[j], a[k])
        }

// 枚举所有子集（位运算）
for (int mask = 0; mask < (1 << n); ++mask) {
    // mask 的二进制表示中，第 i 位为 1 表示选中第 i 个元素
    for (int i = 0; i < n; ++i)
        if (mask >> i & 1) {
            // 处理选中的元素 a[i]
        }
}
```

#### 全排列枚举

```cpp
#include <algorithm>
vector<int> a = {1, 2, 3};
do {
    // 处理当前排列
} while (next_permutation(a.begin(), a.end()));
// 注意：next_permutation 要求初始有序，否则只能生成一部分排列
```

```python
import itertools
for perm in itertools.permutations([1, 2, 3]):
    # 处理当前排列
    pass

for comb in itertools.combinations(arr, 3):
    # 从 arr 中选 3 个的组合
    pass
```

### 9.3 模拟题的常见优化

模拟题容易超时（TLE），核心原因是：**按题目描述的每一步逐一执行，但数据很大**。

优化技巧：

1. **找循环节**：如果状态有限（比如模运算），模拟若干轮后会进入循环，可以用数学方法跳过循环。

2. **批量操作**：不要一个一个处理，用数学公式批量算。

3. **剪枝**：如果当前状态不可能是答案，提前退出。

4. **数据结构加速**：模拟过程需要频繁"找下一个 XXX"，用 set/map 加速查找。

**经典例题：约瑟夫问题**

> n 个人围成一圈，从第 1 个开始报数，报到 m 的出列，问最后剩下的人在原始序列中的位置。

直接模拟淘汰过程（O(n²)），如果 n=10^5 会超时。递推公式是 O(n)：
```
f(1) = 0
f(i) = (f(i-1) + m) % i    // i 表示当前人数
最终答案 = f(n) + 1        // 编号从 1 开始则 +1
```

---

## 10. 边界处理清单

每次写代码时逐条检查：

| 检查项 | 说明 |
|--------|------|
| 数组越界 | `a[i]` 是否保证 `0 ≤ i < a.size()` |
| 空容器 | 调用 `front()`、`back()`、`top()` 前容器是否非空 |
| 除零 | 除法/取模的右操作数是否可能为 0 |
| 负数取模 | C++ 中 `-5 % 3 = -2`，Python 中 `-5 % 3 = 1`！需要时用 `(x % m + m) % m` |
| 向上取整 | `(a + b - 1) / b` 而非 `ceil(a/b)`（整数除法的浮点误差） |
| 数据类型 | int 范围约 ±2.1×10^9，超出用 long long |
| 大数乘法溢出 | `a * b % MOD`：如果 a,b 可达 10^9，先转 long long 再乘 |
| 无符号数 | `a.size() - 1` 当 size=0 时不是 -1 而是一个巨大值 |
| 递归深度 | C++ 默认栈约 8MB（递归深度 ~10^5 可能爆栈）；Python 默认递归限制 1000 |

---

## Day 1 配套练习

> 每道题先自己写，卡住 15 分钟再看答案。每道题必须亲手敲代码，不要复制粘贴。

| 题号 | 题目 | 考察点 |
|------|------|--------|---------|
| 1 | 输入 N 个整数，去重后按升序输出 | set / sort + unique |
| 2 | 输入一个英文句子，统计每个单词出现次数 | map / dict 计数 |
| 3 | N 个学生，每个有姓名、语文、数学成绩。按总分降序，总分相同按语文降序，再同按姓名升序输出 | struct 自定义排序 |
| 4 | 约瑟夫问题（n ≤ 1000，直接模拟） | queue 模拟 |
| 5 | 给出 N 个数，找出所有三元组 (i,j,k) 满足 i<j<k 且 a[i]+a[j]+a[k]=0（N ≤ 200） | 三重循环 + 剪枝 |

> 做完后用自己的话总结：今天哪些操作可以盲打了？哪些还需要查文档？把需要查的加入明天的复习清单。

---

## 快速参考卡片

```
┌─────────────────────────────────────────────────────────┐
│  每天敲一遍，七天形成肌肉记忆                              │
├─────────────────────────────────────────────────────────┤
│  vector<int> a(n);            // 动态数组                │
│  sort(a.begin(), a.end());     // 升序排序               │
│  sort(a.begin(), a.end(), greater<int>()); // 降序      │
│  lower_bound(a.begin(), a.end(), x) - a.begin();         │
│  unordered_map<int, int> cnt;  // 哈希计数               │
│  priority_queue<int> pq;       // 大顶堆                 │
│  priority_queue<int, vector<int>, greater<int>> min_pq;  │
│  cin >> n; while (n--) { }    // 多组数据                │
│  cout << ans << '\n';          // 输出（用 \n 不用 endl） │
│  (ll)a * b % MOD               // 安全乘法取模           │
└─────────────────────────────────────────────────────────┘
```
