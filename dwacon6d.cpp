// WA

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) {
    cin >> x;
    x--;
  }

  if (n == 2) {
    cout << -1 << endl; // impossible
    return 0;
  }

  list<int> ans;
  vector<bool> used(n);
  ans.push_back(0);
  used[0] = true;
  for (int i = 1; i < n; i++) {
    if (used[i]) continue;
    if (a[ans.back()] == i) {
      for (int j = i+1; j < n; j++) {
        ans.push_back(j);
        used[j] = true;
        if (a[ans.back()] != i) {
          ans.push_back(i);
          used[i] = true;
          break;
        }
      }
    }
    else {
      ans.push_back(i);
      used[i] = true;
    }
  }

  for (int i = 0; i < n; i++) {
    if (!used[i]) {
      for (auto it = ans.rbegin(); it != ans.rend(); it++) {
        if (a[*(it)] == i) continue;
        if (it != ans.rbegin()) {
          if (a[i] == *(it.base())) {
            auto itref = ans.rbegin();
            itref++;
            if (it == itref) continue;
            auto it1 = it;
            it1--;
            auto it2 = it1;
            it2--;
            swap(*(it1), *(it2));
          }
        }
        ans.insert(it.base(), i);
        used[i] = true;
        break;
      }
      if (!used[i]) {
        auto it1 = ans.begin();
        if (a[i] == *(it1)) {
          auto it2 = it1;
          it2++;
          swap(*(it1), *(it2));
        }
        ans.push_front(i);
        used[i] = true;
      }
    }
  }

  for (auto &x: ans) cout << (x+1) << " ";
  cout << endl;

  return 0;
}
