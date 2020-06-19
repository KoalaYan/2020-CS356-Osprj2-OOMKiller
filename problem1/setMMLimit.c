#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/unistd.h>

MODULE_LICENSE("Dual BSD/GPL");
#define __NR_smlcall 59

static int (*oldcall)(void);

static int setMMLimit(uid_t uid, unsigned long mm_max)
{
	printk("syscall is invoked. And uid is %d, mm_max is %lu.\n",uid ,mm_max);

	//uid: the id of the user that we want to set memory limit for
	//mm_max: maximum amount of physical memory the user can use(in bytes)
	
	MMLimits_add(&my_mm_limits, uid, mm_max);
	MMLimits_traverse(&my_mm_limits);
	
	return 0;
}

static int addsyscall_init(void)
{
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[__NR_smlcall]);
	syscall[__NR_smlcall] = (unsigned long )setMMLimit;
	printk(KERN_INFO "module load!\n");
	return 0;
}

static void addsyscall_exit(void)
{
	long *syscall = (long*)0xc000d8c4;
	syscall[__NR_smlcall] = (unsigned long )oldcall;
	printk(KERN_INFO "module exit\n");
}


module_init(addsyscall_init);
module_exit(addsyscall_exit);
