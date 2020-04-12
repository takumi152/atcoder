#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <numeric>

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

  vector<int> rsum(n+1), gsum(n+1), bsum(n+1);
  for (int i = 0; i < n; i++) {
    rsum[i+1] = rsum[i];
    gsum[i+1] = gsum[i];
    bsum[i+1] = bsum[i];
    if (s[i] == 'R') rsum[i+1]++;
    if (s[i] == 'G') gsum[i+1]++;
    if (s[i] == 'B') bsum[i+1]++;
  }

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] != 'R') continue;
    for (int j = i + 1; j < n; j++) {
      if (s[j] != 'G') continue;
      ans += bsum[n] - bsum[j+1];
      if (j + (j - i) < n) {
        if (s[j+(j-i)] == 'B') ans--;
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (s[i] != 'G') continue;
    for (int j = i + 1; j < n; j++) {
      if (s[j] != 'R') continue;
      ans += bsum[n] - bsum[j+1];
      if (j + (j - i) < n) {
        if (s[j+(j-i)] == 'B') ans--;
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (s[i] != 'R') continue;
    for (int j = i + 1; j < n; j++) {
      if (s[j] != 'B') continue;
      ans += gsum[n] - gsum[j+1];
      if (j + (j - i) < n) {
        if (s[j+(j-i)] == 'G') ans--;
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (s[i] != 'B') continue;
    for (int j = i + 1; j < n; j++) {
      if (s[j] != 'R') continue;
      ans += gsum[n] - gsum[j+1];
      if (j + (j - i) < n) {
        if (s[j+(j-i)] == 'G') ans--;
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (s[i] != 'G') continue;
    for (int j = i + 1; j < n; j++) {
      if (s[j] != 'B') continue;
      ans += rsum[n] - rsum[j+1];
      if (j + (j - i) < n) {
        if (s[j+(j-i)] == 'R') ans--;
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (s[i] != 'B') continue;
    for (int j = i + 1; j < n; j++) {
      if (s[j] != 'G') continue;
      ans += rsum[n] - rsum[j+1];
      if (j + (j - i) < n) {
        if (s[j+(j-i)] == 'R') ans--;
      }
    }
  }

  cout << ans << endl;

  return 0;
}
