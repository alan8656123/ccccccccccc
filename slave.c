#include "slave.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	
	printf("slave is on !\n");
	

	struct mail_t *get_message_from_master;
	receive_from_fd(0, get_message_from_master);
	
	FILE *test1;
	test1 = fopen( "test1","r" );

	struct mail_t *sent_message_for_master;
	send_to_fd(0, sent_message_for_master);
	exit(0);
}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	int ret_val = write(sysfs_fd,mail,sizeof(mail));
	if (ret_val == ERR_FULL) {
		printf("wirte failed!\n");
	} else {
	}
}

int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
	int ret_val = read(sysfs_fd,mail,sizeof(mail));
	if (ret_val == ERR_EMPTY) {
		printf("read failed!\n");
	} else {

	}
}
