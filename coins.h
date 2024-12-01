#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32_t;

typedef union
{
  u32_t coin_as_ints[13];  // 13 4-byte integers
  char coin_as_chars[53];  // 53=13*4+1 bytes
}
coin_t;

int main(void)
{
  coin_t coin;
  int n;

  if((n = snprintf(coin.coin_as_chars,53,"DETI coin %d ......................................\n",12)) != 52)
  {
    fprintf(stderr,"not exactly 52 bytes... (n=%d)\n",n);
    exit(1);
  }
  for(n = 0;n < 13 * 4;n++)
    printf("%2d %3d [%c]\n",n,coin.coin_as_chars[n],coin.coin_as_chars[n]);
}

