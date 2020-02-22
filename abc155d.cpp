// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, k;
  cin >> n >> k;
  vector<ll> a(n);
  for (auto &x: a) cin >> x;

  sort(a.begin(), a.end(), [](auto &a, auto &b){return abs(a) < abs(b)});

  ll zero = 0;
  ll positive = 0;
  ll negative = 0;

  ll p_zero = 0;
  ll p_positive = 0;
  ll p_negative = 0;
  for (int i = 0; i < n; i++) {
    if (a[i] == 0) {
      p_zero += zero + positive + negative;
      zero++;
    }
    else if (a[i] > 0) {
      p_zero += zero;
      p_positive += positive;
      p_negative += negative;
      positive++;
    }
    else {
      p_zero += zero;
      p_positive += negative;
      p_negative += positive;
      negative++;
    }
  }

  if (negative < k && k <= negative + zero) {
    cout << 0 << endl;
    return 0;
  }

  if (k <= negative) {
    ll target = negative - k + 1;

    zero = 0;
    positive = 0;
    negative = 0;

    p_zero = 0;
    p_positive = 0;
    p_negative = 0;

    for (int i = 0; i < n; i++) {
      if (a[i] == 0) {
        p_zero += zero + positive + negative;
        zero++;
      }
      else if (a[i] > 0) {
        p_zero += zero;
        p_positive += positive;
        p_negative += negative;
        positive++;
      }
      else {
        p_zero += zero;
        p_positive += negative;
        p_negative += positive;
        negative++;
      }

      if (negative >= target) {

      }
    }
  }

  return 0;
}
