#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <list>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;
  int q;
  cin >> q;
  vector<vector<char> > query(q, vector<char>(3));
  for (int i = 0; i < q; i++) {
    cin >> query[i][0];
    if (query[i][0] == '2') cin >> query[i][1] >> query[i][2];
  }

  int n = s.size();

  list<char> str;
  for (int i = 0; i < n; i++) str.push_back(s[i]);

  bool flipped = false;
  for (int i = 0; i < q; i++) {
    if (query[i][0] == '1') {
      flipped = !flipped;
    }
    else {
      if (flipped != (query[i][1] == '1')) {
        str.push_front(query[i][2]);
      }
      else {
        str.push_back(query[i][2]);
      }
    }
  }

  if (!flipped) {
    for (auto it = str.begin(); it != str.end(); it++) cout << *(it);
  }
  else {
    for (auto it = str.rbegin(); it != str.rend(); it++) cout << *(it);
  }
  cout << endl;

  return 0;
}
