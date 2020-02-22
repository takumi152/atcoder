#include <iostream>
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
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  for (auto &x: a) {
    if (x % 2 == 0) {
      if (x % 3 == 0 || x % 5 == 0) continue;
      else {
        cout << "DENIED" << endl;
        return 0;
      }
    }
  }

  cout << "APPROVED" << endl;

  return 0;
}
