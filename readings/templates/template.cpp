/**
 * 算法竞赛 C++ 通用模板
 * 比赛时直接复制粘贴此文件内容，在此基础上写题解
 *
 * 使用说明：
 * 1. 将 solve() 函数中的逻辑替换为每道题的解法
 * 2. 多组测试数据时保留 while(t--) 结构
 * 3. 单组数据时删除 while(t--) 即可
 */

#include <bits/stdc++.h>
using namespace std;

// ============ 类型别名 (加快手速) ============
using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vvi = vector<vi>;
using vvll = vector<vll>;

// ============ 常用常量 ============
const int INF = 0x3f3f3f3f;          // 约 1.06e9，两倍不会溢出
const ll LINF = 0x3f3f3f3f3f3f3f3fLL; // 约 4.5e18
const int MOD = 1e9 + 7;              // 常见模数，或者 998244353
const double EPS = 1e-9;              // 浮点数比较误差

// ============ 常用宏 ============
#define sz(x) ((int)(x).size())
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define fi first
#define se second
#define pb push_back
#define eb emplace_back
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define per(i, a, b) for (int i = (b) - 1; i >= (a); --i)
#define each(i, x) for (auto &i : x)

// ============ 调试宏 (提交前删除或注释掉) ============
// #define DEBUG
#ifdef DEBUG
#define debug(x) cerr << #x << " = " << (x) << endl
#define debug2(x, y) cerr << #x << " = " << (x) << ", " << #y << " = " << (y) << endl
#else
#define debug(x)
#define debug2(x, y)
#endif

// ============ 数学工具函数 ============

// 最大公约数 (C++17 起可直接用 std::gcd)
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }

// 最小公倍数 (注意先除后乘防溢出)
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }

// 快速幂 a^b % m  — 几乎所有取模幂运算都用它
ll qpow(ll a, ll b, ll m = MOD) {
    ll res = 1;
    a %= m;
    while (b) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// 向上取整除法 (a+b-1)/b，或使用此函数
ll ceil_div(ll a, ll b) { return (a + b - 1) / b; }

// 判断是否为质数 (sqrt(n) 复杂度，用于 n <= 1e12)
bool is_prime(ll n) {
    if (n < 2) return false;
    for (ll i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

// 埃氏筛法：返回 [2, n] 内所有质数
vi sieve(int n) {
    vector<bool> is_p(n + 1, true);
    vi primes;
    is_p[0] = is_p[1] = false;
    for (int i = 2; i <= n; ++i) {
        if (!is_p[i]) continue;
        primes.pb(i);
        for (ll j = (ll)i * i; j <= n; j += i)
            is_p[j] = false;
    }
    return primes;
}

// ============ 常用 STL 用法速查 ============
// 见 day1 详细说明，这里只放最常用的模式：

// --- 排序 ---
// sort(all(v));                          // 升序
// sort(all(v), greater<int>());          // 降序
// sort(all(v), [](auto &a, auto &b) {    // 自定义
//     return a.fi != b.fi ? a.fi < b.fi : a.se > b.se;
// });

// --- 去重 (需先排序) ---
// v.erase(unique(all(v)), v.end());

// --- 二分查找 (有序数组) ---
// int pos = lower_bound(all(v), x) - v.begin();   // 第一个 >= x 的位置
// int pos = upper_bound(all(v), x) - v.begin();   // 第一个 > x 的位置
// bool exist = binary_search(all(v), x);

// --- map/unordered_map ---
// unordered_map<int, int> cnt;  // 哈希表，O(1)，无序
// map<int, int> mp;             // 红黑树，O(log n)，有序

// --- set/unordered_set ---
// unordered_set<int> st;        // 哈希集合，O(1)
// set<int> st;                  // 有序集合，O(log n)

// --- 优先队列 ---
// priority_queue<int> pq;                          // 大顶堆 (默认)
// priority_queue<int, vi, greater<int>> min_pq;     // 小顶堆
// priority_queue<pii> pq;                          // 按 first 排序的大顶堆

// --- string 常用 ---
// string s = to_string(123);      // 数字转字符串
// int x = stoi("123");            // 字符串转数字
// ll x = stoll("1234567890123");  // 字符串转 long long
// auto pos = s.find("ab");        // 查找子串，返回位置或 string::npos
// s.substr(pos, len);             // 截取子串
// reverse(all(s));                // 反转

// ============ 输入输出优化 ============
void fast_io() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // 注意：使用 fast_io 后不要混用 scanf/printf 和 cin/cout
    // 如果题目输入量特别大（>10^6 个数），可以加：
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============ 核心：每题的 solve 函数 ============
void solve() {
    // 在此编写每道题的解法
    // 多组数据时：

    // int t;
    // cin >> t;
    // while (t--) {
    //     // 每组数据的逻辑
    // }
}

// ============ 主函数 ============
int main() {
    fast_io();
    // 如果只有一组数据，直接用 solve();
    // 多组数据在 solve 内部处理

    int t = 1;
    // cin >> t;  // 取消注释以支持多组数据
    while (t--) solve();

    return 0;
}
