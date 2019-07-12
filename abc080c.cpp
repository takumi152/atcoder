#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<vector<int> > F(N, vector<int>());
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < 10; j++) {
      int f;
      cin >> f;
      F[i].push_back(f);
    }
  }
  vector<vector<int> > P(N, vector<int>());
  for (int i = 0; i < N; i++){
    for (int j = 0; j < 10+1; j++) {
      int p;
      cin >> p;
      P[i].push_back(p);
    }
  }

  int maxProfit = -1000000007;
  for (int i = 1; i < (1 << 10); i++) {
    vector<int> match(N, 0);
    for (int j = 0; j < N; j++) {
      int x = i;
      for (int k = 0; k < 10; k++) {
        if (x & 1 && F[j][k]) match[j]++;
        x >>= 1;
      }
    }
    int profit = 0;
    for (int j = 0; j < N; j++) {
      profit += P[j][match[j]];
    }
    if (profit > maxProfit) maxProfit = profit;
  }

  cout << maxProfit << endl;
  return 0;
}
