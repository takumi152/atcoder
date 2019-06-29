#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string S;
  cin >> S;

  vector<int> count(256);
  for (int i = 0; i < S.size(); i++) count[(int)S[i]]++;
  int num = 0;
  for (int i = 0; i < count.size(); i++) {
    if (count[i] == 0) continue;
    else if (count[i] == 2) num++;
    else {
      cout << "No" << endl;
      return 0;
    }
  }
  if (num == 2) {
    cout << "Yes" << endl;
  }
  else {
    cout << "No" << endl;
  }

  return 0;
}
