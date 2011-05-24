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
#include "i2c_hal.h"

#define	PROC_LED		"led"

#define	I2C_DEVICE_ADDRESS	0x26
#define	G751_DEVICE_ADDR	0x90

#define	PROC_ROOT		"wdtvrv"

#define	PROC_VERSION		"version"

#define	PROC_FAN		"fan"

#define	PROC_LED_STATUS		"led_status"
#define	PROC_LED_ON_TIME	"led_on_time"
#define	PROC_LED_OFF_TIME	"led_off_time"

#define	PROC_POWER_OFF		"poweroff"

#define	PROC_RTC_SEC		"rtc_sec"
#define	PROC_RTC_MIN		"rtc_min"
#define	PROC_RTC_HOUR		"rtc_hour"
#define	PROC_RTC_DAY		"rtc_day"
#define	PROC_RTC_WEEK		"rtc_week"
#define	PROC_RTC_MONTH		"rtc_month"
#define	PROC_RTC_YEAR		"rtc_year"

#define	PROC_RUN		"run"
#define	PROC_SB			"sb"

#define	PROC_TEMP		"temperature"
#define	PROC_HI_TEMP		"hi_temp"
#define	PROC_LO_TEMP		"lo_temp"
#define	PROC_HI_SPEED		"hi_speed"
#define	PROC_LO_SPEED		"lo_speed"
#define	PROC_TEMP_PERIOD	"temp_period"

#define	TEMPERATURE	0x00	// (Read Only)


#define	PROC_GPIO_8654_BUTTON	"gpio_8654_button"
#define	PROC_GPIO_POWER_KEY	"power_key"
#define	GPIO_8654_BUTTON	2	// Low for pressed
#define	GPIO_POWER_KEY		5	// Low for pressed

static	int	g_751_present = 0;

static	int	temp_period = 10;	
static	int	hi_temp = 0;
static	int	lo_temp = 0;
static	int	hi_speed = 0;
static	int	lo_speed = 0;

static	struct	proc_dir_entry	*proc_ent_root;

static	struct	task_struct	*kthread;

#define LED_RST_BUTTON_NONE	0x0
#define LED_RST_BUTTON_RESET	0x1
#define LED_RST_BUTTON_NTSC_PAL	0x2
#define LED_RST_DEV_NAME	"led_rst"
static char *led_rst_devname = LED_RST_DEV_NAME;
static int major_num = 0;
module_param( major_num, int, 0);
static	wait_queue_head_t	read_wait;
static int led_rst_button = LED_RST_BUTTON_NONE;

static void temp_checking_timer(unsigned long arg)
{
unsigned char low_byte, high_byte;
unsigned short temp;
char	value;

	if (!hi_temp || !lo_temp)
		return;

	if (I2C_Read(G751_DEVICE_ADDR, TEMPERATURE, (unsigned char *)&temp, 2) < 0) {
		printk(KERN_WARNING "%s:%d, Unable to read temperature\n", __FUNCTION__, __LINE__);
		return;
	}
	low_byte = temp >> 8;
	high_byte = temp & 0xff;
	temp = (high_byte << 1) | (low_byte >> 7);
	temp = (temp > 0xff) ? (-0x200+temp)/2 : temp/2;

	if (temp >= hi_temp) {
		value = hi_speed;
		I2C_Write( I2C_DEVICE_ADDRESS, 0x7, &value, 1 );  
		//printk(KERN_WARNING "%s:%d, Temperature is over %d. Set FAN speed to %d\n", __FUNCTION__, __LINE__, hi_temp, hi_speed );
	}
	if (temp <= lo_temp) {
		value = lo_speed;
		I2C_Write( I2C_DEVICE_ADDRESS, 0x7, &value, 1 );  
		//printk(KERN_WARNING "%s:%d, Temperature is under %d. Set FAN speed to %d\n", __FUNCTION__, __LINE__, lo_temp, lo_speed );
	}
}

static int gpio_monitor_thread(void *_data)
{
unsigned char data;
int	button_pressed_count = 0;
int	counter = 0;
	while(1) {
		counter++;

		if (g_751_present && !(counter % temp_period))
			temp_checking_timer(0);

		data = em86xx_gpio_read( GPIO_8654_BUTTON );
		if (!data) {
			printk(KERN_WARNING "pressed\n" );
			button_pressed_count++;
		} else {
			if (button_pressed_count > 0 && button_pressed_count < 10) {
				printk( KERN_WARNING "RESET is triggered\n");
				led_rst_button = LED_RST_BUTTON_RESET;
				wake_up_interruptible( &read_wait );

			} 
			if (button_pressed_count > 9) {
				printk( KERN_WARNING "NTSC PAL is triggered\n");
				led_rst_button = LED_RST_BUTTON_NTSC_PAL;
				wake_up_interruptible( &read_wait );
			}
			button_pressed_count = 0;
		}

		data = em86xx_gpio_read( GPIO_POWER_KEY );
		if (!data) {
			printk(KERN_WARNING "POWER BUTTON pressed. Shut down the device!\n");
			printk(KERN_WARNING "POWER BUTTON pressed. Shut down the device!\n");
			printk(KERN_WARNING "POWER BUTTON pressed. Shut down the device!\n");
			I2C_Write( I2C_DEVICE_ADDRESS, 1, &data, 1); 
			printk(KERN_WARNING "You should never see this on console.\n");
			printk(KERN_WARNING "You should never see this on console.\n");
			printk(KERN_WARNING "You should never see this on console.\n");
		}
		current->state = TASK_UNINTERRUPTIBLE;
		schedule_timeout( HZ );
		if (kthread_should_stop())
			break;
	}
	return 0;
}


static int proc_int_read_generic(char *page, char **start, off_t off, int count, int *eof, void *priv)
{
	int	ret = sprintf( page + off, "%d", *((int *)priv) );
	*eof = 1;
	return ret;
}


static int proc_int_write_generic(struct file *file, const char __user *buffer, unsigned long count, void *priv)
{
	*((int *)priv) = simple_strtol(buffer, NULL, 10);
	return count;
}

static int proc_gpio_read_generic(char *page, char **start, off_t off, int count, int *eof, void *priv)
{
	unsigned char	data = 0;
	int	ret;
	//em86xx_gpio_setdirection( *(unsigned char *)priv, 1);
	data = em86xx_gpio_read( *(unsigned char *)priv );
	ret = sprintf( page + off, "0x%x", data );
	*eof = 1;
	return ret;
}


static int proc_gpio_write_generic(struct file *file, const char __user *buffer, unsigned long count, void *priv)
{
	int cmd = simple_strtol(buffer, NULL, 10);
	em86xx_gpio_setdirection( *(unsigned char *)priv, 1);
	if (cmd)
		em86xx_gpio_write( *(unsigned char *)priv, 1);
	else
		em86xx_gpio_write( *(unsigned char *)priv, 0);
	return count;
}

static int proc_temp_read(char *page, char **start, off_t off, int count, int *eof, void *priv)
{
	int ret = 0;
	unsigned char low_byte, high_byte;
	unsigned short temp;

	if (I2C_Read(G751_DEVICE_ADDR, TEMPERATURE, (unsigned char *)&temp, 2) < 0) {
		ret = sprintf(page + off, "0");
		*eof = 1;
		return ret;
	}

	low_byte = temp >> 8;
	high_byte = temp & 0xff;

	temp = (high_byte << 1) | (low_byte >> 7);

	if (temp > 0xff)
		ret = sprintf(page+off, "%d", (-0x200+temp)/2);
	else
		ret = sprintf(page+off, "%d", temp/2);
	*eof = 1;
	return ret;
}

static int proc_read_generic(char *page, char **start, off_t off, int count, int *eof, void *priv)
{
	unsigned char	data = 0;
	int	ret;
	I2C_Read( I2C_DEVICE_ADDRESS, *(unsigned char *)priv, &data, 1 ); 
	ret = sprintf( page + off, "0x%x", data );
	*eof = 1;
	return ret;
}


static int proc_write_generic(struct file *file, const char __user *buffer, unsigned long count, void *priv)
{
	int cmd;
	unsigned char value;

	cmd = simple_strtol(buffer, NULL, 10);
	printk("cmd = %d\n", cmd);
	value = cmd;
	I2C_Write( I2C_DEVICE_ADDRESS, *(unsigned char *)priv, &value, 1); 
	return count;
}

/* Open the device */
static int led_rst_open(struct inode *inode_ptr, struct file *fptr)
{
	return(0);
}

/* Close the device */
static int led_rst_release(struct inode *inode_ptr, struct file *fptr) 
{
	return 0;
}
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
static unsigned int led_rst_poll(struct file *fptr, struct poll_table_struct *ptable)
{
	poll_wait( fptr, &read_wait, ptable);

	if (led_rst_button != LED_RST_BUTTON_NONE) {
		return POLLIN | POLLRDNORM;
	}

	return 0;
}
static struct file_operations led_rst_fops = {
	open:		led_rst_open,
	read:		led_rst_read,
	poll:		led_rst_poll,
	release:	led_rst_release,
};

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
char	value;

	len = (count > sizeof(buf)) ? sizeof(buf) : count;
	copy_from_user(buf, buffer, len);

	if (!strncmp( buf, STR_SYS_READY, strlen(STR_SYS_READY))) {
	} else if (!strncmp( buf, STR_SYS_DOWN, strlen(STR_SYS_DOWN))) {
	} else if (!strncmp( buf, STR_BLINK_ON, strlen(STR_BLINK_ON))) {
		value = 2;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, STR_BLINK_OFF, strlen(STR_BLINK_OFF))) {
		value = 1;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, PWR_BLINK_ON, strlen(PWR_BLINK_ON))) {
		value = 2;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, PWR_BLINK_OFF, strlen(PWR_BLINK_OFF))) {
		value = 1;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, STR_POWER_LED_ON, strlen(STR_POWER_LED_ON))) {
		value = 1;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, STR_POWER_LED_OFF, strlen(STR_POWER_LED_OFF))) {
		value = 0;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, STR_LED_ON, strlen(STR_LED_ON))) {
		value = 1;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, STR_LED_OFF, strlen(STR_LED_OFF))) {
		value = 0;
		I2C_Write( I2C_DEVICE_ADDRESS, 8 , &value, 1); 
	} else if (!strncmp( buf, STR_USB_POWER_OFF, strlen(STR_USB_POWER_OFF))) {
	} else if (!strncmp( buf, STR_USB_POWER_ON, strlen(STR_USB_POWER_ON))) {
	}
	return count;
}

static int __init wdtv_rv_i2c_dev_init_module(void)
{
struct	proc_dir_entry	*ent;
char	*subadd;
int	status;

	status = register_chrdev(major_num, led_rst_devname, &led_rst_fops);
	if (status < 0) {
		printk(KERN_ERR "%s: cannot get major number\n", led_rst_devname); 
		return(status);
	} else if (major_num == 0)
		major_num = status;	/* Dynamic major# allocation */

	init_waitqueue_head( &read_wait );

	if (!I2C_Device_Detect( I2C_DEVICE_ADDRESS ))
		printk(KERN_WARNING "Found WT69P3 device\n");
	else {
		printk(KERN_WARNING "Can't find WT69P3 device\n");
		unregister_chrdev( major_num, led_rst_devname );
		return -EINVAL;
	}

	if (!I2C_Device_Detect(G751_DEVICE_ADDR)) {
		printk(KERN_WARNING "Found G751 thermal sensor IC!\n");
		g_751_present = 1;
	} else {
		printk(KERN_WARNING "G751 not found!\n");
	}

	proc_ent_root = proc_mkdir( PROC_ROOT, NULL);
	if (!proc_ent_root) {
		printk(KERN_WARNING "Unable to create proc entry: %s\n", PROC_ROOT );
		return -EINVAL;
	}

	if ((ent = create_proc_entry( PROC_POWER_OFF, S_IFREG | 0222, proc_ent_root ))) {
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x01;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_VERSION, S_IFREG | 0444, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x00;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_FAN, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x07;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_LED_STATUS, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x08;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_LED_ON_TIME, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x09;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_LED_OFF_TIME, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x0A;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RTC_SEC, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x21;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RTC_MIN, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x22;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RTC_HOUR, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x23;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RTC_DAY, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x24;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RTC_WEEK, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x25;
		ent->data = (void *)subadd;
	}


	if ((ent = create_proc_entry( PROC_RTC_MONTH, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x26;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RTC_YEAR, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_read_generic;
		ent->write_proc = proc_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 0x27;
		ent->data = (void *)subadd;
	}

	if ((ent = create_proc_entry( PROC_RUN, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_gpio_read_generic;
		ent->write_proc = proc_gpio_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 6;
		ent->data = (void *)subadd;
		em86xx_gpio_setdirection( 6, 1 );
		em86xx_gpio_write( 6, 1 );
	}

	if ((ent = create_proc_entry( PROC_SB, S_IFREG | 0666, proc_ent_root ))) {
		ent->read_proc = proc_gpio_read_generic;
		ent->write_proc = proc_gpio_write_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = 14;
		ent->data = (void *)subadd;

		em86xx_gpio_setdirection( 14, 1 );
		em86xx_gpio_write( 14, 0 );
	}


	if (g_751_present) {
		if ((ent = create_proc_entry( PROC_TEMP, S_IFREG | 0444, proc_ent_root ))) { 
			ent->read_proc = proc_temp_read;
			subadd = (char *)kmalloc(1, GFP_KERNEL);
		}

		if ((ent = create_proc_entry( PROC_HI_TEMP, S_IFREG | 0666, proc_ent_root ))) { 
			ent->read_proc = proc_int_read_generic;
			ent->write_proc = proc_int_write_generic;
			ent->data = (void *)&hi_temp;
		}

		if ((ent = create_proc_entry( PROC_LO_TEMP, S_IFREG | 0666, proc_ent_root ))) {
			ent->read_proc = proc_int_read_generic;
			ent->write_proc = proc_int_write_generic;
			ent->data = (void *)&lo_temp;
		}

		if ((ent = create_proc_entry( PROC_HI_SPEED, S_IFREG | 0666, proc_ent_root ))) {
			ent->read_proc = proc_int_read_generic;
			ent->write_proc = proc_int_write_generic;
			ent->data = (void *)&hi_speed;
		}

		if ((ent = create_proc_entry( PROC_LO_SPEED, S_IFREG | 0666, proc_ent_root ))) {
			ent->read_proc = proc_int_read_generic;
			ent->write_proc = proc_int_write_generic;
			ent->data = (void *)&lo_speed;
		}

		if ((ent = create_proc_entry( PROC_TEMP_PERIOD, S_IFREG | 0666, proc_ent_root ))) {
			ent->read_proc = proc_int_read_generic;
			ent->write_proc = proc_int_write_generic;
			ent->data = (void *)&temp_period;
		}
	}

	if ((ent = create_proc_entry( PROC_GPIO_8654_BUTTON, S_IFREG | 0444, proc_ent_root ))) {
		ent->read_proc = proc_gpio_read_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = GPIO_8654_BUTTON;
		ent->data = (void *)subadd;
		em86xx_gpio_setdirection( GPIO_8654_BUTTON, 0 );
	}

	if ((ent = create_proc_entry( PROC_GPIO_POWER_KEY, S_IFREG | 0444, proc_ent_root ))) {
		ent->read_proc = proc_gpio_read_generic;
		subadd = (char *)kmalloc(1, GFP_KERNEL);
		*subadd = GPIO_POWER_KEY;
		ent->data = (void *)subadd;
		em86xx_gpio_setdirection( GPIO_POWER_KEY, 0 );
	}

	remove_proc_entry( PROC_LED, NULL );
	if ((ent = create_proc_entry( PROC_LED, S_IFREG | 0222, NULL))) {
		ent->write_proc = proc_write_led;
	}

	kthread = kthread_create( gpio_monitor_thread, NULL, "gpio monitor" );
	wake_up_process( kthread );


	return 0;
}

static void __exit wdtv_rv_i2c_dev_cleanup_module(void) 
{
	printk("wdtv_rv_i2c_dev_cleanup_module\n");
	if (proc_ent_root) {
		remove_proc_entry( PROC_VERSION, proc_ent_root );
		remove_proc_entry( PROC_FAN, proc_ent_root );
		remove_proc_entry( PROC_LED_STATUS, proc_ent_root );
		remove_proc_entry( PROC_LED_ON_TIME, proc_ent_root );
		remove_proc_entry( PROC_LED_OFF_TIME, proc_ent_root );
		remove_proc_entry( PROC_POWER_OFF, proc_ent_root );
		remove_proc_entry( PROC_RTC_SEC, proc_ent_root );
		remove_proc_entry( PROC_RTC_MIN, proc_ent_root );
		remove_proc_entry( PROC_RTC_HOUR, proc_ent_root );
		remove_proc_entry( PROC_RTC_DAY, proc_ent_root );
		remove_proc_entry( PROC_RTC_WEEK, proc_ent_root );
		remove_proc_entry( PROC_RTC_MONTH, proc_ent_root );
		remove_proc_entry( PROC_RTC_YEAR, proc_ent_root );
		remove_proc_entry( PROC_RUN, proc_ent_root );
		remove_proc_entry( PROC_SB, proc_ent_root );
		if (g_751_present) {
			remove_proc_entry( PROC_TEMP, proc_ent_root );
			remove_proc_entry( PROC_HI_TEMP, proc_ent_root );
			remove_proc_entry( PROC_LO_TEMP, proc_ent_root );
			remove_proc_entry( PROC_HI_SPEED, proc_ent_root );
			remove_proc_entry( PROC_LO_SPEED, proc_ent_root );
			remove_proc_entry( PROC_TEMP_PERIOD, proc_ent_root );
		}
		remove_proc_entry( PROC_GPIO_8654_BUTTON, proc_ent_root );
		remove_proc_entry( PROC_GPIO_POWER_KEY, proc_ent_root );
		remove_proc_entry( PROC_ROOT, NULL ); 
	}	
	remove_proc_entry( "led", NULL );

	kthread_stop( kthread );
	unregister_chrdev( major_num, led_rst_devname );
}

module_init(wdtv_rv_i2c_dev_init_module);
module_exit(wdtv_rv_i2c_dev_cleanup_module);

