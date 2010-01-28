#include <linux/module.h>     
#include <linux/proc_fs.h> 
#include <linux/acpi.h> 

MODULE_AUTHOR("Lin Ming");
MODULE_LICENSE("GPL");

#define STOP  0
#define START 1
#define CLEAR 2

struct debug_data {
	unsigned long total_tsc;
	unsigned long count;
};

DECLARE_PER_CPU(struct debug_data, my_debug_data);
extern int my_debug_start;

struct proc_dir_entry *test_proc = NULL;

static int test_proc_read(char * buf, char ** start, off_t off,
                int count, int *eof, void *_data)
{
	int n = 0;
	int cpu;
	struct debug_data *data;

	*eof = 1;

	n += sprintf(buf+n, "state: %d\n", my_debug_start);
		
	for_each_online_cpu(cpu) {
		n += sprintf(buf+n, "CPU %d\t", cpu);
		data = &per_cpu(my_debug_data, cpu);
		n += sprintf(buf+n, "Total TSC: %ld\tCount: %ld\tAvg: %ld\n", 
			data->total_tsc, data->count,
			data->count ? data->total_tsc / data->count : 0);
	}

        return n;
}

static int test_proc_write(struct file *file, const char *buffer,
                             unsigned long count, void *_data)
{
        char buf[2];
        unsigned long len = min(sizeof(buf) - 1, count);
        char *p = (char *)buf;
        unsigned long val;
	int cpu;
	struct debug_data *data;

        if (copy_from_user(buf, buffer, len))
                return count;
        buf[len] = 0;
        val = simple_strtoul(p, &p, 10);
        if (p == buf) {
                printk("%s is not in decimal form.\n", buf);
        	return strnlen(buf, count);
	}

	if (val & START)
		my_debug_start = 1;	
	else
		my_debug_start = 0;	

	if (val & CLEAR) {
		for_each_online_cpu(cpu) {
			data = &per_cpu(my_debug_data, cpu);
			data->total_tsc = 0;
			data->count = 0;
		}
	}

        return strnlen(buf, count);
}


static int __init simple_init(void)
{
	test_proc = create_proc_entry("test", S_IFREG | S_IRUGO | S_IWUSR | S_IWGRP | S_IWOTH, NULL);
	if (!test_proc) {
		printk("create /proc/test fail\n");
	} else {
		test_proc->read_proc = test_proc_read;
		test_proc->write_proc = test_proc_write;
		test_proc->data = NULL;
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
