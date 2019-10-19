// WIP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <queue>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  sort(a.begin(), a.end());

  vector<int> ans(n);
  vector<int> hist;
  int now = 0;
  for (int i = 0; i < n; i++) {
    if (a[i] > now) {
      now = a[i];
      hist.push_back(1);
    }
    else hist[hist.size()-1]++;
  }

  sort(hist.rbegin(), hist.rend());

  vector<int> histSum;
  histSum.push_back(hist[0]);
  for (int i = 1; i < hist.size(); i++) histSum.push_back(histSum[histSum.size()-1] + hist[i]);

  return 0;
}
