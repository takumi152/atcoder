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
  vector<int> a(n), b(n);
  for (auto &x: a) cin >> x;
  for (auto &x: b) cin >> x;

  reverse(b.begin(), b.end());

  int front = 0;
  int back = n-1;
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i]) continue;
    if (a[i] != b[back] && a[back] != b[i]) {
      swap(b[i], b[back]);
      back--;
      continue;
    }
    else if (a[i] != b[front] && a[front] != b[i]) {
      swap(b[i], b[front]);
      front++;
      continue;
    }
    else {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;
  for (auto &x: b) cout << x << " ";
  cout << endl;

  return 0;
}
