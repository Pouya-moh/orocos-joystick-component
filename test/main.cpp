#include <iostream>
#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd = open ("/dev/input/js0", O_RDONLY);
	int ax, b;
    ioctl( fd, JSIOCGAXES, &ax );
    ioctl( fd, JSIOCGBUTTONS, &b );    
	std::cout<<ax<<"\t"<<b<<std::endl;
	while(true) {
		usleep(1000);
		struct js_event e;
		read (fd, &e, sizeof(e));

//		struct js_event {
//			__u32 time;     /* event timestamp in milliseconds */
//			__s16 value;    /* value */
//			__u8 type;      /* event type */
//			__u8 number;    /* axis/button number */
//		};
		if (e.type == JS_EVENT_AXIS)
			std::cout<<int(e.number)<<std::endl;	
			// std::cout<<e.value<<std::endl;	
		// else if (e.type == JS_EVENT_BUTTON)
		// 	std::cout<<int(e.number)<<std::endl;
	}
	// close(fd);
	return 0;
}