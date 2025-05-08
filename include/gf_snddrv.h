/**
 * @file gf_snddrv.h
 * @~english
 * @brief Sound driver headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_SNDDRV_H__
#define __GF_SNDDRV_H__

#if defined(SND_WINDOWS)
#include <windows.h>
#include <dsound.h>
#elif defined(SND_MINIAUDIO)
#include <miniaudio.h>
#endif

#endif
