#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, D;
  cin >> N >> D;
  vector<vector<int> > X;
  for (int i = 0; i < N; i++) {
    X.push_back(vector<int>());
    for(int j = 0; j < D; j++) {
      int buf;
      cin >> buf;
      X[i].push_back(buf);
    }
  }

  const double eps = 1e-8;
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = i+1; j < N; j++) {
      double d = 0.0;
      for (int k = 0; k < D; k++) {
        d += abs(X[i][k] - X[j][k]) * abs(X[i][k] - X[j][k]);
      }
      d = sqrt(d);
      if (abs(d - round(d)) < eps) count++;
    }
  }

  cout << count << endl;
  return 0;
}
