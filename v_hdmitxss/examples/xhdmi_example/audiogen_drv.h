/*
 * Copyright (c) 2016 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*****************************************************************************/
/**
 *
 * @file audiogen_drv.h
 *
 * This file contains ..
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date        Changes
 * ----- --- ----------   -----------------------------------------------
 * X.XX  XX  YYYY/MM/DD   ...
 * 1.00  RHe 2014/12/00   First release
 * 1.1   RHe 2015/07/30   Updated ACR GetNVal to be dependent of the
 *                        TMDS character rate instead of the video mode.
 * 1.2   MMO 2017/09/05   Replace U32 with UINTPTR for 64 Bit Addressing Support
 * </pre>
 *
 ******************************************************************************/

#ifndef AUDIOGEN_DRV_H_
#define AUDIOGEN_DRV_H_

#include "xvidc.h"
#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"
#include "sleep.h"

typedef struct {
  u32 TMDSCharRate;
  u32 ACR_NVal[7];
} ACR_N_Table_t;

typedef enum {
  XAUD_SRATE_32K = 0,
  XAUD_SRATE_44K1,
  XAUD_SRATE_48K,
  XAUD_SRATE_88K2,
  XAUD_SRATE_96K,
  XAUD_SRATE_176K4,
  XAUD_SRATE_192K,
  XAUD_NUM_SUPPORTED_SRATE
} AudioRate_t;

typedef enum {
  XAUD_PAT_MUTE = 0,
  XAUD_PAT_SINE = 1,
  XAUD_PAT_PING = 2,
  XAUD_PAT_RAMP = 3,
  XAUD_NUM_SUPPORTED_PAT
} AudioPattern_t;

typedef struct {
  u8 Mult;
  u8 Div;
  u8 Clk0Div;
} XhdmiAudioGen_PLL_t;

typedef struct {
  UINTPTR AudGenBase;
  UINTPTR ACRCtrlBase;
  UINTPTR AudClkGenBase;
  XhdmiAudioGen_PLL_t AudClkPLL;
} XhdmiAudioGen_t;

int XhdmiAudGen_Init(XhdmiAudioGen_t *AudioGen, UINTPTR AudGen_Base,
                                 UINTPTR ACRCtrl_Base, UINTPTR AudClk_Gen_Base);
int XhdmiAudGen_Reset(XhdmiAudioGen_t *AudioGen);
int XhdmiAudGen_Start(XhdmiAudioGen_t *AudioGen, u8 setclr);
int XhdmiAudGen_Drop(XhdmiAudioGen_t *AudioGen, u8 setclr);
int XhdmiAudGen_SetSampleRate(XhdmiAudioGen_t *AudioGen, u32 TMDSCharRate, AudioRate_t SampleRate);
int XhdmiAudGen_SetPattern(XhdmiAudioGen_t *AudioGen, u8 ChannelID, AudioPattern_t Pattern);
int XhdmiAudGen_SetEnabChannels(XhdmiAudioGen_t *AudioGen, u8 NumChannels);
int XhdmiAudGen_GetEnabChannels(XhdmiAudioGen_t *AudioGen);
int XhdmiAudGen_SetChSts(XhdmiAudioGen_t *AudioGen, AudioRate_t SampleRate);
int XhdmiAudGen_UpdateConfig(XhdmiAudioGen_t *AudioGen);

int XhdmiAudGen_SetAudClk(XhdmiAudioGen_t *AudioGen, AudioRate_t SampleRate);
int XhdmiAudGen_SetAudClkParam(XhdmiAudioGen_t *AudioGen, AudioRate_t SampleRate);
int XhdmiAudGen_AudClkConfig(XhdmiAudioGen_t *AudioGen);
int XhdmiAudGen_GetAudClk(AudioRate_t SampleRate);

int XhdmiACRCtrl_AudioReset(XhdmiAudioGen_t *AudioGen, u8 setclr);
int XhdmiACRCtrl_Enab(XhdmiAudioGen_t *AudioGen, u8 setclr);
int XhdmiACRCtrl_Sel(XhdmiAudioGen_t *AudioGen, u8 sel);
int XhdmiACRCtrl_SetNVal(XhdmiAudioGen_t *AudioGen, u32 NVal);

#define XAudGen_In32   Xil_In32  /**< Input Operations */
#define XAudGen_Out32  Xil_Out32 /**< Output Operations */

/*****************************************************************************/
/**
 *
 * This function macro reads the given register.
 *
 * @param  BaseAddress is the base address of the Audio Generator core.
 * @param  RegOffset is the register offset of the register
 *
 * @return The 32-bit value of the register.
 *
 * @note   C-style signature:
 *   u32 XAudGen_ReadReg(u32 BaseAddress, u32 RegOffset)
 *
 ******************************************************************************/
#define XAudGen_ReadReg(BaseAddress, RegOffset) \
    XAudGen_In32((BaseAddress) + ((u32)RegOffset))

/*****************************************************************************/
/**
 *
 * Write the given register.
 *
 * @param  BaseAddress is the base address of the Audio Generator core.
 * @param  RegOffset is the register offset of the register
 * @param  Data is the 32-bit value to write to the register.
 *
 * @return None.
 *
 * @note   C-style signature:
 *   void XAudGen_WriteReg(u32 BaseAddress, u32 RegOffset, u32 Data)
 *
 ******************************************************************************/
#define XAudGen_WriteReg(BaseAddress, RegOffset, Data)   \
    XAudGen_Out32((BaseAddress) + ((u32)RegOffset), (u32)(Data))

#define AUD_CTRL  0x000	// Audio Generator Control Register

// Audio Control Register
#define AUD_CTRL_REG_AUD_RST_MASK     0x1
#define AUD_CTRL_REG_AUD_RST_SHIFT    0
#define AUD_CTRL_REG_AUD_START_MASK   0x1
#define AUD_CTRL_REG_AUD_START_SHIFT  1
#define AUD_CTRL_REG_AUD_UPDCFG_MASK  0x1
#define AUD_CTRL_REG_AUD_UPDCFG_SHIFT 2
#define AUD_CTRL_REG_AUD_DROP_MASK    0x1
#define AUD_CTRL_REG_AUD_DROP_SHIFT   3

// Audio Configuration Register
#define AUD_CFG_REG_SAMPRATE_MASK  0xF // Sample rate
#define AUD_CFG_REG_NUMCH_MASK     0xF // Number of channels

// Channel Control Register
#define CH_CTRL_REG_PATTERN_MASK  0x3 // Audio pattern
#define CH_CTRL_REG_PERIOD_MASK   0xF // Reserved

// ACR Control registers
#define ACR_VER  0x000
#define ACR_CTRL 0x004
#define ACR_CTS  0x008
#define ACR_N    0x00C

// ACR Control Register
#define ACR_CTRL_ENAB_ACR_MASK  0x1
#define ACR_CTRL_ENAB_ACR_SHIFT 0
#define ACR_CTRL_SEL_ACR_MASK   0x1
#define ACR_CTRL_SEL_ACR_SHIFT  1
#define ACR_CTRL_AUD_RST_MASK   0x1
#define ACR_CTRL_AUD_RST_SHIFT  2

#define ACR_SEL_GEN 1  // Select generated ACR values
#endif /* AUDIOGEN_DRV_H_ */