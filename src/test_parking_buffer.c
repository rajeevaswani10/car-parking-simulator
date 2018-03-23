
#include "parking_buffer.h"

int main()
{
    PB_RC rc;
	parking_buffer_t *pb = pb_create((unsigned)2, 1, 0);

	pb_print(pb);

	pb_park(pb,1456);
	sleep(1);
	pb_park(pb,1451);
	sleep(1);
	pb_park(pb,1454);
	sleep(1);
	pb_print(pb);
	sleep(1);
	pb_print(pb);

	pb_destroy(pb);
}

