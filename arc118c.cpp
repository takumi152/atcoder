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

  int n;
  cin >> n;

  if (n == 3) {
    cout << "6 10 15" << endl;
    return 0;
  }

  vector<int> ans = {30, 42, 70, 105};
  unordered_set<int> used_number;
  used_number.insert(30);
  used_number.insert(42);
  used_number.insert(70);
  used_number.insert(105);

  for (int i = 6; i <= 10000; i += 6) {
    if ((int) ans.size() == n) break;
    if (used_number.find(i) == used_number.end()) {
      ans.push_back(i);
      used_number.insert(i);
    }
  }

  for (int i = 10; i <= 10000; i += 10) {
    if ((int) ans.size() == n) break;
    if (used_number.find(i) == used_number.end()) {
      ans.push_back(i);
      used_number.insert(i);
    }
  }

  for (int i = 14; i <= 10000; i += 14) {
    if ((int) ans.size() == n) break;
    if (used_number.find(i) == used_number.end()) {
      ans.push_back(i);
      used_number.insert(i);
    }
  }

  for (auto &x: ans) cout << x << " ";
  cout << endl;

  return 0;
}
