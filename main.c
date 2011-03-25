#include <stdio.h>
#include <stdlib.h>
#include "lib10minmail.h"
int main()
{
    printf("Hello world!\n");
if (tenmail_init() == EXIT_SUCCESS)
{
        tenmail_connect();
        tenmail_cleanup();
}
//tenmail_cleanup();

    return 0;
}
