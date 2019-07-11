#include <iostream>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, K;
  cin >> N >> K;

  int b = K + 1;
  ll cnt = 0;
  for (int i = b; i <= N; i++) {
    int a = 0;
    while (a + i <= N) {
      cnt += i - K;
      a += i;
    }
    if (K == 0) cnt--;
    int remaining = N - a + 1;
    cnt += max(0, remaining - K);
  }

  cout << cnt << endl;

  return 0;
}
