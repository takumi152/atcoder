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

  int n, k;
  cin >> n >> k;
  string s;
  cin >> s;

  vector<int> numAppear(26);
  for (int i = 0; i < n; i++) numAppear[s[i]-'a']++;
  vector<int> numAppearAfter = numAppear;

  int swapCount = 0;
  string ans;
  for (int i = 0; i < n; i++) {
    numAppear[s[i]-'a']--;

    char next = 'a';
    for (char j = 'a'; j <= 'z'; j++) {
      if (numAppearAfter[j-'a'] == 0) continue;
      vector<int> numAppearComp = numAppearAfter;
      numAppearComp[j-'a']--;
      int diff = swapCount;
      if (j != s[i]) diff++;
      for (int l = 'a'; l <= 'z'; l++) diff += max(0, numAppearComp[l-'a'] - numAppear[l-'a']);
      if (diff <= k) {
        next = j;
        break;
      }
    }

    if (next != s[i]) swapCount++;
    ans.push_back(next);
    numAppearAfter[next-'a']--;
  }

  cout << ans << endl;

  return 0;
}
