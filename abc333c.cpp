#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  int num = 0;
  for (int i = 1; i < 18; i++) {
    for (int j = 1; j <= i; j++) {
      for (int k = 1; k <= j; k++) {
        num++;
        if (num == n) {
          string ans;
          for (int x = 0; x < k; x++) ans.insert(ans.begin(), '3');
          for (int x = k; x < j; x++) ans.insert(ans.begin(), '2');
          for (int x = j; x < i; x++) ans.insert(ans.begin(), '1');
          cout << ans << endl;
          return 0;
        }
      }
    }
  }

  return 0;
}
