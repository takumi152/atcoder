#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;

  // n == 2^x - 1 なら自明
  // n == 2^x なら不可能
  // 半開区間でXORが0になるようにする

  int nn = N;
  int oneCount = 0;
  while (nn > 0) {
    if (nn & 1) oneCount++;
    nn >>= 1;
  }
  if (oneCount <= 1) {
    cout << "No" << endl;
    return 0;
  }

  int nplus1 = N+1;
  int bitCount = 0;
  while (nplus1 > 0) {
    nplus1 >>= 1;
    bitCount++;
  }
  int nx = 0;
  for (int i = 0; i < bitCount-1; i++) {
    nx <<= 1;
    nx = nx | 1;
  }
  vector<Pii> edge;
  for (int i = 1; i < nx; i++) {
    edge.push_back(Pii(i, i+1));
  }
  edge.push_back(Pii(nx, N+1));
  for (int i = 1; i < nx; i++) {
    edge.push_back(Pii(N+i, N+i+1));
  }
  for (int i = nx; i < N-1; i+=2) {
    edge.push_back(Pii(i+2, i+1));
    edge.push_back(Pii(i+1, 2));
    edge.push_back(Pii(3, N+i+2));
    edge.push_back(Pii(N+i+2, N+i+1));
  }
  if (N % 2 == 0) {
    int a = N ^ (N-2), p = 2;
    while ((a ^ p) != 0) {
      a ^= p;
      p++;
    }
    edge.push_back(Pii(N, N-2));
    edge.push_back(Pii(p, N+N));
  }

  cout << "Yes" << endl;
  for (auto x: edge) cout << x.first << " " << x.second << endl;
  return 0;
}
/*
001
010
011
100
101
110

110 110
100 010
010 000
*/
