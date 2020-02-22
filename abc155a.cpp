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

  int a, b, c;
  cin >> a >> b >> c;

  vector<int> d({a, b, c});
  sort(d.begin(), d.end());
  if ((d[0] == d[1] && d[1] != d[2]) || (d[0] != d[1] && d[1] == d[2])) cout << "Yes" << endl;
  else cout << "No" << endl;

  return 0;
}
