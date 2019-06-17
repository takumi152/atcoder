#include <vector>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

int main() {
  int H, W;
  cin >> H >> W;
  vector<string> board;
  for (int i = 0; i < H; i++) {
    string row;
    cin >> row;
    board.push_back(row);
  }
  queue<vector<int>> q;
  vector<vector<bool> > doneBoard;
  doneBoard.assign(H, vector<bool>(W, false));
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      if (board[i][j] == '#') {
        q.push(vector<int>({i,j,0}));
        doneBoard[i][j] = true;
      }
    }
  }
  int count = 0;
  while(!q.empty()) {
    auto x = q.front();
    count = max(count, x[2]);
    if (x[0] > 0) {
      if (!doneBoard[x[0]-1][x[1]]){
        q.push(vector<int>({x[0]-1,x[1],count+1}));
        doneBoard[x[0]-1][x[1]] = true;
      }
    }
    if (x[0] < H-1) {
      if (!doneBoard[x[0]+1][x[1]]) {
        q.push(vector<int>({x[0]+1,x[1],count+1}));
        doneBoard[x[0]+1][x[1]] = true;
      }
    }
    if (x[1] > 0) {
      if (!doneBoard[x[0]][x[1]-1]) {
        q.push(vector<int>({x[0],x[1]-1,count+1}));
        doneBoard[x[0]][x[1]-1] = true;
      }
    }
    if (x[1] < W-1) {
      if (!doneBoard[x[0]][x[1]+1]) {
        q.push(vector<int>({x[0],x[1]+1,count+1}));
        doneBoard[x[0]][x[1]+1] = true;
      }
    }
    doneBoard[x[0]][x[1]] = true;
    q.pop();
  }
  cout << count << endl;
  return 0;
}
