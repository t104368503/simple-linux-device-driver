#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define EXAMPLE_MAJOR 60
#define EXAMPLE_NAME "ldd_hello_world"

#define IOCTL_EMPTY _IO(0xCE, 0)
#define IOCTL_SYNC  _IO(0xCE, 1)
#define IOCTL_NAME  _IOW(0xCE, 2, char *)

static const char    g_s_Hello_World_string[] = "Hello world Ray Yang from kernel mode!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);

static uint8_t databuff[20];
static const ssize_t databuff_size = sizeof(databuff);

static unsigned long IOCNTARG;


MODULE_LICENSE("GPL");

static void example_exit(void) {

	unregister_chrdev(EXAMPLE_MAJOR, "EXAMPLE_NAME");
    
	printk("<1>EXAMPLE: exit\n");
}

static int example_open(struct inode *inode, struct file *filp) {
    printk("<1>EXAMPLE: open\n");
    return 0;
}

static int example_close(struct inode *inode, struct file *filp) {
    printk("<1>EXAMPLE: close\n");
    return 0;
}

static ssize_t example_read(
        struct file *filp
        , char *buf
        , size_t size
        , loff_t *f_pos) 
{

    printk("<1>EXAMPLE: read\n");
    
    if (*f_pos >= databuff_size)
        return 0;

    if( *f_pos + size > databuff_size )
       size = databuff_size - *f_pos;

    if( copy_to_user(buf, databuff + *f_pos, size) != 0 )
        return -EFAULT;   

    //if ( *f_pos >= g_s_Hello_World_size )
    //    return 0;
    
    //if( *f_pos + size > g_s_Hello_World_size )
    //    size = g_s_Hello_World_size - *f_pos;

    //if( copy_to_user(buf, g_s_Hello_World_string + *f_pos, size) != 0 )
    //    return -EFAULT;   
    
    *f_pos += size;
    
    return size;
}

static ssize_t example_write(
        struct file *filp
        , const char *buf
        , size_t size
        , loff_t *f_pos) 
{
    
    size_t pos;
    
    uint8_t byte;
    
    printk("<1>EXAMPLE: write  (size=%zu)\n", size);
    
    for (pos = 0; pos < size; ++pos) {
        
        if (copy_from_user(&byte, buf + pos, 1) != 0) {
             break;
        }
        // printk("<1>EXAMPLE: write  (buf[%zu] = %02x)\n", pos, (unsigned)byte);

        printk("<1>EXAMPLE: write  (buf[%zu] = %c)\n", pos, (unsigned)byte);
        databuff[pos] = (unsigned)byte;
    }
    
    return pos;
	
}


static long example_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)

{
                    
    switch (cmd) {
        
        case IOCTL_EMPTY:
            IOCNTARG = arg;
            printk("<1>EXAMPLE ioctl: IOCTL_EMPTY %ld\n",IOCNTARG);
           
        break;
        
        case IOCTL_SYNC:
            IOCNTARG = arg;
            printk("<1>EXAMPLE ioctl: IOCTL_SYNC %ld\n",IOCNTARG);
        break;
                                                        
        case IOCTL_NAME:
        
        break;
        
        default:
            return -ENOTTY;
    }
                                                                                        
    return 0;

}


static struct file_operations example_fops = {
    .open = example_open,
    .release = example_close,
    .read = example_read,
    .write = example_write,
    .unlocked_ioctl = example_ioctl

};

static int example_init(void) {
    
	int result;
    printk("<1>EXAMPLE: init\n");

    /* Register character device */
    result = register_chrdev(EXAMPLE_MAJOR, EXAMPLE_NAME, &example_fops);
    if (result < 0) {
        printk("<1>EXAMPLE: Failed to register character device\n");
        return result;
    }

    return 0;
}

module_init(example_init);
module_exit(example_exit);
