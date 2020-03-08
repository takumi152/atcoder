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

  if (s.size() % 2 != 0) {
    cout << "No" << endl;
    return 0;
  }

  for (int i = 0; i < (int) s.size(); i++) {
    if (i % 2 == 0) {
      if (s[i] != 'h') {
        cout << "No" << endl;
        return 0;
      }
    }
    else {
      if (s[i] != 'i') {
        cout << "No" << endl;
        return 0;
      }
    }
  }

  cout << "Yes" << endl;

  return 0;
}
