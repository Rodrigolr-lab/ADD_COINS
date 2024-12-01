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

typedef union
{
  u32_t coin_as_ints[13];  // 13 4-byte integers
  char coin_as_chars[53];  // 53=13*4+1 bytes
}
coin_t;


static void deti_coins_cpu_special_search(void)
{
  u32_t idx,hash[4u];
  u64_t n_attempts,n_coins;
  u08_t *bytes;

  //
  // iniciar a estrutura
  //
  static u32_t test_data[13u * 4u] __attribute__((aligned(16)));
  static u32_t test_hash[ 4u * 4u] __attribute__((aligned(16)));

  //
  // iniciar valores da coin
  //
  coin_t coin;
  int n;

  if((n = snprintf(coin.coin_as_chars,53,"DETI coin %d ............RODRIGO...................\n",12)) != 52)
  {
    fprintf(stderr,"not exactly 52 bytes... (n=%d)\n",n);
    exit(1);
  }
  // print coin content
  for(n = 0;n < 13 * 4;n++){
    // guardar a coin na test_data?
    test_data[n] = coin.coin_as_ints[n];
    printf("%2d %3d [%c]\n",n,coin.coin_as_chars[n],coin.coin_as_chars[n]);
  }
  //
  // find DETI coins
  //
  for(n_attempts = n_coins = 0ul;stop_request == 0;n_attempts++)
  {
    // para computar MD5 HASH com AVX
    // md5_cpu_avx((v4si *)interleaved_test_data,(v4si *)interleaved_test_hash);

    //
    // compute MD5 hash
    //
    for (int lane = 0; lane < 4; lane++){
      // u32_t *
      md5_cpu((u32_t *)test_data[13u * lane], (u32_t*)test_hash[13u * lane]);
    }

    //
    // byte-reverse each word (that's how the MD5 message digest is printed...)
    //
    hash_byte_reverse(hash);
    //
    // count the number of trailing zeros of the MD5 hash
    //
    n = deti_coin_power(hash);
    //
    // if the number of trailing zeros is >= 32 we have a DETI coin
    //
    if(n >= 32u)
    {
      save_deti_coin(coin);
      n_coins++;
    }

    //
    // try next combination (byte range: 0x20..0x7E)
    //
    // for(idx = 10u;idx < 13u * 4u - 1u && bytes[idx] == (u08_t)126;idx++)
    //   bytes[idx] = ' ';
    // if(idx < 13u * 4u - 1u)
    //   bytes[idx]++;
    u32_t v1 = v2 = 0x20202020;
    v1 = next_value_to_try_ASCII(v1);
    if (v1 == 0x20202020) {
    v2 = next_value_to_try_ASCII(v2);
    }
  }
  STORE_DETI_COINS();
  printf("deti_coins_cpu_special_search: %lu DETI coin%s found in %lu attempt%s (expected %.2f coins)\n",n_coins,(n_coins == 1ul) ? "" : "s",n_attempts,(n_attempts == 1ul) ? "" : "s",(double)n_attempts / (double)(1ul << 32));
}

#endif
