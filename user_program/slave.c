#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>

#define MAP_SIZE 4096*32
#define PAGE_SIZE 4096
#define BUF_SIZE 512

void strdelete(char *str , int size){
	for(int i = 0; i < strlen(str) - size ; i ++){
		str[i] = str[i + size];
	}
	for(int i = strlen(str) - size ; i < strlen(str) ; i++){
		str[i] = '\0';
	}
}

int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret = 1, file_size = 0, data_size = -1 , offset = 0;
	char file_name[50]; int file_n = atoi(argv[1]);
	char method[20];
	char ip[20];
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed
	char *kernel_address, *file_address;

	
	strcpy(ip, argv[file_n + 3]);
	strcpy(method, argv[file_n + 2]);
	
	if( (dev_fd = open("/dev/slave_device", O_RDWR)) < 0)//should be O_RDWR for PROT_WRITE when mmap()
	{
		perror("failed to open /dev/slave_device\n");
		return 1;
	}
	gettimeofday(&start ,NULL);
	kernel_address = mmap(NULL, MAP_SIZE, PROT_WRITE, MAP_SHARED, dev_fd, 0);
	ioctl(dev_fd, 9020 , kernel_address);
for (int i = 0; i < atoi(argv[1]); ++i){
	strcpy(file_name, argv[i + 2]);

	if( (file_fd = open (file_name, O_RDWR | O_CREAT | O_TRUNC)) < 0)
	{
		perror("failed to open input file\n");
		return 1;
	}
	
	if(ioctl(dev_fd, 0x12345677, ip) == -1)	//0x12345677 : connect to master in the device
	{
		perror("ioclt create slave socket error\n");
		return 1;
	}
	
    write(1, "ioctl success\n", 14);
	long long count = 0;
	switch(method[0]){
		case 'f'://fcntl : read()/write()
			do
			{
				//printf("round = %lld\n" , ++count);
				ret = read(dev_fd, buf, sizeof(buf)); // read from the the device
				//printf("get ret = %d\n", ret);
				write(file_fd, buf, ret); //write to the input file
				file_size += ret;

			}while(ret > 0);
			break;
		case 'm':
			offset = 0;
			char buffer[PAGE_SIZE * 3];
			int str_size = 0;
			do{			
				ret = ioctl(dev_fd , 0x12345678);
				//printf("get ret = %d\n", ret);
				strncat(buffer , kernel_address , ret);
				str_size += ret;
				//ioctl(dev_fd, 9020 , file_address);
				//printf("map success , add = %llX\n , offset = %d\n" , file_address, offset);	
				if(str_size >= PAGE_SIZE){
					ftruncate(file_fd , offset + PAGE_SIZE);
					file_address = mmap(NULL, ret, PROT_WRITE, MAP_SHARED, file_fd, offset);
					memcpy(file_address, buffer , PAGE_SIZE);
					//printf("copy successs\n" );
					strdelete(buffer , PAGE_SIZE);
					offset += PAGE_SIZE;
					str_size -= PAGE_SIZE;
				}
				if(ret == 0){
					ftruncate(file_fd , offset + str_size);
					file_address = mmap(NULL, str_size, PROT_WRITE, MAP_SHARED, file_fd, offset);
					memcpy(file_address, buffer , str_size);
					offset += str_size;
					str_size = 0;
				}
			}while( ret > 0);
			file_size = offset;
			//printf("mmap success\n");
			break;
	}

	if(ioctl(dev_fd, 0x12345679) == -1)// end receiving data, close the connection
	{
		perror("ioclt client exits error\n");
		return 1;
	}
}
	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Transmission time: %lf ms, File size: %d bytes\n", trans_time, file_size / 8);


	close(file_fd);
	close(dev_fd);
	return 0;
}


