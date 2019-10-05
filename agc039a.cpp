#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string S;
  cin >> S;
  ll K;
  cin >> K;

  ll N = S.size();

  char lastChar = S[N-1];
  int len = 0;
  for (int i = 0; i < N; i++) {
    if (S[i] == lastChar) len++;
    else len = 0;
  }
  if (len == N) { // single char type
    cout << (N * K) / 2 << endl;
    return 0;
  }
  bool secondStartFromOdd = (len % 2 == 1 && S[0] == lastChar);
  int first = 0, second = 0;
  lastChar = S[0];
  len = 0;
  for (int i = 0; i < N; i++) {
    if (S[i] == lastChar) len++;
    else {
      lastChar = S[i];
      len = 1;
    }
    if (len % 2 == 0) first++;
  }
  lastChar = S[0];
  len = 0;
  if (secondStartFromOdd) len = 1;
  for (int i = 0; i < N; i++) {
    if (S[i] == lastChar) len++;
    else {
      lastChar = S[i];
      len = 1;
    }
    if (len % 2 == 0) second++;
  }

  cout << (first + second * (K-1)) << endl;

  return 0;
}
