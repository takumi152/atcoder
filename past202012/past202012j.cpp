#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;
  ll x;
  cin >> x;

  ll o = 0;
  ll t = 0;
  int p = 0;
  while (true) {
    if ('a' <= s[p] && s[p] <= 'z') {
      o++;
      t++;
      if (t == x) {
        cout << s[p] << endl;
        return 0;
      }
      p++;
    }
    else {
      bool good = true;
      for (char c = '0'; c < s[p]; c++) {
        if (t + o < x) t += o;
        else {
          good = false;
          break;
        }
      }
      if (good) {
        o += o * (s[p] - '0');
        p++;
      }
      else {
        o = 0;
        p = 0;
      }
    }
  }


  return 0;
}
