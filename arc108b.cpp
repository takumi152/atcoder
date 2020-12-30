#include <iostream>
#include <iomanip>
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

  int n;
  cin >> n;
  string s;
  cin >> s;

  vector<char> st;
  int ans = n;
  for (int i = 0; i < n; i++) {
    if (s[i] == 'f' || s[i] == 'o' || s[i] == 'x') {
      st.push_back(s[i]);
      if (st.size() >= 3) {
        if (st[st.size()-3] == 'f' && st[st.size()-2] == 'o' && st[st.size()-1] == 'x') {
          ans -= 3;
          st.pop_back();
          st.pop_back();
          st.pop_back();
        }
      }
    }
    else st.clear();
  }

  cout << ans << endl;

  return 0;
}
