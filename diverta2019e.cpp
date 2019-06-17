// WIP

#include <cstdio>
#include <iostream>
#include <string>

#define nthBit(a, x) a & 1 << x

using namespace std;

int N;
unsigned int A[500010];

unsigned int xor[500010];
unsigned int leftXor[500010];
unsigned int dp[500010][2];
unsigned int

const unsigned int MOD = 1000000007

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cin >> N;
  for (int i = 0; i < N; i++) {
    cin >> A[i];
  }
  for (int i = 0; i < N; i++) {
    cout << A[i] << endl;
  }
  xor[0] = A[0]
  for (int i = 1; i < N; i++) {
    xor[i] = xor[i-1] ^ A[i];
  }
  unsigned int count = 1;
  unsigned int lastXor = xor[N-1];
  unsigned int lastLeftXor = 0;
  for(int i = 0; i < N; i++) {
    lastLeftXor ^= A[0];
    lastXor ^= A[0];
  }
}
