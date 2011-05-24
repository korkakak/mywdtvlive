#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>

#define	LED_ON	0
#define	LED_OFF	1
#define	LED_PWR_PIN		6
#define	LED_ACC_PIN		5
#define	LED_STB_PIN		3

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

MODULE_DESCRIPTION("AVLS-700L Led driver\n");
MODULE_AUTHOR("Steven Kuo");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

/* Global data */
//static int led_pwr_blink = 0;
static int led_acc_blink = 0;
static int led_acc = 0;

static int led_thread(void *_data)
{
	while (1) {
		msleep_interruptible(500);
		if (kthread_should_stop())
			break;
		if (led_acc_blink == 0)
			continue;
		led_acc = !led_acc;
		em86xx_gpio_write(LED_ACC_PIN, led_acc);
	}

	return 0;
}

#define	LED_ACC_BLINK_ON	"status led blink on"
#define	LED_ACC_BLINK_OFF	"status led blink off"

#define	LED_PWR0_ON		"power0 led on"
#define	LED_PWR0_OFF		"power0 led off"
#define	LED_PWR_ON		"power led on"
#define	LED_PWR_OFF		"power led off"
#define	LED_ACC_ON		"status led on"
#define	LED_ACC_OFF		"status led off"
#define	LED_STB_ON		"standby led on"
#define	LED_STB_OFF		"standby led off"

/*
 * Power: Blue
 * Standby: Red
 * Access: Green
 * Link: Yellow
 *
 */

static	int	proc_write_led( struct file *file, const char *buffer, unsigned long count, void *data)
{
char	buf[128];
int	len;

	len = (count > sizeof(buf)) ? sizeof(buf) : count;
	copy_from_user(buf, buffer, len);

	if (!strncmp( buf, LED_PWR_ON, strlen(LED_PWR_ON))) {
		em86xx_gpio_write(LED_STB_PIN, LED_OFF); 
		em86xx_gpio_write(LED_PWR_PIN, LED_ON); 
	} else if (!strncmp( buf, LED_PWR_OFF, strlen(LED_PWR_OFF))) {
		em86xx_gpio_write(LED_STB_PIN, LED_ON); 
		em86xx_gpio_write(LED_PWR_PIN, LED_OFF); 
	} else if (!strncmp( buf, LED_ACC_ON, strlen(LED_ACC_ON))) {
		em86xx_gpio_write(LED_ACC_PIN, LED_ON); 
	} else if (!strncmp( buf, LED_ACC_OFF, strlen(LED_ACC_OFF))) {
		em86xx_gpio_write(LED_ACC_PIN, LED_OFF); 
	} else if (!strncmp( buf, LED_ACC_BLINK_ON, strlen(LED_ACC_BLINK_ON))) {
		led_acc_blink = 1;
	} else if (!strncmp( buf, LED_ACC_BLINK_OFF, strlen(LED_ACC_BLINK_OFF))) {
		led_acc_blink = 0;
		em86xx_gpio_write(LED_ACC_PIN, LED_OFF); 
	} else if (!strncmp( buf, LED_STB_ON, strlen(LED_STB_ON))) {
		em86xx_gpio_write(LED_STB_PIN, LED_ON); 
	} else if (!strncmp( buf, LED_STB_OFF, strlen(LED_STB_OFF))) {
		em86xx_gpio_write(LED_STB_PIN, LED_OFF); 
	} else if (!strncmp( buf, LED_PWR0_ON, strlen(LED_PWR0_ON))) {
		em86xx_gpio_write(LED_PWR_PIN, LED_ON); 
	} else if (!strncmp( buf, LED_PWR0_OFF, strlen(LED_PWR0_OFF))) {
		em86xx_gpio_write(LED_PWR_PIN, LED_OFF); 
	}
	return count;
}

static	struct	proc_dir_entry *ent;
static	struct	task_struct	*kthread;
int __init led_rst_init_module(void)
{
	/* Set GPIO direction of Power LED to output */
	em86xx_gpio_setdirection(LED_PWR_PIN, 1);
	em86xx_gpio_write(LED_PWR_PIN, LED_ON); 
	/* Set GPIO direction of Access LED to output */
	em86xx_gpio_setdirection(LED_ACC_PIN, 1);
	em86xx_gpio_write(LED_ACC_PIN, LED_OFF); 
	/* Set GPIO direction of Standby LED to output */
	em86xx_gpio_setdirection(LED_STB_PIN, 1);
	em86xx_gpio_write(LED_STB_PIN, LED_OFF); 
	kthread = kthread_create( led_thread, NULL, "ledd" );
	if (IS_ERR(kthread)) {
		return -1;
	}
	wake_up_process( kthread );

	ent = create_proc_entry( "led", S_IFREG | 0222, NULL );
	if (ent) {
		ent->nlink = 1;
		ent->write_proc = proc_write_led;
	}
	printk(KERN_INFO "SMP86xx AV-LS500LE LEDs driver loaded.\n");
	return(0);
}

void __exit led_rst_cleanup_module(void)
{
	if (ent)
		remove_proc_entry( "led", NULL );
//	kthread_stop( kthread );
	printk(KERN_INFO "SMP86xx AV-LS500LE LEDs driver unloaded\n");
}

module_init(led_rst_init_module);
module_exit(led_rst_cleanup_module);

