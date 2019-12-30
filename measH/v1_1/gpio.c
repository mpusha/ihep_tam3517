//https://gist.github.com/jadonk/2587524

// example:
//    $ nice -n-19 ./wiegand_test_epoll 5 6
//       here
//     green/data0 is gpio 5 (R4 at WB3.3)
//     white/data1 is gpio 6 (R3 at WB3.3)

// to compile:
//      arm-linux-gnueabi-gcc wiegand_test_epoll.c -o wiegand_test_epoll


#define GPIO_INP0 117  // B2 input
#define GPIO_INP1 118  // KC1 input
#define GPIO_OUT0 119  // meas pulse output

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>

int init_gpio(int gpio,int out_flag)
{
	// export gpio to userspace
	FILE * tmpf = fopen("/sys/class/gpio/export", "w");
	char path[42];
	fprintf(tmpf, "%d\n", gpio);
	fclose(tmpf);

	// set output direction
	sprintf(path, "/sys/class/gpio/gpio%d/direction", gpio);
	tmpf = fopen(path, "w");
    if(out_flag)
      fprintf(tmpf, "%s\n", "out");
    else
    fprintf(tmpf, "%s\n", "in");
	fclose(tmpf);
        
	sprintf(path, "/sys/class/gpio/gpio%d/edge", gpio);
	tmpf = fopen(path, "w");
//	fprintf(tmpf, "%s\n", "falling");
    fprintf(tmpf, "%s\n", "rising");
	fclose(tmpf);

	sprintf(path, "/sys/class/gpio/gpio%d/value", gpio);
	int fd = open(path, O_RDWR | O_NONBLOCK);
	if (fd <= 0) {
      fprintf(stderr, "open of gpio %d returned %d: %s\n", gpio, fd, strerror(errno));
	}
	return fd;
}

int epfd=0;
int fd_d0=0, fd_d1=0;
int fd_out0;
struct epoll_event ev_d0, ev_d1;
struct epoll_event events[10];

int init_gpios(void)
{
	int n;

    epfd = epoll_create(1);

    fd_d0   = init_gpio(GPIO_INP0,0);
    fd_d1   = init_gpio(GPIO_INP1,0);
    fd_out0 = init_gpio(GPIO_OUT0,1);

	if( !(fd_d0 > 0) || !(fd_d1 > 0)) {
		fprintf(stderr, "error opening gpio sysfs entries\n");
		return 1;
	}


    ev_d0.events = EPOLLET;
    ev_d1.events = EPOLLET;

    ev_d0.data.fd = fd_d0;
    ev_d1.data.fd = fd_d1;

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d0, &ev_d0);
    if (n != 0) {
      fprintf(stderr, "epoll_ctl returned %d: %s\n", n, strerror(errno));
      return 1;
	}

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d1, &ev_d1);
    if (n != 0) {
      printf("epoll_ctl returned %d: %s\n", n, strerror(errno));
      return 1;
	}
    return 0;
}

unsigned int polling_gpios(int timePolling)
{
    unsigned int value = 0 ,n,i;

    n = epoll_wait(epfd, events, 10, timePolling);

    for ( i = 0;  i < n; ++i) {
      if (events[i].data.fd == ev_d0.data.fd) value|=2;
      if (events[i].data.fd == ev_d1.data.fd) value|=1;
    }
    return value;
}

void measPulse_gpios(void)
{
  write(fd_out0,"1",2);
  write(fd_out0,"0",2);
}

void close_gpios(void)
{
  if(fd_d0) close(fd_d0);
  if(fd_d1) close(fd_d1);
  if(fd_out0) close(fd_out0);
 // if(epfd) fclose(epfd);
}
