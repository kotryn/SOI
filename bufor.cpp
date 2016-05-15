#include "monitor.h"
#include <iostream>
#include <cstdlib>
#include <deque>
#include <pthread.h>
#include <time.h>

#define N 5 //number of elements

using namespace std;

int size_b;
int k, p;

class Buffer : public Monitor
{
public:
	//Buffer() : {}

	void produce();
	void consume();


protected:
	Condition empty, full;

private:
	 deque<char> buffer;

	int size() {
		return buffer.size();
	}

	void Insert(char product) {
     	if (buffer.size() < size_b) {
        	buffer.push_front(product);
      	}
      	else {
        	cerr << "ERROR with function push_front\n";
        	exit(2);
      	}
	 }

	void Delete() {
    	if (buffer.size() > 0) {
        	buffer.pop_back();
        }
        else{
        	cerr << "ERROR with function pop_back\n";
        	exit(3);
        }
    }

};

void Buffer:: produce()
{
	enter();
	if(buffer.size() == size_b)
	{
		cout << "----------------------BUFFER IS FULL!!!-------------------\n";
		wait(full);
	}
	Insert('P');
	cout << "Producer produce a product,";
	cout.width(42);
	cout << "in buffer "<< buffer.size() << " el.\n";
	signal(empty);
	leave();
}

 void Buffer:: consume()
{
	enter();
	if(buffer.size() == 0){
		cout << "......................BUFFER IS EMPTY!!!......................\n";
		wait(empty);
	}
	Delete();
	cout << "Consumer take a product,";
	cout.width(45);
	cout << "in buffer " << buffer.size() << " el.\n";
	signal(full);
	leave();
}

Buffer buffer;

void * producer(void *){
	for(int i = 0; i != N*k; i++){
		sleep((double) rand()/RAND_MAX);
		buffer.produce();
	}
}

void * consumer(void *){
	for(int i = 0; i != N*p; i++){
		sleep((double) rand()/RAND_MAX);
		buffer.consume();
	}
}

int main(int argc, char *argv[])
{
	if( argc != 4 ) {
        cout << "Please give 3 arguments!\n  The number of producer\n  The number of consumer\n  Buffer size\n";
        exit(0); 
    }
    p = atoi(argv[1]);
    k = atoi(argv[2]);
	size_b = atoi(argv[3]);

  	srand(time(NULL));
  
	pthread_t t_id[k+p];

	for( int i = 0; i < p; i++){
		pthread_create(&(t_id[i]), NULL, producer, (void*)i);
	}
	for(int i = p; i < k+p; i++){
		pthread_create(&(t_id[i]), NULL, consumer, (void*)i);		
	}

	for ( int i = 0; i < k+p; i++){
    	pthread_join(t_id[i],(void **)NULL);
	}
	return 0;	
}

