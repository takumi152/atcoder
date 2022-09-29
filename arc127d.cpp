#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx2")

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

#include <x86intrin.h>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> a(n);
  vector<int> b(n);
  // for (auto &x: a) cin >> x;
  // for (auto &x: b) cin >> x;
  for (int i = 0; i < n; i++) a[i] = (123 * i) % (1 << 18);
  for (int i = 0; i < n; i++) b[i] = (789 * i) % (1 << 18);

  int __attribute__ ((aligned(32))) ab[500100];
  for (int i = 0; i < n - 7; i += 8) {
    for (int j = 0; j < 8; j++) ab[i * 2 + j + 0] = a[i+j];
    for (int j = 0; j < 8; j++) ab[i * 2 + j + 8] = b[i+j];
  }

  ll ans = 0;
  for (int i = 0; i < n; i++) {
    __m256i vec_s32_ai = _mm256_set1_epi32(a[i]);
    __m256i vec_s32_bi = _mm256_set1_epi32(b[i]);
    for (int k = i + 1; k < n; k += 10000){
      __m256i vec_s32_min_sum = _mm256_setzero_si256();
      int j;
      for (j = k; j < min(k + 10000, n - 7); j += 8) {
        const __m256i vec_s32_aj = _mm256_loadu_si256((__m256i*) (a.data() + j));
        const __m256i vec_s32_bj = _mm256_loadu_si256((__m256i*) (b.data() + j));
        const __m256i vec_s32_xor_ai_aj = _mm256_xor_si256(vec_s32_ai, vec_s32_aj);
        const __m256i vec_s32_xor_bi_bj = _mm256_xor_si256(vec_s32_bi, vec_s32_bj);
        const __m256i vec_s32_xor_min = _mm256_min_epi32(vec_s32_xor_ai_aj, vec_s32_xor_bi_bj);
        vec_s32_min_sum = _mm256_add_epi32(vec_s32_min_sum, vec_s32_xor_min);
      }
      const __m256i vec_s32_min_sum_p4 = _mm256_hadd_epi32(vec_s32_min_sum, vec_s32_min_sum);
      const __m256i vec_s32_min_sum_p2 = _mm256_hadd_epi32(vec_s32_min_sum_p4, vec_s32_min_sum_p4);
      ans += (ll) _mm256_extract_epi32(vec_s32_min_sum_p2, 0) + (ll) _mm256_extract_epi32(vec_s32_min_sum_p2, 4);
      for (; j < min(k + 10000, n); j++) {
        ans += min(a[i] ^ a[j], b[i] ^ b[j]);
      }
    }
  }

  // ll ans2 = 0;
  // for (int i = 0; i < n; i++) {
  //   for (int j = i + 1; j < n; j++) ans2 += min(a[i] ^ a[j], b[i] ^ b[j]);
  // }

  cout << ans << endl;
  // cerr << ans2 << endl;

  return 0;
}
