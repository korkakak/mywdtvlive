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

#define	G751_DEVICE_ADDR	0x90

/* Register MAP */ 
#define	TEMPERATURE	0x00	// (Read Only)
#define	CONFIGURATION	0x01	// (Read/Write)
#define	T_HYST		0x02	// (Read/Write)
#define T_OS		0x03	// (Read/Write)	

MODULE_AUTHOR("Steven Wang <Steven_Wang@alphanetworks.com>");
MODULE_DESCRIPTION("Temperature Sensor driver for G751");
MODULE_LICENSE("GPL");

/* Names used in proc_fs */
#define	PROC_ROOT		"thermal"
#define	PROC_CONFIGURATION	"configuration"
#define	PROC_TEMPERATURE	"temperature"
#define	PROC_T_OS		"t_os"
#define	PROC_T_HYSTERESIS	"t_hysteresis"

static struct proc_dir_entry *root = NULL, *conf_ent = NULL, *temp_ent = NULL, *t_os_ent = NULL, *t_hyst_ent = NULL;

static void g751_configuration_load(void)
{
	unsigned char data;

	if (I2C_Read(G751_DEVICE_ADDR, CONFIGURATION, &data, 1) < 0) {
		printk("G751: can't load configuration!\n");
		return;
	}

	printk("D0:Shutdown: %d\n", data&0x01);
	printk("D1:Comparator/Interrupt mode: %d\n", (data>>1)&0x01);
	printk("D2:OS Polarity: %d\n", (data>>2)&0x01);
	if (((data>>3)&0x3) == 0x00)
		printk("D3-D4:Fault Queue: 1\n"); 
	else if (((data>>3)&0x3) == 0x01)
		printk("D3-D4:Fault Queue: 2\n"); 
	else if (((data>>3)&0x3) == 0x02)
		printk("D3-D4:Fault Queue: 4\n"); 
	else if (((data>>3)&0x3) == 0x03)
		printk("D3-D4:Fault Queue: 6\n"); 
}

static void g751_configuration_save(unsigned char value)
{
	if (value > 0x1F) return;

	if (I2C_Write(G751_DEVICE_ADDR, CONFIGURATION, &value, 1) < 0) {
		printk("G751: can't save configuration!\n");
		return;
	}
}

static void g751_temperature_load(void)
{
	unsigned char low_byte, high_byte;
	unsigned short temp;

	if (I2C_Read(G751_DEVICE_ADDR, TEMPERATURE, (unsigned char *)&temp, 2) < 0) {
		printk("G751: can't load temperature!\n");
		return;
	}

	low_byte = temp >> 8;
	high_byte = temp & 0xff;

	temp = (high_byte << 1) | (low_byte >> 7);

	if (temp > 0xff)
		printk("temperature = %d\n", (-0x200+temp)/2);
	else
		printk("temperature = %d\n", temp/2);
}

static void g751_t_hyst_load(void)
{
	unsigned char low_byte, high_byte;
	unsigned short t_hyst;

	if (I2C_Read(G751_DEVICE_ADDR, T_HYST, (unsigned char *)&t_hyst, 2) < 0) {
		printk("G751: can't load T_HYST!\n");
		return;
	}

	low_byte = t_hyst >> 8;
	high_byte = t_hyst & 0xff;

	t_hyst = (high_byte << 1) | (low_byte >> 7);

	if (t_hyst > 0xff)
		printk("T_HYST = %d\n", (-0x200+t_hyst)/2);
	else
		printk("T_HYST = %d\n", t_hyst/2);


}

static void g751_t_hyst_save(int value)
{
	unsigned char low_byte, high_byte;
	unsigned short t_hyst;


	printk("%s: %d\n", __FUNCTION__, value);

	if (value > 0)
		t_hyst = value*2;
	else
		t_hyst = 0x200+value*2;

	high_byte = t_hyst >> 1;
	printk("high_byte = 0x%x\n", high_byte);
	low_byte = (t_hyst & 0x01) << 7;
	printk("low_byte = 0x%x\n", low_byte);

	t_hyst = low_byte << 8 | high_byte;

	if (I2C_Write(G751_DEVICE_ADDR, T_HYST, (unsigned char *)&t_hyst, 2) < 0) {
		printk("G751: can't save T_HYST!\n");
		return;
	}


}

static void g751_t_os_load(void)
{
	unsigned char low_byte, high_byte;
	unsigned short t_os;

	if (I2C_Read(G751_DEVICE_ADDR, T_OS, (unsigned char *)&t_os, 2) < 0) {
		printk("G751: can't load T_OS!\n");
		return;
	}

	low_byte = t_os >> 8;
	high_byte = t_os & 0xff;

	t_os = (high_byte << 1) | (low_byte >> 7);

	if (t_os > 0xff)
		printk("T_OS = %d\n", (-0x200+t_os)/2);
	else
		printk("T_OS = %d\n", t_os/2);


}

static void g751_t_os_save(int value)
{
	unsigned char low_byte, high_byte;
	unsigned short t_os;


	printk("%s: %d\n", __FUNCTION__, value);

	if (value > 0)
		t_os = value*2;
	else
		t_os = 0x200+value*2;

	high_byte = t_os >> 1;
	printk("high_byte = 0x%x\n", high_byte);
	low_byte = (t_os & 0x01) << 7;
	printk("low_byte = 0x%x\n", low_byte);

	t_os = low_byte << 8 | high_byte;

	if (I2C_Write(G751_DEVICE_ADDR, T_OS, (unsigned char *)&t_os, 2) < 0) {
		printk("G751: can't save T_HYST!\n");
		return;
	}


}

static int conf_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;

	g751_configuration_load();
	return ret;
}

static int conf_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char buf[64];
	int conf;

	if (count > ARRAY_SIZE(buf) -1 )
		count = ARRAY_SIZE(buf) - 1;
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	buf[count] = '\0';

	conf = simple_strtol(buf, NULL, 10);

	printk("conf = %d\n", conf);

	g751_configuration_save(conf);

	return count;
}

static int temperature_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;

	g751_temperature_load();
	return ret;
}

static int t_os_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;

	g751_t_os_load();
	return ret;
}

static int t_os_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char buf[64];
	int temp;

	if (count > ARRAY_SIZE(buf) -1 )
		count = ARRAY_SIZE(buf) - 1;
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	buf[count] = '\0';

	temp = simple_strtol(buf, NULL, 10);

	printk("temp = %d\n", temp);

	g751_t_os_save(temp);
	return count;
}

static int t_hyst_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;

	g751_t_hyst_load();
	return ret;
}

static int t_hyst_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char buf[64];
	int temp;

	if (count > ARRAY_SIZE(buf) -1 )
		count = ARRAY_SIZE(buf) - 1;
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	buf[count] = '\0';

	temp = simple_strtol(buf, NULL, 10);

	printk("temp = %d\n", temp);

	g751_t_hyst_save(temp);

	return count;
}

static int __init g751_init_module(void)
{
	printk("g751_init_module\n");

	if (!I2C_Device_Detect(G751_DEVICE_ADDR))
		printk("Found G751 thermal sensor IC!\n");
	else {
		printk("NOT found G751 thermal sensor IC!\n");
		return -1;
	}

	if ((root = proc_mkdir(PROC_ROOT, NULL)) == NULL)
		return -EINVAL;

	if ((conf_ent = create_proc_entry(PROC_CONFIGURATION, S_IRUGO | S_IWUGO, root)) == NULL)
		return -EINVAL;
	else {
		conf_ent->read_proc = conf_read_proc;
		conf_ent->write_proc = conf_write_proc;
	}

	if ((temp_ent = create_proc_entry(PROC_TEMPERATURE, S_IRUGO | S_IWUGO, root)) == NULL)
		return -EINVAL;
	else {
		temp_ent->read_proc = temperature_read_proc;
	}

	if ((t_os_ent = create_proc_entry(PROC_T_OS, S_IRUGO | S_IWUGO, root)) == NULL)
		return -EINVAL;
	else {
		t_os_ent->read_proc = t_os_read_proc;
		t_os_ent->write_proc = t_os_write_proc;
	}

	if ((t_hyst_ent = create_proc_entry(PROC_T_HYSTERESIS, S_IRUGO | S_IWUGO, root)) == NULL)
		return -EINVAL;
	else {
		t_hyst_ent->read_proc = t_hyst_read_proc;
		t_hyst_ent->write_proc = t_hyst_write_proc;
	}

	g751_t_os_save(70);
	g751_t_hyst_save(50);

	return 0;
}

static void __exit g751_cleanup_module(void) 
{
	printk("g751_cleanup_module\n");

	if (root) {
		if (conf_ent)
			remove_proc_entry(PROC_CONFIGURATION, root);
		if (temp_ent)
			remove_proc_entry(PROC_TEMPERATURE, root);
		if (t_os_ent)
			remove_proc_entry(PROC_T_OS, root);
		if (t_hyst_ent)
			remove_proc_entry(PROC_T_HYSTERESIS, root);

		remove_proc_entry(PROC_ROOT, NULL);
	}
}

module_init(g751_init_module);
module_exit(g751_cleanup_module);
