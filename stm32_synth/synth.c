/**
  ******************************************************************************
  * @file    synth.c
  * @author  Cullen Sharp
  * @brief   This file provides the Synth driver core implementation.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025
  * All rights reserved.</center></h2>
  *
  * This software component is licensed under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "synth.h"
#include <string.h>  /* For memset */

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup Synth
  * @{
  */

/* Private variables ---------------------------------------------------------*/
static SYNTH_IO_t SynthIO;
static SYNTH_Ctx_t SynthCtx;

/* Private function prototypes -----------------------------------------------*/
static int32_t SYNTH_DefaultTransmit(uint8_t *pData, uint32_t size);

/**
  * @brief  Register the low-level hardware interface
  * @param  pObj Pointer to Synth object
  * @param  pIO  Pointer to I/O structure
  * @retval Synth status
  */
int32_t SYNTH_RegisterBusIO(void *pObj, SYNTH_IO_t *pIO)
{
  if (pIO == NULL)
  {
    return SYNTH_STATUS_ERROR;
  }

  SynthIO = *pIO;
  return SYNTH_STATUS_OK;
}

/**
  * @brief  Initialize the Synth
  * @param  pObj         Pointer to Synth object
  * @param  sample_rate  Desired sample rate (Hz)
  * @param  channels     Number of audio channels
  * @retval Synth status
  */
int32_t SYNTH_Init(void *pObj, uint32_t sample_rate, uint8_t channels)
{
  (void)pObj;

  if (SynthIO.Init == NULL)
  {
    return SYNTH_STATUS_ERROR;
  }

  /* Initialize low-level interface */
  if (SynthIO.Init() != 0)
  {
    return SYNTH_STATUS_ERROR;
  }

  /* Default context */
  SynthCtx.SampleRate = sample_rate;
  SynthCtx.Channels   = channels;
  SynthCtx.Volume     = SYNTH_DEFAULT_VOLUME;
  SynthCtx.Mute       = 0;
  SynthCtx.Initialized = 1;

  if (SynthIO.SetSampleRate)
  {
    SynthIO.SetSampleRate(sample_rate);
  }

  return SYNTH_STATUS_OK;
}

/**
  * @brief  Deinitialize the Synth
  * @param  pObj Pointer to Synth object
  * @retval Synth status
  */
int32_t SYNTH_DeInit(void *pObj)
{
  (void)pObj;

  if (SynthIO.DeInit)
  {
    SynthIO.DeInit();
  }

  memset(&SynthCtx, 0, sizeof(SYNTH_Ctx_t));

  return SYNTH_STATUS_OK;
}

/**
  * @brief  Play audio from a provided buffer
  * @param  pObj   Pointer to Synth object
  * @param  buffer Pointer to PCM data buffer
  * @param  length Number of samples in buffer
  * @retval Synth status
  */
int32_t SYNTH_PlayBuffer(void *pObj, const int16_t *buffer, uint32_t length)
{
  (void)pObj;

  if (SynthCtx.Initialized == 0)
  {
    return SYNTH_STATUS_ERROR;
  }

  if (SynthIO.Transmit == NULL)
  {
    return SYNTH_STATUS_ERROR;
  }

  /* Convert samples to bytes for transmit */
  uint32_t size = length * sizeof(int16_t);
  return SynthIO.Transmit((uint8_t *)buffer, size);
}

/**
  * @brief  Stop current audio playback
  * @param  pObj Pointer to Synth object
  * @retval Synth status
  */
int32_t SYNTH_Stop(void *pObj)
{
  (void)pObj;
  /* Hardware or software stop logic goes here */
  return SYNTH_STATUS_OK;
}

/**
  * @brief  Set output sample rate
  * @param  pObj         Pointer to Synth object
  * @param  sample_rate  Desired sample rate (Hz)
  * @retval Synth status
  */
int32_t SYNTH_SetSampleRate(void *pObj, uint32_t sample_rate)
{
  (void)pObj;
  SynthCtx.SampleRate = sample_rate;

  if (SynthIO.SetSampleRate)
  {
    SynthIO.SetSampleRate(sample_rate);
  }

  return SYNTH_STATUS_OK;
}

/**
  * @brief  Get current sample rate
  * @param  pObj         Pointer to Synth object
  * @param  sample_rate  Pointer to return variable
  * @retval Synth status
  */
int32_t SYNTH_GetSampleRate(void *pObj, uint32_t *sample_rate)
{
  (void)pObj;
  if (sample_rate == NULL)
  {
    return SYNTH_STATUS_ERROR;
  }

  *sample_rate = SynthCtx.SampleRate;
  return SYNTH_STATUS_OK;
}

/**
  * @brief  Set the output volume
  * @param  pObj    Pointer to Synth object
  * @param  volume  Volume (0-100)
  * @retval Synth status
  */
int32_t SYNTH_SetVolume(void *pObj, uint8_t volume)
{
  (void)pObj;
  if (volume > 100)
  {
    volume = 100;
  }

  SynthCtx.Volume = volume;
  /* Optional: Apply volume in hardware here */

  return SYNTH_STATUS_OK;
}

/**
  * @brief  Get current volume
  * @param  pObj    Pointer to Synth object
  * @param  volume  Pointer to return variable
  * @retval Synth status
  */
int32_t SYNTH_GetVolume(void *pObj, uint8_t *volume)
{
  (void)pObj;

  if (volume == NULL)
  {
    return SYNTH_STATUS_ERROR;
  }

  *volume = SynthCtx.Volume;
  return SYNTH_STATUS_OK;
}

/**
  * @brief  Enable or disable mute
  * @param  pObj    Pointer to Synth object
  * @param  enable  1 to mute, 0 to unmute
  * @retval Synth status
  */
int32_t SYNTH_Mute(void *pObj, uint8_t enable)
{
  (void)pObj;
  SynthCtx.Mute = enable;

  if (SynthIO.Mute)
  {
    SynthIO.Mute(enable);
  }

  return SYNTH_STATUS_OK;
}

/**
  * @brief  Dummy transmit function (used if no bus is registered)
  * @param  pData Pointer to data
  * @param  size  Data length
  * @retval Synth status
  */
static int32_t SYNTH_DefaultTransmit(uint8_t *pData, uint32_t size)
{
  (void)pData;
  (void)size;
  return SYNTH_STATUS_ERROR;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Embedded Systems Team *****END OF FILE****/
