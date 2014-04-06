#include "base/message_loop.h"
#include "base/at_exit.h"
#include "base/threading/thread.h"
#include "base/callback.h"
#include "base/bind.h"
#include <stdio.h>

void Foo() {
    printf("thread working: Foo\n");
};

class SimpleTaskRunner {
public:
    void Start() {
        thread_ = new base::Thread("SimpleTaskRunner");
        thread_->Start();
        message_loop_proxy_ = thread_->message_loop_proxy();
    }

    void Stop() {
        thread_->Stop();
    }

    void PostTask(const base::Closure& task) {
        message_loop_proxy_->PostTask(FROM_HERE, task);
    }

    void PostDelayedTask(const base::Closure& task) {
        message_loop_proxy_->PostDelayedTask(FROM_HERE, task, base::TimeDelta::FromMilliseconds(2000));
    }

private:
    base::Thread* thread_;
    scoped_refptr<base::MessageLoopProxy> message_loop_proxy_;
};

int main() {
  base::AtExitManager at_exit_mgr;

  SimpleTaskRunner* simple_task_runner = new SimpleTaskRunner();
  simple_task_runner->Start();
  simple_task_runner->PostTask(base::Bind(&Foo));
  //simple_task_runner->PostDelayedTask(base::Bind(&Foo));
  Sleep(1000);
  simple_task_runner->Stop();

  return 0;
} 
