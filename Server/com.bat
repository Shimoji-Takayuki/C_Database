gcc -lwsock32 dbserver.c tableselect.c tableinsert.c tableupdate.c tableDelete.c common/common_func.c -lws2_32 -o server
