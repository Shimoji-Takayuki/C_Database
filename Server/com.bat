gcc -lwsock32 dbserver.c tableselect.c tableinsert.c tableupdate.c common/common_func.c -lws2_32 -o server
