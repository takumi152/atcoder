#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> a;
  for (int i = 0; i < N; i++) {
    int buf;
    cin >> buf;
    a.push_back(buf);
  }

  sort(a.begin(), a.end());
  reverse(a.begin(), a.end());
  int alice = 0, bob = 0;
  for (int i = 0; i < N; i++) {
    if (i % 2 == 0) alice += a[i];
    else bob += a[i];
  }

  int ans = alice - bob;
  cout << ans << endl;
  return 0;
}
