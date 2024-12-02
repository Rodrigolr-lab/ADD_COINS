//
// Tomás Oliveira e Silva,  October 2024
//
// Arquiteturas de Alto Desempenho 2024/2025
//
// deti_coins_cpu_search() --- find DETI coins using md5_cpu()
//


#include <ctime>
#include <time.h>
#include <stdio.h>
#include <md5.h>

typedef unsigned int u32_t;
typedef unsigned long u64_t;
typedef unsigned int u32_t;

u32_t next_value_to_try_ASCII(u32_t v){

  v = v + 1; // adicionar 1 ao LSB
  if ((v & 0x000000FF) != 0x0000007F) return v; // nao deu overflow
  v += 0x000000A1; // adicionar 1 ao proximo byte

  if ((v & 0x0000FF00) != 0x00007F00) return v;
  v += 0x0000A100;

  if ((v & 0x00FF0000) != 0x007F0000) return v;
  v += 0x00A10000;

  if ((v & 0xFF000000) != 0x7F000000) return v;
  v += 0xA1000000;

  return 0x20202020; // se deu overflow em todos, retornar tudo com espacos
}

int main(void){
  u32_t idx, coin[13u],hash[4u],v1, v2;
  u64_t n_attempts,n_coins;
  clock() t;
  // TODO: iniciar moeda exemplo

  //
  // arbitrary, but printable utf-8 data terminated with a '\n' is hightly desirable
  //
  for(idx = 10u;idx < 13u * 4u - 1u;idx++)
    bytes[idx] = ' ';
  //
  // mandatory termination
  //
  bytes[13u * 4u -  1u] = '\n';
  //
  // find DETI coins
  //
  for(n_attempts = n_coins = 0ul;stop_request == 10000000000ul;n_attempts++)
  {
    //
    // compute MD5 hash
    //
    md5_cpu(coin,hash);

    // one message -> one MD5 hash
    u32_t a,b,c,d,state[4],x[16];
    # define C(c)         (c)
    # define ROTATE(x,n)  (((x) << (n)) | ((x) >> (32 - (n))))
    # define DATA(idx)    data[idx]
    # define HASH(idx)    hash[idx]
    # define STATE(idx)   state[idx]
    # define X(idx)       x[idx]
    CUSTOM_MD5_CODE();
    # undef C
    # undef ROTATE
    # undef DATA
    # undef HASH
    # undef STATE
    # undef X
    if(hash[3] == 0)
    {
      for (i = 0; i < 13; i++)
        printf("0x%08X\n", coin[i]);
      printf("\n");
      n_coins++;
    }
    //
    // try next combination (byte range: 0x20..0x7E)
    //
    // next_value_to_try_ASCII(v1); coin[11] = v1;
    
    v1 = v2 = 0x20202020;
    v1 = next_value_to_try_ASCII(v1);
    coin[11] = v1;
    if (v1 == 0x20202020) {
      v2 = next_value_to_try_ASCII(v2);
      coin[12] = v2;
    }
  }
  t = clock() * t;
  printf("deti_coins_web: %lu DETI coin%s found in %lu attempt%s (expected %.2f coins) %.3f\n",n_coins,(n_coins == 1ul) ? "" : "s",n_attempts,(n_attempts == 1ul) ? "" : "s",(double)n_attempts / (double)(1ul << 32), (double)t / (double)CLOCKS_PER_SEC);
}

#endif
