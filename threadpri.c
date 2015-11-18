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
	int pri;
	ucontext_t context;
	struct thread_t *next;
	struct thread_t *prev;
	
} thread_t;

thread_t* Main=new thread_t();

thread_t *readyq=NULL,*current_thread=NULL,*head=NULL,*tail=NULL;
thread_t thread_table[MAXTHREADS];




void kill_thread()
{	
		acti--;
		head=head->next;
		
		ucontext_t dum;
		if(acti==0)
		{
		
		swapcontext(&dum,&Main->context);}
		else
		{
		current_thread=head;
		swapcontext(&dum,&head->context);
		}
		
	
	

}
void hello(void* arg)
{
	int i=*(int *)arg; 
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
thread_t* create_thread( void (*func) (void*),void* arg,int priority)
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
 	thr->pri=priority;
	 thr->context.uc_link = 0;
	 thr->context.uc_stack.ss_sp = malloc( THREAD_STACKSIZE );
	 thr->context.uc_stack.ss_size = THREAD_STACKSIZE;
	 thr->context.uc_stack.ss_flags = 0;    
	 makecontext( &thr->context, (void (*)(void)) &initfiber, 2, func,arg );
	 thr->next=NULL;
	 thr->prev=NULL;
	
 		printf("Making threads\n");
	 if(head==NULL)
	 {
	 	
	 	
	 	
				
				if(readyq==NULL)
			 	{
			 		printf("creating main thread\n");
			 		Main->id = thrno++;
				 	Main->active=2;
				 	Main->pri=5;
				 	Main->next = NULL;
					Main->prev = NULL;
				 	//head=tail=Main;
				 	readyq=Main;
				 }
			 	head=thr;
			 	acti++;
			 	current_thread=thr;
			 	swapcontext(&Main->context,&thr->context);
				return thr;

	 }
	 else
	 {
	 	
		 thread_t *t,*p;
		 t=p=head;
		 while( t!=NULL && t->pri>thr->pri)
		 {
		 	p=t;
		 	t=t->next;
		 }

		 if(t==head)
		{
			thr->next=head;
			 head->prev=thr;
			 current_thread=thr;
			 head=thr;
			 swapcontext(&head->next->context,&thr->context);
		 }
		 else
		 {
		 	
			p->next =thr;
			thr->prev = p;
			thr->next=t;
			if(t!=NULL)
			t->prev=thr;
		}
	}

	
	
	
	return thr;


}

void waitforallthreads()
{
	while(acti>0){
	printf("This is main thread\n");

		for (int j=0; j<0x200000; j++) ;

	}
	
	
}

int main()
{
	init();
	
	//getcontext(&Main->context);
	thread_t *t1, *t2;

	int t1no=1,t2no=2;
	t1 = create_thread(hello, (void *)&t1no,6);
	t2 = create_thread(hello, (void *)&t2no,5);
	
	waitforallthreads();
	
}
