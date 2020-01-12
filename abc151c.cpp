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

  int n, m;
  cin >> n >> m;
  vector<int> p(m);
  vector<string> s(m);
  for (int i = 0; i < m; i++) cin >> p[i] >> s[i];

  vector<bool> ac(n);
  vector<int> wa(n);
  for (int i = 0; i < m; i++) {
    if (s[i] == "AC") {
      ac[p[i]-1] = true;
    }
    else {
      if (!ac[p[i]-1]) wa[p[i]-1]++;
    }
  }

  int score = 0;
  int penalty = 0;
  for (int i = 0; i < n; i++) {
    if (ac[i]) {
      score++;
      penalty += wa[i];
    }
  }

  cout << score << " " << penalty << endl;

  return 0;
}
