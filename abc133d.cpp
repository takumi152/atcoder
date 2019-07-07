#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> A;
  for (int i = 0; i < N; i++) {
    int buf;
    cin >> buf;
    A.push_back(buf);
  }

  // x0/2 + x1/2 = a0
  // x1/2 + x2/2 = a1
  // x2/2 + x3/2 = a2
  // x3/2 + x4/2 = a3
  // x4/2 + x0/2 = a4

  // x0 = a0 - a1 + a2 - a3 + a4
  // x1 = a1 - a2 + a3 - a4 + a0
  // x2 = a2 - a3 + a4 - a0 + a1
  // x3 = a3 - a4 + a0 - a1 + a2
  // x4 = a4 - a0 + a1 - a2 + a3
  vector<ll> posSum(1, 0), negSum(1, 0);
  ll pos, neg;
  for (int i = 0; i < N; i++) {
    if (i % 2 == 0) {
      pos = A[i];
      neg = -A[i];
    }
    else {
      pos = -A[i];
      neg = A[i];
    }
    posSum.push_back(posSum[i] + pos);
    negSum.push_back(negSum[i] + neg);
  }
  vector<ll> rain;
  for (int i = 0; i < N; i++) {
    ll x;
    if (i % 2 == 0) x = negSum[i] + (posSum[N] - posSum[i]);
    else x = posSum[i] + (negSum[N] - negSum[i]);
    rain.push_back(x);
  }

  for(int i = 0; i < rain.size(); i++) {
    cout << rain[i];
    if (i < rain.size()-1) cout << " ";
    else cout << endl;
  }
  return 0;
}
