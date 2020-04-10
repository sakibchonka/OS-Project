#include<stdio.h>                 //header files
#include<conio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
#define max_cust 20               //max no. of customers threads.
#define max_chair 20              //max no. of chairs threads.
void *customer(void *n);          //function prototypes
void *barber(void *);
void wait(int sec);
sem_t waiting_room;               // waiting_room limits no. of customers allowed to enter the waiting room at one time
sem_t barber_chair;               // used to sit on and barber sleeps over  it.
sem_t pillow;                     //it shows mutually exclusive access to barber chair
int done=0;                       //variable for all of the work
pthread_t a;                      //making threads
pthread_t b[max_cust];
int chairs,customers,i,m;         //defining variables
int numb[max_cust];               //array of customers
 
void *barber(void *junk)          //barber function
{
	while (!done)             //customers who are left
	{
		printf("\t Barber is sleeping because no customer is in barber room.\n");
		sem_wait(&pillow);                   // pillow being used by barber to sleep on
		printf("\t Barber has finished his job.\n ");
        }
	printf("\n");
	printf("_______________________CLOSED NOW__________________________");
	printf("\n");
}

void *customer(void *num)                                              // customer function
{
	int n=*(int *)num;                                             //declaring new variable which is used as serial no. of customers
	printf("%d.",n);                                               //adding sserial no. to each entry of customer
	printf("\t Customer %d arrived at the shop.\n",n);             //entering shop
	sem_wait(&waiting_room);                                       //waiting for space to open up
	printf("\t Customer %d sits in one of the chairs.\n",n);       //sitting on the chair
	wait(1);                                                       //waiting for 1 second, we can increase the time also
	printf("\t After waiting for some time.\n");
	printf("\t Customer %d entering the barber room.\n",n);        //entering barber room
	sem_wait(&barber_chair);                                       //waiting for barber chair to become free
	sem_post(&waiting_room);
	printf("\t Customer %d waking up the barber.\n",n);
	sem_post(&pillow);
	sem_post(&barber_chair);                                       //giving up the chair
	printf("\t Customer %d leaving the shop after haircut.\n",n);
}

int main(int argc, char *argv[])                                 //main method
{ 
        printf("_________________WELCOME TO THE BARBER SHOP_________________\n");
	printf("\n");
	printf("\t\t Enter how many customers are there: ");
	scanf("%d",&customers);
	if(customers==0)
	{
		printf("\t\t  Barber goes to sleep.\n");
		exit(-1);	
	}
	
	printf("\t\t Enter no. of chairs: ");
	scanf("%d",&chairs);
	printf("\n");
	        

    if(customers>chairs)                                     // customers wont be able to sit if seats are not sufficient
    {
    	printf("\t Sorry, the chairs are Occupied.\n");
        printf("\t Rest of customers leave the shop.\n");
        printf("\t We are left with %d customers in waiting room\n",(customers-chairs));
        for(i=1;i<max_cust;i++)                           // loop to increase the variable for every customer.
        {
    	   numb[i]=i;                                   //initializing array
        }
    	
        sem_init(&waiting_room,0,chairs);                    //initialzing semaphores with initial values.
        sem_init(&barber_chair,0,1);
        sem_init(&pillow,0,0);
    	
    	pthread_create(&a,NULL,barber,NULL);                 //create barber thread
        for(i=1;i<=chairs;i++)                               //loop for creating no. of customers thread
    	{
            pthread_create(&b[i],NULL,customer,(void*)&numb[i]);     //create customer thread
    	    sleep(1);                                                // wait function' formula used 
        }
        for(i=1;i<=chairs;i++)                                               // loop for joining threads
        {
        pthread_join(b[i],NULL);                                     //join threads to wait for them to finish
    	sleep(1);
        }
    	done=1;                               //change it to 1 if all the customers are done with cutting and kill barber thread
    	sem_post(&pillow);                    //waking barber to make him exit
    	pthread_join(a,NULL);
    	exit(-1);
    }
	//the further loop is same as above, the difference is that it is for the chairs>=customers, rest initialzings are same
    for(i=1;i<max_cust;i++)
    {
	numb[i]=i;
    }
	
	sem_init(&waiting_room,0,chairs);
	sem_init(&barber_chair,0,1);
	sem_init(&pillow,0,0);
	
    pthread_create(&a,NULL,barber,NULL);
    for(i=1;i<=customers;i++)
	{
	    pthread_create(&b[i],NULL,customer,(void*)&numb[i]);
	    sleep(1);
        }
    
	for(i=1;i<=customers;i++)
	{
	    pthread_join(b[i],NULL);
	    sleep(1);
        }
        done=1;
	sem_post(&pillow);
	pthread_join(a,NULL);
}
