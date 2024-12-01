#include <stdio.h>
#include <stdlib.h>

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
