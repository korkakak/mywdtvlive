/*
################################################################################
#
# r8169 is the Linux device driver released for RealTek RTL8169S/8110S,
# RTL8169SB/8110SB, and RTL8110SC Gigabit Ethernet controllers with
# PCI interface.
#
# Copyright(c) 2010 Realtek Semiconductor Corp. All rights reserved.
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, see <http://www.gnu.org/licenses/>.
#
# Author:
# Realtek NIC software team <nicfae@realtek.com>
# No. 2, Innovation Road II, Hsinchu Science Park, Hsinchu 300, Taiwan
#
################################################################################
*/

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
 #define CHECKSUM_PARTIAL CHECKSUM_HW
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
 #define irqreturn_t void
 #define IRQ_HANDLED	1
 #define IRQ_NONE	0
 #define IRQ_RETVAL(x)
#endif

#ifndef HAVE_FREE_NETDEV
 #define free_netdev(x)	kfree(x)
#endif

#ifndef SET_NETDEV_DEV
 #define SET_NETDEV_DEV(net, pdev)
#endif

#ifndef SET_MODULE_OWNER
 #define SET_MODULE_OWNER(dev)
#endif

#ifndef SA_SHIRQ
 #define SA_SHIRQ IRQF_SHARED
#endif

#ifndef NETIF_F_GSO
#define gso_size	tso_size
#define gso_segs	tso_segs
#endif

#define CONFIG_R8169_NAPI

#ifndef uintptr_t
 typedef unsigned long uintptr_t;
#endif

#ifdef HAVE_NET_DEVICE_OPS
	#define RTL_NET_DEVICE_OPS(ops)	dev->netdev_ops=&ops
#else
	#ifdef CONFIG_NET_POLL_CONTROLLER
		#define RTL_NET_POLL_CONTROLLER dev->poll_controller=rtl8169_netpoll
	#else
		#define RTL_NET_POLL_CONTROLLER
	#endif

	#ifdef CONFIG_R8169_VLAN
		#define RTL_SET_VLAN dev->vlan_rx_register=rtl8169_vlan_rx_register
	#else
		#define RTL_SET_VLAN
	#endif

	#define RTL_NET_DEVICE_OPS(ops)	dev->open=rtl8169_open; \
					dev->hard_start_xmit=rtl8169_start_xmit; \
					dev->get_stats=rtl8169_get_stats; \
					dev->stop=rtl8169_close; \
					dev->tx_timeout=rtl8169_tx_timeout; \
					dev->set_multicast_list=rtl8169_set_rx_mode; \
					dev->change_mtu=rtl8169_change_mtu; \
					dev->set_mac_address=rtl8169_set_mac_address; \
					dev->do_ioctl=rtl8169_ioctl; \
					RTL_NET_POLL_CONTROLLER; \
					RTL_SET_VLAN;
#endif //HAVE_NET_DEVICE_OPS

#ifndef NET_IP_ALIGN
 #define NET_IP_ALIGN 2
#endif

#define NODE_ADDRESS_SIZE 6

#ifdef CONFIG_R8169_NAPI
 #define NAPI_SUFFIX	"-NAPI"
#else
 #define NAPI_SUFFIX	""
#endif

#define RTL8169_VERSION "6.012.01" NAPI_SUFFIX
#define MODULENAME "r8169"
#define PFX MODULENAME ": "

#ifdef RTL8169_DEBUG
 #define assert(expr) \
	if (!(expr)) {					\
		printk( "Assertion failed! %s,%s,%s,line=%d\n",	\
		#expr,__FILE__,__FUNCTION__,__LINE__);		\
	}
 #define dprintk(fmt, args...)	do { printk(PFX fmt, ## args); } while (0)
#else
 #define assert(expr) do {} while (0)
 #define dprintk(fmt, args...)	do {} while (0)
#endif /* RTL8169_DEBUG */

#define R8169_MSG_DEFAULT \
	(NETIF_MSG_DRV | NETIF_MSG_PROBE | NETIF_MSG_IFUP | NETIF_MSG_IFDOWN)

#define TX_BUFFS_AVAIL(tp) \
	(tp->dirty_tx + NUM_TX_DESC - tp->cur_tx - 1)

#ifdef CONFIG_R8169_NAPI
 #define rtl8169_rx_skb			netif_receive_skb
 #define rtl8169_rx_hwaccel_skb		vlan_hwaccel_receive_skb
 #define rtl8169_rx_quota(count, quota)	min(count, quota)
#else
 #define rtl8169_rx_skb			netif_rx
 #define rtl8169_rx_hwaccel_skb		vlan_hwaccel_rx
 #define rtl8169_rx_quota(count, quota)	count
#endif

/* MAC address length */
#ifndef MAC_ADDR_LEN
 #define MAC_ADDR_LEN	6
#endif

#ifndef MAC_PROTOCOL_LEN
 #define MAC_PROTOCOL_LEN	2
#endif

#define Reserved2_data	7
#define RX_DMA_BURST	7	/* Maximum PCI burst, '7' is unlimited */
#define TX_DMA_BURST	7	/* Maximum PCI burst, '7' is unlimited */
#define Reserved1_data 	0x3F
#define RxPacketMaxSize	0x3FE8	/* 16K - 1 - ETH_HLEN - VLAN - CRC... */
#define Jumbo_Frame_7k	(7 * 1024)

#define IFG0	(1 << 24)
#define IFG1	(1 << 25)
#define IFG2	(1 << 19)

#define R8169_REGS_SIZE		256
#define R8169_NAPI_WEIGHT	64
#define NUM_TX_DESC	256	/* Number of Tx descriptor registers */
#define NUM_RX_DESC	256	/* Number of Rx descriptor registers */
#define RX_BUF_SIZE	1536	/* Rx Buffer size */
#define R8169_TX_RING_BYTES	(NUM_TX_DESC * sizeof(struct TxDesc))
#define R8169_RX_RING_BYTES	(NUM_RX_DESC * sizeof(struct RxDesc))

#define RTL8169_TX_TIMEOUT	(6*HZ)
#define RTL8169_PHY_TIMEOUT	(10*HZ)
#define RTL8169_ESD_TIMEOUT	(2*HZ)

/* write/read MMIO register */
#define RTL_W8(reg, val8)	writeb ((val8), ioaddr + (reg))
#define RTL_W16(reg, val16)	writew ((val16), ioaddr + (reg))
#define RTL_W32(reg, val32)	writel ((val32), ioaddr + (reg))
#define RTL_R8(reg)		readb (ioaddr + (reg))
#define RTL_R16(reg)		readw (ioaddr + (reg))
#define RTL_R32(reg)		((unsigned long) readl (ioaddr + (reg)))

#ifndef	DMA_64BIT_MASK
 #define DMA_64BIT_MASK	0xffffffffffffffffULL
#endif

#ifndef	DMA_32BIT_MASK
 #define DMA_32BIT_MASK	0x00000000ffffffffULL
#endif

#ifndef	NETDEV_TX_OK
 #define NETDEV_TX_OK 0		/* driver took care of packet */
#endif

#ifndef	NETDEV_TX_BUSY
 #define NETDEV_TX_BUSY 1	/* driver tx path was busy*/
#endif

#ifndef	NETDEV_TX_LOCKED
 #define NETDEV_TX_LOCKED -1	/* driver tx lock was already taken */
#endif

#ifndef	ADVERTISED_Pause
 #define ADVERTISED_Pause	(1 << 13)
#endif

#ifndef	ADVERTISED_Asym_Pause
 #define ADVERTISED_Asym_Pause	(1 << 14)
#endif

#ifndef	ADVERTISE_PAUSE_CAP
 #define ADVERTISE_PAUSE_CAP	0x400
#endif

#ifndef	ADVERTISE_PAUSE_ASYM
 #define ADVERTISE_PAUSE_ASYM	0x800
#endif

#ifndef	MII_CTRL1000
 #define MII_CTRL1000		0x09
#endif

#ifndef	ADVERTISE_1000FULL
 #define ADVERTISE_1000FULL	0x200
#endif

#ifndef	ADVERTISE_1000HALF
 #define ADVERTISE_1000HALF	0x100
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)
#ifdef __CHECKER__
#define __iomem	__attribute__((noderef, address_space(2)))
extern void __chk_io_ptr(void __iomem *);
#define __bitwise __attribute__((bitwise))
#else
#define __iomem
#define __chk_io_ptr(x) (void)0
#define __bitwise
#endif
#endif	//LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)

#ifndef module_param
#define module_param(v,t,p) MODULE_PARM(v, "i");
#endif

#ifndef PCI_DEVICE
#define PCI_DEVICE(vend,dev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID
#endif

/*****************************************************************************/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
	#define	RTLDEV	tp
#else
	#define	RTLDEV	dev
#endif	//LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22)
/*****************************************************************************/

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
	typedef struct net_device *napi_ptr;
	typedef int *napi_budget;

	#define napi dev
	#define RTL_NAPI_CONFIG(ndev, priv, function, weig)	ndev->poll=function;	\
								ndev->weight=weig;
	#define RTL_NAPI_QUOTA(budget, ndev)			min(*budget, ndev->quota)
	#define RTL_GET_PRIV(stuct_ptr, priv_struct)		netdev_priv(stuct_ptr)
	#define RTL_GET_NETDEV(priv_ptr)
	#define RTL_RX_QUOTA(ndev, budget)			ndev->quota
	#define RTL_NAPI_QUOTA_UPDATE(ndev, work_done, budget)	*budget -= work_done;	\
								ndev->quota -= work_done;
	#define RTL_NETIF_RX_COMPLETE(dev, napi)		netif_rx_complete(dev)
	#define RTL_NETIF_RX_SCHEDULE_PREP(dev, napi)		netif_rx_schedule_prep(dev)
	#define __RTL_NETIF_RX_SCHEDULE(dev, napi)		__netif_rx_schedule(dev)
	#define RTL_NAPI_RETURN_VALUE				work_done >= work_to_do
	#define RTL_NAPI_ENABLE(dev, napi)			netif_poll_enable(dev)
	#define RTL_NAPI_DISABLE(dev, napi)			netif_poll_disable(dev)
#else
	typedef struct napi_struct *napi_ptr;
	typedef int napi_budget;

	#define RTL_NAPI_CONFIG(ndev, priv, function, weight)	netif_napi_add(ndev, &priv->napi, function, weight)
	#define RTL_NAPI_QUOTA(budget, ndev)			min(budget, budget)
	#define RTL_GET_PRIV(stuct_ptr, priv_struct)		container_of(stuct_ptr, priv_struct, stuct_ptr)
	#define RTL_GET_NETDEV(priv_ptr)			struct net_device *dev = priv_ptr->dev;
	#define RTL_RX_QUOTA(ndev, budget)			budget
	#define RTL_NAPI_QUOTA_UPDATE(ndev, work_done, budget)
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29)
	#define RTL_NETIF_RX_COMPLETE(dev, napi)		netif_rx_complete(dev, napi)
	#define RTL_NETIF_RX_SCHEDULE_PREP(dev, napi)		netif_rx_schedule_prep(dev, napi)
	#define __RTL_NETIF_RX_SCHEDULE(dev, napi)		__netif_rx_schedule(dev, napi)
 #endif
 #if LINUX_VERSION_CODE == KERNEL_VERSION(2,6,29)
	#define RTL_NETIF_RX_COMPLETE(dev, napi)		netif_rx_complete(napi)
	#define RTL_NETIF_RX_SCHEDULE_PREP(dev, napi)		netif_rx_schedule_prep(napi)
	#define __RTL_NETIF_RX_SCHEDULE(dev, napi)		__netif_rx_schedule(napi)
 #endif
 #if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,29)
	#define RTL_NETIF_RX_COMPLETE(dev, napi)		napi_complete(napi)
	#define RTL_NETIF_RX_SCHEDULE_PREP(dev, napi)		napi_schedule_prep(napi)
	#define __RTL_NETIF_RX_SCHEDULE(dev, napi)		__napi_schedule(napi)
 #endif
	#define RTL_NAPI_RETURN_VALUE work_done
	#define RTL_NAPI_ENABLE(dev, napi)			napi_enable(napi)
	#define RTL_NAPI_DISABLE(dev, napi)			napi_disable(napi)
#endif	//LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)

/*****************************************************************************/

/* 2.4.22 => 2.4.17 */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,4,22) )
#define pci_name(x)	((x)->slot_name)
#endif /* 2.4.22 => 2.4.17 */

/*****************************************************************************/
/* 2.6.5 => 2.6.0 */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,5) )
#define pci_dma_sync_single_for_cpu	pci_dma_sync_single
#define pci_dma_sync_single_for_device	pci_dma_sync_single_for_cpu
#endif /* 2.6.5 => 2.6.0 */

/*****************************************************************************/
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10) )
#ifdef module_param_array_named
#undef module_param_array_named
#define module_param_array_named(name, array, type, nump, perm)          \
	static struct kparam_array __param_arr_##name                    \
	= { ARRAY_SIZE(array), nump, param_set_##type, param_get_##type, \
	    sizeof(array[0]), array };                                   \
	module_param_call(name, param_array_set, param_array_get,        \
			  &__param_arr_##name, perm)
#endif /* module_param_array_named */
#endif /* < 2.6.10 */

/*****************************************************************************/
/* 2.6.0 => 2.5.28 */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) )
#define MODULE_INFO(version, _version)
#ifndef CONFIG_E1000_DISABLE_PACKET_SPLIT
#define CONFIG_E1000_DISABLE_PACKET_SPLIT 1
#endif

#define pci_set_consistent_dma_mask(dev,mask) 1

#undef dev_put
#define dev_put(dev) __dev_put(dev)

#ifndef skb_fill_page_desc
#define skb_fill_page_desc _kc_skb_fill_page_desc
extern void _kc_skb_fill_page_desc(struct sk_buff *skb, int i, struct page *page, int off, int size);
#endif

#ifndef pci_dma_mapping_error
#define pci_dma_mapping_error _kc_pci_dma_mapping_error
static inline int _kc_pci_dma_mapping_error(dma_addr_t dma_addr)
{
	return dma_addr == 0;
}
#endif

#undef ALIGN
#define ALIGN(x,a) (((x)+(a)-1)&~((a)-1))

#endif /* 2.6.0 => 2.5.28 */

/*****************************************************************************/

/* 2.6.4 => 2.6.0 */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,6,4) )
#define MODULE_VERSION(_version) MODULE_INFO(version, _version)
#endif /* 2.6.4 => 2.6.0 */

/*****************************************************************************/
/* 2.5.28 => 2.4.23 */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,5,28) )

static inline void _kc_synchronize_irq(void)
{
	synchronize_irq();
}
#undef synchronize_irq
#define synchronize_irq(X) _kc_synchronize_irq()

#include <linux/tqueue.h>
#define work_struct tq_struct
#undef INIT_WORK
#define INIT_WORK(a,b,c) INIT_TQUEUE(a,(void (*)(void *))b,c)
#undef container_of
#define container_of list_entry
#define schedule_work schedule_task
#define flush_scheduled_work flush_scheduled_tasks
#endif /* 2.5.28 => 2.4.17 */

/*****************************************************************************/
/* 2.6.4 => 2.6.0 */
#if ( LINUX_VERSION_CODE < KERNEL_VERSION(2,4,25) || \
    ( LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0) && \
      LINUX_VERSION_CODE < KERNEL_VERSION(2,6,4) ) )
#define ETHTOOL_OPS_COMPAT
#endif /* 2.6.4 => 2.6.0 */

/*****************************************************************************/

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
/*
 * initialize a work-struct's func and data pointers:
 */
#define PREPARE_WORK(_work, _func, _data)			\
	do {							\
		(_work)->func = _func;				\
		(_work)->data = _data;				\
	} while (0)

#endif
/*****************************************************************************/

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
#undef ethtool_ops
#define ethtool_ops _kc_ethtool_ops

struct _kc_ethtool_ops {
	int  (*get_settings)(struct net_device *, struct ethtool_cmd *);
	int  (*set_settings)(struct net_device *, struct ethtool_cmd *);
	void (*get_drvinfo)(struct net_device *, struct ethtool_drvinfo *);
	int  (*get_regs_len)(struct net_device *);
	void (*get_regs)(struct net_device *, struct ethtool_regs *, void *);
	void (*get_wol)(struct net_device *, struct ethtool_wolinfo *);
	int  (*set_wol)(struct net_device *, struct ethtool_wolinfo *);
	u32  (*get_msglevel)(struct net_device *);
	void (*set_msglevel)(struct net_device *, u32);
	int  (*nway_reset)(struct net_device *);
	u32  (*get_link)(struct net_device *);
	int  (*get_eeprom_len)(struct net_device *);
	int  (*get_eeprom)(struct net_device *, struct ethtool_eeprom *, u8 *);
	int  (*set_eeprom)(struct net_device *, struct ethtool_eeprom *, u8 *);
	int  (*get_coalesce)(struct net_device *, struct ethtool_coalesce *);
	int  (*set_coalesce)(struct net_device *, struct ethtool_coalesce *);
	void (*get_ringparam)(struct net_device *, struct ethtool_ringparam *);
	int  (*set_ringparam)(struct net_device *, struct ethtool_ringparam *);
	void (*get_pauseparam)(struct net_device *,
	                       struct ethtool_pauseparam*);
	int  (*set_pauseparam)(struct net_device *,
	                       struct ethtool_pauseparam*);
	u32  (*get_rx_csum)(struct net_device *);
	int  (*set_rx_csum)(struct net_device *, u32);
	u32  (*get_tx_csum)(struct net_device *);
	int  (*set_tx_csum)(struct net_device *, u32);
	u32  (*get_sg)(struct net_device *);
	int  (*set_sg)(struct net_device *, u32);
	u32  (*get_tso)(struct net_device *);
	int  (*set_tso)(struct net_device *, u32);
	int  (*self_test_count)(struct net_device *);
	void (*self_test)(struct net_device *, struct ethtool_test *, u64 *);
	void (*get_strings)(struct net_device *, u32 stringset, u8 *);
	int  (*phys_id)(struct net_device *, u32);
	int  (*get_stats_count)(struct net_device *);
	void (*get_ethtool_stats)(struct net_device *, struct ethtool_stats *,
	                          u64 *);
} *ethtool_ops = NULL;

#undef SET_ETHTOOL_OPS
#define SET_ETHTOOL_OPS(netdev, ops) (ethtool_ops = (ops))

#endif //LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)

/*****************************************************************************/
/* Installations with ethtool version without eeprom, adapter id, or statistics
 * support */

#ifndef ETH_GSTRING_LEN
#define ETH_GSTRING_LEN 32
#endif

#ifndef ETHTOOL_GSTATS
#define ETHTOOL_GSTATS 0x1d
#undef ethtool_drvinfo
#define ethtool_drvinfo k_ethtool_drvinfo
struct k_ethtool_drvinfo {
	u32 cmd;
	char driver[32];
	char version[32];
	char fw_version[32];
	char bus_info[32];
	char reserved1[32];
	char reserved2[16];
	u32 n_stats;
	u32 testinfo_len;
	u32 eedump_len;
	u32 regdump_len;
};

struct ethtool_stats {
	u32 cmd;
	u32 n_stats;
	u64 data[0];
};
#endif /* ETHTOOL_GSTATS */

#ifndef ETHTOOL_PHYS_ID
#define ETHTOOL_PHYS_ID 0x1c
#endif /* ETHTOOL_PHYS_ID */

#ifndef ETHTOOL_GSTRINGS
#define ETHTOOL_GSTRINGS 0x1b
enum ethtool_stringset {
	ETH_SS_TEST             = 0,
	ETH_SS_STATS,
};
struct ethtool_gstrings {
	u32 cmd;            /* ETHTOOL_GSTRINGS */
	u32 string_set;     /* string set id e.c. ETH_SS_TEST, etc*/
	u32 len;            /* number of strings in the string set */
	u8 data[0];
};
#endif /* ETHTOOL_GSTRINGS */

#ifndef ETHTOOL_TEST
#define ETHTOOL_TEST 0x1a
enum ethtool_test_flags {
	ETH_TEST_FL_OFFLINE	= (1 << 0),
	ETH_TEST_FL_FAILED	= (1 << 1),
};
struct ethtool_test {
	u32 cmd;
	u32 flags;
	u32 reserved;
	u32 len;
	u64 data[0];
};
#endif /* ETHTOOL_TEST */

#ifndef ETHTOOL_GEEPROM
#define ETHTOOL_GEEPROM 0xb
#undef ETHTOOL_GREGS
struct ethtool_eeprom {
	u32 cmd;
	u32 magic;
	u32 offset;
	u32 len;
	u8 data[0];
};

struct ethtool_value {
	u32 cmd;
	u32 data;
};
#endif /* ETHTOOL_GEEPROM */

#ifndef ETHTOOL_GLINK
#define ETHTOOL_GLINK 0xa
#endif /* ETHTOOL_GLINK */

#ifndef ETHTOOL_GREGS
#define ETHTOOL_GREGS		0x00000004 /* Get NIC registers */
#define ethtool_regs _kc_ethtool_regs
/* for passing big chunks of data */
struct _kc_ethtool_regs {
	u32 cmd;
	u32 version; /* driver-specific, indicates different chips/revs */
	u32 len; /* bytes */
	u8 data[0];
};
#endif /* ETHTOOL_GREGS */

#ifndef ETHTOOL_GMSGLVL
#define ETHTOOL_GMSGLVL		0x00000007 /* Get driver message level */
#endif
#ifndef ETHTOOL_SMSGLVL
#define ETHTOOL_SMSGLVL		0x00000008 /* Set driver msg level, priv. */
#endif
#ifndef ETHTOOL_NWAY_RST
#define ETHTOOL_NWAY_RST	0x00000009 /* Restart autonegotiation, priv */
#endif
#ifndef ETHTOOL_GLINK
#define ETHTOOL_GLINK		0x0000000a /* Get link status */
#endif
#ifndef ETHTOOL_GEEPROM
#define ETHTOOL_GEEPROM		0x0000000b /* Get EEPROM data */
#endif
#ifndef ETHTOOL_SEEPROM
#define ETHTOOL_SEEPROM		0x0000000c /* Set EEPROM data */
#endif
#ifndef ETHTOOL_GCOALESCE
#define ETHTOOL_GCOALESCE	0x0000000e /* Get coalesce config */
/* for configuring coalescing parameters of chip */
#define ethtool_coalesce _kc_ethtool_coalesce
struct _kc_ethtool_coalesce {
	u32	cmd;	/* ETHTOOL_{G,S}COALESCE */

	/* How many usecs to delay an RX interrupt after
	 * a packet arrives.  If 0, only rx_max_coalesced_frames
	 * is used.
	 */
	u32	rx_coalesce_usecs;

	/* How many packets to delay an RX interrupt after
	 * a packet arrives.  If 0, only rx_coalesce_usecs is
	 * used.  It is illegal to set both usecs and max frames
	 * to zero as this would cause RX interrupts to never be
	 * generated.
	 */
	u32	rx_max_coalesced_frames;

	/* Same as above two parameters, except that these values
	 * apply while an IRQ is being serviced by the host.  Not
	 * all cards support this feature and the values are ignored
	 * in that case.
	 */
	u32	rx_coalesce_usecs_irq;
	u32	rx_max_coalesced_frames_irq;

	/* How many usecs to delay a TX interrupt after
	 * a packet is sent.  If 0, only tx_max_coalesced_frames
	 * is used.
	 */
	u32	tx_coalesce_usecs;

	/* How many packets to delay a TX interrupt after
	 * a packet is sent.  If 0, only tx_coalesce_usecs is
	 * used.  It is illegal to set both usecs and max frames
	 * to zero as this would cause TX interrupts to never be
	 * generated.
	 */
	u32	tx_max_coalesced_frames;

	/* Same as above two parameters, except that these values
	 * apply while an IRQ is being serviced by the host.  Not
	 * all cards support this feature and the values are ignored
	 * in that case.
	 */
	u32	tx_coalesce_usecs_irq;
	u32	tx_max_coalesced_frames_irq;

	/* How many usecs to delay in-memory statistics
	 * block updates.  Some drivers do not have an in-memory
	 * statistic block, and in such cases this value is ignored.
	 * This value must not be zero.
	 */
	u32	stats_block_coalesce_usecs;

	/* Adaptive RX/TX coalescing is an algorithm implemented by
	 * some drivers to improve latency under low packet rates and
	 * improve throughput under high packet rates.  Some drivers
	 * only implement one of RX or TX adaptive coalescing.  Anything
	 * not implemented by the driver causes these values to be
	 * silently ignored.
	 */
	u32	use_adaptive_rx_coalesce;
	u32	use_adaptive_tx_coalesce;

	/* When the packet rate (measured in packets per second)
	 * is below pkt_rate_low, the {rx,tx}_*_low parameters are
	 * used.
	 */
	u32	pkt_rate_low;
	u32	rx_coalesce_usecs_low;
	u32	rx_max_coalesced_frames_low;
	u32	tx_coalesce_usecs_low;
	u32	tx_max_coalesced_frames_low;

	/* When the packet rate is below pkt_rate_high but above
	 * pkt_rate_low (both measured in packets per second) the
	 * normal {rx,tx}_* coalescing parameters are used.
	 */

	/* When the packet rate is (measured in packets per second)
	 * is above pkt_rate_high, the {rx,tx}_*_high parameters are
	 * used.
	 */
	u32	pkt_rate_high;
	u32	rx_coalesce_usecs_high;
	u32	rx_max_coalesced_frames_high;
	u32	tx_coalesce_usecs_high;
	u32	tx_max_coalesced_frames_high;

	/* How often to do adaptive coalescing packet rate sampling,
	 * measured in seconds.  Must not be zero.
	 */
	u32	rate_sample_interval;
};
#endif /* ETHTOOL_GCOALESCE */

#ifndef ETHTOOL_SCOALESCE
#define ETHTOOL_SCOALESCE	0x0000000f /* Set coalesce config. */
#endif
#ifndef ETHTOOL_GRINGPARAM
#define ETHTOOL_GRINGPARAM	0x00000010 /* Get ring parameters */
/* for configuring RX/TX ring parameters */
#define ethtool_ringparam _kc_ethtool_ringparam
struct _kc_ethtool_ringparam {
	u32	cmd;	/* ETHTOOL_{G,S}RINGPARAM */

	/* Read only attributes.  These indicate the maximum number
	 * of pending RX/TX ring entries the driver will allow the
	 * user to set.
	 */
	u32	rx_max_pending;
	u32	rx_mini_max_pending;
	u32	rx_jumbo_max_pending;
	u32	tx_max_pending;

	/* Values changeable by the user.  The valid values are
	 * in the range 1 to the "*_max_pending" counterpart above.
	 */
	u32	rx_pending;
	u32	rx_mini_pending;
	u32	rx_jumbo_pending;
	u32	tx_pending;
};
#endif /* ETHTOOL_GRINGPARAM */

#ifndef ETHTOOL_SRINGPARAM
#define ETHTOOL_SRINGPARAM	0x00000011 /* Set ring parameters, priv. */
#endif
#ifndef ETHTOOL_GPAUSEPARAM
#define ETHTOOL_GPAUSEPARAM	0x00000012 /* Get pause parameters */
/* for configuring link flow control parameters */
#define ethtool_pauseparam _kc_ethtool_pauseparam
struct _kc_ethtool_pauseparam {
	u32	cmd;	/* ETHTOOL_{G,S}PAUSEPARAM */

	/* If the link is being auto-negotiated (via ethtool_cmd.autoneg
	 * being true) the user may set 'autonet' here non-zero to have the
	 * pause parameters be auto-negotiated too.  In such a case, the
	 * {rx,tx}_pause values below determine what capabilities are
	 * advertised.
	 *
	 * If 'autoneg' is zero or the link is not being auto-negotiated,
	 * then {rx,tx}_pause force the driver to use/not-use pause
	 * flow control.
	 */
	u32	autoneg;
	u32	rx_pause;
	u32	tx_pause;
};
#endif /* ETHTOOL_GPAUSEPARAM */

#ifndef ETHTOOL_SPAUSEPARAM
#define ETHTOOL_SPAUSEPARAM	0x00000013 /* Set pause parameters. */
#endif
#ifndef ETHTOOL_GRXCSUM
#define ETHTOOL_GRXCSUM		0x00000014 /* Get RX hw csum enable (ethtool_value) */
#endif
#ifndef ETHTOOL_SRXCSUM
#define ETHTOOL_SRXCSUM		0x00000015 /* Set RX hw csum enable (ethtool_value) */
#endif
#ifndef ETHTOOL_GTXCSUM
#define ETHTOOL_GTXCSUM		0x00000016 /* Get TX hw csum enable (ethtool_value) */
#endif
#ifndef ETHTOOL_STXCSUM
#define ETHTOOL_STXCSUM		0x00000017 /* Set TX hw csum enable (ethtool_value) */
#endif
#ifndef ETHTOOL_GSG
#define ETHTOOL_GSG		0x00000018 /* Get scatter-gather enable
					    * (ethtool_value) */
#endif
#ifndef ETHTOOL_SSG
#define ETHTOOL_SSG		0x00000019 /* Set scatter-gather enable
					    * (ethtool_value). */
#endif
#ifndef ETHTOOL_TEST
#define ETHTOOL_TEST		0x0000001a /* execute NIC self-test, priv. */
#endif
#ifndef ETHTOOL_GSTRINGS
#define ETHTOOL_GSTRINGS	0x0000001b /* get specified string set */
#endif
#ifndef ETHTOOL_PHYS_ID
#define ETHTOOL_PHYS_ID		0x0000001c /* identify the NIC */
#endif
#ifndef ETHTOOL_GSTATS
#define ETHTOOL_GSTATS		0x0000001d /* get NIC-specific statistics */
#endif
#ifndef ETHTOOL_GTSO
#define ETHTOOL_GTSO		0x0000001e /* Get TSO enable (ethtool_value) */
#endif
#ifndef ETHTOOL_STSO
#define ETHTOOL_STSO		0x0000001f /* Set TSO enable (ethtool_value) */
#endif

#ifndef ETHTOOL_BUSINFO_LEN
#define ETHTOOL_BUSINFO_LEN	32
#endif

/*****************************************************************************/

#ifdef CONFIG_SMP
 #define rtl_wmb()	smp_wmb()
 #define rtl_rmb()	smp_rmb()
#else
 #define rtl_wmb()	wmb()
 #define rtl_rmb()	rmb()
#endif

enum mcfg {
	CFG_METHOD_1 = 0x00,
	CFG_METHOD_2 = 0x01,
	CFG_METHOD_3 = 0x02,
	CFG_METHOD_4 = 0x04,
	CFG_METHOD_5 = 0x05,
	CFG_METHOD_6 = 0x06,
};

enum pcfg {
	PCFG_METHOD_1 = 0x03, /* PHY Reg 0x03 bit0-3 == 0x0000 */
	PCFG_METHOD_2 = 0x04, /* PHY Reg 0x03 bit0-3 == 0x0000 */
	PCFG_METHOD_3 = 0x05, /* PHY Reg 0x03 bit0-3 == 0x0000 */
	PCFG_METHOD_4 = 0x06, /* PHY Reg 0x03 bit0-3 == 0x0001 */
	PCFG_METHOD_5 = 0x07, /* PHY Reg 0x03 bit0-3 == 0x0002 */
	PCFG_METHOD_6 = 0x08, /* PHY Reg 0x03 bit0-3 == 0x0003 */
};

enum RTL8169_registers {
	MAC0 = 0x00,		/* Ethernet hardware address. */
	MAC4 = 0x04,
	MAR0 = 0x08,		/* Multicast filter. */
	CounterAddrLow = 0x10,
	CounterAddrHigh = 0x14,
	TxDescStartAddrLow = 0x20,
	TxDescStartAddrHigh = 0x24,
	TxHDescStartAddrLow = 0x28,
	TxHDescStartAddrHigh = 0x2c,
	FLASH = 0x30,
	ERSR = 0x36,
	ChipCmd = 0x37,
	TxPoll = 0x38,
	IntrMask = 0x3C,
	IntrStatus = 0x3E,
	TxConfig = 0x40,
	RxConfig = 0x44,
	RxMissed = 0x4C,
	Cfg9346 = 0x50,
	Config0 = 0x51,
	Config1 = 0x52,
	Config2 = 0x53,
	Config3 = 0x54,
	Config4 = 0x55,
	Config5 = 0x56,
	MultiIntr = 0x5C,
	PHYAR = 0x60,
	PHYstatus = 0x6C,
	Offset_7Ch = 0x7C,
	RxMaxSize = 0xDA,
	CPlusCmd = 0xE0,
	IntrMitigate = 0xE2,
	RxDescAddrLow = 0xE4,
	RxDescAddrHigh = 0xE8,
	Reserved1 = 0xEC,
	FuncEvent = 0xF0,
	FuncEventMask = 0xF4,
	FuncPresetState = 0xF8,
	FuncForceEvent = 0xFC,
};

enum RTL8169_register_content {
	/* InterruptStatusBits */
	SYSErr = 0x8000,
	PCSTimeout = 0x4000,
	SWInt = 0x0100,
	TxDescUnavail = 0x80,
	RxFIFOOver = 0x40,
	LinkChg = 0x20,
	RxOverflow = 0x10,
	TxErr = 0x08,
	TxOK = 0x04,
	RxErr = 0x02,
	RxOK = 0x01,

	/* RxStatusDesc */
	RxRWT	= (1 << 22),
	RxRES	= (1 << 21),
	RxRUNT	= (1 << 20),
	RxCRC	= (1 << 19),

	/* ChipCmdBits */
	CmdReset = 0x10,
	CmdRxEnb = 0x08,
	CmdTxEnb = 0x04,
	RxBufEmpty = 0x01,

	/* Cfg9346Bits */
	Cfg9346_Lock = 0x00,
	Cfg9346_Unlock = 0xC0,

	/* rx_mode_bits */
	AcceptErr = 0x20,
	AcceptRunt = 0x10,
	AcceptBroadcast = 0x08,
	AcceptMulticast = 0x04,
	AcceptMyPhys = 0x02,
	AcceptAllPhys = 0x01,

	/* Transmit Priority Polling*/
	HPQ = 0x80,
	NPQ = 0x40,
	FSWInt = 0x01,

	/* RxConfigBits */
	Reserved2_shift = 13,
	RxCfgDMAShift = 8,

	/* TxConfigBits */
	TxInterFrameGapShift = 24,
	TxDMAShift = 8,	/* DMA burst value (0-7) is shift this many bits */
	TxMACLoopBack = (1 << 17),	/* MAC loopback */

	/* Config1 register p.24 */
	PMEnable	= (1 << 0),	/* Power Management Enable */
	VPDEnable	= (1 << 1),	/* RTL8169 VPD eanble */

	/* Config2 register p.26 */
	PCI_Clock_66MHz	= 0x01,
	PCI_Clock_33MHz	= 0x00,

	/* Config3 register p.25 */
	MagicPacket	= (1 << 5),	/* Wake up when receives a Magic Packet */
	LinkUp		= (1 << 4),	/* Wake up when the cable connection is re-established */

	/* Config5 register p.27 */
	BWF		= (1 << 6),	/* Accept Broadcast wakeup frame */
	MWF		= (1 << 5),	/* Accept Multicast wakeup frame */
	UWF		= (1 << 4),	/* Accept Unicast wakeup frame */
	LanWake		= (1 << 1),	/* LanWake enable/disable */
	PMEStatus	= (1 << 0),	/* PME status can be reset by PCI RST# */

	/* CPlusCmd p.31 */
	EnAnaPLL	= (1 << 14),
	RxVlan		= (1 << 6),
	RxChkSum	= (1 << 5),
	PCIDAC		= (1 << 4),
	PCIMulRW	= (1 << 3),

	/* rtl8169_PHYstatus */
	TxFlowCtrl = 0x40,
	RxFlowCtrl = 0x20,
	_1000bpsF = 0x10,
	_100bps = 0x08,
	_10bps = 0x04,
	LinkStatus = 0x02,
	FullDup = 0x01,

	/* DumpCounterCommand */
	CounterDump = 0x8,

	/* PHY access */
	PHYAR_Flag = 0x80000000,
	PHYAR_Write = 0x80000000,
	PHYAR_Read = 0x00000000,
	PHYAR_Reg_Mask = 0x1f,
	PHYAR_Reg_shift = 16,
	PHYAR_Data_Mask = 0xffff,
};

enum _DescStatusBit {
	DescOwn		= (1 << 31), /* Descriptor is owned by NIC */
	RingEnd		= (1 << 30), /* End of descriptor ring */
	FirstFrag	= (1 << 29), /* First segment of a packet */
	LastFrag	= (1 << 28), /* Final segment of a packet */

	/* Tx private */
	LargeSend	= (1 << 27), /* TCP Large Send Offload (TSO) */
	MSSShift	= 16,        /* MSS value position */
	MSSMask		= 0xfff,     /* MSS value + LargeSend bit: 12 bits */
	IPCS		= (1 << 18), /* Calculate IP checksum */
	UDPCS		= (1 << 17), /* Calculate UDP/IP checksum */
	TCPCS		= (1 << 16), /* Calculate TCP/IP checksum */
	TxVlanTag	= (1 << 17), /* Add VLAN tag */

	/* Rx private */
	PID1		= (1 << 18), /* Protocol ID bit 1/2 */
	PID0		= (1 << 17), /* Protocol ID bit 2/2 */

#define RxProtoUDP	(PID1)
#define RxProtoTCP	(PID0)
#define RxProtoIP	(PID1 | PID0)
#define RxProtoMask	RxProtoIP

	IPFail		= (1 << 16), /* IP checksum failed */
	UDPFail		= (1 << 15), /* UDP/IP checksum failed */
	TCPFail		= (1 << 14), /* TCP/IP checksum failed */
	RxVlanTag	= (1 << 16), /* VLAN tag available */
};

#define RsvdMask	0x3fffc000

struct TxDesc {
	u32 opts1;
	u32 opts2;
	u64 addr;
};

struct RxDesc {
	u32 opts1;
	u32 opts2;
	u64 addr;
};

struct ring_info {
	struct sk_buff	*skb;
	u32		len;
	u8		__pad[sizeof(void *) - sizeof(u32)];
};

struct pci_resource {
	u8	cmd;
	u8	cls;
	u16	io_base_h;
	u16	io_base_l;
	u16	mem_base_h;
	u16	mem_base_l;
	u8	ilr;
	u16	resv_0x20_h;
	u16	resv_0x20_l;
	u16	resv_0x24_h;
	u16	resv_0x24_l;
};

struct rtl8169_private {
	void __iomem *mmio_addr;	/* memory map physical address */
	struct pci_dev *pci_dev;	/* Index of PCI device */
	struct net_device *dev;
#ifdef CONFIG_R8169_NAPI
   #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
	struct napi_struct napi;
   #endif
#endif
	struct net_device_stats stats;	/* statistics of net device */
	spinlock_t lock;		/* spin lock flag */
	spinlock_t phy_lock;		/* spin lock flag of phy */
	u32 msg_enable;
	int chipset;
	int mcfg;
	int pcfg;
	u32 cur_rx; /* Index into the Rx descriptor buffer of next Rx pkt. */
	u32 cur_tx; /* Index into the Tx descriptor buffer of next Rx pkt. */
	u32 dirty_rx;
	u32 dirty_tx;
	struct TxDesc *TxDescArray;	/* 256-aligned Tx descriptor ring */
	struct RxDesc *RxDescArray;	/* 256-aligned Rx descriptor ring */
	dma_addr_t TxPhyAddr;
	dma_addr_t RxPhyAddr;
	struct sk_buff *Rx_skbuff[NUM_RX_DESC];	/* Rx data buffers */
	struct ring_info tx_skb[NUM_TX_DESC];	/* Tx data buffers */
	unsigned align;
	unsigned rx_buf_sz;
	struct timer_list timer;
	struct timer_list esd_timer;
	struct pci_resource pci_cfg_space;
	unsigned int esd_flag;
	unsigned int pci_cfg_is_read;
	u16 cp_cmd;
	u16 intr_mask;
	int phy_auto_nego_reg;
	int phy_1000_ctrl_reg;
	u8 mac_addr[NODE_ADDRESS_SIZE];

#ifdef CONFIG_R8169_VLAN
	struct vlan_group *vlgrp;
#endif
	int (*set_speed)(struct net_device *, u8 autoneg, u16 speed, u8 duplex);
	void (*get_settings)(struct net_device *, struct ethtool_cmd *);
	void (*phy_reset_enable)(struct net_device *);
	unsigned int (*phy_reset_pending)(struct net_device *);
	unsigned int (*link_ok)(struct net_device *);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	struct work_struct task;
#else
	struct delayed_work task;
#endif
	unsigned wol_enabled;
	unsigned features;
};

