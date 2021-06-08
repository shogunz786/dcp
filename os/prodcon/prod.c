/**
 * Description:
 * The producer program reads an input file and loads it on a circular buffer
 * for consumption of the consumer. The size of the circular buffer and filename
 * should be provided at command line.
 * Usage: prod <circular buffer size> <input file name>
 * Future Improvements:
 * 1. Make Producer less sentive to OS interrupts by handling EINTR.
      By using an inline like below.
 *      do{ 
            syscall();
            if(errno != EINTR)
              break;
         }while(1);
   2. signal handling to provide better user experience.
 * Author: shobhan C
 * Date: 6/26/2018
 *
 **/
#include "shared.h"

int producer(char *filename, int num_buff)
{

	sem_t *buf_cnt_sem_p, *buf_cnt_sem_c, *one_buf_sem;
	ssize_t max_line_size = BUFFER_SIZE - sizeof(unsigned long);
	ssize_t read = 0;
	size_t len = 0;
	int shm_sz = 0, p_index =0, shm_fd_p, pendinread = 0, line_sz = 0;
	int full = 0, buffer_index = 0, rc = SUCCESS;
	char *line = NULL, *pending = NULL, *ptr = NULL, *shm_area_p = NULL;
        FILE *fd = NULL;
	unsigned long line_len = 0;
#ifdef DEBUG
        int buff_cnt = 0, p_val = 0;
#endif

	shm_sz = (sizeof(char)*BUFFER_SIZE*num_buff);

	if((buf_cnt_sem_p = sem_open(SEM_BUF_CNT_NAME_P, O_CREAT|O_EXCL,
					0644, num_buff)) == SEM_FAILED){
		perror("Failed to create producer semaphore");
		goto p_sem_fail;
	}

	if((buf_cnt_sem_c = sem_open(SEM_BUF_CNT_NAME_C, O_CREAT|O_EXCL,
					0644, 0)) == SEM_FAILED){
		perror("Failed to create consumer semaphore");
                rc = FAILED;
		goto c_sem_fail;
	}

	if((one_buf_sem = sem_open(SEM_ONE_BUF, O_CREAT,
					0644, 1)) == SEM_FAILED){
		perror("Failed to create buffer index mutex");
                rc = FAILED;
		goto mutex_fail;
	}


	if((shm_fd_p = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644)) == -1){
		perror("Failed to create shm");
                rc = FAILED;
		goto shm_fail;
        }
	if(ftruncate(shm_fd_p, shm_sz))/* configure size of shared memory*/
        {
		perror("Failed to truncate shm");
                rc = FAILED;
		goto shm_map_fail;
        }

        shm_area_p = mmap(0, shm_sz,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,shm_fd_p, 0);
	if(shm_area_p == MAP_FAILED)
        {
		perror("Failed to mmap shm");
                rc = FAILED;
		goto shm_map_fail;
        }
#ifdef DEBUG
	printf("Hello from producer %d\n",p_val);
#endif
	fd = fopen(filename, "r");
	if(fd)
	{
		while(1) {
                        //ready to process new buffer slot
			if(line_sz == 0){
                                //take producer sem
				sem_wait(buf_cnt_sem_p);

                                //get a slot in the buffer to fill
				sem_wait(one_buf_sem);//take buffer slot index mutex
                                //reset buffer index to go circular
				if(p_index>=num_buff)
					p_index = 0;
				buffer_index = p_index;
				p_index++;
				sem_post(one_buf_sem);//release buffer slot index mutex
#ifdef DEBUG
                                sem_getvalue(buf_cnt_sem_p, &p_val);
				printf("%d[%d]) Buffer %d\n",buff_cnt++,p_val,buffer_index);
#endif
				full = 0;//start loading buffer
                                //get location of the buffer slot
				ptr = (char *)(shm_area_p + (BUFFER_SIZE * (buffer_index)));
#ifdef DEBUG
				printf("get lock\n");
#endif
			}
                        //leftover entry that could not be added to previous slot
			if(pending){
				line = pending;
				read = pendinread;//line length
				pending = NULL;//reset pending
				pendinread = 0;
			}else{
				read = getline(&line, &len, fd);
                                //reached end of file, prepare to exit producer
				if(read == -1){
					line_sz = 0;
#ifdef DEBUG
					printf("3 unlock\n");
#endif
					sem_post(buf_cnt_sem_c);
					break;
				}
				/* ignore empty and
				   lines greater than max allowed size */
				if((read == 1 && line[0] == '\n') ||
						(read-1)>max_line_size)
					continue;
			}


			if((line_sz+read+sizeof(unsigned long)-1)<=BUFFER_SIZE)
			{
				line_len = read -1;//do not copy end of line
                                //copy entry size
				strncpy((char *)(ptr + line_sz),
                                        (char *)(&line_len),
                                        sizeof(unsigned long));
                                //update line size with header size
				line_sz +=sizeof(unsigned long);
                                //copy line from file as entry 
				strncpy((char *)(ptr + line_sz),
                                        line,
                                        line_len);
                                //update line size with line size minus end of line char
				line_sz +=line_len;
				printf("%ld-[%s]\n", line_len, line);//, line);
#ifdef DEBUG
				printf("line_sz %d - buffer_index %d",
						line_sz, buffer_index);
#endif
                                //buffer slot is full, start loading next slot
				if(line_sz == BUFFER_SIZE)
					full = 1;
			}else{  //buffer slot does not have room to fit current entry
                                //save entry that could not fit the current buffer slot
				pending = line;
				pendinread = read;
                                //declare buffer slot full, start loading next buffer
				full = 1;
			}
			if(full){
				full = 0;
				line_sz = 0;
#ifdef DEBUG
                                printf("\n");
				printf("2 unlock\n");
#endif
                                //signal consumer to start processing buffer slots
				sem_post(buf_cnt_sem_c);
			}
		}
		free(line);
		fclose(fd);
        }else{
          printf("Failed to read from file %s\n",filename);
          rc = FAILED;
        }

	if(munmap(shm_area_p, shm_sz))
        {
                perror("Failed to unmap shm");
		rc = FAILED;
        }
shm_map_fail:
	if(close(shm_fd_p)==-1)
	{
                perror("Failed to close shm");
		rc = FAILED;
	}
shm_fail:
	if(one_buf_sem != SEM_FAILED)
	{
		if(sem_close(one_buf_sem))
                {
                  perror("Failed to close mutex");
		  rc = FAILED;
                }
	}
mutex_fail:
	if(buf_cnt_sem_c != SEM_FAILED)
	{
		if(sem_close(buf_cnt_sem_c))
                {
                  perror("Failed to close consumer sem");
		  rc = FAILED;
                }
	}
c_sem_fail:
	if(buf_cnt_sem_p != SEM_FAILED)
	{
		if(sem_close(buf_cnt_sem_p))
                {
                  perror("Failed to close producer sem");
		  rc = FAILED;
                }
	}
p_sem_fail:
#ifdef DEBUG
	printf("Exit Producer - rc(%d)\n", rc);
#endif
       return rc;
}


int main(int argc, char *argv[])
{
	char *filename = NULL;

	if(argc != 3)
	{
		printf("Usage: prod <buf_cnt> <filename>\n");
		return 0; 
	}

	int num_buff = atoi(argv[1]);
	if(num_buff<=0)
	{
		printf("Invalid circular buffer size\n");
		return 1;
	}
	filename = argv[2];

	return producer(filename, num_buff);
}
