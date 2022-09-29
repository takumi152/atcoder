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

const ll mod = 998244353;

ll modinv(ll x, ll m = mod) {
  ll b = m;
  ll u = 1;
  ll v = 0;
  ll tmp;
  while (b) {
    ll t = x / b;
    x -= t * b;
    tmp = x;
    x = b;
    b = tmp;
    u -= t * v;
    tmp = u;
    u = v;
    v = tmp;
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll p, a, b;
  cin >> p >> a >> b;

  if (p == 2) {
    cout << "Yes" << endl;
    cout << "1 1" << endl;
    return 0;
  }

  if (a == 1 && b == 1) {
    cout << "No" << endl;
    return 0;
  }

  ll a_inv = modinv(a, p);
  ll b_inv = modinv(b, p);

  ll a_cycle = 1;
  {
    ll x = a;
    while (x != 1) {
      a_cycle++;
      x = (x * a) % p;
    }
  }

  ll b_cycle = 1;
  {
    ll x = b;
    while (x != 1) {
      b_cycle++;
      x = (x * b) % p;
    }
  }

  cerr << a_cycle << " " << b_cycle << endl;

  if (a_cycle == p-1 || b_cycle == p-1) {
    if (a_cycle != p-1) {
      swap(a, b);
      swap(a_cycle, b_cycle);
    }
    vector<int> ans(p);
    ans[0] = 1;
    for (int i = 1; i < p; i++) ans[i] = (ans[i-1] * a) % p;
    cout << "Yes" << endl;
    for (auto &x: ans) cout << x << " ";
    cout << endl;
    return 0;
  }

  if (a == 1 || b == 1) {
    cout << "No" << endl;
    return 0;
  }

  ll a_cycle_number = (p-1) / a_cycle;
  ll b_cycle_number = (p-1) / b_cycle;

  cerr << a_cycle_number << " " << b_cycle_number << endl;

  {
    bool good = true;
    vector<int> ans(p);
    vector<bool> visited(p);
    ans[0] = 1;
    int next = 1;
    for (int i = 0; i < a_cycle; i++) {
      for (int j = 0; j < a_cycle_number-1; j++) {
        if (i % 2 == 0) {
          ans[next] = (ans[next-1] * b) % p;
          if (visited[ans[next]]) good = false;
          visited[ans[next]] = true;
        }
        else {
          ans[next] = (ans[next-1] * b_inv) % p;
          if (visited[ans[next]]) good = false;
          visited[ans[next]] = true;
        }
        next++;
      }
      ans[next] = (ans[next-1] * a) % p;
      if (visited[ans[next]]) good = false;
      visited[ans[next]] = true;
      next++;
    }

    if (good) {
      cout << "Yes" << endl;
      for (auto &x: ans) cout << x << " ";
      cout << endl;
      return 0;
    }
  }

  swap(a, b);
  swap(a_inv, b_inv);
  swap(a_cycle, b_cycle);
  swap(a_cycle_number, b_cycle_number);

  {
    bool good = true;
    vector<int> ans(p);
    vector<bool> visited(p);
    ans[0] = 1;
    int next = 1;
    for (int i = 0; i < a_cycle; i++) {
      for (int j = 0; j < a_cycle_number-1; j++) {
        if (i % 2 == 0) {
          ans[next] = (ans[next-1] * b) % p;
          if (visited[ans[next]]) good = false;
          visited[ans[next]] = true;
        }
        else {
          ans[next] = (ans[next-1] * b_inv) % p;
          if (visited[ans[next]]) good = false;
          visited[ans[next]] = true;
        }
        next++;
      }
      ans[next] = (ans[next-1] * a) % p;
      if (visited[ans[next]]) good = false;
      visited[ans[next]] = true;
      next++;
    }

    if (good) {
      cout << "Yes" << endl;
      for (auto &x: ans) cout << x << " ";
      cout << endl;
      return 0;
    }
  }

  cout << "No" << endl;

  return 0;
}
