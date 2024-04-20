#include <ctime>
#include <iostream>

using namespace std;

const int MaxN = 1e5 + 10;

struct S {
  int w, v, l, r, sum;
} a[MaxN];

int n, root, tot;

void update(int k) {
  a[k].sum = k ? a[a[k].l].sum + a[a[k].r].sum + 1 : 0;
}

void split(int k, int v, int &x, int &y) {
  if (!k) {
    return;
  }
  if (a[k].w <= v) {
    x = k;
    int r = a[k].r;
    split(r, v, a[k].r = 0, y);
  } else {
    y = k;
    int l = a[k].l;
    split(l, v, x, a[k].l = 0);
  }
  update(k);
}

int merge(int x, int y) {
  if (!x || !y) {
    return x + y;
  }
  if (a[x].v <= a[y].v) {
    a[x].r = merge(a[x].r, y), update(x);
    return x;
  } else {
    a[y].l = merge(x, a[y].l), update(y);
    return y;
  }
}

void insert(int &k, int val) {
  int x = 0, y = 0;
  a[++tot] = {val, rand(), 0, 0, 1};
  split(k, val, x, y);
  k = merge(merge(x, tot), y);
}

void delet(int &k, int x) {
  if (a[k].w == x) {
    k = merge(a[k].l, a[k].r);
  } else {
    a[k].w <= x ? delet(a[k].r, x) : delet(a[k].l, x);
    update(k);
  } 
}

int upgrt(int k, int x) {
  if (!k) {
    return 0;
  }
  if (a[k].w >= x) {
    return upgrt(a[k].l, x);
  }
  return upgrt(a[k].r, x) + a[a[k].l].sum + 1;
}

int At(int k, int x) {
  if (!k) { 
    return 0;
  }
  if (a[a[k].l].sum < x && x <= a[a[k].l].sum + 1) {
    return a[k].w;
  } else if (a[a[k].l].sum + 1 < x) {
    return At(a[k].r, x - a[a[k].l].sum - 1);
  }
  return At(a[k].l, x);
}

int main() {
  srand(time(NULL));
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
      cout << At(root, upgrt(root, x)) << '\n';
    } else {
      cout << At(root, upgrt(root, x + 1) + 1)  << '\n';
    }
  }
  return 0;
}
