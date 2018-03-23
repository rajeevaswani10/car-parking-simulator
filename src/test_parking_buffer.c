
#include "parking_buffer.h"

int main()
{
    PB_RC rc;
	parking_buffer_t *pb = pb_create((unsigned)15, 5, 2);

	pb_print(pb);

	sleep(1);
	pb_destroy(pb);
}

