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

  int n;
  cin >> n;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  sort(a.begin(), a.end());
  vector<int> b;
  b.push_back(a[0]);
  int spare_book = 0;
  for (int i = 1; i < n; i++) {
    if (a[i] != a[i-1]) b.push_back(a[i]);
    else spare_book++;
  }
  int left = 0;
  int right = b.size();
  int next_volume = 1;
  while (left < right) {
    if (b[left] == next_volume) {
      left++;
      next_volume++;
    }
    else if (b[left] < next_volume) {
      left++;
    }
    else {
      if (spare_book >= 2) {
        spare_book -= 2;
        next_volume++;
      }
      else if (spare_book >= 1) {
        spare_book--;
        right--;
        next_volume++;
      }
      else {
        right -= 2;
        if (left > right) break;
        next_volume++;
      }
    }
  }
  while (spare_book >= 2) {
    spare_book -= 2;
    next_volume++;
  }

  cout << next_volume - 1 << endl;

  return 0;
}
