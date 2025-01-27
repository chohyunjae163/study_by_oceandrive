//https://www.acmicpc.net/problem/2293
// hyunjaecho
#include <stdio.h>

int main()
{
    int n,k;
    int memo[100001] = { 0 };
    memo[0] = 0;
    scanf("%d %d", &n, &k);
    
    for(int i = 0; i < n; ++i) {
        int c;
        scanf("%d", &c);
        memo[c] = memo[c] + 1;
        for(int j = c; j <= k; ++j) {
            memo[j] = memo[j] + memo[ j - c ];
        }
    }
    printf("%d", memo[k]);
    return 0;
}
