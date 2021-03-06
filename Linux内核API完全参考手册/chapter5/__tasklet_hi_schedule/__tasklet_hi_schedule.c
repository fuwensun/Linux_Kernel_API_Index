/********************************************************************************
* File Name:	__tasklet_hi_schedule.c
* Description:	第5章实例训练          
* Reference book:《Linux内核API完全参考手册》邱铁，周玉，邓莹莹 ，机械工业出版社.2010.9  
* E_mail:openlinux2100@gmail.com			                
*
********************************************************************************/

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/init.h>
MODULE_LICENSE("GPL");
static unsigned long data=0;
static struct tasklet_struct tasklet,tasklet1;

//自定义软中断处理函数，在此只是显示的作用
static void irq_tasklet_action(unsigned long data)
{
	printk("<0>in irq_tasklet_action the state of the tasklet is :%ld\n",(&tasklet)->state);
  	printk("<0>tasklet running. by author\n");
   	return;
}
//自定义软中断处理函数，在此只是显示的作用
static void irq_tasklet_action1(unsigned long data)
{
	printk("<0>in irq_tasklet_action1 the state of the tasklet1 is :%ld\n",(&tasklet1)->state);
  	printk("<0>tasklet1 running. by author\n");
   	return;
}

static int  __init __tasklet_hi_schedule_init(void) 
{
  	printk("<0>into __tasklet_hi_schedule\n");
         
	//申请两个软中断 
	tasklet_init(&tasklet,irq_tasklet_action,data);
	tasklet_init(&tasklet1,irq_tasklet_action1,data);
	printk("<0>The state of the tasklet is :%ld\n",(&tasklet)->state);  //显示当前中断的状态
	printk("<0>The state of the tasklet1 is :%ld\n",(&tasklet1)->state);
	tasklet_schedule(&tasklet);               //把中断送入普通中断队列
	if(!test_and_set_bit(TASKLET_STATE_SCHED, &tasklet1.state))
		__tasklet_hi_schedule(&tasklet1); //调用函数__tasklet_hi_schedule()把中断送入高优先级队列，
       //tasklet_schedule(&tasklet1);        
   	tasklet_kill(&tasklet);                  //等待中断处理函数执行完毕，恢复调度之前的状态
	tasklet_kill(&tasklet1);
	printk("<0>out __tasklet_hi_schedule\n");
	return 0; 
}

static void  __exit __tasklet_hi_schedule_exit(void)
{
	printk("<0>Goodbye __tasklet_hi_schedule\n");
 	return;
}

module_init(__tasklet_hi_schedule_init);
module_exit(__tasklet_hi_schedule_exit);
