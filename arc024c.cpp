#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  string s;
  cin >> s;

  map<vector<int>, vector<int> > appearance;
  vector<int> appearCount(26);
  for (int i = 0; i < n; i++) {
    appearCount[s[i]-'a']++;
    if (i-k >= 0) appearCount[s[i-k]-'a']--;
    if (i-k+1 >= 0) appearance[appearCount].push_back(i-k+1);
  }

  for (auto &x: appearance) {
    if (x.second[x.second.size()-1] - x.second[0] >= k) {
      cout << "YES" << endl;
      return 0;
    }
  }

  cout << "NO" << endl;

  return 0;
}
