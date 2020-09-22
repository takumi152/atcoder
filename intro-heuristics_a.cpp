#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <random>
#include <cmath>
#include <cassert>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x000000000000ffff) * mod) >> 48;
  }
  inline unsigned int nextUInt() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUIntMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x00000000ffffffff) * mod) >> 32;
  }
  inline unsigned long long int nextULL() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline double nextDouble() {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return (double)x * 5.42101086242752217e-20;
  }
};

struct timer {
  double t = 0.0;
  double lastStop = 0.0;
  bool stopped = false;
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
    stopped = false;
  }
  inline void start() {
    if (stopped) {
      t += now() - lastStop;
      stopped = false;
    }
  }
  inline void stop() {
    if (!stopped) {
      lastStop = now();
      stopped = true;
    }
  }
  inline double time() {
    if (stopped) return lastStop - t;
    else return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    #ifdef LOCAL
    return (double)(l | h << 32) * 2.857142857142857e-10; // 1 / 3.5e9, for local (Ryzen 9 3950X)
    #else
    //return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9, for AWS EC2 C3 (Xeon E5-2680 v2)
    //return (double)(l | h << 32) * 3.4482758620689656e-10; // 1 / 2.9e9, for AWS EC2 C4 (Xeon E5-2666 v3)
    return (double)(l | h << 32) * 3.333333333333333e-10; // 1 / 3.0e9, for AWS EC2 C5 (Xeon Platinum 8124M / Xeon Platinum 8275CL)
    #endif
  }
};

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;

//#define DEBUG

constexpr int d = 365;
constexpr int t = 26;
vector<int> c(t);
vector<vector<int> > s(t, vector<int>(d));

vector<int> contests(d);
vector<int> contests_best(d);

vector<vector<short> > last(t, vector<short>(d+1, -1));
vector<vector<int> > happiness(t, vector<int>(d+1, 0));

vector<vector<short> > last_before(t, vector<short>(d+1, -1));
vector<vector<int> > happiness_before(t, vector<int>(d+1, 0));

void greedySelect() {
  vector<int> lastChosen(26, -1);
  for (int i = 0; i < d; i++) {
    vector<int> penalty(26);
    int penaltyTotal = 0;
    for (int j = 0; j < t; j++) {
      penalty[j] = c[j] * (i - lastChosen[j]);
      penaltyTotal += penalty[j];
    }
    int bestContest = 0;
    int bestScore = -penaltyTotal;
    for (int j = 0; j < t; j++) {
      int score = -penaltyTotal + penalty[j] + s[j][i];
      if (score > bestScore) {
        bestContest = j;
        bestScore = score;
      }
    }
    contests[i] = bestContest;
    lastChosen[bestContest] = i;
  }
}

void greedySelect2() {
  for (int i = 0; i < d; i++) {
    int bestContest = 0;
    int bestScore = 0;
    for (int j = 0; j < t; j++) {
      if (s[j][i] > bestScore) {
        bestContest = j;
        bestScore = s[j][i];
      }
    }
    contests[i] = bestContest;
  }
}

void greedySelect3() {
  vector<int> bestResult;
  int bestScoreOverall = -99999999;
  for (int i = 0; i < 1000; i++) {
    int score = 0;
    vector<int> lastChosen(26, -1);
    for (int i = 0; i < d; i++) {
      vector<int> penalty(26);
      int penaltyTotal = 0;
      for (int j = 0; j < t; j++) {
        penalty[j] = c[j] * (i - lastChosen[j]);
        penaltyTotal += penalty[j];
      }
      int bestContest = 0;
      int bestScore = -penaltyTotal;
      for (int j = 0; j < t; j++) {
        int score = -penaltyTotal + penalty[j] + s[j][i];
        if (score > bestScore && theRandom.nextDouble() < 0.98) {
          bestContest = j;
          bestScore = score;
        }
      }
      score += bestScore;
      contests[i] = bestContest;
      lastChosen[bestContest] = i;
    }
    if (score > bestScoreOverall) {
      bestScoreOverall = score;
      bestResult = contests;
    }
  }
  contests = bestResult;
}

int updateHappiness(int day, int con) {
  for (int i = day; i < d; i++) {
    happiness[con][i+1] = happiness[con][i];
    last[con][i+1] = last[con][i];
    if (contests[i] == con) {
      happiness[con][i+1] += s[con][i];
      last[con][i+1] = i;
    }
    else {
      happiness[con][i+1] -= c[con] * (i - last[con][i]);
    }
  }
  return happiness[con][d];
}

void greedySelect4() {
  vector<int> bestResult;
  int bestScoreOverall = -99999999;
  for (int k = 0; k < 26; k++) {
    vector<int> lastChosen(26, -1);
    for (int i = 0; i < d; i++) {
      vector<int> penalty(26);
      int penaltyTotal = 0;
      for (int j = 0; j < t; j++) {
        penalty[j] = c[j] * ((i + k) - lastChosen[j]);
        penaltyTotal += penalty[j];
      }
      int bestContest = 0;
      int bestScore = -penaltyTotal;
      for (int j = 0; j < t; j++) {
        int score = -penaltyTotal + penalty[j] + s[j][i] - c[j] * ((k * (k + 1)) / 2);
        if (score > bestScore) {
          bestContest = j;
          bestScore = score;
        }
      }
      contests[i] = bestContest;
      lastChosen[bestContest] = i;
    }
    int scoreOverAll = 0;
    for (int i = 0; i < 26; i++) scoreOverAll += updateHappiness(0, i);
    if (scoreOverAll > bestScoreOverall) {
      bestScoreOverall = scoreOverAll;
      bestResult = contests;
    }
  }
  contests = bestResult;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  // input
  int _d;
  cin >> _d;
  for (auto &x: c) cin >> x;
  for (int i = 0; i < d; i++) {
    for (int j = 0; j < t; j++) cin >> s[j][i];
  }

  // preprocessing
  greedySelect4();
  contests_best = contests;
  int score = 0;
  for (int i = 0; i < t; i++) score += updateHappiness(0, i);

  last_before = last;
  happiness_before = happiness;

  // optimization
  int lastScore = score;
  int bestScore = score;

  double baseTemperature = 1.5e3;
  double temperature = baseTemperature;
  double decayRate = 5e-7;
  double timeLimit = 1.980;
  int iterCount = 0;

  while (theTimer.time() < timeLimit) {
    double roll = theRandom.nextDouble();
    if (roll < 0.1) {
      int day = theRandom.nextUIntMod(d);
      int con = theRandom.nextUIntMod(t);
      if (contests[day] == con) continue;

      int contest_before = contests[day];
      contests[day] = con;
      score -= happiness[contest_before][d];
      score += updateHappiness(day, contest_before);
      score -= happiness[con][d];
      score += updateHappiness(day, con);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        for (int i = day; i < d; i++) {
          last_before[contest_before][i+1] = last[contest_before][i+1];
          happiness_before[contest_before][i+1] = happiness[contest_before][i+1];
          last_before[con][i+1] = last[con][i+1];
          happiness_before[con][i+1] = happiness[con][i+1];
        }
        if (score > bestScore) {
          bestScore = score;
          contests_best = contests;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
        for (int i = day; i < d; i++) {
          last_before[contest_before][i+1] = last[contest_before][i+1];
          happiness_before[contest_before][i+1] = happiness[contest_before][i+1];
          last_before[con][i+1] = last[con][i+1];
          happiness_before[con][i+1] = happiness[con][i+1];
        }
      }
      else { // rollback
        contests[day] = contest_before;
        for (int i = day; i < d; i++) {
          last[contest_before][i+1] = last_before[contest_before][i+1];
          happiness[contest_before][i+1] = happiness_before[contest_before][i+1];
          last[con][i+1] = last_before[con][i+1];
          happiness[con][i+1] = happiness_before[con][i+1];
        }
        score = lastScore;
      }
    }
    else if (roll < 0.25) {
      int day1 = theRandom.nextUIntMod(d);
      int con1 = theRandom.nextUIntMod(t);
      if (contests[day1] == con1) continue;
      int day2 = max(0, min(d-1, (int) theRandom.nextUIntMod(21) - 10 + day1));
      if (day1 == day2) continue;
      int con2 = contests[day1];
      if (contests[day2] == con2) continue;

      int contest_before1 = contests[day1];
      contests[day1] = con1;
      int contest_before2 = contests[day2];
      contests[day2] = con2;
      score -= happiness[contest_before1][d];
      score += updateHappiness(min(day1, day2), contest_before1);
      score -= happiness[con1][d];
      score += updateHappiness(day1, con1);
      score -= happiness[contest_before2][d];
      score += updateHappiness(day2, contest_before2);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (day1 < day2) {
          for (int i = day1; i < d; i++) {
            last_before[contest_before1][i+1] = last[contest_before1][i+1];
            happiness_before[contest_before1][i+1] = happiness[contest_before1][i+1];
            last_before[con1][i+1] = last[con1][i+1];
            happiness_before[con1][i+1] = happiness[con1][i+1];
          }
          for (int i = day2; i < d; i++) {
            last_before[contest_before2][i+1] = last[contest_before2][i+1];
            happiness_before[contest_before2][i+1] = happiness[contest_before2][i+1];
          }
        }
        else {
          for (int i = day1; i < d; i++) {
            last_before[con1][i+1] = last[con1][i+1];
            happiness_before[con1][i+1] = happiness[con1][i+1];
          }
          for (int i = day2; i < d; i++) {
            last_before[contest_before2][i+1] = last[contest_before2][i+1];
            happiness_before[contest_before2][i+1] = happiness[contest_before2][i+1];
            last_before[con2][i+1] = last[con2][i+1];
            happiness_before[con2][i+1] = happiness[con2][i+1];
          }
        }
        if (score > bestScore) {
          bestScore = score;
          contests_best = contests;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
        if (day1 < day2) {
          for (int i = day1; i < d; i++) {
            last_before[contest_before1][i+1] = last[contest_before1][i+1];
            happiness_before[contest_before1][i+1] = happiness[contest_before1][i+1];
            last_before[con1][i+1] = last[con1][i+1];
            happiness_before[con1][i+1] = happiness[con1][i+1];
          }
          for (int i = day2; i < d; i++) {
            last_before[contest_before2][i+1] = last[contest_before2][i+1];
            happiness_before[contest_before2][i+1] = happiness[contest_before2][i+1];
          }
        }
        else {
          for (int i = day1; i < d; i++) {
            last_before[con1][i+1] = last[con1][i+1];
            happiness_before[con1][i+1] = happiness[con1][i+1];
          }
          for (int i = day2; i < d; i++) {
            last_before[contest_before2][i+1] = last[contest_before2][i+1];
            happiness_before[contest_before2][i+1] = happiness[contest_before2][i+1];
            last_before[con2][i+1] = last[con2][i+1];
            happiness_before[con2][i+1] = happiness[con2][i+1];
          }
        }
      }
      else { // rollback
        if (day1 < day2) {
          contests[day1] = contest_before1;
          for (int i = day1; i < d; i++) {
            last[contest_before1][i+1] = last_before[contest_before1][i+1];
            happiness[contest_before1][i+1] = happiness_before[contest_before1][i+1];
            last[con1][i+1] = last_before[con1][i+1];
            happiness[con1][i+1] = happiness_before[con1][i+1];
          }
          contests[day2] = contest_before2;
          for (int i = day2; i < d; i++) {
            last[contest_before2][i+1] = last_before[contest_before2][i+1];
            happiness[contest_before2][i+1] = happiness_before[contest_before2][i+1];
          }
        }
        else {
          contests[day1] = contest_before1;
          for (int i = day1; i < d; i++) {
            last[con1][i+1] = last_before[con1][i+1];
            happiness[con1][i+1] = happiness_before[con1][i+1];
          }
          contests[day2] = contest_before2;
          for (int i = day2; i < d; i++) {
            last[contest_before2][i+1] = last_before[contest_before2][i+1];
            happiness[contest_before2][i+1] = happiness_before[contest_before2][i+1];
            last[con2][i+1] = last_before[con2][i+1];
            happiness[con2][i+1] = happiness_before[con2][i+1];
          }
        }
        score = lastScore;
      }
    }
    else {
      int day1 = theRandom.nextUIntMod(d);
      int day2 = max(0, min(d-1, (int) theRandom.nextUIntMod(21) - 10 + day1));
      if (day1 == day2) continue;
      if (contests[day1] == contests[day2]) continue;
      if (day1 > day2) swap(day1, day2);

      swap(contests[day1], contests[day2]);
      score -= happiness[contests[day1]][d];
      score -= happiness[contests[day2]][d];
      score += updateHappiness(day1, contests[day1]);
      score += updateHappiness(day1, contests[day2]);

      #ifdef DEBUG
      if (iterCount % 100000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << theTimer.time() << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        for (int i = day1; i < d; i++) {
          last_before[contests[day1]][i+1] = last[contests[day1]][i+1];
          happiness_before[contests[day1]][i+1] = happiness[contests[day1]][i+1];
          last_before[contests[day2]][i+1] = last[contests[day2]][i+1];
          happiness_before[contests[day2]][i+1] = happiness[contests[day2]][i+1];
        }
        if (score > bestScore) {
          bestScore = score;
          contests_best = contests;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
        for (int i = day1; i < d; i++) {
          last_before[contests[day1]][i+1] = last[contests[day1]][i+1];
          happiness_before[contests[day1]][i+1] = happiness[contests[day1]][i+1];
          last_before[contests[day2]][i+1] = last[contests[day2]][i+1];
          happiness_before[contests[day2]][i+1] = happiness[contests[day2]][i+1];
        }
      }
      else { // rollback
        swap(contests[day1], contests[day2]);
        for (int i = day1; i < d; i++) {
          last[contests[day1]][i+1] = last_before[contests[day1]][i+1];
          happiness[contests[day1]][i+1] = happiness_before[contests[day1]][i+1];
          last[contests[day2]][i+1] = last_before[contests[day2]][i+1];
          happiness[contests[day2]][i+1] = happiness_before[contests[day2]][i+1];
        }
        score = lastScore;
      }
    }

    if (iterCount % 500000 == 0) {
      contests = contests_best;
      for (int i = 0; i < t; i++) updateHappiness(0, i);
      last_before = last;
      happiness_before = happiness;
      score = bestScore;
      lastScore = bestScore;
    }

    iterCount++;
    //temperature *= 1.0 - decayRate;
    temperature = baseTemperature * ((timeLimit - theTimer.time()) / timeLimit);
  }

  contests = contests_best;

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "temperature = " << temperature << endl;
  cerr << "score       = " << score << endl;
  cerr << "bestScore   = " << bestScore << endl;

  // postprocess & output
  for (auto &x: contests) cout << x+1 << endl;

  return 0;
}
