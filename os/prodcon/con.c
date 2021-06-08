/**
 * Description:
 * The consumer program reads from circular buffer one buffer slot at a time.
 * Each buffer is slot is processed one entry at a time. The data layout
 * is |size of entry|entry payload|.A pattern provided by the user is searched
 * in the entry. The entry is printed to console if match is found.
 * The size of the circular buffer and pattern should be provided at command line.
 * Usage: cons <circular buffer size> <pattern>
 * Future Improvements:
 * 1. Make Consumer less sentive to OS interrupts by handling EINTR.
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


sem_t *g_buf_cnt_sem_p, *g_buf_cnt_sem_c, *g_one_buf_sem;
char *g_shm_area_c;
int g_shm_sz = 0, g_shm_fd_c;
int consumer(int num_buff, char *search_str)
{
	int line_sz = 0, c_index = 0, rc = SUCCESS;
	int buff_index = 0;
	int search_str_len = strlen(search_str);
	char *ptr = NULL;
#ifdef DEBUG
	int buff_cnt = 0;
	int p_val = 0;
#endif
	g_shm_sz = (sizeof(char)*BUFFER_SIZE*num_buff);

	if((g_buf_cnt_sem_p = sem_open(SEM_BUF_CNT_NAME_P, 0,
					0644, num_buff)) == SEM_FAILED){
		perror("Failed to open producer semaphore");
		rc = FAILED;
		goto p_sem_fail;
	}

	if((g_buf_cnt_sem_c = sem_open(SEM_BUF_CNT_NAME_C, 0,
					0644, 0)) == SEM_FAILED){
		perror("Failed to open consumer semaphore");
		rc = FAILED;
		goto c_sem_fail;
	}

	if((g_one_buf_sem = sem_open(SEM_ONE_BUF, 0,
					0644, 1)) == SEM_FAILED){
		perror("Failed to open buffer index mutex");
		rc = FAILED;
		goto mutex_fail;
	}

	if((g_shm_fd_c = shm_open(SHM_NAME, O_RDONLY, 0644)) == -1)
	{
		perror("Failed to read shm");
		rc = FAILED;
		goto shm_fail;
	}

	g_shm_area_c = mmap(NULL, g_shm_sz,
			PROT_READ,
			MAP_SHARED,g_shm_fd_c, 0);
	if(g_shm_area_c == MAP_FAILED)
	{
		perror("Failed to mmap shm");
		rc = FAILED;
		goto shm_map_fail;
	}


	while(1){
#ifdef DEBUG
		sem_getvalue(g_buf_cnt_sem_c, &p_val);
		printf("Hello from consumer - %d\n",p_val);
		sem_getvalue(g_buf_cnt_sem_p, &p_val);
		printf("producer %d\n",p_val);
#endif
		sem_wait(g_buf_cnt_sem_c);
		sem_wait(g_one_buf_sem);
		if(c_index>=num_buff)
			c_index =0;
		buff_index = c_index;
		c_index++;
		sem_post(g_one_buf_sem);
#ifdef DEBUG
		printf("%d) Buffer %d\n", buff_cnt++, buff_index);
#endif
		line_sz = 0;
		ptr = g_shm_area_c+(BUFFER_SIZE*buff_index);
		while(line_sz<BUFFER_SIZE)
		{
			unsigned long read_len = *((unsigned long *)(ptr+line_sz));
			line_sz += sizeof(unsigned long);
                        if(read_len == 0)
                            continue;
			if((line_sz+read_len) > BUFFER_SIZE){
#ifdef DEBUG
				printf("\ninvalid len %ld,", read_len);
#endif
				break;
			}
#ifdef DEBUG
			printf("%ld - ", read_len);
			printf("consumer %d - len =%ld\n",
					c_index, read_len);
#endif
			if(memmem((char *)(ptr+line_sz),read_len, 
						search_str, search_str_len))
			{
				fwrite((char *)(ptr+line_sz), sizeof(char),
						read_len, stdout);
				printf("\n");
			}
			line_sz += read_len;
		}
		sem_post(g_buf_cnt_sem_p);
	}

	if(munmap(g_shm_area_c, g_shm_sz))
	{
		perror("Failed to unmap shm");
		rc = FAILED;
	}
shm_map_fail:
	if(close(g_shm_fd_c)==-1)
	{
		perror("Failed to close shm");
		rc = FAILED;
	}
shm_fail:
	if(g_one_buf_sem != SEM_FAILED)
	{
		if(sem_close(g_one_buf_sem))
		{
			perror("Failed to close mutex");
			rc = FAILED;
		}
	}
	sem_unlink(SEM_ONE_BUF);
mutex_fail:
	if(g_buf_cnt_sem_c != SEM_FAILED)
	{
		if(sem_close(g_buf_cnt_sem_c))
		{
			perror("Failed to close consumer sem");
			rc = FAILED;
		}
	}
	sem_unlink(SEM_BUF_CNT_NAME_C);
c_sem_fail:
	if(g_buf_cnt_sem_p != SEM_FAILED)
	{
		if(sem_close(g_buf_cnt_sem_p))
		{
			perror("Failed to close producer sem");
			rc = FAILED;
		}
	}
	sem_unlink(SEM_BUF_CNT_NAME_P);
p_sem_fail:
	shm_unlink(SHM_NAME);
#ifdef DEBUG
	printf("Exit Consumer - rc(%d)\n", rc);
#endif
	return rc;
}

void int_handler(int sig)
{
	char c;
	signal(sig, SIG_IGN);
	printf("End Consumer ?[y/n]");
	c = getchar();
	if(c != 'y')
	{
		signal(SIGINT, int_handler);
	}else{
                printf("Cleaning up\n");
		if(munmap(g_shm_area_c, g_shm_sz))
		{
			perror("Failed to unmap shm");
		}
		if(close(g_shm_fd_c)==-1)
		{
			perror("Failed to close shm");
		}
		if(g_one_buf_sem != SEM_FAILED)
		{
			if(sem_close(g_one_buf_sem))
			{
				perror("Failed to close mutex");
			}
		}
		sem_unlink(SEM_ONE_BUF);
		if(g_buf_cnt_sem_c != SEM_FAILED)
		{
			if(sem_close(g_buf_cnt_sem_c))
			{
				perror("Failed to close consumer sem");
			}
		}
		sem_unlink(SEM_BUF_CNT_NAME_C);
		if(g_buf_cnt_sem_p != SEM_FAILED)
		{
			if(sem_close(g_buf_cnt_sem_p))
			{
				perror("Failed to close producer sem");
			}
		}
		sem_unlink(SEM_BUF_CNT_NAME_P);
		shm_unlink(SHM_NAME);
                printf("Done\n");
	}
}

int main(int argc, char *argv[])
{
        signal(SIGINT, int_handler);

	if(argc != 3)
	{
		printf("Usage: cons <buf_cnt> <search_string>\n");
		return 0; 
	}

	int num_buff = atoi(argv[1]);
        if(num_buff <= 0)
        {
            printf("Invalid circular buffer size\n");
            return 1;
        }
        if(strlen(argv[2])==0)
        {
            printf("Invalid pattern\n");
            return 1;
        }
	return consumer(num_buff, argv[2]);
}
