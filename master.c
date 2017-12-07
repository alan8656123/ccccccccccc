#include "master.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
void several_slave(int);				//fork and exce slave 
void master_process_code();				//the code of master process
char *QUERY_WORD=NULL,*DIRECTORY=NULL;	//2 mandatory arguments: QUERY_WORD and DIRECTORY
int main(int argc, char **argv)
{
	int NUM_SLAVE=1;						//1 optional argument: NUM_SLAVE(default value =1)	
	int i;
	if(argc<5){
		printf("erro arguments");
		exit(1);
	}
	//input arguments,The order of the three arguments may change 
	for(i=1;i<argc-1;i=i+2){
		if(!strcmp(argv[i],"-q")){
			QUERY_WORD=argv[i+1];
		}
		if(!strcmp(argv[i],"-d")){
			DIRECTORY=argv[i+1];
		}
		if(!strcmp(argv[i],"-s")){
			NUM_SLAVE=atoi(argv[i+1]);
		}
	}
	//QUERY_WORD and DIRECTORY must be mandatory arguments
	if(QUERY_WORD==NULL||DIRECTORY==NULL){
		printf("erro arguments");
		exit(1);
	}
	//fork and exec slaves
   several_slave(NUM_SLAVE);
}
void several_slave(int num_of_slave){
if (num_of_slave>0){
		pid_t pID = fork();
	   	if (pID == 0)                		// slave[num_of_slave]
	   	{
			//printf("slave%d process on ",num_of_slave);
			execl("./slave", "./slave"," ");
			
		}
		else if (pID < 0)           		// slave[num_of_slave] failed to fork
		{
			exit(1);
		}
		else{
			several_slave(num_of_slave-1);
			if(num_of_slave==1){
				master_process_code();		//master process
			}
		}
	}
}
void master_process_code(){					//the code of master process
	printf("master process on \n");

	//send mail
	struct mail_t *first_message;
	strcpy(first_message->data.query_word,QUERY_WORD);
	strcpy(first_message->file_path,DIRECTORY);
	send_to_fd(0,first_message);
	
	//receive from slave
	struct mail_t *get_message;
	receive_from_fd(0,get_message);

	//kill slave
	errno = 0;
   	int  killReturn = killpg( 0, -9);  // Kill child process group

   	if(killReturn == -1)
   	{
       if( errno == ESRCH)      // pid does not exist
       {
          printf("Group does not exist!");
       }
       else if( errno == EPERM) // No permission to send signal
       {
          printf("No permission to send signal!");
       }
       else
          printf("Signal sent. All Ok!");
   	}
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
