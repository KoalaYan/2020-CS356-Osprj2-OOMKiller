/*
 *  linux/arch/arm/kernel/init_task.c
 */
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/mqueue.h>
#include <linux/uaccess.h>

#include <asm/pgtable.h>

static struct signal_struct init_signals = INIT_SIGNALS(init_signals);
static struct sighand_struct init_sighand = INIT_SIGHAND(init_sighand);
/*
 * Initial thread structure.
 *
 * We need to make sure that this is 8192-byte aligned due to the
 * way process stacks are handled. This is done by making sure
 * the linker maps this in the .text segment right after head.S,
 * and making head.S ensure the proper alignment.
 *
 * The things we do for performance..
 */
union thread_union init_thread_union __init_task_data =
	{ INIT_THREAD_INFO(init_task) };

/*
 * Initial task structure.
 *
 * All other task structs will be allocated on slabs in fork.c
 */
struct task_struct init_task = INIT_TASK(init_task);
struct MMLimits my_mm_limits = 
{
	.num_entry = 0,
};

void MMLimits_add(struct MMLimits *ml, uid_t uid, unsigned long mm_max){
	int i;
	for(i = 0;i < ml->num_entry;i++){
		if(ml->id[i] == uid){
			ml->limits[i] = mm_max;
			break;
		}
	}
	if(i == ml->num_entry){
		if(ml->num_entry < max_entry){
			ml->id[i] = uid;
			ml->limits[i] = mm_max;
		}
		else{
			printk("out of bound, cannot add the limitation.\n");
		}
	}
}

void MMLimits_traverse(struct MMLimits *ml){
	int i;
	for(i = 0;i < ml->num_entry;i++){
		printk("uid=%d, mm_max=%d", ml->id[i], ml->limits[i]);
	}
}

unsigned long MMLimits_search(struct MMLimits *ml, uid_t uid){
	int i;
	for(i = 0;i < ml->num_entry;i++){
		if(ml->id[i] == uid)
			return ml->limits[i];
	}
	if(i == ml->num_entry)
		return 0;
}

EXPORT_SYMBOL(init_task);
EXPORT_SYMBOL(my_mm_limits);
EXPORT_SYMBOL(MMLimits_add);
EXPORT_SYMBOL(MMLimits_traverse);
EXPORT_SYMBOL(MMLimits_search);

