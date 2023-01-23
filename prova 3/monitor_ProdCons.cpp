#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

using namespace std;

const int QTD_PRODUCERS = 30;

mutex mutex_;


class Monitor{
    private:
        int *buffer;
        int rear, front;
        int tam_buffer;
        sem_t buffer_empty, buffer_full;

    public:
        Monitor(int tamanho):rear(0),front(0),tam_buffer(tamanho){
            buffer = (int*)malloc(sizeof(int)*tam_buffer);
            sem_init(&buffer_empty, 0, tam_buffer);
            sem_init(&buffer_full, 0, 0);
        }

        int fetch(){
            int result;

            sem_wait(&buffer_full);

            result = buffer[front];
            front = (front + 1) % tam_buffer;

            sem_post(&buffer_empty);

            return result;
        }

        void deposit(int deposit){
            sem_wait(&buffer_empty);

            buffer[rear] = deposit;
            rear = (rear + 1) % tam_buffer;

            sem_post(&buffer_full);
        }
};

void producer(Monitor *monitor){
    int data;
    for(int i = 0; i < QTD_PRODUCERS; i++){
        data = rand() % 300;
        monitor->deposit(data);

        mutex_.lock();
        cout << "[producer] Produzi " << data << endl;
        mutex_.unlock();
        usleep(1);
    }
}

void consumer(Monitor *monitor){
    int data;
    for(int i = 0; i < QTD_PRODUCERS; i++){
        data = monitor->fetch();
        mutex_.lock();
        cout << "[consumer] Consumi " << data << endl;
        mutex_.unlock();
        usleep(2);
    }
}

int main(void){

    Monitor *monitor = new Monitor(8);
    thread prod(producer, monitor);
    thread cons(consumer,monitor);
    time_t t;

    srand(time(&t));

    prod.join();
    cons.join();

}