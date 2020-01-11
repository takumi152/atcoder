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
  vector<int> t(n);
  for (int i = 0; i < n; i++) cin >> s[i] >> t[i];
  string x;
  cin >> x;

  int total = 0;
  int sleep = false;
  for (int i = 0; i < n; i++) {
    if (sleep) total += t[i];
    if (s[i] == x) sleep = true;
  }

  cout << total << endl;

  return 0;
}
