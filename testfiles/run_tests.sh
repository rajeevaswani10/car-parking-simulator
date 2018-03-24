
gcc test_list.c ../src/list.c -I../  -o test_list

gcc test_queue.c ../src/queue.c ../src/list.c -I../  -o test_queue

gcc test_parking_buffer.c ../src/parking_buffer.c -I../  -o test_parking_buffer

echo "******* TEST START ************"
echo

./test_list
./test_queue
./test_parking_buffer

echo
echo "****************** FINISH *****************"
echo
