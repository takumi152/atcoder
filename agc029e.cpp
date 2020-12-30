#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

struct lazy_segtree {
  int n;
  vector<int> data;
  vector<int> lazy;
  vector<bool> lazyFlag;

  lazy_segtree(const int s) {
    init(s);
  }

  lazy_segtree(const vector<int> &v) {
    int s = v.size();
    init(s);
    for (int i = 0; i < s; i++) {
      data[i+n-1] = v[i];
    }
    for (int i = n-2; i >= 0; i--) {
      data[i] = min(data[i*2+1], data[i*2+2]);
    }
  }

  void init(const int s) {
    n = 1;
    while (n < s) n <<= 1;
    data = vector<int>(2*n-1);
    lazy = vector<int>(2*n-1);
    lazyFlag = vector<bool>(2*n-1);
  }

  void evaluate(int p, int a, int b) {
    if (lazyFlag[p]) {
      data[p] += lazy[p];
      if (b - a > 1) {
        lazy[p*2+1] += lazy[p];
        lazy[p*2+2] += lazy[p];
        lazyFlag[p*2+1] = true;
        lazyFlag[p*2+2] = true;
      }
      lazy[p] = 0;
      lazyFlag[p] = false;
    }
  }

  void update(int l, int r, int v, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init

    // evaluate lazy value
    evaluate(p, a, b);

    if (r <= a || b <= l) return; // out of range
    if (l <= a && b <= r) { // fully covered
      lazy[p] += v;
      lazyFlag[p] = true;
      evaluate(p, a, b);
    }
    else {
      update(l, r, v, p*2+1, a, (a + b) / 2); // left
      update(l, r, v, p*2+2, (a + b) / 2, b); // right
      data[p] = min(data[p*2+1], data[p*2+2]); // update
    }
    return;
  }

  int query(int l, int r, int p = 0, int a = 0, int b = -1) {
    if (b < 0) b = n; // init
    if (r <= a || b <= l) return 0x7fffffff; // out of range

    // evaluate lazy value
    evaluate(p, a, b);

    if (l <= a && b <= r) return data[p];// fully covered

    int vl = query(l, r, p*2+1, a, (a + b) / 2); // left
    int vr = query(l, r, p*2+2, (a + b) / 2, b); // right
    return min(vl, vr);
  }
};

void calc_accessible(int now, int prev, int head, int tail, vector<unordered_map<int, int> > &edge, vector<int> &lim, vector<int> &history, lazy_segtree &lst) {
  history.push_back(now);
  lim.push_back(max(lim[tail], now-1));

  if (tail >= 2) {
    while (head < tail) {
      if (lim[head] < now) head++;
      else break;
    }
    lst.update(head, tail, 1);
  }
  else head++;

  // cerr << now << " " << head << " " << tail << endl;
  // for (auto &x: lim) cerr << x << " ";
  // cerr << endl;
  // for (auto &x: history) cerr << x << " ";
  // cerr << endl;
  // for (int i = 0; i < 8; i++) cerr << lst.query(i, i+1) << " ";
  // cerr << endl;
  // cerr << endl;

  for (auto &next: edge[now]) {
    if (next.first != prev) {
      lst.update(tail, tail+1, -lst.query(tail, tail+1));
      calc_accessible(next.first, now, head, tail+1, edge, lim, history, lst);
    }
  }

  if (tail > 0) edge[prev][now] = lst.query(tail-1, tail);

  lim.pop_back();
  history.pop_back();
}

void calc_ans(int now, int prev, vector<unordered_map<int, int> > &edge, vector<int> &ans) {
  if (now != 0) {
    int total = 1;
    for (auto &next: edge[now]) total += next.second;
    ans[now] = ans[prev] + total - edge[prev][now];
  }
  for (auto &next: edge[now]) {
    if (next.first != prev) calc_ans(next.first, now, edge, ans);
  }
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<unordered_map<int, int> > edge(n);
  for (int i = 0; i < n-1; i++) {
    int a, b;
    cin >> a >> b;
    edge[a-1][b-1] = 0;
    edge[b-1][a-1] = 0;
  }

  vector<int> lim;
  lim.push_back(0);
  vector<int> history;
  lazy_segtree lst(n);
  calc_accessible(0, 0, 0, 0, edge, lim, history, lst);

  vector<int> ans(n);
  calc_ans(0, 0, edge, ans);

  // for (int i = 0; i < n; i++) {
  //   for (auto &x: edge[i]) cerr << i << " " << x.first << " " << x.second << endl;
  // }

  for (int i = 1; i < n; i++) cout << ans[i] << " ";
  cout << endl;

  return 0;
}
