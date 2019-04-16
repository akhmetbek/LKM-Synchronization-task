#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h> 	//kthreads 
#include <linux/delay.h> 	//ssleep 
#include <linux/signal.h>	//signals  
#include <linux/semaphore.h>

static struct task_struct *t_thread, *g_thread; 
struct semaphore turn;

int take_thread(void *unused){
	printk("the take thread was generated firstly\n");
	
	while(!kthread_should_stop()){
		down(&turn);	
		printk("take\n"); 
	}
	printk(KERN_INFO "take thread stopping\n"); 
	do_exit(0); 
	return 0; 
}

int give_thread(void *unused){
	printk("the give thread was generated secondly\n");
	while(!kthread_should_stop()){
		up(&turn);		
		printk("give\n"); 
		ssleep(1);
	}
	printk(KERN_INFO "give thread stopping\n"); 
	do_exit(0); 
	return 0; 
}

static int __init lkm1_init(void){
	printk("kthread init called\n"); 
	sema_init(&turn, 0);

	t_thread = kthread_create(take_thread, NULL, "take_thread"); 
	if (t_thread){
		wake_up_process(t_thread); 
		printk(KERN_ERR "thread create success\n"); 
	}else{
		printk(KERN_ERR "can't create thread\n"); 
	}
	
	g_thread = kthread_create(give_thread, NULL, "give_thread"); 
	if (g_thread){
		wake_up_process(g_thread); 
		printk(KERN_ERR "thread create success\n"); 
	}else{
		printk(KERN_ERR "can't create thread\n"); 
	} 


	return 0; 
}

static void __exit lkm1_exit(void){
	printk("lkm exit called\n");
	kthread_stop(t_thread);
 	kthread_stop(g_thread);
}

module_init(lkm1_init);
module_exit(lkm1_exit); 

MODULE_LICENSE("GPL"); 
