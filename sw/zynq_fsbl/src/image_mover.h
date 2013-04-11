/******************************************************************************
*
* (c) Copyright 2011-12 Xilinx, Inc. All rights reserved.
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
* @file image_mover.h
*
* This file contains the interface for moving the image from FLASH to OCM

*
* <pre>
* MODIFICATION HISTORY:
*
* Ver	Who	Date		Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a jz	03/04/11	Initial release
* 2.00a jz	06/04/11	partition header expands to 12 words
*
* </pre>
*
* @note
*
******************************************************************************/
#ifndef ___IMAGE_MOVER_H___
#define ___IMAGE_MOVER_H___


#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "fsbl.h"

/************************** Constant Definitions *****************************/
/* Boot Image Header defines */
#define IMAGE_PHDR_OFFSET			0x09C	/**< Start of partition headers */

/* Partition Header defines */
#define PARTITION_IMAGE_WORD_LEN_OFFSET	0x00	/**< Word length of image */
#define PARTITION_DATA_WORD_LEN_OFFSET	0x04	/**< Word length of data */
#define PARTITION_WORD_LEN_OFFSET	0x08	/**< Word length of partition */
#define PARTITION_LOAD_ADDRESS_OFFSET	0x0C	/**< Load addr in DDR	*/
#define PARTITION_EXEC_ADDRESS_OFFSET	0x10	/**< Addr to start executing */
#define PARTITION_ADDR_OFFSET		0x14	/**< Partition word offset */
#define PARTITION_ATTRIBUTE_OFFSET	0x18	/**< Partition type */
#define PARTITION_HDR_CHECKSUM_OFFSET	0x3C	/**< Header Checksum offset */
#define PARTITION_HDR_CHECKSUM_WORD_COUNT 0xF	/**< Checksum word count */
#define PARTITION_HDR_WORD_COUNT	0x10	/**< Header word len */
#define PARTITION_HDR_TOTAL_LEN		0x40	/**< One partition hdr length*/

/* Attribute word defines */
#define ATTRIBUTE_PS_IMAGE_MASK		0x10	/**< Code partition */
#define ATTRIBUTE_PL_IMAGE_MASK		0x20	/**< Bit stream partition */

/**************************** Type Definitions *******************************/
typedef u32 (*ImageMoverType)( u32 SourceAddress,
				u32 DestinationAddress,
				u32 LengthBytes);

/***************** Macros (Inline Functions) Definitions *********************/
#define MoverIn32		Xil_In32
#define MoverOut32		Xil_Out32

/************************** Function Prototypes ******************************/

u32 PartitionMove(u32 ImageAddr, int PartitionNumber);

/************************** Variable Definitions *****************************/
#ifdef __cplusplus
}
#endif


#endif /* ___IMAGE_MOVER_H___ */

