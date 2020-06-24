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
#define PAGE_SIZE 4096
#define MAP_SIZE 4096
#define BUF_SIZE 512
size_t get_filesize(const char* filename);//get the size of the input file


int main (int argc, char* argv[])
{
	
	char buf[BUF_SIZE];
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size, offset, tmp, total = 0;
	char file_name[50], method[20];
	char *kernel_address = NULL, *file_address = NULL;
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed
	
	if( (dev_fd = open("/dev/master_device", O_RDWR)) < 0)
	{
		perror("failed to open /dev/master_device\n");
		return 1;
	}
	gettimeofday(&start ,NULL);
	
	kernel_address = mmap(NULL, MAP_SIZE, PROT_WRITE, MAP_SHARED, dev_fd, 0); 
	ioctl(dev_fd, 9020 , kernel_address);
	for(int i = 0; i < atoi(argv[1]) ; i++){
		strcpy(file_name , argv[i + 2]);
		if( (file_fd = open (file_name, O_RDWR)) < 0 )
		{
			perror("failed to open input file\n");
			return 1;
		}
	
		if( (file_size = get_filesize(file_name)) < 0)
		{
			perror("failed to get filesize\n");
			return 1;
		}
		total += file_size;
		if(ioctl(dev_fd, 0x12345677) == -1) //0x12345677 : create socket and accept the connection from the slave
		{
			perror("ioclt server create socket error\n");
			return 1;
		}
		strcpy(method , argv[argc - 1]);
		long long remain = (long long)file_size , offset = 0;
		switch(method[0])
		{
			case 'f': //fcntl : read()/write()
				do
				{
					ret = read(file_fd, buf, sizeof(buf)); // read from the input file
					int err = write(dev_fd, buf, ret);//write to the the device			
					if(err == -1){
						fprintf(stderr,"error!");
					}
				}while(ret > 0);
				break;
			case 'm':
				while( remain > 0){			
					size_t len = remain;
					if(remain > MAP_SIZE){
						len = MAP_SIZE;
					}
					file_address = mmap(NULL, MAP_SIZE, PROT_READ, MAP_SHARED, file_fd, offset);
					offset += len;
					remain -= len;					
					memcpy(kernel_address, file_address , len);
			//		printf("memcpy success!\n");
					ioctl(dev_fd, 0x12345678, len);
				}
				//munmap(kernel_address, MAP_SIZE);
				//munmap(file_address, MAP_SIZE);
				//printf("mmap success\n");
				break;
		}

		if(ioctl(dev_fd, 0x12345679) == -1) // end sending data, close the connection
		{
			perror("ioclt server exits error\n");
			return 1;
		}
	
	}
	gettimeofday(&end, NULL);

	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Transmission time: %lf ms, File size: %d bytes\n", trans_time, total);

	close(file_fd);
	close(dev_fd);
	return 0;
}

size_t get_filesize(const char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}
