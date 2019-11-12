/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2016 Intel Corporation. All rights reserved.
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *         Keyon Jie <yang.jie@linux.intel.com>
 */

#ifndef __INCLUDE_DEBUG__
#define __INCLUDE_DEBUG__

#include <sof/sof.h>
#include <config.h>
#include <sof/mailbox.h>
#include <platform/platform.h>
#include <platform/memory.h>
#include <stdint.h>
#include <stdlib.h>

#if CONFIG_GDB_DEBUG
#define DEBUG_GDB	1
#else
#define DEBUG_GDB	0
#endif

#ifdef CONFIG_DEBUG

#define DEBUG_SET_FW_READY_FLAGS				\
(								\
	SOF_IPC_INFO_BUILD |					\
	(DEBUG_LOCKS ? SOF_IPC_INFO_LOCKS : 0) |		\
	(DEBUG_LOCKS_VERBOSE ? SOF_IPC_INFO_LOCKSV : 0) |	\
	(DEBUG_GDB ? SOF_IPC_INFO_GDB : 0)			\
)

/* dump file and line to start of mailbox or shared memory */
#define dbg() \
	do { \
		volatile uint32_t *__m = (uint32_t *)mailbox_get_debug_base(); \
		*(__m++) = (__FILE__[0] << 24) + (__FILE__[1] << 16) +\
			 (__FILE__[2] << 8) + (__FILE__[3]); \
		*(__m++) = (__func__[0] << 24) + (__func__[1] << 16) + \
			(__func__[2] << 8) + (__func__[3]); \
		*__m = __LINE__; \
	} while (0)

/* dump file and line to offset in mailbox or shared memory */
#define dbg_at(__x) \
	do { \
		volatile uint32_t *__m = \
			(uint32_t *)mailbox_get_debug_base() + __x; \
		*(__m++) = (__FILE__[0] << 24) + (__FILE__[1] << 16) +\
			 (__FILE__[2] << 8) + (__FILE__[3]); \
		*(__m++) = (__func__[0] << 24) + (__func__[1] << 16) + \
			(__func__[2] << 8) + (__func__[3]); \
		*__m = __LINE__; \
	} while (0)

/* dump value to start of mailbox or shared memory */
#define dbg_val(__v) \
	do { \
		volatile uint32_t *__m = \
			(volatile uint32_t *)mailbox_get_debug_base(); \
		*__m = __v; \
	} while (0)

/* dump value to offset in mailbox or shared memory */
#define dbg_val_at(__v, __x) \
	do { \
		volatile uint32_t *__m = \
			(volatile uint32_t *)mailbox_get_debug_base() + __x; \
		*__m = __v; \
	} while (0)

/* dump data area at addr and size count to start of mailbox or shared memory */
#define dump(addr, count) \
	do { \
		volatile uint32_t *__m = (uint32_t *)mailbox_get_debug_base(); \
		volatile uint32_t *__a = (uint32_t *)addr; \
		volatile int __c = count; \
		while (__c--) \
			*(__m++) = *(__a++); \
	} while (0)

/* dump data area at addr and size count at mailbox offset or shared memory */
#define dump_at(addr, count, offset) \
	do { \
		volatile uint32_t *__m = \
			(uint32_t *)mailbox_get_debug_base() + offset; \
		volatile uint32_t *__a = (uint32_t *)addr; \
		volatile int __c = count; \
		while (__c--) \
			*(__m++) = *(__a++); \
	} while (0)

/* dump object to start of mailbox */
#define dump_object(__o) \
	do {						\
		dbg();					\
		dump(&__o, sizeof(__o) >> 2);		\
	} while (0)

/* dump object from pointer at start of mailbox */
#define dump_object_ptr(__o) \
	do {						\
		dbg();					\
		dump(__o, sizeof(*(__o)) >> 2);		\
	} while (0)

#define dump_object_ptr_at(__o, __at) \
	do {						\
		dbg();					\
		dump_at(__o, sizeof(*(__o)) >> 2, __at);\
	} while (0)

#else

#define DEBUG_SET_FW_READY_FLAGS				\
(								\
	(DEBUG_LOCKS ? SOF_IPC_INFO_LOCKS : 0) |		\
	(DEBUG_LOCKS_VERBOSE ? SOF_IPC_INFO_LOCKSV : 0) |	\
	(DEBUG_GDB ? SOF_IPC_INFO_GDB : 0)			\
)

#define dbg()
#define dbg_at(__x)
#define dbg_val(__v)
#define dbg_val_at(__v, __x)
#define dump(addr, count)
#define dump_object(__o)
#define dump_object_ptr(__o)
#endif

/* dump stack as part of panic */
static inline uint32_t dump_stack(uint32_t p, void *addr, size_t offset,
				  size_t limit, uintptr_t *stack_ptr)
{
	return p;
}

#endif
