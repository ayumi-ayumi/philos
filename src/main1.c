#include <stdio.h>
#include <pthread.h>

#define NUM_THREAD 4
#define ADD_NUM 100000

long long cnt = 0;

pthread_mutex_t mutex;

typedef struct thread_data
{
	int	addNum;
}	THREAD_DATA;


void	*routine(void *arg)
{
	THREAD_DATA *data = (THREAD_DATA *)arg;
	for (int i = 0; i < data->addNum; i++)
	{
		pthread_mutex_lock(&mutex);
		// if (cnt == 100000)
		// {
		// 	pthread_mutex_unlock(&mutex);
		// 	break ;
		// }
		cnt = cnt + 1;
		pthread_mutex_unlock(&mutex);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t thread[NUM_THREAD];
	THREAD_DATA data[NUM_THREAD];
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < NUM_THREAD; i++)
	{
		data[i].addNum = ADD_NUM / NUM_THREAD;
	}
	for (int i = 0; i < NUM_THREAD; i++)
	{
		pthread_create(&thread[i], NULL, &routine, &data[i]);

	}
	for (int i = 0; i < NUM_THREAD; i++)
	{
		pthread_join(thread[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	printf("cnt -> %lld\n", cnt);
}
