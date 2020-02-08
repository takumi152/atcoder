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

  int d, n;
  cin >> d >> n;

  int ans = n;
  if (n == 100) ans++;
  for (int i = 0; i < d; i++) ans *= 100;

  cout << ans << endl;

  return 0;
}
