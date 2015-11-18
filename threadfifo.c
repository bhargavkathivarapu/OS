#include <malloc.h>
#include <ucontext.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include<stdlib.h>
#include<signal.h>
#define THREAD_STACKSIZE 32767
#define MAXTHREADS	20

int acti=0;
typedef struct thread_t
{
	int id;
	int active;
	ucontext_t context;
	struct thread_t *next;
	struct thread_t *prev;
	
} thread_t;

thread_t* Main=new thread_t();

thread_t *current_thread=NULL;
thread_t thread_table[MAXTHREADS];



void kill_thread()
{	
	 swapcontext(&current_thread->context,&Main->context);
}
void hello(void* arg)
{
	int i=*(int *)arg; 
	printf("Hello world .This is %d\n",i);
	
}
void pello(void* arg)
{
	int i=*(int *)arg; 
	while(1)
	printf("Hello world .This is %d\n",i);
	
}
void initfiber(void (*func)(void*),void* arg)   
{
   printf("hi");
  
	func(arg);
	printf("hi");
	current_thread->active=0;
	kill_thread();
	
    
}
void init()
{
	for ( int i = 0; i < MAXTHREADS; ++ i )
	{
		thread_table[i].active = 0;
	}
}
thread_t* create_thread( void (*func) (void*),void* arg)
{
   
 	struct itimerval t1;
 	static int thrno = 1;
 	thread_t *thr;
 	
	for (int i=0; i<MAXTHREADS; i++)
	{
		if (thread_table[i].active == 0)
		{
			printf("%d\n",i);
			thr=&thread_table[i];
			thread_table[i].active=1;
			break;
		}
	}

 	getcontext(&thr->context);
 
	 thr->context.uc_link = 0;
	 thr->context.uc_stack.ss_sp = malloc( THREAD_STACKSIZE );
	 thr->context.uc_stack.ss_size = THREAD_STACKSIZE;
	 thr->context.uc_stack.ss_flags = 0;    
	 makecontext( &thr->context, (void (*)(void)) &initfiber, 2, func,arg );
	 current_thread=thr;
	 swapcontext(&Main->context,&thr->context);
	
	return thr;


}



int main()
{
	init();
	
	//getcontext(&Main->context);
	thread_t *t1, *t2;

	int t1no=1,t2no=2;
	t1 = create_thread(pello, (void *)&t1no);
	t2 = create_thread(hello, (void *)&t2no);
	
	//waitforallthreads();
	
}
