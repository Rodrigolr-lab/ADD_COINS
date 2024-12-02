//
// Tomás Oliveira e Silva,  October 2024
//
// Arquiteturas de Alto Desempenho 2024/2025
//
// deti_coins_cpu_search() --- find DETI coins using md5_cpu()
//

#include <stdio.h>
#include <stdlib.h>

#ifndef DETI_COINS_CPU_SPECIAL_SEARCH
#define DETI_COINS_CPU_SPECIAL_SEARCH

typedef unsigned int u32_t;
typedef unsigned  int u32_t;
typedef unsigned long u64_t;

typedef union
{
  u32_t coin_as_ints[13];  // 13 4-byte integers
  char coin_as_chars[53];  // 53=13*4+1 bytes
}
coin_t;

u32_t next_value_to_try_ASCII(u32_t v1);

static void deti_coins_cpu_special_search(void)
{
  u64_t n_attempts,n_coins;
  u32_t idx;
  u32_t v1, v2;

  //
  // iniciar a estrutura
  //
  static u32_t test_data[13u][4u] __attribute__((aligned(16)));
  static u32_t test_hash[ 4u][4u] __attribute__((aligned(16)));

  //
  // iniciar valores da coin
  //
  coin_t coin;
  int n;

  // falta adicionar identificador the Thread e Lane
  if((n = snprintf(coin.coin_as_chars,53,"DETI coin %d .....................................0\n", 12 )) != 52)
  {
    fprintf(stderr,"not exactly 52 bytes... (n=%d)\n",n);
    exit(1);
  }

  v1 = coin.coin_as_ints[10];
  v2 = coin.coin_as_ints[11];

  // guardar coins na estrutura?
  for(int lane = 0u;lane < 4u;lane++)                       // for each message number
  {
      for(idx = 0u;idx < 12u;idx++)                     //  for each message word
        test_data[idx][lane] = coin.coin_as_ints[idx];  //   interleave
      test_data[idx][lane] = coin.coin_as_ints[idx] + (lane << 16);  //   interleave
  }

// testa o que esta dento
#if 1
  for (int lane = 0; lane < 4; lane++)
  {
    for(idx = 0u;idx < 13u;idx++)                     //  for each message word
      coin.coin_as_ints[idx] = test_data[idx][lane];
     printf("template %d: %52.52s",lane,coin.coin_as_chars);
  }
#endif

  //
  // find DETI coins
  //
  // nao sei se devia ser incrementado de 4 em 4
  for(n_attempts = n_coins = 0ul;stop_request == 0;n_attempts+=4)
  {
    // para computar MD5 HASH com AVX
    md5_cpu_avx((v4si *)test_data,(v4si *)test_hash);

    for (int lane = 0; lane < 4; lane++) {
      if(test_hash[3][lane] == 0u) // ultimos 4 bytes tem de ser 0
      {
        for(idx = 0u;idx < 13u;idx++)                     //  for each message word
          coin.coin_as_ints[idx] = test_data[idx][lane];
        save_deti_coin(coin.coin_as_ints);
        n_coins++;
      }
    }

    //
    // try next combination (byte range: 0x20..0x7E)
    //
    // for(idx = 10u;idx < 13u * 4u - 1u && bytes[idx] == (u08_t)126;idx++)
    //   bytes[idx] = ' ';
    // if(idx < 13u * 4u - 1u)
    //   bytes[idx]++;
    v1 = next_value_to_try_ASCII(v1);
    for (int lane = 0; lane < 4; lane++) test_data[10][lane] = v1;
    if (v1 == 0x20202020)
    {
      v2 = next_value_to_try_ASCII(v2);
      for (int lane = 0; lane < 4; lane++) test_data[11][lane] = v2;
    }
  }
  STORE_DETI_COINS();
  printf("deti_coins_cpu_special_search: %lu DETI coin%s found in %lu attempt%s (expected %.2f coins)\n",n_coins,(n_coins == 1ul) ? "" : "s",n_attempts,(n_attempts == 1ul) ? "" : "s",(double)n_attempts / (double)(1ul << 32));
}

#endif
