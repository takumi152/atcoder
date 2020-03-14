// WA

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

bool checkmatch(string &a, string &b, int x) {
  for (int i = x; i < (int) min(a.size(), b.size() + x); i++) {
    if (a[i] == '?' || b[i-x] == '?') continue;
    if (a[i] != b[i-x]) return false;
  }
  return true;
}

bool checkmatch2(string &a, string &b, string &c, int x, int y) {
  int p = x + y;
  int q = y;
  int r = 0;
  while ((p < (int) a.size() || q < (int) b.size()) && r < (int) c.size()) {
    char t = '?';
    if (p < (int) a.size()) {
      if (a[p] != '?') t = a[p];
    }
    if (q < (int) b.size()) {
      if (b[q] != '?') t = b[q];
    }
    if (t == '?') {
      p++;
      q++;
      r++;
      continue;
    }
    if (c[r] == '?') {
      p++;
      q++;
      r++;
      continue;
    }
    if (c[r] != t) return false;
    p++;
    q++;
    r++;
  }
  return true;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string a, b, c;
  cin >> a;
  cin >> b;
  cin >> c;

  int x = a.size();
  int y = b.size();
  int z = c.size();

  vector<int> ab, ac, ba, bc, ca, cb;
  for (int i = 0; i < x; i++) {
    if (checkmatch(a, b, i)) ab.push_back(i);
    if (checkmatch(a, c, i)) ac.push_back(i);
  }
  ab.push_back(x);
  ac.push_back(x);
  for (int i = 0; i < y; i++) {
    if (checkmatch(b, a, i)) ba.push_back(i);
    if (checkmatch(b, c, i)) bc.push_back(i);
  }
  ba.push_back(y);
  bc.push_back(y);
  for (int i = 0; i < z; i++) {
    if (checkmatch(c, a, i)) ca.push_back(i);
    if (checkmatch(c, b, i)) cb.push_back(i);
  }
  ca.push_back(z);
  cb.push_back(z);

  int ans = x + y + z;
  for (auto &p: ab) {
    if (max(x, y + p) >= ans) break;
    for (auto &q: bc) {
      //cerr << "abc " << p << " " << q << " " << ans << endl;
      if (max(max(x, y + p), z + p + q) >= ans) break;
      if (checkmatch2(a, b, c, p, q)) ans = max(max(x, y + p), z + p + q);
    }
  }
  for (auto &p: ac) {
    if (max(x, z + p) >= ans) break;
    for (auto &q: cb) {
      //cerr << "acb " << p << " " << q << " " << ans << endl;
      if (max(max(x, z + p), y + p + q) >= ans) break;
      if (checkmatch2(a, c, b, p, q)) ans = max(max(x, z + p), y + p + q);
    }
  }
  for (auto &p: ba) {
    if (max(y, x + p) >= ans) break;
    for (auto &q: ac) {
      //cerr << "bac " << p << " " << q << " " << ans << endl;
      if (max(max(y, x + p), z + p + q) >= ans) break;
      if (checkmatch2(b, a, c, p, q)) ans = max(max(y, x + p), z + p + q);
    }
  }
  for (auto &p: bc) {
    if (max(y, z + p) >= ans) break;
    for (auto &q: ca) {
      //cerr << "bca " << p << " " << q << " " << ans << endl;
      if (max(max(y, z + p), x + p + q) >= ans) break;
      if (checkmatch2(b, c, a, p, q)) ans = max(max(y, z + p), x + p + q);
    }
  }
  for (auto &p: ca) {
    if (max(z, x + p) >= ans) break;
    for (auto &q: ab) {
      //cerr << "cab " << p << " " << q << " " << ans << endl;
      if (max(max(z, x + p), y + p + q) >= ans) break;
      if (checkmatch2(c, a, b, p, q)) ans = max(max(z, x + p), y + p + q);
    }
  }
  for (auto &p: cb) {
    if (max(z, y + p) >= ans) break;
    for (auto &q: ba) {
      //cerr << "cba " << p << " " << q << " " << ans << endl;
      if (max(max(z, y + p), x + p + q) >= ans) break;
      if (checkmatch2(c, b, a, p, q)) ans = max(max(z, y + p), x + p + q);
    }
  }

  cout << ans << endl;

  return 0;
}
