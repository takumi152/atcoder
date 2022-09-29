#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  int k;
  cin >> s >> k;

  sort(s.begin(), s.end());

  for (int i = 1; i < k; i++) next_permutation(s.begin(), s.end());

  cout << s << endl;

  return 0;
}
