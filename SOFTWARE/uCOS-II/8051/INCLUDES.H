/*****************************************************************************
|*
|*  Copyright (c) 2001 TASKING, Inc.
|*
|*  Description:
|*
|*	Header file for uC/OS
|*
 ****************************************************************************/

#ifndef _INCLUDES_H_
#define _INCLUDES_H_

/*
***************************************************************
*                       INCLUDES.H
***************************************************************
*/

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>

#if (_MODEL == 'r')
#  include    "os_cpu.h"
#else
#  error Only reentrant memory model is supported
#endif

/* === Include Application specific constants and definitions === */
#include    "os_cfg.h"

/* === Include Target-Independent constants and definitions === */
#include    "UCOS_II.h"

#endif 	/* _INCLUDES_H_ */


