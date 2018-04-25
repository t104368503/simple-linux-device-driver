#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/ioctl.h>

#define IOCTL_EMPTY _IO(0xCE, 0)
#define IOCTL_SYNC  _IO(0xCE, 1)
#define IOCTL_NAME  _IOW(0xCE, 2, char *)


int main(void)
{
    int fd;

                    
    fd = open("/dev/ldd_hello_world", O_RDWR);
                                
    ioctl(fd, IOCTL_EMPTY,12);
    
    ioctl(fd, IOCTL_SYNC,4567);
    
    close(fd);
}
