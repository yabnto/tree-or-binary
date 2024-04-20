#include <ctime>
#include <iostream>
#include <random>

using namespace std;

const int MaxN = 1e5 + 10;

struct Node {
  int d, v, l, r, sum, cnt;
} w[MaxN];

int n, root, tot;

void update(int x) {
  w[x].sum = w[w[x].l].sum + w[w[x].r].sum + w[x].cnt;
}

void left(int &k) {
  int p = w[k].l;
  w[k].l = w[p].r, w[p].r = k, k = p;
  update(w[k].r), update(k);
}

void right(int &k) {
  int p = w[k].r;
  w[k].r = w[p].l, w[p].l = k, k = p;
  update(w[k].l), update(k);
}

void insert(int &k, int x) {
  if (!k) {
    k = ++tot;
    w[k] = {x, rand(), 0, 0, 1, 1};
    return;
  }
  if (w[k].d == x) {
    w[k].cnt++;
  } else if (w[k].d < x) {
    insert(w[k].r, x);
    if (w[w[k].r].v < w[k].v) {
      right(k);
    }
  } else {
    insert(w[k].l, x);
    if (w[w[k].l].v < w[k].v) {
      left(k);
    }
  }
  update(k);
}

void delet(int &k, int x) {
  if (!k) {
    return;
  }
  if (w[k].d == x) {
    if (w[k].cnt) {
      w[k].cnt--, update(k);
    } else if (w[k].l || w[k].r) {
      if (!w[k].r || w[w[k].l].d > w[w[k].r].d) {
        left(k), delet(w[k].l, x);
      } else {
        right(k), delet(w[k].r, x);
      }
      update(k);
    } else {
      k = 0;
    }
  }
  if (w[k].d < x) {
    delet(w[k].r, x);
  } else {
    delet(w[k].l, x);
  }
  update(k);
}

int upbd(int k, int x) {
  if (!k) {
    return 1;
  }
  if (w[k].d == x) {
    return w[w[k].l].sum + w[k].cnt + 1;
  } else if (w[k].d < x) {
    return upbd(w[k].r, x) + w[w[k].l].sum + w[k].cnt;
  }
  return upbd(w[k].l, x);
}
int upgrt(int k, int x) {
  if (!k) {
    return 0;
  }
  if (w[k].d == x) {
    return w[w[k].l].sum;
  } else if (x < w[k].d) {
    return upgrt(w[k].l, x);
  }
  return upgrt(w[k].r, x) + w[w[k].l].sum + w[k].cnt;
}

int At(int k, int x) {
  if (!k) {
    return 0;
  }
  if (w[w[k].l].sum < x && x <= w[w[k].l].sum + w[k].cnt) {
    return w[k].d;
  } else if (w[w[k].l].sum + w[k].cnt < x) {
    return At(w[k].r, x - w[w[k].l].sum - w[k].cnt);
  }
  return At(w[k].l, x);
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
      cout << At(root, upbd(root, x)) << '\n';
    }
  }
  return 0;
}
