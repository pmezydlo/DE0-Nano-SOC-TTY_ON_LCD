#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

#include "hps_0.h"
#include "TFT.h"
#include "IMG.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

#define Hsize 480
#define Vsize 272



/*
float abs(float n)
{
    if (n<0)
        n=n*(-1.0);

    return n;

}*/




int main()
{

    void *virtual_base;

    void *h2p_lw_addr;

    int fd;

    // map the address space for the LED registers into user space so we can interact with them.
    // we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

    if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 )
    {
        printf( "ERROR: could not open \"/dev/mem\"...\n" );
        return( 1 );
    }

    virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

    if( virtual_base == MAP_FAILED )
    {
        printf( "ERROR: mmap() failed...\n" );
        close( fd );
        return( 1 );
    }

    h2p_lw_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + ONCHIP_MEMORY2_0_BASE ) & ( unsigned long)( HW_REGS_MASK ) );


    TFT_bitmap(h2p_lw_addr,img,Hsize,Vsize);

    while(1)
    {


    } // while



    // clean up our memory mapping and exit

    if( munmap( virtual_base, HW_REGS_SPAN ) != 0 )
    {
        printf( "ERROR: munmap() failed...\n" );
        close( fd );
        return( 1 );
    }

    close( fd );

    return( 0 );
}
