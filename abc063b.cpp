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

  vector<bool> found(26);
  for (int i = 0; i < s.length(); i++) {
    int idx = s[i] - 'a';
    if (found[idx]) {
      cout << "no" << endl;
      return 0;
    }
    found[idx] = true;
  }

  cout << "yes" << endl;

  return 0;
}
