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
  vector<char> s(n);
  for (auto &x: s) cin >> x;

  for (auto &x: s) {
    if (x == 'Y') {
      cout << "Four" << endl;
      return 0;
    }
  }

  cout << "Three" << endl;

  return 0;
}
