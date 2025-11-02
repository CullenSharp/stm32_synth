/**
  ******************************************************************************
  * @file    synth.h
  * @author  Cullen Sharp
  * @brief   This file contains all the function prototypes for the Synth driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SYNTH_H
#define SYNTH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup Synth
  * @{
  */

/** @defgroup SYNTH_Exported_Constants Synth Exported Constants
  * @{
  */

#define SYNTH_STATUS_OK             0x00U
#define SYNTH_STATUS_ERROR          0x01U
#define SYNTH_STATUS_BUSY           0x02U
#define SYNTH_STATUS_TIMEOUT        0x03U

/* Default audio configuration values */
#define SYNTH_DEFAULT_SAMPLE_RATE   44100U   /*!< Default sample rate in Hz */
#define SYNTH_DEFAULT_CHANNELS      2U       /*!< Stereo output by default  */
#define SYNTH_DEFAULT_VOLUME        75U      /*!< Volume (0-100 scale)      */

/**
  * @}
  */

/** @defgroup SYNTH_Exported_Types Synth Exported Types
  * @{
  */

/**
  * @brief  Synth driver function structure
  */
typedef struct
{
  /* Initialization and configuration */
  int32_t (*Init)              (void*, uint32_t sample_rate, uint8_t channels);
  int32_t (*DeInit)            (void*);
  int32_t (*Reset)             (void*);
  int32_t (*SetSampleRate)     (void*, uint32_t sample_rate);
  int32_t (*GetSampleRate)     (void*, uint32_t *sample_rate);

  /* Audio playback */
  int32_t (*PlayBuffer)        (void*, const int16_t *buffer, uint32_t length);
  int32_t (*Stop)              (void*);
  int32_t (*Pause)             (void*);
  int32_t (*Resume)            (void*);

  /* Sound control */
  int32_t (*SetVolume)         (void*, uint8_t volume);
  int32_t (*GetVolume)         (void*, uint8_t *volume);
  int32_t (*Mute)              (void*, uint8_t enable);

  /* Voice and waveform control (optional extension) */
  int32_t (*SetWaveform)       (void*, uint8_t waveform_id);
  int32_t (*SetFrequency)      (void*, float frequency);
  int32_t (*NoteOn)            (void*, uint8_t note, uint8_t velocity);
  int32_t (*NoteOff)           (void*, uint8_t note);

} SYNTH_Drv_t;

/**
  * @brief  Synth I/O function structure
  *         (hardware abstraction for audio interface)
  */
typedef struct
{
  int32_t (*Init)              (void);
  int32_t (*DeInit)            (void);
  int32_t (*Transmit)          (uint8_t *pData, uint32_t size);
  int32_t (*SetSampleRate)     (uint32_t sample_rate);
  int32_t (*GetSampleRate)     (uint32_t *sample_rate);
  int32_t (*Mute)              (uint8_t enable);
} SYNTH_IO_t;

/**
  * @brief  Synth context structure
  *         Used to keep runtime configuration
  */
typedef struct
{
  uint32_t SampleRate;
  uint8_t  Channels;
  uint8_t  Volume;
  uint8_t  Mute;
  uint8_t  Initialized;
} SYNTH_Ctx_t;

/**
  * @}
  */

/** @defgroup SYNTH_Exported_Functions Synth Exported Functions
  * @{
  */

int32_t SYNTH_RegisterBusIO(void *pObj, SYNTH_IO_t *pIO);
int32_t SYNTH_Init(void *pObj, uint32_t sample_rate, uint8_t channels);
int32_t SYNTH_DeInit(void *pObj);
int32_t SYNTH_PlayBuffer(void *pObj, const int16_t *buffer, uint32_t length);
int32_t SYNTH_Stop(void *pObj);
int32_t SYNTH_SetSampleRate(void *pObj, uint32_t sample_rate);
int32_t SYNTH_GetSampleRate(void *pObj, uint32_t *sample_rate);
int32_t SYNTH_SetVolume(void *pObj, uint8_t volume);
int32_t SYNTH_GetVolume(void *pObj, uint8_t *volume);
int32_t SYNTH_Mute(void *pObj, uint8_t enable);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SYNTH_H */

/************************ (C) COPYRIGHT Embedded Systems Team *****END OF FILE****/
