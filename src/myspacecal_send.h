/*
 *	myspacecal_send.h
 *
 *	Include file for myspacecal_send functions
 */

#define MYSPACECAL_DEV
//#define MYSPACECAL_TEST
//#define MYSPACECAL_PRODUCTION

#include <stdbool.h>
 
#define MYSPACECAL_PRECISE	1
#define MYSPACECAL_DEFFAULT	2

#define MYSPACECAL_MAX_BUFFER	1000

#define MYSPACECAL_BAD_EASY	-1

#if defined MYSPACECAL_PRODUCTION
#define MYSPACECAL_URL "169.24.100.1"
#elif defined MYSPACECAL_TEST
#define MYSPACECAL_URL "169.24.100.1"
#elif defined MYSPACECAL_DEV
#define MYSPACECAL_URL "169.24.100.1"
#endif

bool myspacecal_init();				// Curl global init, call once

void myspacecal_cleanup();			// Curl global cleanup, call once

int myspacecal_create 				// Returns id > 0, or error < 0	
(
    long	satellite_id,			// Satellite id
    char	*target_name,			// Star catalogue name
    char	*right_ascension,		// in hours:minutes:seconds.tenths
    char	*declination,			// in degrees:minutes:seconds.tenths
    int		revolution,			// orbit count
    char	*start_time,			// in yyyy-mm-dd hh:mm:ss
    char	*end_time,			// in yyyy-mm-dd hh:mm:ss
    double	low_wavelength,			// in metres
    double	high_wavelengh,			// in metres
    int		wavelength_flag			// Defined constants
);
