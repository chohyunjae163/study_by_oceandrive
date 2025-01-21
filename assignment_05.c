
#include <stdio.h>

#define MAX_VAL_PLUS_ONE 100001
#define NUM_MEMO 100001
int memo[NUM_MEMO];

int main()
{
    for(int i = 0; i < NUM_MEMO;++i )
    {
        memo[i] = MAX_VAL_PLUS_ONE;
    }
    memo[0] = 0;
    int n,k;
    scanf("%d %d",&n,&k);

    for(int i = 0; i < n; ++i)
    {
        int c;
        scanf("%d", &c);
        for(int j = c; j <= k; ++j)
        {
            int q = memo[j - c] + 1;
            if(q < memo[j])
            {
                memo[j] = q;
            }
           
        }
    }
    if(memo[k] == MAX_VAL_PLUS_ONE)
    {
        printf("-1");
    }
    else
    {
        printf("%d", memo[k]);
    }
   
    return 0;
}
