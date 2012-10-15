/*
 * Copyright (c) 2010, 2012 Richard Braun.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _X86_BOOT_H
#define _X86_BOOT_H

#include <lib/macros.h>
#include <machine/param.h>

/*
 * The kernel is physically loaded at BOOT_OFFSET by the boot loader. It
 * is divided in two parts: the .init section which uses physical addresses
 * and the main kernel code and data at KERNEL_OFFSET.
 *
 * See the linker script for more information.
 */
#define BOOT_OFFSET     DECL_CONST(0x100000, UL)
#define KERNEL_OFFSET   VM_MAX_KERNEL_ADDRESS

/*
 * Virtual to physical address translation macro.
 */
#define BOOT_VTOP(addr) ((addr) - KERNEL_OFFSET)

/*
 * Size of the stack used to bootstrap the kernel.
 */
#define BOOT_STACK_SIZE PAGE_SIZE

/*
 * Address where the MP trampoline code is copied and run at.
 *
 * It must reside at a free location in the first segment and be page
 * aligned.
 */
#define BOOT_MP_TRAMPOLINE_ADDR 0x7000

#ifndef __ASSEMBLY__

#include <machine/multiboot.h>
#include <machine/pmap.h>

/*
 * Stack used to bootstrap the kernel.
 */
extern char boot_stack[BOOT_STACK_SIZE];

/*
 * Common stack used by APs to bootstrap.
 */
extern char boot_ap_stack[BOOT_STACK_SIZE];

/*
 * This variable contains the CPU ID of an AP during its early boot.
 */
extern unsigned long boot_ap_id;

/*
 * After its early boot, an AP enables paging and jumps to virtual
 * addresses. At this point, it switches to a per-AP preallocated
 * stack. This variable contains the (virtual) address of that stack.
 */
extern unsigned long boot_ap_stack_addr;

/*
 * Size of the trampoline code used for APs.
 */
extern uint32_t boot_mp_trampoline_size;

/*
 * Address of the MP trampoline code.
 */
void boot_mp_trampoline(void);

/*
 * Print the given message and halt the system immediately.
 *
 * This function allows early initialization code to print something helpful
 * before printk is available.
 */
void __noreturn boot_panic(const char *s);

/*
 * This function is called by the bootstrap code before paging is enabled.
 * It establishes a direct mapping of the kernel at virtual addresses and
 * returns the physical address of the page directory. It is up to the
 * caller to actually enable paging.
 *
 * TODO Update comment.
 */
pmap_pte_t * boot_setup_paging(const struct multiboot_raw_info *mbi,
                               unsigned long eax);

/*
 * Main entry point, called directly after basic paging is initialized.
 */
void boot_main(void);

/*
 * Entry point for APs.
 */
void boot_ap(void);

#endif /* __ASSEMBLY__ */

#endif /* _X86_BOOT_H */
