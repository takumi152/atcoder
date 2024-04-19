#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  ll n, m, k;
  cin >> n >> m >> k;
  string s;
  cin >> s;

  vector<ll> o_length;
  {
    int length_now = 0;
    for (int i = 0; i < n; i++) {
      if (s[i] == 'o') length_now++;
      else {
        o_length.push_back(length_now);
        length_now = 0;
      }
    }
    o_length.push_back(length_now);
  }

  ll x_count = (int) o_length.size() - 1;

  ll begin_cycle = 0;
  ll begin_pos = 0;
  ll end_cycle = (k - 1) / x_count;
  ll end_pos = (k - 1) % x_count;

  ll o_count;
  {
    o_count = n * end_cycle;
    for (int i = 0; i <= end_pos; i++) {
      o_count += o_length[i] + 1;
    }
    o_count += o_length[end_pos + 1];
    if (end_pos == x_count - 1 && end_cycle < m - 1) o_count += o_length[0];
  }

  ll ans = o_count;
  while (begin_cycle <= 1) {
    ans = max(ans, o_count);

    if (end_pos == x_count - 1 && end_cycle == m - 1) break;
    
    o_count -= o_length[begin_pos] + 1;
    if (begin_pos == 0 && begin_cycle > 0) o_count -= o_length[o_length.size()-1];
    begin_pos++;
    if (begin_pos == x_count) {
      begin_pos = 0;
      begin_cycle++;
    }

    end_pos++;
    if (end_pos == x_count) {
      end_pos = 0;
      end_cycle++;
    }
    o_count += o_length[end_pos + 1] + 1;
    if (end_pos == x_count - 1 && end_cycle < m - 1) o_count += o_length[0];
  }

  cout << ans << endl;

  return 0;
}
