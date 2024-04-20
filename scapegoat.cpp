#include <algorithm>
#include <iostream>

using namespace std;

const int MaxN = 4e5 + 10;
const double eps = 0.75;

int d[MaxN], l[MaxN], r[MaxN], cnt[MaxN], sum[MaxN], sz[MaxN], a[MaxN], tot, n, root, len;

void update(int x) {  // 更新
  sum[x] = sum[l[x]] + sum[r[x]] + cnt[x];
  sz[x] = sz[l[x]] + sz[r[x]] + 1;
}

bool Check(int x) {  // 判断是否需要重构
  return cnt[x] && 1.0 * max(sz[l[x]], sz[r[x]]) > eps * sz[x];
}

void dfs(int x) {  // 中序遍历
  if (!x) {
    return;
  }
  dfs(l[x]);
  cnt[x] && (a[++len] = x);
  dfs(r[x]);
}

int build(int pl, int pr) {  // 二分重构
  if (pl > pr) {
    return 0;
  }
  int mid = (pl + pr) >> 1;
  l[a[mid]] = build(pl, mid - 1);
  r[a[mid]] = build(mid + 1, pr);
  update(a[mid]);
  return a[mid];
}

int G(int k) {
  len = 0;
  dfs(k);
  return k = build(1, len);
}

void insert(int &k, int x) {  // 添加
  if (!k) {
    k = ++tot;
    (!root) && (root = tot);
    d[tot] = x, l[tot] = r[tot] = 0, sum[tot] = cnt[tot] = sz[tot] = 1;
    return;
  }
  if (d[k] == x) {
    cnt[k]++;
  } else if (d[k] < x) {
    insert(r[k], x);
  } else {
    insert(l[k], x);
  }
  update(k);
  if (Check(k)) {
    k = G(k);
  }
}

void delet(int &k, int x) {  // 删除
  if (!k) {
    return;
  }
  if (d[k] == x) {
    cnt[k] && (cnt[k]--);
  } else if (d[k] < x) {
    delet(r[k], x);
  } else {
    delet(l[k], x);
  }
  update(k);
  if (Check(k)) {
    k = G(k);
  }
}

int At(int k, int x) {  // @
  if (!k) {
    return 0;
  }
  if (sum[l[k]] < x && x <= sum[l[k]] + cnt[k]) {
    return d[k];
  } else if (sum[l[k]] + cnt[k] < x) {
    return At(r[k], x - sum[l[k]] - cnt[k]);
  }
  return At(l[k], x);
}

int upbd(int k, int x) {  // 大于
  if (!k) {
    return 1;
  }
  if (d[k] == x && cnt[k]) {
    return sum[l[k]] + cnt[k] + 1;
  } else if (x < d[k]) {
    return upbd(l[k], x);
  }
  return upbd(r[k], x) + sum[l[k]] + cnt[k];
}

int upgrt(int k, int x) {  // 小于
  if (!k) {
    return 0;
  }
  if (d[k] == x && cnt[k]) {
    return sum[l[k]];
  } else if (d[k] < x) {
    return upgrt(r[k], x) + sum[l[k]] + cnt[k];
  }
  return upgrt(l[k], x);
}

int p(int x) {  // 前驱
  return At(root, upgrt(root, x));
}

int h(int x) {  // 后继
  return At(root, upbd(root, x));
}

int main() {
  cin >> n;
  for (int op, x; n; n--) {
    cin >> op >> x;
    if (op == 1) {
      insert(root, x);
    } else if (op == 2) {
      delet(root, x);
    } else if (op == 3) {
      cout << upgrt(root, x) + 1 << '\n';
    } else if (op == 4) {
      cout << At(root, x) << '\n';
    } else if (op == 5) {
      cout << p(x) << '\n';
    } else {
      cout << h(x) << '\n';
    }
  }
  return 0;
}
