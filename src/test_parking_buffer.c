
#include "parking_buffer.h"

int main()
{
    PB_RETCODE rc;
	unsigned int slot;
	parking_buffer_t *pb = pb_create((unsigned)15);
    pb_print(pb);

    rc = pb_park(pb, 8102, &slot); pb_print(pb);
    rc = pb_park(pb, 100, &slot); pb_print(pb);
    rc = pb_park(pb, 2001, &slot); pb_print(pb);
    pb_unpark(pb, 2001, slot); pb_print(pb);
}

