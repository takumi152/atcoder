#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>

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

  vector<bool> berryExists(h);
  vector<vector<int> > berryNum(h);
  vector<vector<int> > group(h, vector<int>(w));
  int m = 1;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (s[i][j] == '#') {
        berryExists[i] = true;
        berryNum[i].push_back(m);
        group[i][j] = m;
        m++;
      }
    }
  }

  for (int i = 0; i < h; i++) {
    if (berryExists[i]) {
      int p = 0;
      int j = 0;
      while (p < berryNum[i].size()) {
        if (group[i][j] > 0) {
          p++;
        }
        else {
          group[i][j] = berryNum[i][p];
        }
        j++;
      }
      while (j < w) {
        group[i][j] = group[i][j-1];
        j++;
      }
    }
  }

  for (int i = 1; i < h; i++) {
    if (!berryExists[i] && berryExists[i-1]) {
      for (int j = 0; j < w; j++) {
        group[i][j] = group[i-1][j];
      }
      berryExists[i] = true;
    }
  }

  for (int i = h-2; i >= 0; i--) {
    if (!berryExists[i] && berryExists[i+1]) {
      for (int j = 0; j < w; j++) {
        group[i][j] = group[i+1][j];
      }
      berryExists[i] = true;
    }
  }

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      cout << group[i][j];
      if (j < w-1) cout << " ";
      else cout << endl;
    }
  }

  return 0;
}
