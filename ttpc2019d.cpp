// WA

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

typedef long long int ll;

vector<bool> eratosthenes(int n) {
  vector<bool> primeList(n+1, true);
  primeList[0] = false;
  primeList[1] = false;
  for (int i = 2; i <= n; i++) {
    if (primeList[i]) {
      for (int j = i + i; j <= n; j += i) {
        primeList[j] = false;
      }
    }
  }
  return primeList;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> a(N);
  for (auto &x: a) cin >> x;

  if (N % 2 == 1) {
    cout << "An" << endl; // first
    return 0;
  }

  vector<bool> primeList = eratosthenes(*max_element(a.begin(), a.end()));
  vector<int> remaining(N, 1);
  for (int i = 0; i < N; i++) {
    while (primeList[a[i] - remaining[i] * 2]) remaining[i]++;
  }
  int takeTotal = accumulate(remaining.begin(), remaining.end(), 0);
  if (takeTotal % 2 == 0) {
    cout << "Ai" << endl; // second
  }
  else {
    cout << "An" << endl; // first
  }

  return 0;
}
