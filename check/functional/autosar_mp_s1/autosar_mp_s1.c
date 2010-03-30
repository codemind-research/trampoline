#include "Os.h"
#include "tpl_app_define.h"

#if TARGET_ARCH == ARCH_POSIX
  #include <stdio.h>
#endif

/**
 * OS_CODE
 *
 */
#define OS_START_SEC_CODE
#include "tpl_memmap.h"
FUNC(int, OS_CODE) main(void)
{
	StartOS(OSDEFAULTAPPMODE);
	return 0;
}
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, OS_VAR) var_os = 0;
#define OS_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

DeclareEvent(Event1);
DeclareTask(t1_app_nontrusted1);
DeclareTask(t1_app_nontrusted2);
DeclareTask(t1_app_trusted1);
DeclareTask(t1_app_trusted2);

/**
 * Vars
 *
 */
#define APP_OS_APP_app_nontrusted1_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, OS_APPL_DATA) var_app_nontrusted1 = 0;
#define APP_OS_APP_app_nontrusted1_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"
#define APP_OS_APP_app_nontrusted2_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, OS_APPL_DATA) var_app_nontrusted2 = 0;
#define APP_OS_APP_app_nontrusted2_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"
#define APP_OS_APP_app_trusted1_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, OS_APPL_DATA) var_app_trusted1 = 0;
#define APP_OS_APP_app_trusted1_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"
#define APP_OS_APP_app_trusted2_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, OS_APPL_DATA) var_app_trusted2 = 0;
#define APP_OS_APP_app_trusted2_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

#define APP_Task_t1_app_nontrusted1_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, AUTOMATIC) var_t1_app_nontrusted1 = 0;
#define APP_Task_t1_app_nontrusted1_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"
#define APP_Task_t1_app_trusted1_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, AUTOMATIC) var_t1_app_trusted1 = 0;
#define APP_Task_t1_app_trusted1_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

/**
 * t1_app_nontrusted1
 *
 */
#define APP_Task_t1_app_nontrusted1_START_SEC_CODE
#include "tpl_memmap.h"
TASK(t1_app_nontrusted1)
{
#if TARGET_ARCH == ARCH_POSIX
  printf("1.t1_app_nontrusted1 \n");
#endif
  
  /* OS086 : read/write OS application's data section from same OS application -> YES
   - read
   - write   
   */
  var_app_nontrusted1++; /* if just read, the linker discards the variable... */
  
  /* OS026-OS207 : read/write other OS application's data section from non-trusted OS application -> NO
   - trusted - read
   - trusted - write
   - non-trusted - read
   - non-trusted - write
  */
  var_t1_app_nontrusted1 = var_app_trusted1;
  // TODO : CALL PROTECTION_HOOK
  var_app_trusted1 = var_t1_app_nontrusted1;
  // TODO : CALL PROTECTION_HOOK
  var_t1_app_nontrusted1 = var_app_nontrusted2;
  // TODO : CALL PROTECTION_HOOK
  var_app_nontrusted2 = var_t1_app_nontrusted1;
  // TODO : CALL PROTECTION_HOOK
  
  /* OS198 : (read/)write OS's data section from non-trusted OS application -> NO
   - read
   - write   
   */
  var_app_nontrusted1 = var_os;
  // TODO : CALL PROTECTION_HOOK
  var_os = var_app_nontrusted1;
  // TODO : CALL PROTECTION_HOOK
  
  WaitEvent(Event1);
  
#if TARGET_ARCH == ARCH_POSIX
  printf("4.t1_app_nontrusted1 - Event received -> ShutdownOS \n");
#endif

  ShutdownOS(E_OK);
}
#define APP_Task_t1_app_nontrusted1_STOP_SEC_CODE
#include "tpl_memmap.h"

/**
 * t1_app_nontrusted2
 *
 */
#define APP_Task_t1_app_nontrusted2_START_SEC_CODE
#include "tpl_memmap.h"
TASK(t1_app_nontrusted2)
{
  /* We shouldn't be here but it's just to force the variable to be in the mapping */
  var_app_nontrusted2++;
  
}
#define APP_Task_t1_app_nontrusted2_STOP_SEC_CODE
#include "tpl_memmap.h"

/**
 * t1_app_trusted1
 *
 */
#define APP_Task_t1_app_trusted1_START_SEC_CODE
#include "tpl_memmap.h"
TASK(t1_app_trusted1)
{
#if TARGET_ARCH == ARCH_POSIX
  printf("2.t1_app_trusted1 \n");
#endif
  /* OS086 : read/write OS application's data section from same OS application -> YES
   - read
   - write   
   */
  var_app_trusted1++; /* if just read, the linker discards the variable... */
  
  /* read/write other OS application's data section from trusted OS application -> YES (OS026-OS207)
   - trusted - read
   - trusted - write
   - non-trusted - read
   - non-trusted - write
   */
  var_t1_app_trusted1 = var_app_trusted2;
  var_app_trusted2 = var_t1_app_trusted1;
  var_t1_app_trusted1 = var_app_nontrusted1;
  var_app_nontrusted1 = var_t1_app_trusted1;
  
  /* (read/)write OS's data section from non-trusted OS application -> YES (OS198)
   - read
   - write   
   */
  var_app_trusted1 = var_os;
  var_os = var_app_trusted1;
  
  SetEvent(t1_app_nontrusted1, Event1);
  
#if TARGET_ARCH == ARCH_POSIX
  printf("3.t1_app_trusted1 - Event set\n");
#endif  
  
  TerminateTask();
  
}
#define APP_Task_t1_app_trusted1_STOP_SEC_CODE
#include "tpl_memmap.h"

/**
 * t1_app_trusted2
 *
 */
#define APP_Task_t1_app_trusted2_START_SEC_CODE
#include "tpl_memmap.h"
TASK(t1_app_trusted2)
{
  /* We shouldn't be here but it's just to force the variable to be in the mapping */
  var_app_trusted2++;
  
}
#define APP_Task_t1_app_trusted2_STOP_SEC_CODE
#include "tpl_memmap.h"