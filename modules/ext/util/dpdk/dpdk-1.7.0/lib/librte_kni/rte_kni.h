/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RTE_KNI_H_
#define _RTE_KNI_H_

/**
 * @file
 * RTE KNI
 *
 * The KNI library provides the ability to create and destroy kernel NIC
 * interfaces that may be used by the RTE application to receive/transmit
 * packets from/to Linux kernel net interfaces.
 *
 * This library provide two APIs to burst receive packets from KNI interfaces,
 * and burst transmit packets to KNI interfaces.
 */

#include <rte_pci.h>
#include <rte_mbuf.h>

#include <exec-env/rte_kni_common.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rte_kni;

/**
 * Structure which has the function pointers for KNI interface.
 */
struct rte_kni_ops {
	uint8_t port_id; /* Port ID */

	/* Pointer to function of changing MTU */
	int (*change_mtu)(uint8_t port_id, unsigned new_mtu);

	/* Pointer to function of configuring network interface */
	int (*config_network_if)(uint8_t port_id, uint8_t if_up);
};

/**
 * Structure for configuring KNI device.
 */
struct rte_kni_conf {
	/*
	 * KNI name which will be used in relevant network device.
	 * Let the name as short as possible, as it will be part of
	 * memzone name.
	 */
	char name[RTE_KNI_NAMESIZE];
	uint32_t core_id;   /* Core ID to bind kernel thread on */
	uint16_t group_id;  /* Group ID */
	unsigned mbuf_size; /* mbuf size */
	struct rte_pci_addr addr;
	struct rte_pci_id id;

	uint8_t force_bind : 1; /* Flag to bind kernel thread */
#ifdef RTE_LIBRW_PIOT
  	uint8_t no_data : 1; /* Flag to indicate no ethtool support*/
        uint8_t no_pci : 1;     /* Flag to indicate that there is no pci*/
        uint8_t no_tx : 1;     /* Flag to indicate that there is no pci*/
        uint8_t always_up: 1;
        uint8_t loopback:1;
        uint8_t no_user_ring:1;
        uint32_t ifindex;       /* ifindex to be used by the kerenl when creating netdevice*/
        uint16_t mtu;
        uint16_t vlanid;
        char     mac[6];
        char     netns_name[64];
        int      netns_fd;
#ifdef RTE_LIBRW_NOHUGE
  uint8_t nohuge;
  uint32_t nl_pid;
#endif
#endif
};

/**
 * Initialize and preallocate KNI subsystem
 *
 * This function is to be executed on the MASTER lcore only, after EAL
 * initialization and before any KNI interface is attempted to be
 * allocated
 *
 * @param max_kni_ifaces
 *  The maximum number of KNI interfaces that can coexist concurrently
 */
extern void rte_kni_init(unsigned int max_kni_ifaces);


/**
 * Allocate KNI interface according to the port id, mbuf size, mbuf pool,
 * configurations and callbacks for kernel requests.The KNI interface created
 * in the kernel space is the net interface the traditional Linux application
 * talking to.
 *
 * The rte_kni_alloc shall not be called before rte_kni_init() has been
 * called. rte_kni_alloc is thread safe.
 *
 * @param pktmbuf_pool
 *  The mempool for allocting mbufs for packets.
 * @param conf
 *  The pointer to the configurations of the KNI device.
 * @param ops
 *  The pointer to the callbacks for the KNI kernel requests.
 *
 * @return
 *  - The pointer to the context of a KNI interface.
 *  - NULL indicate error.
 */
extern struct rte_kni *rte_kni_alloc(struct rte_mempool *pktmbuf_pool,
				     const struct rte_kni_conf *conf,
				     struct rte_kni_ops *ops);

/**
 * It create a KNI device for specific port.
 *
 * Note: It is deprecated and just for backward compatibility.
 *
 * @param port_id
 *  Port ID.
 * @param mbuf_size
 *  mbuf size.
 * @param pktmbuf_pool
 *  The mempool for allocting mbufs for packets.
 * @param ops
 *  The pointer to the callbacks for the KNI kernel requests.
 *
 * @return
 *  - The pointer to the context of a KNI interface.
 *  - NULL indicate error.
 */
extern struct rte_kni *rte_kni_create(uint8_t port_id,
				      unsigned mbuf_size,
				      struct rte_mempool *pktmbuf_pool,
				      struct rte_kni_ops *ops) \
				      __attribute__ ((deprecated));

/**
 * Release KNI interface according to the context. It will also release the
 * paired KNI interface in kernel space. All processing on the specific KNI
 * context need to be stopped before calling this interface.
 *
 * rte_kni_release is thread safe.
 *
 * @param kni
 *  The pointer to the context of an existent KNI interface.
 *
 * @return
 *  - 0 indicates success.
 *  - negative value indicates failure.
 */
extern int rte_kni_release(struct rte_kni *kni);

/**
 * It is used to handle the request mbufs sent from kernel space.
 * Then analyzes it and calls the specific actions for the specific requests.
 * Finally constructs the response mbuf and puts it back to the resp_q.
 *
 * @param kni
 *  The pointer to the context of an existent KNI interface.
 *
 * @return
 *  - 0
 *  - negative value indicates failure.
 */
extern int rte_kni_handle_request(struct rte_kni *kni);

/**
 * Retrieve a burst of packets from a KNI interface. The retrieved packets are
 * stored in rte_mbuf structures whose pointers are supplied in the array of
 * mbufs, and the maximum number is indicated by num. It handles the freeing of
 * the mbufs in the free queue of KNI interface.
 *
 * @param kni
 *  The KNI interface context.
 * @param mbufs
 *  The array to store the pointers of mbufs.
 * @param num
 *  The maximum number per burst.
 *
 * @return
 *  The actual number of packets retrieved.
 */
extern unsigned rte_kni_rx_burst(struct rte_kni *kni,
		struct rte_mbuf **mbufs, unsigned num);

/**
 * Send a burst of packets to a KNI interface. The packets to be sent out are
 * stored in rte_mbuf structures whose pointers are supplied in the array of
 * mbufs, and the maximum number is indicated by num. It handles allocating
 * the mbufs for KNI interface alloc queue.
 *
 * @param kni
 *  The KNI interface context.
 * @param mbufs
 *  The array to store the pointers of mbufs.
 * @param num
 *  The maximum number per burst.
 *
 * @return
 *  The actual number of packets sent.
 */
extern unsigned rte_kni_tx_burst(struct rte_kni *kni,
		struct rte_mbuf **mbufs, unsigned num);

/**
 * Get the port id from KNI interface.
 *
 * Note: It is deprecated and just for backward compatibility.
 *
 * @param kni
 *  The KNI interface context.
 *
 * @return
 *  On success: The port id.
 *  On failure: ~0x0
 */
extern uint8_t rte_kni_get_port_id(struct rte_kni *kni) \
				__attribute__ ((deprecated));

/**
 * Get the KNI context of its name.
 *
 * @param name
 *  pointer to the KNI device name.
 *
 * @return
 *  On success: Pointer to KNI interface.
 *  On failure: NULL.
 */
extern struct rte_kni *rte_kni_get(const char *name);

/**
 * Get the KNI context of the specific port.
 *
 * Note: It is deprecated and just for backward compatibility.
 *
 * @param port_id
 *  the port id.
 *
 * @return
 *  On success: Pointer to KNI interface.
 *  On failure: NULL
 */
extern struct rte_kni *rte_kni_info_get(uint8_t port_id) \
				__attribute__ ((deprecated));

/**
 * Register KNI request handling for a specified port,and it can
 * be called by master process or slave process.
 *
 * @param kni
 *  pointer to struct rte_kni.
 * @param ops
 *  ponter to struct rte_kni_ops.
 *
 * @return
 *  On success: 0
 *  On failure: -1
 */
extern int rte_kni_register_handlers(struct rte_kni *kni,
			struct rte_kni_ops *ops);

/**
 *  Unregister KNI request handling for a specified port.
 *
 *  @param kni
 *   pointer to struct rte_kni.
 *
 *  @return
 *   On success: 0
 *   On failure: -1
 */
extern int rte_kni_unregister_handlers(struct rte_kni *kni);

/**
 *  close KNI device.
 *
 *  @param void
 *
 *  @return
 *   void
 */
extern void rte_kni_close(void);
#ifdef RTE_LIBRW_PIOT
/**
 * This function called by piot will make an ioctl to the kernel to get the socket
 * filters if any
 * @param[in]  kni  - kni instance
 * @param[in]  inode-id - inode-id of the AF_PACKET socket
 * @param[in]  skt - socket pointer for verification
 * @param[out] len - number of filters
 * @param[out ] filter - the actual filters
 *
 * @returns 0  if success
 * @returns -1 if error
 */
extern int rte_kni_get_packet_socket_info(struct rte_kni *kni, uint32_t inode_id,
                                          void *skt, unsigned short *len,
                                          struct sock_filter *skt_filter);
  extern int rte_kni_fifo_get(void *fifo, void **mbufs, int num);
  extern int rte_kni_fifo_put(void *fifo, void **mbufs, int num);
  extern int
rte_kni_clear_packet_counters(void);

/**
 * KNI context
 */
struct rte_kni {
	char name[RTE_KNI_NAMESIZE];        /**< KNI interface name */
	uint16_t group_id;                  /**< Group ID of KNI devices */
	uint32_t slot_id;                   /**< KNI pool slot ID */
	struct rte_mempool *pktmbuf_pool;   /**< pkt mbuf mempool */
	unsigned mbuf_size;                 /**< mbuf size */

	struct rte_kni_fifo *tx_q;          /**< TX queue */
	struct rte_kni_fifo *rx_q;          /**< RX queue */
	struct rte_kni_fifo *alloc_q;       /**< Allocated mbufs queue */
	struct rte_kni_fifo *free_q;        /**< To be freed mbufs queue */

	/* For request & response */
	struct rte_kni_fifo *req_q;         /**< Request queue */
	struct rte_kni_fifo *resp_q;        /**< Response queue */
	void * sync_addr;                   /**< Req/Resp Mem address */

	struct rte_kni_ops ops;             /**< operations for request */
	uint8_t in_use : 1;                 /**< kni in use */
};
#endif
#ifdef __cplusplus
}
#endif

#endif /* _RTE_KNI_H_ */

