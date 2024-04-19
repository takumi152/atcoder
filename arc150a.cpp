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

  int t;
  cin >> t;
  vector<int> n(t), k(t);
  vector<vector<char>> s(t);
  for (int i = 0; i < t; i++) {
    cin >> n[i] >> k[i];
    s[i] = vector<char>(n[i]);
    for (auto &x: s[i]) cin >> x;
  }

  vector<string> ans;
  for (int i = 0; i < t; i++) {
    int total_one = 0;
    for (int j = 0; j < n[i]; j++) {
      if (s[i][j] == '1') total_one++;
    }
    
    int one_count = 0;
    int question_count = 0;
    for (int j = 0; j < k[i]; j++) {
      if (s[i][j] == '1') one_count++;
      if (s[i][j] == '?') question_count++;
    }

    int valid_pattern = 0;
    int left = 0;
    int right = k[i];
    while (right <= n[i]) {
      if (one_count == total_one && one_count + question_count == k[i]) valid_pattern++;
      if (right == n[i]) break;
      if (s[i][left] == '1') one_count--;
      if (s[i][left] == '?') question_count--;
      if (s[i][right] == '1') one_count++;
      if (s[i][right] == '?') question_count++;
      left++;
      right++;
    }

    if (valid_pattern == 1) ans.push_back("Yes");
    else ans.push_back("No");
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
