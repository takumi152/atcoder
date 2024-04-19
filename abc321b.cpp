#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, x;
  cin >> n >> x;
  vector<int> a(n-1);
  for (auto &x: a) cin >> x;

  int ans = -1;
  for (int i = 0; i <= 100; i++) {
    auto a_final = a;
    a_final.push_back(i);
    sort(a_final.begin(), a_final.end());
    int score_total = 0;
    for (int j = 1; j < (int) a_final.size() - 1; j++) {
      score_total += a_final[j];
    }
    if (score_total >= x) {
      ans = i;
      break;
    }
  }

  cout << ans << endl;

  return 0;
}
