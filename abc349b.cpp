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

  string s;
  cin >> s;

  int n = s.size();

  vector<int> appear_num(26);
  for (int i = 0; i < n; i++) {
    appear_num[s[i] - 'a']++;
  }

  for (int i = 1; i <= n; i++) {
    int char_num = 0;
    for (int j = 0; j < 26; j++) {
      if (appear_num[j] == i) char_num++;
    }
    if (!(char_num == 0 || char_num == 2)) {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
