#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <cassert>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct segtree {
  int n;
  vector<int> data;

  segtree(const int s) {
    init(s);
  }

  segtree(const int s, const int u) {
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = u;
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = 0;
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<int>(2*n-1);
  }

  void update(int x, int v) {
    int p = x + n - 1;

    // update bottom
    data[p] = v;

    // update rest
    while (p > 0) {
      p = (p - 1) >> 1;
      data[p] = min(data[p*2+1], data[p*2+2]);
    }
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x7fffffff; // out of range

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return min(vl, vr);
  }
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) {
    cin >> x;
    x--; // zero index
  }
  int q;
  cin >> q;
  vector<Pii> xy(q);
  for (auto &x: xy) {
    cin >> x.first >> x.second;
    x.first--; // zero index
    x.second--; // zero index
  }

  vector<set<int> > p(n);
  for (int i = 0; i < n; i++) {
    p[a[i]].insert(i);
  }

  segtree st(n * n, 1000000007);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        st.update(i*n + j, n - min(1, (int)p[i].size()));
        continue;
      }
      auto it1 = p[i].begin();
      auto it2 = p[j].begin();
      int rem = 0;
      int next = -1;
      while (true) {
        if (it1 == p[i].end()) break;
        while ((*it1) <= next) {
          it1++;
          if (it1 == p[i].end()) break;
        }
        if (it1 == p[i].end()) break;
        rem++;
        next = (*it1);
        if (it2 == p[j].end()) break;
        while ((*it2) <= next) {
          it2++;
          if (it2 == p[j].end()) break;
        }
        if (it2 == p[j].end()) break;
        rem++;
        next = (*it2);
      }
      st.update(i*n + j, n - rem);
    }
  }

  vector<int> ans(q+1);
  ans[0] = st.query(0, n * n);

  for (int k = 0; k < q; k++) {
    int point = xy[k].first;
    int to = xy[k].second;
    int before = a[point];
    int after = to;
    auto it = p[before].find(point);
    p[before].erase(it);
    p[after].insert(point);
    a[point] = after;

    for (int i = before; i < before+1; i++) {
      for (int j = 0; j < n; j++) {
        if (i == j) {
          st.update(i*n + j, n - min(1, (int)p[i].size()));
          continue;
        }
        auto it1 = p[i].begin();
        auto it2 = p[j].begin();
        int rem = 0;
        int next = -1;
        while (true) {
          if (it1 == p[i].end()) break;
          while ((*it1) <= next) {
            it1++;
            if (it1 == p[i].end()) break;
          }
          if (it1 == p[i].end()) break;
          rem++;
          next = (*it1);
          if (it2 == p[j].end()) break;
          while ((*it2) <= next) {
            it2++;
            if (it2 == p[j].end()) break;
          }
          if (it2 == p[j].end()) break;
          rem++;
          next = (*it2);
        }
        st.update(i*n + j, n - rem);
      }
    }
    for (int i = 0; i < n; i++) {
      for (int j = before; j < before+1; j++) {
        if (i == j) {
          st.update(i*n + j, n - min(1, (int)p[i].size()));
          continue;
        }
        auto it1 = p[i].begin();
        auto it2 = p[j].begin();
        int rem = 0;
        int next = -1;
        while (true) {
          if (it1 == p[i].end()) break;
          while ((*it1) <= next) {
            it1++;
            if (it1 == p[i].end()) break;
          }
          if (it1 == p[i].end()) break;
          rem++;
          next = (*it1);
          if (it2 == p[j].end()) break;
          while ((*it2) <= next) {
            it2++;
            if (it2 == p[j].end()) break;
          }
          if (it2 == p[j].end()) break;
          rem++;
          next = (*it2);
        }
        st.update(i*n + j, n - rem);
      }
    }

    for (int i = after; i < after+1; i++) {
      for (int j = 0; j < n; j++) {
        if (i == j) {
          st.update(i*n + j, n - min(1, (int)p[i].size()));
          continue;
        }
        auto it1 = p[i].begin();
        auto it2 = p[j].begin();
        int rem = 0;
        int next = -1;
        while (true) {
          if (it1 == p[i].end()) break;
          while ((*it1) <= next) {
            it1++;
            if (it1 == p[i].end()) break;
          }
          if (it1 == p[i].end()) break;
          rem++;
          next = (*it1);
          if (it2 == p[j].end()) break;
          while ((*it2) <= next) {
            it2++;
            if (it2 == p[j].end()) break;
          }
          if (it2 == p[j].end()) break;
          rem++;
          next = (*it2);
        }
        st.update(i*n + j, n - rem);
      }
    }
    for (int i = 0; i < n; i++) {
      for (int j = after; j < after+1; j++) {
        if (i == j) {
          st.update(i*n + j, n - min(1, (int)p[i].size()));
          continue;
        }
        auto it1 = p[i].begin();
        auto it2 = p[j].begin();
        int rem = 0;
        int next = -1;
        while (true) {
          if (it1 == p[i].end()) break;
          while ((*it1) <= next) {
            it1++;
            if (it1 == p[i].end()) break;
          }
          if (it1 == p[i].end()) break;
          rem++;
          next = (*it1);
          if (it2 == p[j].end()) break;
          while ((*it2) <= next) {
            it2++;
            if (it2 == p[j].end()) break;
          }
          if (it2 == p[j].end()) break;
          rem++;
          next = (*it2);
        }
        st.update(i*n + j, n - rem);
      }
    }

    ans[k+1] = st.query(0, n * n);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
