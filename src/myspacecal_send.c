/*
 *	myspacecal_send.c
 *
 *	Implementation of myspaceecal_send functions
 */

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "myspacecal_send.h"

size_t copy_data(void *buffer, size_t size, size_t nmemb, void *userp); 

int send_update					// Send confirm or cancel
(
    char	*action,			// PUT = confirm, DELETE = cancel
    long	satellite_id,			// Satellite id
    long	observation_id			// Observation id
);
 
bool myspacecal_init()				// libcurl global init, call once
{
    int result = curl_global_init(CURL_GLOBAL_ALL);
    
    return result == 0;
}

void myspacecal_cleanup()			// libcurl global cleanup, call once
{
    curl_global_cleanup();
}

long myspacecal_create 				// Returns id > 0, or error < 0	
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
    
    char *header = "target,ra,decl,revolution,start time,end time,low wavelength,high wavelength,wavelength flag\n"; 
    char buffer[MYSPACECAL_MAX_BUFFER];
    char inbuffer[MYSPACECAL_MAX_BUFFER];
    inbuffer[0] = '\0';

    size_t length = snprintf(buffer, MYSPACECAL_MAX_BUFFER, 
        "%s%s,%s,%s,%d,%s,%s,%e,%e,%d\n",
        header,target_name, right_ascension, declination, revolution, 
        start_time, end_time, low_wavelength, high_wavelength, wavelength_flag);
        
    CURL *handle = curl_easy_init();
    
    if (handle != NULL) 
    {
        char url_buffer[MYSPACECAL_MAX_BUFFER];
        snprintf(url_buffer, MYSPACECAL_MAX_BUFFER, "http://%s?satellite=%ld", 
            MYSPACECAL_URL, satellite_id);
        curl_easy_setopt(handle, CURLOPT_URL, url_buffer);
            
        struct curl_slist *slist=NULL; 
        slist = curl_slist_append(slist, "Content-Type:text/plain");
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);
         
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, buffer);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, copy_data);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, inbuffer); 

        CURLcode code = curl_easy_perform(handle); 
        
        if (code == 0) 
        {
            char *rest;
            int data = strtol(inbuffer, &rest, 10);
            result = data;
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
    size_t	result;
    
    char *output = (char*)userp;
    size_t done = strlen(output);
    size_t todo = size * nmemb;
    
    if (done + todo < MYSPACECAL_MAX_BUFFER)
    {
        memcpy(output + done, buffer, todo);
        output[done + todo] = '\0';
        result = todo;
    }
    else
    {
        result = 0;
    }

    return result;
}

int myspacecal_confirm				// returns error code
(
    long	satellite_id,			// Satellite id
    long	observation_id			// Observation id
)
{
    return send_update("PUT", satellite_id, observation_id);
}
        
        
int myspacecal_cancel				// returns error code
(
    long	satellite_id,			// Satellite id
    long	observation_id			// Observation id
)
{
    return send_update("DELETE", satellite_id, observation_id);
}

int send_update					// Send confirm or cancel
(
    char	*action,			// PUT = confirm, DELETE = cancel
    long	satellite_id,			// Satellite id
    long	observation_id			// Observation id
)
{
    int result = 0;
            
    CURL *handle = curl_easy_init();
    
    if (handle != NULL) 
    {
        char url_buffer[MYSPACECAL_MAX_BUFFER];
        snprintf(url_buffer, MYSPACECAL_MAX_BUFFER, "http://%s?satellite=%ld&observation=%ld", 
            MYSPACECAL_UPDATE_URL, satellite_id, observation_id);
        curl_easy_setopt(handle, CURLOPT_URL, url_buffer);
            
        curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, action);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, copy_data);

        CURLcode code = curl_easy_perform(handle); 
        
        result = -code;
         
        curl_easy_cleanup(handle);
    }
    else
    {
        result = MYSPACECAL_BAD_EASY;
    }        

    return result;
}


