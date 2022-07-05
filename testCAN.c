#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
 
#include <linux/can.h>
#include <linux/can/raw.h>
 
int
main(void)
{
        int s,i;
        int nbytes;
        struct sockaddr_can addr;
        struct can_frame frame;
        struct ifreq ifr;
 
        char *ifname = "can0";
 
        if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
                perror("Error while opening socket");
                return -1;
        }
 
        strcpy(ifr.ifr_name, ifname);
        ioctl(s, SIOCGIFINDEX, &ifr);
 
        addr.can_family  = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex; 
 
        printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
 
        if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("Error in socket bind");
                return -2;
        }
	
	frame.can_id  = 0x0F6;
        frame.can_dlc = 8;
        frame.data[0] = 0x10;
        frame.data[1] = 0x78;
        frame.data[2] = 0x32;
        frame.data[3] = 0x50;
        frame.data[4] = 0x00;
        frame.data[5] = 0x00;
        frame.data[6] = 0x00;
        frame.data[7] = 0x00;
        
        nbytes = write(s, &frame, sizeof(struct can_frame));
	printf("Wrote %d bytes\n", nbytes);
        
        frame.can_id  = 0x0B6;
        frame.can_dlc = 8;
        frame.data[0] = 0x30;
        frame.data[1] = 0x48;
        frame.data[2] = 0x25;
        frame.data[3] = 0x37;
        frame.data[4] = 0x00;
        frame.data[5] = 0x00;
        frame.data[6] = 0x00;
        frame.data[7] = 0x00;
 
        nbytes = write(s, &frame, sizeof(struct can_frame));
	printf("Wrote %d bytes\n", nbytes);
 
	/* Read a message back from the CAN bus */
        nbytes = read( s, &frame, sizeof(struct can_frame));
	printf("Identifiant: %x [%d] ", frame.can_id, frame.can_dlc);
	for(i=0; i<frame.can_dlc; i++)
	  printf("%x ",frame.data[i]);
	
	printf("\n");
        return 0;
}
