#include <linux/module.h>     
#include <linux/proc_fs.h> 
#include <linux/acpi.h> 

MODULE_AUTHOR("Lin Ming");
MODULE_LICENSE("GPL");

struct debug_data {
        unsigned long sibling;
        unsigned long mc;
        unsigned long cpu;
        unsigned long node;
};

DECLARE_PER_CPU(struct debug_data, my_debug_data);

struct proc_dir_entry *test_proc = NULL;

static int test_proc_read(char * buf, char ** start, off_t off,
                int count, int *eof, void *_data)
{
	int n = 0;
	int cpu;
	struct debug_data *data;

	*eof = 1;

	for_each_online_cpu(cpu) {
		data = &per_cpu(my_debug_data, cpu);
		n += sprintf(buf+n, "sibling: %ld\t\tmc: %ld\t\tcpu: %ld\t\tnode: %ld\t\t\n", 
			data->sibling, data->mc,
			data->cpu, data->node); 
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
