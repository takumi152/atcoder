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

  int h, w, k;
  cin >> h >> w >> k;
  vector<string> s(h);
  for (auto &x: s) cin >> x;

  ll ans = 1000000007;
  for (int i = 0; i < (1 << (h - 1)); i++) {
    ll part = 0;
    vector<Pii> section;
    int low = 0;
    int high = 1;
    int mask = i;
    while (mask > 0) {
      if (mask & 1) {
        section.emplace_back(low, high);
        low = high;
        part++;
      }
      mask >>= 1;
      high++;
    }
    section.emplace_back(low, h);
    vector<int> white(section.size());
    for (int j = 0; j < w; j++) {
      vector<int> whiteone(section.size());
      bool partneed = false;
      for (int p = 0; p < (int) section.size(); p++) {
        for (int r = section[p].first; r < section[p].second; r++) {
          if (s[r][j] == '1') whiteone[p]++;
        }
        if (white[p] + whiteone[p] > k) partneed = true;
        if (whiteone[p] > k) part += 1000000007;
        white[p] += whiteone[p];
      }
      if (partneed) {
        part++;
        white = whiteone;
      }
    }
    if (part < ans) ans = part;
  }

  cout << ans << endl;

  return 0;
}
