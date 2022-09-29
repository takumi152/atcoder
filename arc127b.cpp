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

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, l;
  cin >> n >> l;

  vector<int> p3(16);
  p3[0] = 1;
  for (int i = 1; i < 16; i++) p3[i] = p3[i-1] * 3;

  vector<string> ans;
  for (int i = 0; i < n; i++) {
    string s0 = "0";
    string s1 = "1";
    string s2 = "2";
    int rem = i;
    for (int j = l-2; j >= 0; j--) {
      if (rem / p3[j] == 0) {
        s0.push_back('2');
        s1.push_back('1');
        s2.push_back('0');
      }
      else if (rem / p3[j] == 1) {
        s0.push_back('0');
        s1.push_back('2');
        s2.push_back('1');
      }
      else {
        s0.push_back('1');
        s1.push_back('0');
        s2.push_back('2');
      }
      rem %= p3[j];
    }
    ans.push_back(s0);
    ans.push_back(s1);
    ans.push_back(s2);
  }

  sort(ans.begin(), ans.end());

  for (auto &x: ans) cout << x << endl;

  return 0;
}
