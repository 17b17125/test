#include <stdio.h>

#define N_MAX 1000
#define W_MAX 1000000

int p[N_MAX];                 //価値の列
int w[N_MAX];                 //サイズの列
int W;                    //ナップザックの容量
int dp[N_MAX][W_MAX];     //global にして初期化を任す

int main(void){
  int n,i,j,tmp;

  scanf("%d %d", &n, &W);
  i=0;
  while(i < n){
    scanf("%d", &p[i]);
    i++;
  }
  i=0;
  while(i < n){
    scanf("%d", &w[i]);
    i++;
  }
  for(i=1; i<n; i++){
    for(j=0; j<W; j++){
      if(j < w[i]) dp[i][j] = dp[i-1][j];
      else dp[i][j] = dp[i-1][j] - (dp[i-1][j-w[i]] + p[i])? dp[i-1][j]: dp[i-1][j-w[i]] + p[i];
    }
  }
  printf("%d\n", dp[n][W]);
  printf("(id) price weight\n");
  i = n;
  j = W;
  while(0 < dp[i][j]){
    if(dp[i-1][j-w[i]] + p[i] == dp[i][j]){
      printf("(%d) %d %d\n", i, p[i], w[i]);
      j -= w[i];
    }
    else if(dp[i-1][j] == dp[i][j]) {}//printf("(Hadn't choose.\n)");
    else printf("Something wrong!\n");
    i--;
  }
  return 0;
}
