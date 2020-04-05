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
  vector<pair<char, int> > cx(n);
  for (auto &x: cx) {
    cin >> x.second >> x.first;
    if (x.first == 'B') x.first = 'Z';
  }

  sort(cx.begin(), cx.end());

  for (auto &x: cx) cout << x.second << endl;

  return 0;
}
