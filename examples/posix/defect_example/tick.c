#include <stdio.h>

int v1 = 0;
int st = 0;


void tick()
{
  v1++;
  printf("Activation #%d\r\n",v1);
      
}

void tick2()
{
    st++;
    printf("Tick #%d\r\n",st);
}
