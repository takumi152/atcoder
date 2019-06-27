#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

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
  for (int i = 0; i < N; i++) {
    A[i] -= i + 1;
  }
  sort(A.begin(), A.end());
  int b = A[N/2];
  long long int sadness = 0;
  for (int i = 0; i < N; i++) {
    sadness += abs(A[i] - b);
  }
  cout << sadness << endl;
  return 0;
}
