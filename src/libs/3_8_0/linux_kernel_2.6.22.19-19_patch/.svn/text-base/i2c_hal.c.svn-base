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

//#define I2C_HAL_DEBUG

#ifdef I2C_HAL_DEBUG
#define	I2C_DBG_MSG	printk
#else
#define I2C_DBG_MSG(...)
#endif

#define	HW_I2C_MASTER0_CONFIG	0x480
#define	HW_I2C_REG		REG_BASE_system_block|HW_I2C_MASTER0_CONFIG

#define	GPIO_CLK	0
#define	GPIO_SDA	1

MODULE_AUTHOR("Steven Wang <Steven_Wang@alphanetworks.com>");
MODULE_DESCRIPTION("I2C protocol implement by GPIO");
MODULE_LICENSE("GPL");

static DECLARE_MUTEX(lock);

static	struct	task_struct	*kthread;

static void gpio_setdirection(int gpio, int dir)
{
	em86xx_gpio_setdirection(gpio, dir);
}

static void gpio_write(int gpio, int level)
{
	em86xx_gpio_write(gpio, level);
}

static int gpio_read(int gpio)
{
	return em86xx_gpio_read(gpio);
}

static int I2C_Clock(int clock)
{
	gpio_setdirection(GPIO_CLK, 1);

	if (clock)
		gpio_write(GPIO_CLK, 1);
	else 
		gpio_write(GPIO_CLK, 0);

	udelay(10);

	return 0;
}

static int I2C_Data(int data)
{
	gpio_setdirection(GPIO_SDA, 1);

	if (data)
		gpio_write(GPIO_SDA, 1);
	else
		gpio_write(GPIO_SDA, 0);

	udelay(10);

	return 0;
}

static int I2C_WrBit(int data)
{
	int err;

	if ((err = I2C_Data(data)) < 0) return -1;
	I2C_DBG_MSG("%d", data?1:0);
	if ((err = I2C_Clock(1)) < 0) return -1;
	if ((err = I2C_Clock(0)) < 0) return -1;

	return 0;
}

static int I2C_RdBit(int *data)
{
	int err;

	if ((err = I2C_Clock(1)) < 0) return -1;

	gpio_setdirection(GPIO_SDA, 0);
	*data = gpio_read(GPIO_SDA);
	I2C_DBG_MSG("%d", *data?1:0);
	if ((err = I2C_Clock(0)) < 0) return -1;

	return 0;
}

static int I2C_Start(void)
{
	int err;

	if ((err = I2C_Data(1)) < 0) return -1;
	if ((err = I2C_Clock(1)) < 0) return -1;
	if ((err = I2C_Data(0)) < 0) return -1;
	if ((err = I2C_Clock(0)) < 0) return -1;

	return 0;
}

static int I2C_Stop(void)
{
	int err;

	if ((err = I2C_Data(0)) < 0) return -1;
	if ((err = I2C_Clock(1)) < 0) return -1;
	if ((err = I2C_Clock(1)) < 0) return -1;
	if ((err = I2C_Data(1)) < 0) return -1;

	return 0;
}

static int I2C_SendNack(void)
{
	return I2C_WrBit(1);
}

static int I2C_SendAck(void)
{
	return I2C_WrBit(0);
}

static int I2C_WaitAck(void)
{
	int err;
	unsigned int lastRecvMs;

	gpio_setdirection(GPIO_SDA, 0);

	if ((err = I2C_Clock(1)) < 0) return -1;

	lastRecvMs = jiffies_to_msecs(jiffies);

	while(gpio_read(GPIO_SDA)&1) {
		if((jiffies_to_msecs(jiffies) - lastRecvMs) > 0) {
			//printk("No ACK\n");
			return -1;
		}
	}
	I2C_DBG_MSG("0");

	if ((err = I2C_Clock(0)) < 0) return -1;

	udelay(100);

	return 0;
}

static int I2C_WrByte(unsigned char ByteIn)
{
	unsigned int i;
	int err;


	for (i = 0; i < 8; i++) {
		if ((err = I2C_WrBit(ByteIn & 0x80)) < 0) return -1;
		ByteIn <<= 1;
	}

	return 0;
}

static int I2C_RdByte(unsigned char *data)
{
	unsigned int i;
	int bit, err;

	for (i = 0; i < 8; i++) {
		if ((err = I2C_RdBit(&bit)) < 0) return -1;
		*data = (*data << 1) | (bit ? 1 : 0);
	}

	return 0;
}

int I2C_Write(unsigned char SlaveAddress, unsigned char SubAddress, unsigned char *data, unsigned int n)
{
	unsigned int i;
	int err;

	down(&lock);

	I2C_DBG_MSG("%s: ", __FUNCTION__);
	if ((err = I2C_Start()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("S ");
	if ((err = I2C_WrByte(SlaveAddress|0x0)) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG(" ");
	if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}
	if ((err = I2C_WrByte(SubAddress)) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG(" ");
	if ((err = I2C_SendAck()) < 0) {up(&lock); return -1;}
	for (i = 0; i < n; i++) {
		I2C_DBG_MSG(" ");
		if ((err = I2C_WrByte(data[i])) < 0) {up(&lock); return -1;}
		I2C_DBG_MSG(" ");
		if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}
		I2C_DBG_MSG("SA");
	}
	I2C_DBG_MSG(" ");

	if ((err = I2C_Stop()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("P\n");

	up(&lock);

	return 0;
}

int I2C_Read(unsigned char SlaveAddress, unsigned char SubAddress, unsigned char *data, unsigned int n)
{
	unsigned int i;
	int err;

	down(&lock);

	if (n < 1) {up(&lock); return -1;}

	I2C_DBG_MSG("%s: ", __FUNCTION__);
	if ((err = I2C_Start()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("S ");
	if ((err = I2C_WrByte(SlaveAddress|0x0)) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG(" ");
	if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("SA ");
	if ((err = I2C_WrByte(SubAddress)) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG(" ");
	if ((err = I2C_SendAck()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("MA ");

	if ((err = I2C_Start()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("S ");
	if ((err = I2C_WrByte(SlaveAddress|0x1)) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG(" ");
	if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("SA");

	for (i = 0; i < n; i++) {
		I2C_DBG_MSG(" ");
		if ((err = I2C_RdByte(&(data[i]))) < 0) {up(&lock); return -1;}
		I2C_DBG_MSG(" ");
		if (i == (n - 1)) {
			if ((err = I2C_SendNack()) < 0) {up(&lock); return -1;}
			I2C_DBG_MSG("MNA");
		} else {
			if ((err = I2C_SendAck()) < 0) {up(&lock); return -1;}
			I2C_DBG_MSG("MA");
		}
	}
	I2C_DBG_MSG(" ");

	if ((err = I2C_Stop()) < 0) {up(&lock); return -1;}
	I2C_DBG_MSG("P\n");

	up(&lock);

	return 0;
}

int I2C_Write_NoSubAddress(unsigned char SlaveAddress, unsigned char *data, unsigned int n)
{
	unsigned int i;
	int err;

	down(&lock);

	if ((err = I2C_Start()) < 0) {up(&lock); return -1;}
	if ((err = I2C_WrByte(SlaveAddress|0x0)) < 0) {up(&lock); return -1;}
	if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}

	for (i = 0; i < n; i++) {
		if ((err = I2C_WrByte(data[i])) < 0) {up(&lock); return -1;}
		if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}
	}

	if ((err = I2C_Stop()) < 0) {up(&lock); return -1;}

	up(&lock);

	return 0;
}

int I2C_Read_NoSubAddress(unsigned char SlaveAddress, unsigned char *data, unsigned int n)
{
	unsigned int i;
	int err;

	down(&lock);

	if (n < 1) {up(&lock); return -1;}

	if ((err = I2C_Start()) < 0) {up(&lock); return -1;}
	if ((err = I2C_WrByte(SlaveAddress|0x1)) < 0) {up(&lock); return -1;}
	if ((err = I2C_WaitAck()) < 0) {up(&lock); return -1;}

	for (i = 0; i < n; i++) {
		if ((err = I2C_RdByte(&(data[i]))) < 0) {up(&lock); return -1;}
		if (i == (n - 1)) {
			if ((err = I2C_SendNack()) < 0) {up(&lock); return -1;}
		} else {
			if ((err = I2C_SendAck()) < 0) {up(&lock); return -1;}
		}
	}

	if ((err = I2C_Stop()) < 0) {up(&lock); return -1;}

	up(&lock);

	return 0;
}

int I2C_Device_Detect(unsigned char SlaveAddress)
{
	int err;

	I2C_DBG_MSG("%s: ", __FUNCTION__);
	if ((err = I2C_Start()) < 0) return -1;
	I2C_DBG_MSG("S ");
	if ((err = I2C_WrByte(SlaveAddress|0x0)) < 0) {I2C_DBG_MSG("\n"); return -1;}
	I2C_DBG_MSG(" ");
	if ((err = I2C_WaitAck()) < 0) {I2C_DBG_MSG("\n"); return -1;}
	I2C_DBG_MSG("SA ");
	if ((err = I2C_Stop()) < 0) {I2C_DBG_MSG("\n"); return -1;}
	I2C_DBG_MSG("P\n");

	return 0;
}

static int i2c_thread(void *data)
{
	int reg;

	while (1) {

		reg = gbus_read_reg32(HW_I2C_REG);
		if (reg != 0) { 
			printk("Warnning: someone change the HW_I2C_REG setting!\n"); 
			gbus_write_reg32(HW_I2C_REG, 0);
		}

		msleep_interruptible(1000);

		if (kthread_should_stop())
			break;
	}

	return 0;
}

static int __init i2c_hal_init_module(void)
{
	printk("i2c_hal_init_module\n");

	/* Reference SMP8654-DS-A-07 page 169. Write 0 to address 0x480 to disable HW I2C */
	gbus_write_reg32(HW_I2C_REG, 0);
	printk("HW_I2C_REG: 0x%x = %d\n", HW_I2C_REG, (int)gbus_read_reg32(HW_I2C_REG)); 

	kthread = kthread_create(i2c_thread, NULL, "ki2cd");
	if (IS_ERR(kthread)) {
		printk("i2c_hal_init_module: create kernel thread fail!\n");
		return -1;
	}
	wake_up_process(kthread);

	return 0;
}

static void __exit i2c_hal_cleanup_module(void)
{
	printk("i2c_hal_cleanup_module\n");
	kthread_stop(kthread);
}

EXPORT_SYMBOL(I2C_Write);
EXPORT_SYMBOL(I2C_Read);
EXPORT_SYMBOL(I2C_Write_NoSubAddress);
EXPORT_SYMBOL(I2C_Read_NoSubAddress);
EXPORT_SYMBOL(I2C_Device_Detect);

module_init(i2c_hal_init_module);
module_exit(i2c_hal_cleanup_module);
