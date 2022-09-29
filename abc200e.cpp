#include <iostream>
#include <iomanip>
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

  if (n == 1) {
    cout << 1 << " " << 1 << " " << 1 << endl;
    return 0;
  }

  vector<ll> num_count(3 * n - 2);
  for (ll i = 0; i < n; i++) {
    num_count[i] = ((i + 1) * (i + 2)) / 2;
  }
  for (ll i = n; i < 2 * n; i++) {
    num_count[i] = ((i + 1) * (i + 2)) / 2 - (((i + 1) - n) * ((i + 2) - n) * 3) / 2;
  }
  for (ll i = 2 * n; i < 3 * n - 2; i++) {
    num_count[i] = num_count[(3 * n - 2) - i - 1];
  }

  ll num_count_sum = 0;
  int ans_total = 0;
  for (int i = 0; i < 3 * n - 2; i++) {
    num_count_sum += num_count[i];
    if (num_count_sum >= k) {
      ans_total = i;
      k -= (num_count_sum - num_count[i]);
      break;
    }
  }

  ll i_sum = 0;
  int ans_i = 0;
  for (int i = 0; i < n; i++) {
    i_sum += max(0LL, n - abs((ans_total - i) - (n - 1)));
    if (i_sum >= k) {
      ans_i = i;
      k -= (i_sum - (max(0LL, n - abs((ans_total - i) - (n - 1)))));
      break;
    }
  }

  int ans_j = 0;
  int ans_k = 0;
  for (int j = 0; j < n; j++) {
    if (ans_total - ans_i - j >= 0 && ans_total - ans_i - j < n) k--;
    if (k == 0) {
      ans_j = j;
      ans_k = ans_total - ans_i - j;
      break;
    }
  }

  cout << ans_i + 1 << " " << ans_j + 1 << " " << ans_k + 1 << endl;

  return 0;
}
