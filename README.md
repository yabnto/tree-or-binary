## ST 表
本来不想写的，但是我考试因为 ST表写错，痛失 $100$ 分，想想还是写吧
### 简介
原型是倍增，不过它是用来求区间最值（其实也可以求和），而且是静态的（不如线段树），区间最值也可以写成：$RMQ$ 问题，ST表可以让查询最值达到 $O(logn)$，算是很高效了。
### 思路
将区间dp的 $dp[i][j]$ 变成 $f[i][j - i + 1]$ 然后
$j - i + 1$，在特定的情况下可以表示为 $2^k$，而 $k$ 的范围是 $log2(n)$ 所以可以枚举 $k$。然后两个小区间可以接起来成为一个大区间，这是区间dp的思想，利用这一点我们可以得到：$f[i][j] = max/min(f[i][j - 1], f[i + (1 << (j - 1))][j - 1])$ 预处理就写完了。但是由于我们预处理的其实是一个特定的情况，也就是 $2^k$，所以我们可以将你要求的区间长度转换成二进制，然后逐个处理，也就是说对于长度的二进制的第 $i$ 位是 $1$，那么答案就涵盖 $f[l][i]$，然后逐个求解。
### 代码
#### 结构体
```cpp
#include <cmath>
#include <iostream>

using namespace std;

const int MaxN = 5e4 + 10, MaxK = 17;

struct ST {
  int f[MaxN][MaxK];  // 倍增数组
  bool op;            // 是求大值(1)，还是小值(0)

  int cmp(int x, int y) {  // 比较函数
    return op ? max(x, y) : min(x, y);
  }

  void init(int a[], int n, bool how) {  // 初始化
    op = how;
    for (int i = 1; i <= n; i++) {
      f[i][0] = a[i];
    }
    for (int j = 1; j < MaxK; j++) {
      for (int i = 1; i + (1 << j) - 1 <= n; i++) {
        f[i][j] = cmp(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  int find(int l, int g) {  // 求答案
    int ans = op ? -1e9 : 1e9;
    for (int i = 0; i < MaxK; i++) {
      if (g >> i & 1) {
        ans = cmp(ans, f[l][i]);
        l += (1 << i);
      }
    }
    return ans;
  }
} l;

int main() {
  return 0;
}
```
## 树状数组
一个二进制且树的东东
### 单点
区间修改，是一个区间，所以我们选择差分，可是我们要都次频繁的查询，所以我们考虑类似线段树的方法通过统计已经统计过的区间来算，于是我们将一个数组依照二进制的第一位以及它的原始编号分好类，即类似于
![](https://img2024.cnblogs.com/blog/2903414/202403/2903414-20240320223553187-956986045.png)

的东西，然后插入时就一点一点往上更新，那查找的时候由于是一个一个区间往下查的，必然会成一个前缀，所以我们运用前缀和将前面的剪掉即可。
#### code
```cpp
#include <iostream>

using namespace std;

const int MaxN = 5e5 + 10;

long long d[MaxN], n, q;

int lb(int x) {
  return x & (-x);
}

void in(int id, int x) {
  for (int i = id; i <= n; d[i] += x, i += lb(i)) {
  }
}

long long Sum(int x) {
  long long res = 0;
  for (int i = x; i; res += d[i], i -= lb(i)) {
  }
  return res;
}

int main() {
  cin >> n >> q;
  for (int i = 1, x; i <= n; i++) {
    cin >> x, in(i, x);
  }
  for (int op, l, r; q; q--) {
    cin >> op >> l >> r;
    if (op == 1) {
      in(l, r);
    } else {
      cout << Sum(r) - Sum(l - 1) << '\n';
    }
  }
  return 0;
}
```
### 区间
推个柿子，我们如果要求区间查询：

插入还是一样的差分

查询将会变成（如果是区间 $[l,r]$ 的话，其实最终会用前缀和的方法变成一样）：

$d_1+(d_1+d_2)+(d_1+d_2+d_3)+\cdots$

$d_1+d_1+\cdots+d_2+d_2+\cdots$

$nd_1+(n-1)d_2+\cdots$

$n(d_1+\cdots)-0d_1-1d_2-\cdots$

此时就变成了一个熟悉的前缀和，所以在维护一个树状数组，而对于 $i$ 的数加入按照上式要乘一个 $i$ 那对于区间 $[l, r]$，就得用个前缀和（好像没必要推吧，常识问题？），然后区间 $[l,r]$ 的问题就解决了（还没懂我也没办法）。
#### code
```cpp
#include <iostream>

using namespace std;
using ll = long long;

const int MaxN = 1e5 + 10;

ll g[MaxN], d[MaxN], n, q;

int lb(int x) {
  return x & (-x);
}

void G(int l, int r, ll x) {
  ll lx = (n - (n - (l - 1))) * x, rx = (n - (n - r)) * x;
  for (l; l <= n; l += lb(l)) {
    d[l] += x, g[l] += lx;
  }
  for (r++; r <= n; r += lb(r)) {
    d[r] -= x, g[r] -= rx;
  }
}

ll A(int x) {
  ll res = 0;
  for (int i = x; i; i -= lb(i)) {
    res += d[i];
  }
  res *= x;
  for (int i = x; i; i -= lb(i)) {
    res -= g[i];
  }
  return res;
} 

int main() {
  cin >> n >> q;
  for (int i = 1, x; i <= n; i++) {
    cin >> x;
    G(i, i, x);
  }
  for (int op, x, y, k; q; q--) {
    cin >> op >> x >> y;
    if (op == 1) {
      cin >> k;
      G(x, y, k);
    } else {
      cout << A(y) - A(x - 1) << endl;
    }
  }
  return 0;
}
```
