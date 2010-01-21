#include <linux/module.h>     
#include <linux/proc_fs.h> 
#include <linux/acpi.h> 

MODULE_AUTHOR("Lin Ming");
MODULE_LICENSE("GPL");

enum func_id {
        resched_cpu_id,
        set_user_nice_id,
        normalize_task_id,
        check_preempt_tick_id,
        entity_tick_id,
        hrtick_start_fair_id,
        check_preempt_wakeup_id,
        task_fork_fair_id,
        prio_changed_fair_id,
        switched_to_fair_id,
        check_preempt_curr_idle_id,
        switched_to_idle_id,
        prio_changed_idle_id,
        sched_rt_rq_enqueue_id,
        update_curr_rt_id,
        check_preempt_equal_prio_id,
        check_preempt_curr_rt_id,
        push_rt_task_id,
        switched_to_rt_id,
        prio_changed_rt_id,
        task_new_fair_id,
        func_id_max
};

char *func_names[func_id_max] = {
        "resched_cpu",
        "set_user_nice",
        "normalize_task",
        "check_preempt_tick",
        "entity_tick",
        "hrtick_start_fair",
        "check_preempt_wakeup",
        "task_fork_fair",
        "prio_changed_fair",
        "switched_to_fair",
        "check_preempt_curr_idle",
        "switched_to_idle",
        "prio_changed_idle",
        "sched_rt_rq_enqueue",
        "update_curr_rt",
        "check_preempt_equal_prio",
        "check_preempt_curr_rt",
        "push_rt_task",
        "switched_to_rt",
        "prio_changed_rt",
        "task_new_fair"
};

DECLARE_PER_CPU(struct debug_data, my_debug_data);

struct proc_dir_entry *test_proc = NULL;

static int test_proc_read(char * buf, char ** start, off_t off,
                int count, int *eof, void *_data)
{
	int n = 0;
	int cpu;
	struct debug_data *data;
	char *name;
	int i;

	*eof = 1;

	n += sprintf(buf+n, "\t");
	for_each_online_cpu(cpu) {
		n += sprintf(buf+n, "%d\t", cpu);
	}
	n += sprintf(buf+n, "\n");

	for (i = 0; i < func_id_max; i++) {		
		n += sprintf(buf+n, "%s: ", func_names[i]);
		
		for_each_online_cpu(cpu) {
			data = &per_cpu(my_debug_data, cpu);
			n += sprintf(buf+n, "%ld\t", data->count[i]);
		}

		n += sprintf(buf+n, "\n");
	}

        return n;
}


static int __init simple_init(void)
{
	test_proc = create_proc_read_entry("test", S_IRUGO, NULL, test_proc_read, NULL);
	if (!test_proc) {
		printk("create /proc/test fail\n");
	}
	
	return 0;
}

static void simple_cleanup(void)
{
	if (test_proc)
		remove_proc_entry("test", NULL);
}


module_init(simple_init);
module_exit(simple_cleanup);
