#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  long long int N, A, B;
  cin >> N >> A >> B;
  vector<long long int> X;
  for (int i = 0; i < N; i++) {
    long long int buf;
    cin >> buf;
    X.push_back(buf);
  }

  long long int fatigue = 0;
  for(int i = 0; i < N-1; i++) {
    if ((X[i+1] - X[i]) * A < B) fatigue += (X[i+1] - X[i]) * A;
    else fatigue += B;
  }

  cout << fatigue << endl;
  return 0;
}
