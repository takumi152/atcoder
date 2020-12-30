#include <iostream>
#include <iomanip>
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

vector<vector<char> > winner;

vector<int> powk2modn;

char solve(int n, int k, string& s) {
  if (winner[k][n] == '.') {
    if (k == 0) {
      winner[k][n] = s[n];
    }
    else {
      auto left = solve(n, k-1, s);
      auto right = solve((n + powk2modn[k-1]) % s.size(), k-1, s);
           if ((left == 'R' && right == 'S') || (left == 'S' && right == 'R')) winner[k][n] = 'R';
      else if ((left == 'P' && right == 'R') || (left == 'R' && right == 'P')) winner[k][n] = 'P';
      else if ((left == 'S' && right == 'P') || (left == 'P' && right == 'S')) winner[k][n] = 'S';
      else winner[k][n] = left;
    }
  }
  return winner[k][n];
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  string s;
  cin >> s;

  winner = vector<vector<char> >(k+1, vector<char>(n+1, '.'));

  powk2modn = vector<int>(k+1);
  powk2modn[0] = 1 % n;
  for (int i = 1; i <= k; i++) powk2modn[i] = (powk2modn[i-1] * 2) % n;

  auto ans = solve(0, k, s);

  cout << ans << endl;

  return 0;
}
