
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/slab.h>

struct task_struct *task1;
struct task_struct *task2;
int data;

int thread_function_one(void *data)
{
    int ret = 10;
    int cpu;
    printk(KERN_INFO "IN THREAD FUNCTION 1 \n");

    while(!kthread_should_stop()){
       cpu = get_cpu();
       put_cpu();
       printk("t1 cpu = %d\n",cpu);
       msleep(5000);
    }
    printk(KERN_INFO "EXIT from thread function 1\n");
    return ret;
}

int thread_function_two(void *data)
{
    int ret = 10;
    int cpu;
    printk(KERN_INFO "IN THREAD FUNCTION 2 \n");

    while(!kthread_should_stop()){
        cpu = get_cpu();
        put_cpu();
        printk("t2 cpu = %d\n",cpu);
        msleep(6000);
    }
    printk(KERN_INFO "EXIT from thread function 2\n");
    return ret;
}

static int __init kernel_init(void)
{
   int cpu;
   printk(KERN_INFO "module_init\n");

   cpu = get_cpu();
   put_cpu();
   printk("main thread cpu = %d \n",cpu);

   cpu = 6;
   task1 = kthread_create(&thread_function_one,(void *)&data,"one");
   kthread_bind(task1, cpu);
   wake_up_process(task1);

   cpu = 5;
   task2 = kthread_create(&thread_function_two,(void *)&data,"two");
   kthread_bind(task2, cpu);
   wake_up_process(task2);

   return 0;
}

static void  __exit kernel_exit(void)
{
   kthread_stop(task1);
   kthread_stop(task2);
   printk(KERN_INFO "module_exit\n");
}

module_init(kernel_init);
module_exit(kernel_exit);
MODULE_AUTHOR("K_K");
MODULE_LICENSE("GPL");