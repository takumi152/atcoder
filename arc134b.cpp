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

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  string s;
  cin >> s;

  int last_j = n;
  int last_i = -1;
  for (char c = 'a'; c <= 'z'; c++) {
    for (int j = last_j - 1; j > last_i; j--) {
      if (s[j] == c) {
        bool found = false;
        int i = last_i + 1;
        for (; i < j; i++) {
          if (s[i] > c) {
            found = true;
            break;
          }
        }
        if (!found) break;
        swap(s[i], s[j]);
        last_j = j;
        last_i = i;
      }
    }
  }

  cout << s << endl;

  return 0;
}
