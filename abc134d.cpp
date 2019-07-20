#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> a(N);
  for (auto &x: a) cin >> x;

  // b1 + b2 + b3 + b4 + b5 + b6 = a1
  //      b2      + b4      + b6 = a2
  //           b3           + b6 = a3
  //                b4           = a4
  //                     b5      = a5
  //                          b6 = a6

  vector<bool> b(N);
  for (int i = N; i >= 1; i--) {
    int ballCount = 0;
    for (int j = i * 2; j <= N; j+=i) {
      if (b[j-1]) ballCount++;
    }
    if (ballCount % 2 == a[i-1]) continue;
    else b[i-1] = true;
  }
  vector<int> ans;
  for (int i = 0; i < N; i++) {
    if (b[i]) ans.push_back(i+1);
  }

  cout << ans.size() << endl;
  for (auto &x: ans) cout << x << endl;

  return 0;
}
