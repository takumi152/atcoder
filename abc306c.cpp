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

  int n;
  cin >> n;
  vector<int> a(n * 3);
  for (auto &x: a) cin >> x;

  vector<int> mid_pos(n);
  {
    vector<int> appear_count(n);
    for (int i = 0; i < n * 3; i++) {
      appear_count[a[i]-1]++;
      if (appear_count[a[i]-1] == 2) mid_pos[a[i]-1] = i;
    }
  }

  vector<int> ans(n);
  for (int i = 0; i < n; i++) ans[i] = i + 1;
  sort(ans.begin(), ans.end(), [&](auto &a, auto &b){return mid_pos[a-1] < mid_pos[b-1];});

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
