#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int n = s.length();

  int left = 0;
  int right = n-1;
  while (left < right) {
    if (s[left] == s[right]) {
      left++;
      right--;
    }
    else {
      cout << "No" << endl;
      return 0;
    }
  }
  left = 0;
  right = (n-1) / 2 - 1;
  while (left < right) {
    if (s[left] == s[right]) {
      left++;
      right--;
    }
    else {
      cout << "No" << endl;
      return 0;
    }
  }
  left = (n+1) / 2;
  right = n-1;
  while (left < right) {
    if (s[left] == s[right]) {
      left++;
      right--;
    }
    else {
      cout << "No" << endl;
      return 0;
    }
  }

  cout << "Yes" << endl;

  return 0;
}
