#include <linux/module.h>     
#include <linux/proc_fs.h> 
#include <linux/acpi.h> 

MODULE_AUTHOR("Lin Ming");
MODULE_LICENSE("GPL");

extern void acpi_db_batch_execute(char *arg_count);

struct proc_dir_entry *test_proc = NULL;

static int test_cache(char * buf, char ** start, off_t off,
                int count, int *eof, void *_data)
{
	int n = 0;
	cycles_t t0, t1;
	int loops = 1000;

	*eof = 1;

	rdtscll(t0);
	
	while (loops--)
		acpi_db_batch_execute ("100");
	
	rdtscll(t1);
	
	n = sprintf(buf, "Total TSC: %Ld\n", t1 - t0);

        return n;
}


static int __init simple_init(void)
{
	test_proc = create_proc_read_entry("test", S_IRUGO, NULL, test_cache, NULL);
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
