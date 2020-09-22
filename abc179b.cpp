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
  vector<Pii> d(n);
  for (auto &x: d) cin >> x.first >> x.second;

  int count = 0;
  for (auto &x: d) {
    if (x.first == x.second) {
      count++;
      if (count >= 3) {
        cout << "Yes" << endl;
        return 0;
      }
    }
    else count = 0;
  }

  cout << "No" << endl;

  return 0;
}
