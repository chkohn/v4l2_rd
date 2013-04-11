/******************************************************************************
*
* (c) Copyright 2011-2012 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
*******************************************************************************/
/*****************************************************************************/
/**
*
* @file fsbl.h
*
* Contains the function prototypes, defines and macros for the
* First Stage Boot Loader (FSBL) functionality
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver	Who	Date		Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a	jz	03/04/11	Initial release
* 2.00a	mb 	06/06/12	Removed the qspi define, will be picked from
*				xparameters.h file
* 3.00a np/mb 	08/08/12	Added the error codes for the FSBL hook errors.
* 				Added the debug levels
* </pre>
*
* @note
*
* Flags in FSBL
*
* FSBL_PERF
*
* This Flag can be set at compilation time. This flag is set for
* measuring the performance of FSBL.That is the time taken to execute is
* measured.when this flag is set.Execution time with reference to 
* global timer is taken here
*
* Total Execution time is the time taken for executing FSBL till handoff
* to any application .
* If there is a bitstream in the partition header then the
* execution time includes the copying of the bitstream to DDR
* (in case of SD/NAND bootmode)
* and programming the devcfg dma is accounted.
*
* FSBL provides two debug levels
* DEBUG GENERAL - fsbl_printf under this category will appear only when the
* FSBL_DEBUG flag is set during compilation
* DEBUG_INFO - fsbl_printf under this category will appear when the
* FSBL_DEBUG_INFO flag is set during compilation
* For a more detailed output log can be used.
* These macros are input to the fsbl_printf function
*
* DEBUG LEVELS
* FSBL_DEBUG level is level 1, when this flag is set all the fsbl_prints
* that are with the DEBUG_GENERAL argument are shown
* FSBL_DEBUG_INFO is level 2, when this flag is set during the
* compilation , the fsbl_printf with DEBUG_INFO will appear on the com port
*
* DEFAULT LEVEL
* By default no print messages will appear.
*
*
* NON_PS_INSTANTIATED_BITSTREAM
*
* FSBL will not enable the level shifters for a NON PS instantiated
* Bitstream.This flag can be set during compilation for a NON PS instantiated
* bitstream
*
* ECC_ENABLE
* This flag will be defined in the ps7_init.h file when ECC is enabled
* in the DDR configuration (XPS GUI)
*
*
*
*******************************************************************************/
#ifndef XIL_FSBL_H
#define XIL_FSBL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xil_io.h"
#include "xparameters.h"
#include "xpseudo_asm.h"
#include "xil_printf.h"
#include "pcap.h"
#include "fsbl_debug.h"
#include "ps7_init.h"
#ifdef FSBL_PERF
#include "xtime_l.h"
#include <stdio.h>
#endif


/************************** Constant Definitions *****************************/
#define WORD_LENGTH_SHIFT	2

/* On a Successful handoff to an application FSBL sets this SUCCESS code
 */
#define SUCCESSFUL_HANDOFF		0x1	/**< Successful Handoff */
/* ERROR CODES
 * The following are the Error codes that FSBL uses
 * If the Debug prints are enabled only then the error codes will be
 * seen on the com port.Without the debug prints enabled no error codes will
 * be visible.There are not saved in any register
 * Boot Mode States used for error and status output
 * Error codes are defined below
 *
 */
#define ILLEGAL_BOOT_MODE		0xA000 /**< Illegal boot mode */
#define ILLEGAL_RETURN			0xA001 /**< Illegal return */
#define EXCEPTION_TAKEN_FAIL		0xA002 /**< Exception Error */
#define PCAP_INIT_FAIL			0xA003 /**< Pcap driver Init Failed */
#define DECRYPTION_FAIL			0xA004 /**< Decryption Failed */
#define BITSTREAM_DOWNLOAD_FAIL		0xA005 /**< Bitstream download fail */
#define DMA_TRANSFER_FAIL		0xA006 /**< DMA Transfer Fail */
#define FSBL_HANGS			0xA007 /**< FSBL hangs */
#define INVALID_FLASH_ADDRESS		0xA008 /**< Invalid Flash Address */
#define DDR_INIT_FAIL			0xA009 /**< DDR Init Fail */
#define NO_DDR				0xA010 /**< DDR missing */
#define SD_INIT_FAIL			0xA011 /**< SD Init fail */
#define NAND_INIT_FAIL			0xA012 /**< Nand Init Fail */
#define DDR_ECC_INIT_FAIL		0xA013 /**< DDR ECC Init Fail */

#define PARTITION_HEADER_CORRUPTION	0xA100 /**< Partitions Header Corruption */
#define IMAGE_HAS_NO_PARTITIONS		0xA101 /**< No Partitions */
#define INVALID_PARTITION_LENGTH	0xA102 /**< Invalid Partition Length */
#define INVALID_PARTITION_HEADER	0xA103 /**< Invalid Partition Header */
#define EMPTY_PARTITION_HEADER		0xA104 /**< No Partition header */
#define FLASH_ENCRYPTION_UNSUPPORTED	0xA105 /**< No decryption on NAND, SD */
#define SRC_UNALIGNED			0xA106 /**< DMA SRC not 64-byte aligned */
#define DST_UNALIGNED			0xA107 /**< DMA DEST not 64-byte aligned */
#define TOO_MANY_PARTITIONS		0xA108 /**< More Partitions than supported */
#define ILLEGAL_DECRYPTION		0xA109 /**< No encryption allowed */

/* FSBL Exception error codes */
#define EXCEPTION_ID_UNDEFINED_INT	0xA301 /**< Undefined INT Exception */
#define EXCEPTION_ID_SWI_INT		0xA302 /**< SWI INT Exception */
#define EXCEPTION_ID_PREFETCH_ABORT_INT	0xA303 /**< Prefetch Abort xception */
#define EXCEPTION_ID_DATA_ABORT_INT	0xA304 /**< Data Abort Exception */
#define EXCEPTION_ID_IRQ_INT		0xA305 /**< IRQ Exception Occurred */
#define EXCEPTION_ID_FIQ_INT		0xA306 /**< FIQ Exception Occurred */

/* FSBL hook routine failures */
#define FSBL_HANDOFF_HOOK_FAIL		0xA401 /** FSBL handoff hook failed */
#define FSBL_BEFORE_BSTREAM_HOOK_FAIL	0xA402 /** FSBL before bit stream 
						download hook failed */
#define FSBL_AFTER_BSTREAM_HOOK_FAIL	0xA403 /** FSBL after bitstream
						download hook failed */

/* Watchdog related Error codes */
#define WDT_RESET_OCCURED		0xA501 /** WDT Reset happened in FSBL */
#define WDT_INIT_FAIL			0xA502 /** WDT driver INIT failed */

/* SLCR Register offsets used by FSBL */
#define PS_RST_CTRL_REG			(XPS_SYS_CTRL_BASEADDR + 0x200)
#define FPGA_RESET_REG			(XPS_SYS_CTRL_BASEADDR + 0x240)
#define RESET_REASON_REG		(XPS_SYS_CTRL_BASEADDR + 0x250)
#define RESET_REASON_CLR		(XPS_SYS_CTRL_BASEADDR + 0x254)
#define REBOOT_STATUS_REG		(XPS_SYS_CTRL_BASEADDR + 0x258)
#define BOOT_MODE_REG			(XPS_SYS_CTRL_BASEADDR + 0x25C)
#define PS_LVL_SHFTR_EN			(XPS_SYS_CTRL_BASEADDR + 0x900)
/* Level Shifter Register */

/* PS reset control register define */
#define PS_RST_MASK			0x1	/**< PS software reset */
/* SLCR BOOT Mode Register defines */
#define BOOT_MODES_MASK			0x00000007 /**< FLASH types */
/* Boot Modes */
#define JTAG_MODE			0x00000000 /**< JTAG Boot Mode */
#define QSPI_MODE			0x00000001 /**< QSPI Boot Mode */
#define NOR_FLASH_MODE			0x00000002 /**< NOR Boot Mode */
#define NAND_FLASH_MODE			0x00000004 /**< NAND Boot Mode */
#define SD_MODE				0x00000005 /**< SD Boot Mode */

#define RESET_REASON_SRST		0x00000020 /* * Reason for reset is SRST */
#define RESET_REASON_SWDT		0x00000001 /* * Reason for reset is SWDT */
/* Move image failure flag */
#define MOVE_IMAGE_FAIL			0xFFFFFFFF
/* Golden image offset */
#define GOLDEN_IMAGE_OFFSET		0x8000

/* Silicon Version */
#define SILICON_VERSION_0 0
#define SILICON_VERSION_1 1

/* ECC Enabling */
#ifdef ECC_ENABLE
#define DDR_ADDR_END 	XPAR_PS7_DDR_0_S_AXI_HIGHADDR + 1
#define DDR_ADDR_START 	XPAR_PS7_DDR_0_S_AXI_BASEADDR
#define DDR_LENGTH 	(DDR_ADDR_END - DDR_ADDR_START)
#define SRCADDR_OCM 	0x00000000
/* Clear the ECC ERROR CODE */
#define DDR_CHE_ECC_CONTROL_REG_OFFSET	(XPS_DDR_CTRL_BASEADDR + 0xC4)
#define CLEAR_ERROR_ECC	0x3
#endif
/* After the DDR is initialised from the ps7_init() FSBL will check
 * whether the DDR initialisation is properly done by doing a sanity test
 * To improved performance this could be removed.
 * These macros are used in the Check_ddr_init function
 *
 */
#define DDR_MEMORY_1	0x00100000
#define DDR_MEMORY_2	0x00200000
#define PATTERN		0xAA55AA55

/* IO accessors */ 
#define FsblIn32	Xil_In32
#define FsblOut32	Xil_Out32
/* These are the SLCR lock and unlock macros */
#define SlcrUnlock()	Xil_Out32(XPS_SYS_CTRL_BASEADDR + 0x08, 0xDF0DDF0D)
#define SlcrLock()	Xil_Out32(XPS_SYS_CTRL_BASEADDR + 0x04, 0x767B767B)

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

void OutputStatus(u32 State);
void FsblFallback(void);

int FsblSetNextPartition(int Num);
void *(memcpy_rom)(void * s1, const void * s2, u32 n);
char *strcpy_rom(char *Dest, const char *Src);

void ClearFSBLIn(void);
int Check_ddr_init(void);

#ifdef FSBL_PERF
/* Global Timer is always clocked at half of the CPU frequency */
#define COUNTS_PER_SECOND	(XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ /2)
void FsblGetGlobalTime (XTime tCur);
void FsblMeasurePerfTime (XTime tCur, XTime tEnd);
#endif
void GetSiliconVersion(void);
void FsblHandoffExit(u32 FsblStartAddr);
void FsblHandoffJtagExit();
/************************** Variable Definitions *****************************/
extern int SkipPartition;

/***************** Macros (Inline Functions) Definitions *********************/

#ifdef __cplusplus
}
#endif

#endif	/* end of protection macro */
