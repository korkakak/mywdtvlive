#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>

#if 1	// This is for Hut, made by Wistron

#define	LED_ON	1
#define	LED_OFF	0
#define	LED_RST_PWR_PIN		2
#define	LED_RST_STA_PIN		6
#define	LED_RST_RST_PIN		1

#define EM86XX_CHIP EM86XX_CHIPID_TANGO3
#include <linux/interrupt.h>
#include <asm/tango3/rmem86xxid.h>
#include <asm/tango3/rmdefs.h>
#include <asm/tango3/emhwlib_dram.h>
#include <asm/tango3/tango3_gbus.h>
#include <asm/tango3/tango3.h>
#include <asm/tango3/tango3api.h>
#include <asm/tango3/hardware.h>

static	int em86xx_uart1_get_gpio_mode(void)
{
	return gbus_read_reg32(REG_BASE_cpu_block + CPU_uart1_gpio_mode) & 0x7f;
}

static	int em86xx_uart1_set_gpio_mode(int mode)
{
	int old_mode = em86xx_uart1_get_gpio_mode();
	gbus_write_reg32(REG_BASE_cpu_block + CPU_uart1_gpio_mode, 0x7f00 | (mode & 0x7f));
	return(old_mode);
}

#if 0 //define in the linux-2.6.22.19/include/asm/tango3/tango3api.h
static	int em86xx_uart1_gpio_read(int gpio)
{
	return (gbus_read_reg32(REG_BASE_cpu_block + CPU_uart1_gpio_data) >> gpio) & 1;
}

static	void em86xx_uart1_gpio_write(int gpio, int data)
{
	gbus_write_reg32(REG_BASE_cpu_block + CPU_uart1_gpio_data,
			data ? UART_GPIO_DATA_SET(gpio) : UART_GPIO_DATA_CLEAR(gpio));
}

static	void em86xx_uart1_gpio_setdirection(int gpio, int dir)
{
	gbus_write_reg32(REG_BASE_cpu_block + CPU_uart1_gpio_dir,
			dir ? UART_GPIO_DIR_OUTPUT(gpio) : UART_GPIO_DIR_INPUT(gpio));
}
#endif
/* set GPIO output value -- assume direction has been set already */
static void set_gpio_out(int pin, int val)
{
	em86xx_uart1_gpio_write( pin, val );
}

/* get GPIO input value -- assume direction has been set already */
static int get_gpio_input(int pin)
{
	return em86xx_uart1_gpio_read( pin );
}

/* set GPIO direction  -- 1:output 0:input */
static void set_gpio_dir(int pin, int val)
{
	em86xx_uart1_set_gpio_mode( 0x7f );
	em86xx_uart1_gpio_setdirection( pin, val );

}

#endif	// This is for Hut, made by Wistron	


/*
 * Hardware independent below
 */

/* The major device number and name */
#define LED_RST_DEV_MAJOR	0
#define LED_RST_DEV_NAME	"led_rst"

#define DRIVER_VERSION         "0.1"

#define LED_RST_IOCSETPWR_ON		0x0001
#define LED_RST_IOCSETPWR_OFF		0x0002
#define LED_RST_IOCSETSTA_ON		0x0003
#define LED_RST_IOCSETSTA_OFF		0x0004
#define LED_RST_IOCSETSTA_BLINK_ON 	0x0005
#define LED_RST_IOCSETSTA_BLINK_OFF	0x0006
#define LED_RST_IOCSETPWR_BLINK_ON 	0x0007
#define LED_RST_IOCSETPWR_BLINK_OFF	0x0008

#define LED_RST_BUTTON_NONE	0x0
#define LED_RST_BUTTON_RESET	0x1
#define LED_RST_BUTTON_NTSC_PAL	0x2

MODULE_DESCRIPTION("TVDOCK Led and Reset button driver\n");
MODULE_AUTHOR("Steven Kuo");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

/* Wait queue, may be used if block mode is on */
//DECLARE_WAIT_QUEUE_HEAD(led_rst_wq);

/* Some prototypes */
static int led_rst_open(struct inode *, struct file *);
static int led_rst_release(struct inode *, struct file *);
static int led_rst_read(struct file *, char *, size_t, loff_t *);
static int led_rst_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
static unsigned int led_rst_poll(struct file *, struct poll_table_struct *);

/* Global data */
static char *led_rst_devname = LED_RST_DEV_NAME;
static int major_num = LED_RST_DEV_MAJOR;
module_param( major_num, int, 0);
static	wait_queue_head_t	read_wait;
static int led_rst_button = LED_RST_BUTTON_NONE;
static int led_rst_sta = 0;
static int led_rst_pwr = 0;
static int led_rst_sta_blink = 0;
static int led_rst_pwr_blink = 0;

static struct file_operations led_rst_fops = {
	open:		led_rst_open,
	read:		led_rst_read,
	ioctl:		led_rst_ioctl,
	poll:		led_rst_poll,
	release:	led_rst_release,
};


/* Reading data */
static int led_rst_read(struct file *fptr, char *bufptr, size_t size, loff_t *fp)
{
	if (wait_event_interruptible( read_wait, led_rst_button) < 0)
		return -1;

	/* Get the data to user */
	if (copy_to_user(bufptr, (char *)&led_rst_button, 1)) 
		return(-EFAULT);

	led_rst_button = LED_RST_BUTTON_NONE;
	return(1);
}

static int led_rst_thread(void *_data)
{
	int t1;
	int pwr_t = 0;
	int flag = 1;

	while (1) {
		t1 = 0;
#ifndef CONFIG_ALPHA_LED_GPIO
		if (get_gpio_input(LED_RST_RST_PIN) == 0) {
			msleep_interruptible(200);
			pwr_t++;
			if (led_rst_sta_blink) {
				led_rst_sta = !led_rst_sta;
				set_gpio_out(LED_RST_STA_PIN, led_rst_sta);
			}
			if (led_rst_pwr_blink && (pwr_t % 5) == 0) {
				led_rst_pwr = !led_rst_pwr;
				set_gpio_out(LED_RST_PWR_PIN, led_rst_pwr);
			}
			t1 = 1;
			while (get_gpio_input(LED_RST_RST_PIN) == 0) {
				msleep_interruptible(200);
				pwr_t++;
				if (led_rst_sta_blink) {
					led_rst_sta = !led_rst_sta;
					set_gpio_out(LED_RST_STA_PIN, led_rst_sta);
				}
				if (led_rst_pwr_blink && (pwr_t % 5) == 0) {
					led_rst_pwr = !led_rst_pwr;
					set_gpio_out(LED_RST_PWR_PIN, led_rst_pwr);
				}
				t1++;
				if (t1 > 10 * 5 && flag) {
					flag = 0;
					led_rst_button = LED_RST_BUTTON_NTSC_PAL;
					wake_up_interruptible( &read_wait );
					pwr_t++;
				}
			}
			flag = 1;
		}
		if (t1 > 0 && t1 < 3 * 5) {
			led_rst_button = LED_RST_BUTTON_RESET;
			wake_up_interruptible( &read_wait );
		}
#endif
		msleep_interruptible(200);
		if (kthread_should_stop())
			break;
		pwr_t++;
		if (led_rst_sta_blink) {
			led_rst_sta = !led_rst_sta;
			set_gpio_out(LED_RST_STA_PIN, led_rst_sta);
		}
		if (led_rst_pwr_blink && (pwr_t % 5) == 0) {
			led_rst_pwr = !led_rst_pwr;
			set_gpio_out(LED_RST_PWR_PIN, led_rst_pwr);
		}
		pwr_t %= 100;
	}

	return 0;
}

/* ioctl function */
static int led_rst_ioctl(struct inode *inode, struct file *fptr, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		case LED_RST_IOCSETPWR_ON:
			set_gpio_out(LED_RST_PWR_PIN, LED_ON);
			break;
		case LED_RST_IOCSETPWR_OFF:
			set_gpio_out(LED_RST_PWR_PIN, LED_OFF);
			break;
		case LED_RST_IOCSETSTA_ON:
			set_gpio_out(LED_RST_STA_PIN, LED_ON);
			led_rst_sta = LED_ON;
			break;
		case LED_RST_IOCSETSTA_OFF:
			set_gpio_out(LED_RST_STA_PIN, LED_OFF);
			led_rst_sta = LED_OFF;
			break;
		case LED_RST_IOCSETSTA_BLINK_ON:
			led_rst_sta_blink = 1;
			break;
		case LED_RST_IOCSETSTA_BLINK_OFF:
			led_rst_sta_blink = 0;
			set_gpio_out(LED_RST_STA_PIN, LED_OFF);
			break;
		case LED_RST_IOCSETPWR_BLINK_ON:
			led_rst_pwr_blink = 1;
			break;
		case LED_RST_IOCSETPWR_BLINK_OFF:
			led_rst_pwr_blink = 0;
			set_gpio_out(LED_RST_PWR_PIN, LED_OFF);
			break;
		default:
                        return(-EIO);
	}

	return(0);
}

/* Poll function */
static unsigned int led_rst_poll(struct file *fptr, struct poll_table_struct *ptable)
{
	poll_wait( fptr, &read_wait, ptable);

	if (led_rst_button != LED_RST_BUTTON_NONE) {
		return POLLIN | POLLRDNORM;
	}

	return 0;
}

/* Open the device */
static int led_rst_open(struct inode *inode_ptr, struct file *fptr)
{
	return(0);
}

/* Close the device */
static int led_rst_release(struct inode *inode_ptr, struct file *fptr) 
{
	return(0);
}

#define	STR_BLINK_ON		"status led blink on"
#define	STR_BLINK_OFF		"status led blink off"
#define	PWR_BLINK_ON		"power led blink on"
#define	PWR_BLINK_OFF		"power led blink off"
#define	STR_LED_ON		"status led on"
#define	STR_LED_OFF		"status led off"
#define	STR_POWER_LED_ON	"power led on"
#define	STR_POWER_LED_OFF	"power led off"
#define	STR_USB_POWER_OFF	"usb power off"
#define	STR_USB_POWER_ON	"usb power on"
#define	STR_SYS_READY		"system ready"
#define	STR_SYS_DOWN		"system down"

static	int	proc_write_led( struct file *file, const char *buffer, unsigned long count, void *data)
{
char	buf[128];
int	len;

	len = (count > sizeof(buf)) ? sizeof(buf) : count;
	copy_from_user(buf, buffer, len);

	if (!strncmp( buf, STR_SYS_READY, strlen(STR_SYS_READY))) {
		em86xx_gpio_setdirection( 2, 1 );
		em86xx_gpio_write( 2, 1 );
	} else if (!strncmp( buf, STR_SYS_DOWN, strlen(STR_SYS_DOWN))) {
		em86xx_gpio_setdirection( 2, 1 );
		em86xx_gpio_write( 2, 0 );
		em86xx_gpio_setdirection( 3, 1 );
		em86xx_gpio_write( 3, 0 );
	} else if (!strncmp( buf, STR_BLINK_ON, strlen(STR_BLINK_ON))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETSTA_BLINK_ON, 0);  
	} else if (!strncmp( buf, STR_BLINK_OFF, strlen(STR_BLINK_OFF))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETSTA_BLINK_OFF, 0);  
	} else if (!strncmp( buf, PWR_BLINK_ON, strlen(PWR_BLINK_ON))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETPWR_BLINK_ON, 0);  
	} else if (!strncmp( buf, PWR_BLINK_OFF, strlen(PWR_BLINK_OFF))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETPWR_BLINK_OFF, 0);  
	} else if (!strncmp( buf, STR_POWER_LED_ON, strlen(STR_POWER_LED_ON))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETPWR_ON, 0);  
	} else if (!strncmp( buf, STR_POWER_LED_OFF, strlen(STR_POWER_LED_OFF))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETPWR_OFF, 0);  
	} else if (!strncmp( buf, STR_LED_ON, strlen(STR_LED_ON))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETSTA_ON, 0);  
	} else if (!strncmp( buf, STR_LED_OFF, strlen(STR_LED_OFF))) {
		led_rst_ioctl( NULL, NULL, LED_RST_IOCSETSTA_OFF, 0);  
	} else if (!strncmp( buf, STR_USB_POWER_OFF, strlen(STR_USB_POWER_OFF))) {
		em86xx_gpio_write( 15, 0);
	} else if (!strncmp( buf, STR_USB_POWER_ON, strlen(STR_USB_POWER_ON))) {
		em86xx_gpio_write( 15, 1);
	}
	return count;
}

static	struct	proc_dir_entry *ent;
static	struct	task_struct	*kthread;
int __init led_rst_init_module(void)
{
	int status = 0;
	/* Turn on USB power control: GPIO 15 */
	em86xx_gpio_setdirection( 15, 1);
	em86xx_gpio_write( 15, 1);
	/* Set GPIO direction of Power LED to output */
	set_gpio_dir(LED_RST_PWR_PIN, 1);
	/* Set GPIO direction of Status LED to output */
	set_gpio_dir(LED_RST_STA_PIN, 1);
	/* Set GPIO direction of Reset button to input */
	set_gpio_dir(LED_RST_RST_PIN, 0);
	/* Register device, and may be allocating major# */
	status = register_chrdev(major_num, led_rst_devname, &led_rst_fops);
	if (status < 0) {
		printk(KERN_ERR "%s: cannot get major number\n", led_rst_devname); 
		return(status);
	} else if (major_num == 0)
		major_num = status;	/* Dynamic major# allocation */

	init_waitqueue_head( &read_wait );

	kthread = kthread_create( led_rst_thread, NULL, "ledd" );
	if (IS_ERR(kthread)) {
		return -1;
	}
	wake_up_process( kthread );

	ent = create_proc_entry( "led", S_IFREG | 0222, NULL );
	if (ent) {
		ent->nlink = 1;
		ent->write_proc = proc_write_led;
	}
	printk(KERN_INFO "SMP86xx %s (%d:0): driver loaded.\n", led_rst_devname, major_num);
	return(0);
}

void __exit led_rst_cleanup_module(void)
{
	if (ent)
		remove_proc_entry( "led", NULL );
	unregister_chrdev(major_num, led_rst_devname);
	kthread_stop( kthread );
	printk(KERN_INFO "%s: driver unloaded\n", led_rst_devname);
}

module_init(led_rst_init_module);
module_exit(led_rst_cleanup_module);

