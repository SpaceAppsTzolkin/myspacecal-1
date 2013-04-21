/*
 *	myspacecal_send.c
 *
 *	Implementation of myspaceecal_send functions
 */

#include <stdio.h>
#include <curl/curl.h>

#include "myspacecal_send.h"

size_t copy_data(void *buffer, size_t size, size_t nmemb, void *userp); 
 
bool myspacecal_init()				// libcurl global init, call once
{
    int result = curl_global_init(CURL_GLOBAL_ALL);
    
    return result == 0;
}

void myspacecal_cleanup()			// libcurl global cleanup, call once
{
    curl_global_cleanup();
}

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
    double	high_wavelength,		// in metres
    int		wavelength_flag			// Defined constants
)
{
    int result = 0;
    
    char *header = "satellite,target,ra,decl,revolution,start time,end time,low wavelength,high wavelength,wavelength flag\n"; 
    char buffer[MYSPACECAL_MAX_BUFFER];
    char inbuffer[MYSPACECAL_MAX_BUFFER];

    size_t length = snprintf(buffer, MYSPACECAL_MAX_BUFFER, 
        "%s%s,%s,%s,%d,%s,%s,%e,%e,%d\n",
        header,target_name, right_ascension, declination, revolution, 
        start_time, end_time, low_wavelength, high_wavelength, wavelength_flag);
        
    CURL *handle = curl_easy_init();
    
    if (handle != NULL) 
    {
        char url_buffer[MYSPACECAL_MAX_BUFFER];
        snprintf(url_buffer, MYSPACECAL_MAX_BUFFER, "http://%s/?satellite=%ld", 
            MYSPACECAL_URL, satellite_id);
        curl_easy_setopt(handle, CURLOPT_URL, url_buffer);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, buffer);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, copy_data);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, inbuffer); 

        CURLcode code = curl_easy_perform(handle); 
        
        if (code == 0) 
        {
        }
        else
        {
            result = -code;
        }
         
        curl_easy_cleanup(handle);
    }
    else
    {
        result = MYSPACECAL_BAD_EASY;
    }        

    return result;
}

size_t copy_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
}

