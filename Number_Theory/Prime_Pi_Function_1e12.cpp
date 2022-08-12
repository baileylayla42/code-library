#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define MAXN 10000010      // initial sieve limit
#define MAX_PRIMES 1000010 // max size of the prime array for sieve
#define PHI_N 100000
#define PHI_K 100

int len = 0; // total number of primes generated by sieve
int primes[MAX_PRIMES];
int pref[MAXN];       // pref[i] --> number of primes <= i
int dp[PHI_N][PHI_K]; // precal of yo(n,k)
bitset<MAXN> f;

void sieve(int n) {
  f[1] = true;
  for (int i = 4; i <= n; i += 2)
    f[i] = true;
  for (int i = 3; i * i <= n; i += 2) {
    if (!f[i]) {
      for (int j = i * i; j <= n; j += i << 1)
        f[j] = 1;
    }
  }
  for (int i = 1; i <= n; i++) {
    if (!f[i])
      primes[len++] = i;
    pref[i] = len;
  }
}

// returns the number of integers less or equal n which are
// not divisible by any of the first k primes
// recurrence --> yo(n, k) = yo(n, k-1) - yo(n / p_k , k-1)
// for sum of primes yo(n, k) = yo(n, k-1) - p_k * yo(n / p_k , k-1)
long long yo(long long n, int k) {
  if (n < PHI_N && k < PHI_K)
    return dp[n][k];
  if (k == 1)
    return ((++n) >> 1);
  if (primes[k - 1] >= n)
    return 1;
  return yo(n, k - 1) - yo(n / primes[k - 1], k - 1);
}

// runs under 0.5s for n = 1e12
// PrimePi(i) <= number of primes <= i
long long PrimePi_A000720_Lehmer(long long n) {
  if (n < MAXN)
    return pref[n];
  long long w, res = 0;
  int b = sqrt(n), c = PrimePi_A000720_Lehmer(cbrt(n)),
      a = PrimePi_A000720_Lehmer(sqrt(b));
  b = PrimePi_A000720_Lehmer(b);
  res = yo(n, a) + ((1LL * (b + a - 2) * (b - a + 1)) >> 1);
  for (int i = a; i < b; i++) {
    w = n / primes[i];
    int lim = PrimePi_A000720_Lehmer(sqrt(w));
    res -= PrimePi_A000720_Lehmer(w);
    if (i <= c) {
      for (int j = i; j < lim; j++) {
        res += j;
        res -= PrimePi_A000720_Lehmer(w / primes[j]);
      }
    }
  }
  return res;
}

void precompute() {
  // precalculation of phi upto size (PHI_N,PHI_K)
  for (int n = 0; n < PHI_N; n++)
    dp[n][0] = n;
  for (int k = 1; k < PHI_K; k++) {
    for (int n = 0; n < PHI_N; n++) {
      dp[n][k] = dp[n][k - 1] - dp[n / primes[k - 1]][k - 1];
    }
  }
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  sieve(MAXN - 1);
  precompute();
  for (int i = 1; i < MAXN; i++) {
    auto x = PrimePi_A000720_Lehmer(i);
    if (i % (MAXN / 10) == 0)
      cout << x << endl;
  }
  cout << PrimePi_A000720_Lehmer(1e12) << endl;
  return 0;
}
// https://github.com/kimwalisch/primecount