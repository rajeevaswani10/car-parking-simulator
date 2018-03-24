
#include <header/parking_buffer.h>
#include <assert.h>

int main()
{
    PB_RC rc;
	uLong slot = 0;
	parking_buffer_t *pb = pb_create((unsigned)15);

	pb_print(pb);

	rc = pb_park(pb,1456,&slot);
    assert(rc == PB_SUCCESS);

	rc = pb_park(pb,1451,&slot);
    assert(rc == PB_SUCCESS);

	rc = pb_park(pb,1454,&slot);
    assert(rc == PB_SUCCESS);

	pb_print(pb);

	rc = pb_unpark(pb,1451);
    assert(rc == PB_SUCCESS);

	pb_print(pb);

	rc = pb_unpark(pb,1456);
    assert(rc == PB_SUCCESS);

	rc = pb_unpark(pb,100);
	assert(rc == PB_NOT_FOUND);

	pb_destroy(pb);
}

