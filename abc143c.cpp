#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;
const ll inf = (ll) 1e18;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string s;
  cin >> s;

  list<char> slime;
  for (int i = 0; i < n; i++) slime.push_back(s[i]);
  auto it = slime.begin();
  while (it != slime.end()) {
    auto it2 = it; it2++;
    if (it2 == slime.end()) break;
    if (*it == *it2) slime.erase(it2);
    else it++;
  }

  cout << slime.size() << endl;

  return 0;
}
