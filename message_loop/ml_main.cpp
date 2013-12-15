#include <objbase.h>
#include "base/message_loop.h"
#include "base/at_exit.h"

int main() {
  base::AtExitManager at_exit_mgr;
  MessageLoop msg_loop;
  msg_loop.Run();

  return 0;
} 
