#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, A, B;
  cin >> N >> A >> B;

  int a = A * N;
  int b = B;
  if (a < b) cout << a << endl;
  else cout << b << endl;

  return 0;
}
