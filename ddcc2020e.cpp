#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <cstdlib>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;
vector<bool> isRed, isBlue;

string ask(vector<int> query) {
  for (int i = 0; i < isRed.size(); i++) cerr << isRed[i] << " ";
  cerr << endl;
  for (int i = 0; i < isBlue.size(); i++) cerr << isBlue[i] << " ";
  cerr << endl;
  cout << "? ";
  for (int i = 0; i < query.size(); i++) {
    cout << query[i]+1;
    if (i < query.size()-1) cout << " ";
    else cout << endl;
  }
  cout << flush;
  string response;
  cin >> response;
  if (response == "-1") exit(0); // fail
  else return response;
}

string answer(vector<char> ans) {
  cout << "! ";
  for (int i = 0; i < ans.size(); i++) {
    cout << ans[i];
  }
  cout << endl << flush;
  exit(0); // done
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;

  isRed = vector<bool>(2 * n);
  isBlue = vector<bool>(2 * n);
  vector<int> firstQuery;
  for (int i = 0; i < n; i++) firstQuery.push_back(i);
  string firstResponse = ask(firstQuery);
  int left = 0;
  int right = n-1;
  while (left < right - 2) {
    int center = (left + right) / 2;
    vector<int> query;
    for (int j = center; j < center + n; j++) query.push_back(j);
    string response = ask(query);
    if (firstResponse == response) left = center;
    else right = center;
  }
  int p = left+1;
  while (true) {
    vector<int> query;
    for (int j = p; j < p + n; j++) query.push_back(j);
    string response = ask(query);
    if (firstResponse != response) { // 境界
      int exclusion = p-1;
      if (firstResponse == "Red") { // 赤→青の境界
        isRed[exclusion] = true;
        isBlue[exclusion + n] = true;
      }
      else { // 青→赤の境界
        isBlue[exclusion] = true;
        isRed[exclusion + n] = true;
      }
      for (int j = 0; j < 2 * n; j++) {
        if (j < exclusion || exclusion + n < j) {
          vector<int> query;
          for (int k = p; k < p + n - 1; k++) query.push_back(k);
          query.push_back(j);
          string response = ask(query);
          if (response == "Red") isRed[j] = true;
          else isBlue[j] = true;
        }
      }
      int bc = 0;
      int rc = 0;
      vector<int> baseQuery;
      for (int j = 0; j < 2 * n; j++) {
        if (bc < n / 2 && isBlue[j]) {
          baseQuery.push_back(j);
          bc++;
        }
        else if (rc < n / 2 && isRed[j]) {
          baseQuery.push_back(j);
          rc++;
        }
      }
      for (int j = 0; j < 2 * n; j++) {
        if (!isBlue[j] && !isRed[j]) {
          vector<int> query = baseQuery;
          query.push_back(j);
          string response = ask(query);
          if (response == "Red") isRed[j] = true;
          else isBlue[j] = true;
        }
      }
      vector<char> ans;
      for (int j = 0; j < 2 * n; j++) {
        if (isRed[j]) ans.push_back('R');
        else if (isBlue[j]) ans.push_back('B');
      }
      answer(ans);
    }
    p++;
  }

  return 0;
}
