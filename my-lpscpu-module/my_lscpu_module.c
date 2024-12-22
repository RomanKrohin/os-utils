#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <asm/processor.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Module to expose CPU information via /proc");
MODULE_VERSION("1.0");

#define PROC_NAME "my_lscpu"

static int my_lscpu_show(struct seq_file *m, void *v) {
    struct file *f;
    char *buf;
    loff_t pos = 0;
    ssize_t len;

    buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!buf) {
        seq_printf(m, "Error: failed to allocate memory\n");
        return -ENOMEM;
    }

    f = filp_open("/proc/cpuinfo", O_RDONLY, 0);
    if (IS_ERR(f)) {
        seq_printf(m, "Error opening /proc/cpuinfo\n");
        kfree(buf);
        return PTR_ERR(f);
    }

    len = kernel_read(f, buf, PAGE_SIZE - 1, &pos);
    if (len < 0) {
        seq_printf(m, "Error reading /proc/cpuinfo\n");
        filp_close(f, NULL);
        kfree(buf);
        return len;
    }

    buf[len] = '\0';
    seq_printf(m, "%s", buf);

    filp_close(f, NULL);
    kfree(buf);

    return 0;
}



static int my_lscpu_open(struct inode *inode, struct file *file) {
    return single_open(file, my_lscpu_show, NULL);
}

static const struct proc_ops proc_file_ops = {
    .proc_open = my_lscpu_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init my_lscpu_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_file_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit my_lscpu_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(my_lscpu_init);
module_exit(my_lscpu_exit);
