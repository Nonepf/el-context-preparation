# Day 5：图论（存储、DFS/BFS、Dijkstra、拓扑排序）

> **学习目标**：掌握图的邻接表存储，BFS/DFS 遍历模板，Dijkstra 最短路的堆优化版本，以及拓扑排序的 Kahn 算法。
>

---

## 1. 图的存储

### 1.1 三种存储方式对比

| 方式 | 空间 | 判边 | 遍历邻居 | 适用场景 |
|------|------|------|----------|----------|
| 邻接矩阵 | O(n²) | O(1) | O(n) | n ≤ 1000 |
| 邻接表 | O(n + m) | O(度) | O(度) | **最常用** |
| 链式前向星 | O(n + m) | O(度) | O(度) | 需要按添加顺序遍历边时 |

**比赛中 95% 的情况用邻接表。**

### 1.2 邻接表（带权图和不带权图）

#### C++

```cpp
int n, m;  // n 个点，m 条边
cin >> n >> m;

// 不带权图
vector<vector<int>> g(n);
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    u--; v--;  // 如果题目是 1-indexed，转为 0-indexed
    g[u].push_back(v);
    g[v].push_back(u);  // 无向图需要存双向！有向图不需要这行
}

// 带权图
struct Edge {
    int to, weight;
};
vector<vector<Edge>> g(n);
for (int i = 0; i < m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    u--; v--;
    g[u].push_back({v, w});
    g[v].push_back({u, w});  // 无向图
}
```

#### Python

```python
n, m = map(int, input().split())

# 不带权图
g = [[] for _ in range(n)]
for _ in range(m):
    u, v = map(int, input().split())
    u -= 1; v -= 1
    g[u].append(v)
    g[v].append(u)  # 无向图

# 带权图
g = [[] for _ in range(n)]
for _ in range(m):
    u, v, w = map(int, input().split())
    u -= 1; v -= 1
    g[u].append((v, w))
    g[v].append((u, w))  # 无向图
```

### 1.3 邻接矩阵（小数据用）

```cpp
// n ≤ 1000 时可用
vector<vector<int>> mat(n, vector<int>(n, 0));
// 或 vector<vector<int>> mat(n, vector<int>(n, INF))  带权图
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    mat[u][v] = mat[v][u] = 1;
}
```

---

## 2. DFS（深度优先搜索）

### 2.1 递归版本（最常用）

```cpp
vector<vector<int>> g;
vector<bool> vis;

void dfs(int u) {
    vis[u] = true;
    // 处理节点 u（前序）

    for (int v : g[u]) {
        if (!vis[v]) {
            dfs(v);
        }
    }

    // 处理节点 u（后序，需要时使用）
}

// 主函数
int main() {
    // ... 建图 ...
    vis.assign(n, false);
    for (int i = 0; i < n; ++i) {
        if (!vis[i]) {
            dfs(i);  // 每个连通分量
        }
    }
}
```

### 2.2 栈模拟版本（避免递归深度问题）

C++ 默认栈空间约 8MB，递归深度超过 ~10^5 可能爆栈。用显式栈可以避免。

```cpp
// 栈模拟 DFS（前序遍历）
void dfs_iter(int start) {
    vector<bool> vis(n, false);
    stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int u = st.top();
        st.pop();
        if (vis[u]) continue;
        vis[u] = true;
        // 处理 u

        for (int v : g[u]) {
            if (!vis[v]) st.push(v);
        }
    }
}
```

### 2.3 DFS 的应用场景

| 场景 | 做法 |
|------|------|
| 连通块计数 | 对每个未访问点启动 DFS，每次计数 +1 |
| 检测环（无向图） | DFS 记录 parent，若访问到非 parent 的已访问点 → 有环 |
| 检测环（有向图） | 三色标记法：0=未访问，1=递归中，2=已完成；访问到状态 1 的节点 → 有环 |
| 拓扑排序 | 后序入栈（见下文 5.3） |
| 二分图判定 | DFS 交替染色（染色法） |
| 树的直径 | 两次 DFS |

### 2.4 二分图判定（染色法）

```cpp
// 能否把图分成两部分，每条边连接两个不同部分？
vector<int> color;  // 0=未染色, 1=颜色A, 2=颜色B

bool dfs(int u, int c) {
    color[u] = c;
    for (int v : g[u]) {
        if (color[v] == c) return false;  // 相邻节点颜色相同，不是二分图
        if (color[v] == 0 && !dfs(v, 3 - c)) return false;  // 3-c: 1变2, 2变1
    }
    return true;
}

bool isBipartite() {
    color.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && !dfs(i, 1))
            return false;
    }
    return true;
}
```

#### Python

```python
def is_bipartite(g, n):
    color = [0] * n
    def dfs(u, c):
        color[u] = c
        for v in g[u]:
            if color[v] == c:
                return False
            if color[v] == 0 and not dfs(v, 3 - c):
                return False
        return True

    for i in range(n):
        if color[i] == 0 and not dfs(i, 1):
            return False
    return True
```

---

## 3. BFS（广度优先搜索）

### 3.1 BFS 模板

BFS 的核心是**队列**，保证"先被发现的节点先处理"，从而自然实现了层次遍历。

```cpp
#include <queue>

vector<vector<int>> g;
vector<int> dist;  // 从起点出发的距离，-1 表示未访问

void bfs(int start) {
    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : g[u]) {
            if (dist[v] == -1) {  // 未访问
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

// 主函数
int main() {
    // ... 建图 ...
    dist.assign(n, -1);
    bfs(0);
    // dist[i] 就是从 0 到 i 的最短距离（无权图）
}
```

#### Python

```python
from collections import deque

def bfs(g, start):
    n = len(g)
    dist = [-1] * n
    q = deque([start])
    dist[start] = 0

    while q:
        u = q.popleft()  # 注意是 popleft 不是 pop
        for v in g[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                q.append(v)
    return dist
```

### 3.2 BFS 的应用场景

| 场景 | 做法 |
|------|------|
| 无权图最短路径 | BFS 天然按距离层次访问，`dist` 直接是最短距离 |
| 迷宫最短路径 | 网格中的 BFS，上下左右四个方向 |
| 层次遍历 | 二叉树/树的层序，图的层序 |
| 多源 BFS | 初始时多个起点一起入队（如"腐烂的橘子"） |
| 0-1 BFS | 边权只有 0 和 1 的最短路，用 deque 代替 queue |

### 3.3 网格 BFS（迷宫问题）

```cpp
// n×m 的网格，0 表示空地，1 表示障碍，从 (sx,sy) 到 (tx,ty)
int dx[] = {0, 1, 0, -1};  // 右、下、左、上
int dy[] = {1, 0, -1, 0};

int bfs_grid(vector<vector<int>>& grid, int sx, int sy, int tx, int ty) {
    int n = grid.size(), m = grid[0].size();
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<int, int>> q;

    q.push({sx, sy});
    dist[sx][sy] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == tx && y == ty) return dist[x][y];

        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;  // 越界
            if (grid[nx][ny] == 1) continue;                       // 障碍
            if (dist[nx][ny] != -1) continue;                      // 已访问

            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }
    return -1;  // 无法到达
}
```

#### Python 网格 BFS

```python
from collections import deque

def bfs_grid(grid, sx, sy, tx, ty):
    n, m = len(grid), len(grid[0])
    dist = [[-1] * m for _ in range(n)]
    q = deque([(sx, sy)])
    dist[sx][sy] = 0
    directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]

    while q:
        x, y = q.popleft()
        if x == tx and y == ty:
            return dist[x][y]
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < n and 0 <= ny < m and grid[nx][ny] == 0 and dist[nx][ny] == -1:
                dist[nx][ny] = dist[x][y] + 1
                q.append((nx, ny))
    return -1
```

---

## 4. Dijkstra（最短路径，带权图）

### 4.1 核心思想

Dijkstra 解决**非负权图**的单源最短路径问题。

**做法**：每次从"未确定最短距离"的点中取距离最小的，用它松弛（relax）它的邻居。

**暴力版**：O(n²)，适合稠密图（n ≤ 5000, m 接近 n²）。
**堆优化版**：O(m log n)，适合稀疏图（绝大多数情况）。

### 4.2 堆优化 Dijkstra（必须背熟的模板）

```cpp
#include <queue>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

struct Edge {
    int to, w;
};

vector<ll> dijkstra(int n, vector<vector<Edge>>& g, int start) {
    vector<ll> dist(n, INF);
    dist[start] = 0;

    // 优先队列：{距离, 节点编号}，小顶堆
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // 关键！如果弹出的是过时的"旧距离"，直接忽略
        if (d != dist[u]) continue;

        for (auto &e : g[u]) {
            int v = e.to;
            ll nd = d + e.w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }
    return dist;
}
```

**第 18 行的 `if (d != dist[u]) continue;` 是正确性的关键！** 解释：
- 同一个节点可能被多次入队（每次距离被更新时）
- 当弹出的距离比当前记录的 dist 大时，说明这已经是"过时的旧信息"，忽略它

### 4.3 Python 版本

```python
import heapq
from typing import List, Tuple

INF = 10 ** 18

def dijkstra(n: int, g: List[List[Tuple[int, int]]], start: int) -> List[int]:
    dist = [INF] * n
    dist[start] = 0
    pq = [(0, start)]  # (距离, 节点)

    while pq:
        d, u = heapq.heappop(pq)
        if d != dist[u]:
            continue
        for v, w in g[u]:
            nd = d + w
            if nd < dist[v]:
                dist[v] = nd
                heapq.heappush(pq, (nd, v))
    return dist
```

### 4.4 Dijkstra 不能处理负权边

如果图中可能有负权边，需要用 SPFA（Bellman-Ford 的队列优化）。但比赛题一般不会故意卡 SPFA（除非数据被设计成卡它），所以如果题目没有负权边，安心用 Dijkstra。

---

## 5. 拓扑排序

### 5.1 什么是拓扑排序

**适用场景**：有向无环图（DAG）。给节点一个线性排序，使得每条边 u→v 都满足 u 排在 v 前面。

**典型例子**：课程安排（先修课关系）、任务依赖、编译顺序。

### 5.2 Kahn 算法（BFS 实现，推荐）

```cpp
vector<int> topoSort(int n, vector<vector<int>>& g) {
    vector<int> indegree(n, 0);  // 每个节点的入度

    // 1. 统计入度
    for (int u = 0; u < n; ++u) {
        for (int v : g[u]) {
            indegree[v]++;
        }
    }

    // 2. 入度为 0 的节点入队
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    // 3. BFS
    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);

        for (int v : g[u]) {
            indegree[v]--;
            if (indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    // 4. 如果 order.size() < n，说明图中有环，无法拓扑排序
    // if (order.size() != n) return {};  // 或报告错误

    return order;
}
```

#### Python

```python
from collections import deque

def topo_sort(n, g):
    indegree = [0] * n
    for u in range(n):
        for v in g[u]:
            indegree[v] += 1

    q = deque([i for i in range(n) if indegree[i] == 0])
    order = []

    while q:
        u = q.popleft()
        order.append(u)
        for v in g[u]:
            indegree[v] -= 1
            if indegree[v] == 0:
                q.append(v)

    # order 长度 < n 说明有环
    return order if len(order) == n else []
```

### 5.3 DFS 后序实现

```cpp
vector<int> order;
vector<bool> vis, in_stack;

bool dfs(int u, vector<vector<int>>& g) {
    vis[u] = true;
    in_stack[u] = true;
    for (int v : g[u]) {
        if (!vis[v]) {
            if (!dfs(v, g)) return false;
        } else if (in_stack[v]) {
            return false;  // 发现环（v 在当前递归栈中）
        }
    }
    in_stack[u] = false;
    order.push_back(u);  // 后序入栈，最后需要反转
    return true;
}

vector<int> topoSortDFS(int n, vector<vector<int>>& g) {
    vis.assign(n, false);
    in_stack.assign(n, false);
    order.clear();

    for (int i = 0; i < n; ++i) {
        if (!vis[i]) {
            if (!dfs(i, g)) return {};  // 有环
        }
    }

    reverse(order.begin(), order.end());
    return order;
}
```

---

## 6. 最短路径问题的选择指南

| 条件 | 用什么 | 复杂度 |
|------|--------|--------|
| 无权图 | BFS | O(n + m) |
| 边权只有 0 和 1 | 0-1 BFS (deque) | O(n + m) |
| 非负权，一般图 | Dijkstra 堆优化 | O(m log n) |
| 有负权边 | SPFA / Bellman-Ford | O(nm) |
| 全源最短路，n ≤ 300 | Floyd | O(n³) |
| DAG（有向无环图） | 拓扑序 DP | O(n + m) |

---

## 7. BFS/DFS 选择速查

| 目标 | 选哪个 |
|------|--------|
| 无权最短路径 | BFS |
| 判断连通性 | 都行，DFS 代码更短 |
| 连通块计数 | DFS（更直观） |
| 找任意一条路径 | DFS（记录 parent） |
| 找环 | DFS（有向图用三色法，无向图记录 parent） |
| 二分图判定 | DFS 或 BFS（交替染色） |
| 拓扑排序 | BFS（Kahn 算法） |
| 层序遍历 | BFS |

---

## Day 5 配套练习

| 题号 | 题目 | 考察点 |
|------|------|--------|
| 1 | 图中连通块个数（DFS） | DFS 基础 |
| 2 | 迷宫最短路径（网格 BFS） | BFS |
| 3 | Dijkstra 模板题：求起点到各点最短距离 | Dijkstra |
| 4 | 拓扑排序模板题：输出任意合法拓扑序 | Kahn |
| 5 | 课程表（检测能否完成所有课程 = 检测是否有环） | 拓扑排序 / DFS 判环 |
| 6 | 判断二分图 | 染色法 |

---

## 快速参考卡片

```
┌──────────────────────────────────────────────────────────┐
│  图的存储: vector<vector<int>> g(n);  // 邻接表         │
│  带权图: vector<vector<pair<int,int>>> g(n);            │
│                                                             │
│  DFS 模板                                                  │
│  void dfs(u): vis[u]=1; for(v:g[u]) if(!vis[v]) dfs(v)  │
│                                                             │
│  BFS 模板                                                  │
│  q.push(start); dist[start]=0                            │
│  while q: u=q.front(); for v: if(dist[v]==-1) dist[v]=...│
│                                                             │
│  Dijkstra 模板                                             │
│  dist.fill(INF); pq.push({0,start})                       │
│  while pq: {d,u}=pq.top(); if(d!=dist[u]) continue;       │
│     for{v,w}: if(dist[v]>d+w) dist[v]=d+w; pq.push(...)  │
│                                                             │
│  拓扑 (Kahn): 入度表+队列，入度为0的入队                   │
│  判环: order.size() < n → 有环                            │
└──────────────────────────────────────────────────────────┘
```
