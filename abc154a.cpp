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

  string s, t;
  cin >> s >> t;
  int a, b;
  cin >> a >> b;
  string u;
  cin >> u;

  if (s == u) a--;
  else b--;

  cout << a << " " << b;

  return 0;
}
