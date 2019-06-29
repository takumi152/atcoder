#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> d;
  for (int i = 0; i < N; i++) {
    int buf;
    cin >> buf;
    d.push_back(buf);
  }

  if (N % 2 == 1) {
    cout << 0 << endl; // impossible
    return 0;
  }
  sort(d.begin(), d.end());
  int ans = d[N/2] - d[N/2-1];

  cout << ans << endl;
  return 0;
}
