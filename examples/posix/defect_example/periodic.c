#include <stdio.h>
#include "tpl_os.h"

int v1 = 0;
int st = 0;

int main(void)
{
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

DeclareAlarm(one_second);

TASK(my_periodic_task)
{
  static int occurence = 0;
    
  occurence++;
  v1++;
  printf("Activation #%d %d\r\n",occurence,v1);
    
  TerminateTask();
}

TASK(stop)
{
  CancelAlarm(one_second);
  printf("Shutdown\r\n");
  ShutdownOS(E_OK);
  TerminateTask();
}
