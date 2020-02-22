// WIP

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

  string s;
  cin >> s;

  vector<int> n;
  n.push_back(0);
  for (auto &x: s) n.push_back(x - '0');

  int d = n.size();

  vector<int> n_sum(d+1);
  for (int i = 0; i < d; i++) {
    n_sum[i+1] = n_sum[i] + n[i];
  }

  vector<vector<vector<int> > > dp(d+2, vector<vector<int> >(2, vector<int>(10, 1000000007))); // digit, sufficient, number
  for (int i = 0; i < d; i++) {
    for (int j = 0; j <= 9; j++) {

    }
  }


  return 0;
}
