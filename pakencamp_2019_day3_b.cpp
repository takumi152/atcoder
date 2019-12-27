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
  vector<string> s(n);
  for (auto &x: s) cin >> x;

  int b = 0, w = 0;
  for (auto &x: s) {
    if (x == "black") b++;
    else if (x == "white") w++;
  }

  if (b > w) cout << "black" << endl;
  else cout << "white" << endl;

  return 0;
}
