/* 
 * vim:cindent:ts=8:
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>


#define EM86XX_CHIP EM86XX_CHIPID_TANGO3
#include <linux/interrupt.h>
#include <asm/tango3/rmem86xxid.h>
#include <asm/tango3/rmdefs.h>
#include <asm/tango3/emhwlib_dram.h>
#include <asm/tango3/tango3_gbus.h>
#include <asm/tango3/tango3.h>
#include <asm/tango3/tango3api.h>
#include <asm/tango3/hardware.h>

/*
 * Hardware independent below
 */

/* The major device number and name */
#define DRIVER_VERSION         "0.1"
#define	LED_RST_DEV_NAME	"led_rst"
#define	LED_RST_DEV_MAJOR	0

MODULE_DESCRIPTION("LMP555 GPIO\n");
MODULE_AUTHOR("William Chen");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

#define	JM_RESET_PIN	2
#define	POWER_LED_PIN	3
#define	SATA_USB_PIN	10
#define	JM_MODE_PIN	4
#define	FAN_PIN		15
#define	BOARD_RESET_SW_PIN	5
#define	THERMAL_INT_PIN	11
#define	CPU_RUNNING_PIN	6	// high: CPU mode, IrDA signal gets by CPU; low: microP mode, IrDA gets by microP
#define	POWER_OFF_PIN	14	// high: microP cuts power to CPU. Default: low

#define	LED_POWER_ON	0
#define	LED_POWER_OFF	1
#define	JM_RESET	0
#define	JM_NORNAL	1
#define	JM_USB_MODE	1
#define	JM_SATA_MODE	0
#define	FAN_ON		1
#define	FAN_OFF		0
#define	CPU_RUNNING	1
#define	CPU_STOP	0
#define	POWER_ON	0
#define	POWER_OFF	1

#define GPIO_VAL_MASK	(0xFF000000)
#define GPIO_BLINK_ONCE	(0x01000000)


#define	STR_LED_PWR_ON		"power led on"
#define	STR_LED_PWR_OFF		"power led off"
#define	STR_LED_PWR_BLINK_ONCE	"power led blink once"
#define	STR_JM_RESET		"jm reset"
#define	STR_JM_NORMAL		"jm normal"
#define	STR_JM_USB_MODE		"usb mode"
#define	STR_JM_SATA_MODE	"sata mode"

#define	STR_SATA_ENSLAVED	"SATA_enslaved"
#define	STR_SATA_FREE		"SATA_is_free"

#define STR_LED_BLINK_FAST  	"led blink fast"
#define STR_LED_BLINK_STD	"led blink std"
#define STR_LED_BLINK_SLOW	"led blink slow"

#define	STR_FAN_ON		"fan on"
#define	STR_FAN_OFF		"fan off"

#define	STR_CPU_RUNNING		"cpu_run"
#define	STR_CPU_STOP		"cpu_stop"

#define	STR_POWER_ON		"power_on"
#define	STR_POWER_OFF		"power_off"

#define	LED_RST_BUTTON_NONE	0x0
#define	LED_RST_BUTTON_RESET	0x1
#define	LED_RST_BUTTON_NTSC_PAL	0x2
#define	LED_RST_BUTTON_STANDBY	0x4
#define	LED_RST_BUTTON_POWER	0x8

static int led_rst_open(struct inode *, struct file *);
static int led_rst_release(struct inode *, struct file *);
static int led_rst_read(struct file *, char *, size_t, loff_t *);
static int led_rst_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
static unsigned int led_rst_poll(struct file *, struct poll_table_struct *);

static char *led_rst_devname = LED_RST_DEV_NAME;
static int major_num = LED_RST_DEV_MAJOR;
module_param( major_num, int, 0);
static	wait_queue_head_t	read_wait;
static int led_rst_button = LED_RST_BUTTON_NONE;

static struct file_operations led_rst_fops = {
	open:		led_rst_open,
	read:		led_rst_read,
	ioctl:		led_rst_ioctl,
	poll:		led_rst_poll,
	release:	led_rst_release,
};

/* ioctl function */
static int led_rst_ioctl(struct inode *inode, struct file *fptr, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
		default:
                        return	-EFAULT;
	}

	return 0;
}

/* Reading data */
static int led_rst_read(struct file *fptr, char *bufptr, size_t size, loff_t *fp)
{
	if (wait_event_interruptible( read_wait, led_rst_button) < 0)
		return -1;

	/* Get the data to user */
	if (copy_to_user(bufptr, (char *)&led_rst_button, 1)) 
		return(-EFAULT);

	led_rst_button = LED_RST_BUTTON_NONE;

	return 1;
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


struct	IO_table_t	{
	char	*label_string;
	int	gpio;
	int	val;
	int	period;
	int	blink;
	int	current_val;
} IO_table[] = {
	{	STR_LED_PWR_ON,			POWER_LED_PIN,		LED_POWER_ON,	0	},
	{	STR_LED_PWR_OFF,		POWER_LED_PIN,		LED_POWER_OFF,	0	},
	{	STR_LED_PWR_BLINK_ONCE,		POWER_LED_PIN,		LED_POWER_OFF| GPIO_BLINK_ONCE | (LED_POWER_ON << 8) ,	0	},
	{	STR_JM_RESET,			JM_RESET_PIN,		JM_RESET,	0	},
	{	STR_JM_NORMAL,			JM_RESET_PIN,		JM_NORNAL,	0	},
	{	STR_JM_USB_MODE,		JM_MODE_PIN,		JM_USB_MODE,	0	},
	{	STR_JM_SATA_MODE,		JM_MODE_PIN,		JM_SATA_MODE,	0	},
#if 0
	{	STR_LED_BLINK_FAST,		POWER_LED_PIN,		-1,		1	},
	{	STR_LED_BLINK_STD,		POWER_LED_PIN,		-1,		2	},
	{	STR_LED_BLINK_SLOW,		POWER_LED_PIN,		-1,		4	},
#endif
	{	STR_FAN_ON,			FAN_PIN,		FAN_ON,		0	},
	{	STR_FAN_OFF,			FAN_PIN,		FAN_OFF,	0	},

	{	STR_CPU_RUNNING,		CPU_RUNNING_PIN,	CPU_RUNNING,	0	},
	{	STR_CPU_STOP,			CPU_RUNNING_PIN,	CPU_STOP,	0	},

	{	STR_POWER_ON,			POWER_OFF_PIN,		POWER_ON,	0	},
	{	STR_POWER_OFF,			POWER_OFF_PIN,		POWER_OFF,	0	},
};

static int led_thread(void *_data)
{
	int	reset_pin_pressed_count = 0;
	int	counter = 0;
	int	i = 0;
	while(1) {
		if (!em86xx_gpio_read(THERMAL_INT_PIN)) {
			/* temperature higher than the T_OS */
			// TODO: speedup FAN?
			em86xx_gpio_write( FAN_PIN, FAN_ON );
		} else {
			/* temperature lower than the T_HYSTERESIS */
			// TODO: speeddown FAN?
			em86xx_gpio_write( FAN_PIN, FAN_OFF );
		}
		
		if (!em86xx_gpio_read(BOARD_RESET_SW_PIN) && (reset_pin_pressed_count < 20)) {
			reset_pin_pressed_count++;
		} else {
			if (reset_pin_pressed_count >= 20) { //  >= 5 seconds
				printk("System Reset!\n");
				led_rst_button = LED_RST_BUTTON_RESET;
				wake_up_interruptible(&read_wait);

			} else if (reset_pin_pressed_count >= 8) { // >= 2 seconds
				printk("System Standby!\n");
				led_rst_button = LED_RST_BUTTON_STANDBY;
				wake_up_interruptible(&read_wait);
			} else if (reset_pin_pressed_count >= 1) { // < 2 seconds
				printk("System send GPIO Power key!\n");
				led_rst_button = LED_RST_BUTTON_POWER;
				wake_up_interruptible(&read_wait);
			}

			reset_pin_pressed_count = 0;
		}

		counter++;
		msleep_interruptible(250);

		for (i=0;i<sizeof(IO_table)/sizeof(struct IO_table_t); i++) {	
			if (IO_table[i].blink && !(counter % IO_table[i].period)) {
				IO_table[i].current_val = !(IO_table[i].current_val);
				em86xx_gpio_write(IO_table[i].gpio, IO_table[i].current_val);
			}
		}
		if (kthread_should_stop())
			break;
	}
	return 0;
}

static	int	proc_write_led( struct file *file, const char *buffer, unsigned long count, void *data)
{
	char	buf[128];
	int	len;
	int	i;

	len = (count > sizeof(buf)) ? sizeof(buf) : count;
	copy_from_user(buf, buffer, len);

	for (i=0;i<sizeof(IO_table)/sizeof(struct IO_table_t); i++) {
		if (!strncmp( buf, IO_table[i].label_string, strlen( IO_table[i].label_string ))) {
			if (IO_table[i].period) {
				if (IO_table[i].blink){
					IO_table[i].blink = 0;
					IO_table[i].current_val = 0;
					em86xx_gpio_write( POWER_LED_PIN, IO_table[i].current_val);
				}
				else
					IO_table[i].blink = 1;
			} else {
				switch (IO_table[i].val & GPIO_VAL_MASK) {
					case GPIO_BLINK_ONCE:	// Blink once, use 1st value(0xFF), and then 2nd value(0xFF00)
						em86xx_gpio_write( IO_table[i].gpio, IO_table[i].val & 0xFF );
						msleep_interruptible(100);
						em86xx_gpio_write( IO_table[i].gpio, (IO_table[i].val & 0xFF00) >> 8 );
						break;
					default:
						em86xx_gpio_write( IO_table[i].gpio, IO_table[i].val );
						break;
				}
			}
		}
	}
	return count;
}

static	int	proc_read_led(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	extern	int	cache_size;
	int	ret;
	if (em86xx_gpio_read( SATA_USB_PIN ))
		ret = sprintf( page + off, "%s", STR_SATA_FREE );
	else
		ret = sprintf( page + off, "%s", STR_SATA_ENSLAVED );
	*eof = 1;
	return ret;
}

static	struct	proc_dir_entry *ent;
static	struct	task_struct	*kthread;
int __init led_rst_init_module(void)
{
	int status = 0;

	em86xx_gpio_setdirection( POWER_LED_PIN, 1);
	em86xx_gpio_write( POWER_LED_PIN, LED_POWER_ON);

	em86xx_gpio_setdirection( JM_RESET_PIN, 1);
	em86xx_gpio_write( JM_RESET_PIN, JM_NORNAL);

	em86xx_gpio_setdirection( JM_MODE_PIN, 1);
	em86xx_gpio_write( JM_MODE_PIN, JM_SATA_MODE );


	em86xx_gpio_setdirection( SATA_USB_PIN, 0);

	em86xx_gpio_setdirection( FAN_PIN, 1);
	em86xx_gpio_write( FAN_PIN, FAN_ON );

	em86xx_gpio_setdirection(CPU_RUNNING_PIN, 1);	// output mode
	em86xx_gpio_write(CPU_RUNNING_PIN, CPU_RUNNING);   

	em86xx_gpio_setdirection(POWER_OFF_PIN, 1);	// output mode

	em86xx_gpio_setdirection(BOARD_RESET_SW_PIN, 0);
	em86xx_gpio_setdirection(THERMAL_INT_PIN, 0);

	/* Register device, and may be allocating major# */
	status = register_chrdev(major_num, led_rst_devname, &led_rst_fops);
	if (status < 0) {
		printk(KERN_ERR "%s: cannot get major number\n", led_rst_devname); 
		return(status);
	} else if (major_num == 0)
		major_num = status;	/* Dynamic major# allocation */

	init_waitqueue_head( &read_wait );

#if 1
	kthread = kthread_create( led_thread, NULL, "ledd" );
	if (IS_ERR(kthread)) {
		return -1;
	}
	wake_up_process( kthread );
#endif

	ent = create_proc_entry( "led", S_IFREG | 0222, NULL );
	if (ent) {
		ent->nlink = 1;
		ent->write_proc = proc_write_led;
		ent->read_proc = proc_read_led;
	}
	printk(KERN_INFO "SMP865x LMP GPIO: %s(%d:0) driver loaded.\n", led_rst_devname, major_num);
	return(0);
}

void __exit led_rst_cleanup_module(void)
{
	if (ent)
		remove_proc_entry( "led", NULL );
	unregister_chrdev(major_num, led_rst_devname);
	kthread_stop( kthread );
	printk(KERN_INFO "SMP865x LMP GPIO driver unloaded\n");
}

module_init(led_rst_init_module);
module_exit(led_rst_cleanup_module);

