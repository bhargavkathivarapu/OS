#include  <sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>

int ip=0,op=0,f=0;
char* fileout;
void parse(char *line, char **argv)
{
	int flag=0;
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';
          if(flag==1)
          {fileout=line;flag=2;}    
          if( flag==0 && *line =='>') flag=1;   
          if(flag==0 && *line !='&' && *line !='<')   	     /* replace white spaces with 0    */
          *argv++ = line;   
           
            							/* save the argument position     */
          
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
            {   
            	if(*line=='>')op=1;
            	if(*line=='<')ip=1;
            	if(*line=='&')f=1;
            	line++;    
            }        				 /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
    
}
void  execute(char **argv,int n)
{
     pid_t  pid;
     int    status;
     int fin,fout,f1,f2;

     if ((pid = fork()) < 0) {     // fork a child process           
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {   
     	
     		if(ip==1)
			{
				 fin=open(argv[2],O_RDONLY);
				f1=dup(0);
				dup2(fin,0);
				
			}
			if(op==1)
			{
				fout=creat(fileout,O_RDWR);
				fout=open(fileout,O_RDWR);
				f2=dup(1);
				dup2(fout,1);
				
				
			}
           // for the child process:         
          if (execvp(*argv, argv) < 0) {     // execute the command  
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
          
          if(ip==1)
			{
				close(0);
				dup(f1);
				
			}
			
			if(op==1)
			{
				close(1);
				dup(f2);
				
			}
     }
     else {   
     
     	if(f!=1)
     	{
     		while (wait(&status) != pid) ;
		}	
		else
		{
			printf("process executing in background\n");
			fflush(stdout);
		}                                     
          
     }
}
int print(char** argv)
{
int i=0;
	for( i=0;argv[i]!='\0';i++)
	{
	printf("%s\n",argv[i]);
	}
	printf("%d %d %d",i,ip,op);
	fflush(stdout);
	return i-1;
	
}


int main()
{
     char  line[1024];          
     char  *argv[64];              
	int n=0;
     while (1) {    
     	ip=0;op=0;f=0;              
          printf("$");  
          fflush(stdout);		   
          read(0,line,1024)  ;          
          
          parse(line, argv);
          n=print(argv);  
         
          if (strcmp(argv[0], "exit") == 0)  
               exit(0);            
          execute(argv,n);           
     }
}
