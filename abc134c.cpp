#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> A(N);
  for (auto &a: A) cin >> a;

  vector<int> aSort = A;
  sort(aSort.begin(), aSort.end());
  int firstLargest = aSort[N-1];
  int secondLargest = aSort[N-2];
  vector<int> ans;
  for (auto &a: A) {
    if (a == firstLargest) ans.push_back(secondLargest);
    else ans.push_back(firstLargest);
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
