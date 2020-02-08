#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

vector<ll> erathosthenes(ll n) {
  vector<bool> mask(n+1, true);
  mask[0] = false;
  mask[1] = false;
  for (int i = 2; i * i <= n; i++) {
    for (int j = i * i; j <= n; j += i) {
      mask[j] = false;
    }
  }
  vector<ll> prime = {0};
  for (int i = 0; i <= n; i++) {
    if (mask[i]) prime.push_back(i);
  }
  return prime;
}

void output(ll x, ll sx, ll ax, ll dx, ll y, ll sy, ll ay, ll dy, ll ta, ll wa, ll wb, ll wc, ll we) {
  cout << x << "," << sx << "," << ax << "," << dx << "," << y << "," << sy << "," << ay << "," << dy << "," << ta << "," << wa << "," << wb << "," << wc << "," << we << endl;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll la, lb, lc, na, nb, nc, ne, ma, mb, mc, tc;
  cin >> la >> lb >> lc >> na >> nb >> nc >> ne >> ma >> mb >> mc >> tc;

  auto prime = erathosthenes(1000000);

  ll filla = 500, fillb = 2000, fillc = 3000;

  ll atime = 0;
  ll filled = 0;
  for (int k = 0; k < 10; k++) {
    // move to a
    filla = 500;
    for (int i = 500; i < 6000; i++) {
      if (atime + i >= prime.size()) return 0;
      ll bt = prime[atime + i] % 180;
      if (bt > 90) bt = 180 - bt;
      if (bt >= 80) {
        filla = i;
        break;
      }
    }
    output(5, 500, 60, 60, 900 - la + 5, 500, 60, 60, 90, filla, 0, 0, 0);
    filled += filla * na;
    atime += filla;
    // move to c
    ll cx = 900 - cos((double) tc * (3.1415926535 / 180.0)) * lc - 5;
    ll cy = 900 - sin((double) tc * (3.1415926535 / 180.0)) * lc + 5;
    output(cx, 500, 300, 300, cy, 500, 300, 300, 90, 0, 0, fillc, 0);
    filled += fillc * nc;
    // move to drain
    ll draintime = filled / ne + 20;
    output(90, 500, 1800, 1800, 50, 500, 1800, 1800, 90, 0, 0, 0, draintime);
    filled = 0;
    // move to b
    if (atime >= prime.size()) break;
    ll bangle = prime[atime] % 180;
    if (bangle > 90) bangle = 180 - bangle;
    ll bx = 900 - sin((double) bangle * (3.1415926535 / 180.0)) * lb - 5;
    ll by = cos((double) bangle * (3.1415926535 / 180.0)) * lb + 5;
    output(bx, 500, 60, 60, by, 500, 60, 60, 90, 0, fillb, 0, 0);
    filled += fillb * nb;
    // move to drain
    draintime = filled / ne + 20;
    output(90, 500, 1800, 1800, 50, 500, 1800, 1800, 90, 0, 0, 0, draintime);
    filled = 0;
  }

  return 0;
}
