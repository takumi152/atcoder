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

  int n;
  cin >> n;
  vector<int> p(n);
  for (auto &x: p) cin >> x;

  if (n == 2) {
    if (p[0] == 0) {
      cout << 0 << endl;
      return 0;
    }
    else {
      cout << 1 << endl;
      cout << 0 << endl;
      return 0;
    }
  }
  if (n == 3) {
    vector<int> ans;
    int now = 0;
    while (p[now] != 1) now++;
    while (!(p[0] == 0 && p[1] == 1 && p[2] == 2)) {
      swap(p[now], p[(now+1)%n]);
      ans.push_back(now);
      now = (now+1)%n;
    }
    cout << ans.size() << endl;
    for (auto &x: ans) cout << x << endl;
    return 0;
  }

  vector<int> ans;
  int now = 0;
  // move 1 before 2
  while (p[now] != 1) now++;
  while (p[(now+1)%n] != 2) {
    swap(p[now], p[(now+1)%n]);
    ans.push_back(now);
    now = (now+1)%n;
  }

  // 1 2 x ...
  int no_swap_count = 0;
  while (true) {
    if (p[(now+2)%n] > p[(now+3)%n] && p[(now+3)%n] != 0) {
      swap(p[(now+1)%n], p[(now+3)%n]);
      swap(p[(now+0)%n], p[(now+1)%n]);
      swap(p[(now+1)%n], p[(now+2)%n]);
      ans.push_back((now+1)%n);
      ans.push_back((now+0)%n);
      ans.push_back((now+1)%n);
      now = (now+2)%n;
      no_swap_count = 0;
    }
    else {
      swap(p[(now+0)%n], p[(now+1)%n]);
      swap(p[(now+0)%n], p[(now+2)%n]);
      ans.push_back((now+0)%n);
      ans.push_back((now+0)%n);
      now = (now+1)%n;
      no_swap_count++;
    }
    if (no_swap_count == n) break;
  }

  // align
  while (!p[(now+2)%n] == 0) {
    swap(p[(now+0)%n], p[(now+1)%n]);
    swap(p[(now+0)%n], p[(now+2)%n]);
    ans.push_back((now+0)%n);
    ans.push_back((now+0)%n);
    now = (now+1)%n;
  }
  swap(p[(now+0)%n], p[(now+1)%n]);
  swap(p[(now+0)%n], p[(now+2)%n]);
  ans.push_back((now+0)%n);
  ans.push_back((now+0)%n);
  now = (now+1)%n;
  while (!(p[0] == 0 && p[1] == 1)) {
    swap(p[(now+0)%n], p[(now+1)%n]);
    ans.push_back((now+0)%n);
    now = (now+1)%n;
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x << endl;

  return 0;
}
