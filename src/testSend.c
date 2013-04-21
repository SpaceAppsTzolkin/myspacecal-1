/*
 *  testSend.c
 *
 *  Test myspacecal_send create, confirm and cancel functions
 */

#include <stdio.h>

#include "myspacecal_send.h"
 
int main 
(
    int		argc,			// Arg count
    char	*argv[]			// Arg list
)
{
    int code = 0;
    
    if (myspacecal_init())
    {
    
        int result = myspacecal_create(1, "target", "10:30:20.1", "120:40:20.5", 200, 
            "2013-04-20 00:23:00",  "2013-04-21 00:23:00", 1.0, 10.0, MYSPACECAL_PRECISE);
 
        if (result >= 0)
        {
            result = myspacecal_confirm(1, result);
            
            if (result < 0)
            {
                code = 1;
                printf("Confirm error code =%d\n", result);
            }
        }
        else 
        {
            code = 1;
            printf("Error code =%d\n", result);
        }

        result = myspacecal_create(1, "target2", "11:35:20.1", "125:45:20.5", 200, 
            "2013-04-21 00:23:30",  "2013-04-22 00:23:30", 1.0, 10.0, MYSPACECAL_PRECISE);
 
        if (result >= 0)
        {
            result = myspacecal_cancel(1, result);
            
            if (result < 0)
            {
                code = 1;
                printf("Cancel error code =%d\n", result);
            }
        }
        else 
        {
            code = 1;
            printf("Error code =%d\n", result);
        }

    
    
        myspacecal_cleanup();
    }
    else
    {
        code = 2;
    }
    
    return code;
}
 
