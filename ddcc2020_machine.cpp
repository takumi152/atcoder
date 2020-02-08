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

  ll amount = 115000;
  if (ne == 1) amount = 57500;

  ll filla = 500, fillb = 500, fillc = 500;
  if (ma > mb && ma > mc) filla = max(500LL, min(ma, (amount - (nb * 500 + nc * 500)) / na));
  else if (mb > mc) fillb = max(500LL, min(mb, (amount - (na * 500 + nc * 500)) / nb));
  else fillc = max(500LL, min(mc, (amount - (na * 500 + nb * 500)) / nc));

  ll atime = 0;
  ll filled = 0;
  for (int k = 0; k < 10; k++) {
    // move to a
    output(0, 500, 60, 60, 900 - la, 500, 60, 60, 90, filla, 0, 0, 0);
    filled += filla * na;
    atime += filla;
    // move to c
    ll cx = 900 - cos((double) tc * (3.1415926535 / 180.0)) * lc;
    ll cy = 900 - sin((double) tc * (3.1415926535 / 180.0)) * lc;
    output(cx, 500, 60, 60, cy, 500, 60, 60, 90, 0, 0, fillc, 0);
    filled += fillc * nc;
    // move to b
    if (atime >= prime.size()) break;
    ll bangle = prime[atime] % 180;
    if (bangle > 90) bangle = 180 - bangle;
    ll bx = 900 - sin((double) bangle * (3.1415926535 / 180.0)) * lb;
    ll by = cos((double) bangle * (3.1415926535 / 180.0)) * lb;
    output(bx, 500, 60, 60, by, 500, 60, 60, 90, 0, fillb, 0, 0);
    filled += fillb * nb;
    // move to drain
    ll draintime = filled / ne + 1;
    output(90, 500, 60, 60, 50, 500, 60, 60, 90, 0, 0, 0, draintime);
    filled = 0;
  }

  return 0;
}
