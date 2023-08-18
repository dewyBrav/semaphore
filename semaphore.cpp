#include <iostream>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// с использованием семафоров(целочисленная переменная, содержащая количество активации, отложенных на будуйщее)
#define N 100
typedef int semaphore;

semaphore mutex = 1;
semaphore empty = N;
semaphore full = 0;

int buffer[N];
int producerItem = 1000;

void insert_item(int item)
{
    buffer[full] = item; // добавляем значение в буфер
    std::cout << "item was added" << std::endl;
}
void consumer_item(int item) { std::cout << buffer[full] << std::endl; }
int remove_item() { return buffer[full]; }      // получаем значение из буфера и удаляем
int producer_item() { return ++producerItem; }; // генерация данных

void down(semaphore *var)
{
    *(--var);
    std::cout << "semaphore --" << std::endl;
}
void up(semaphore *var)
{
    *(++var);
    std::cout << "semaphore ++" << std::endl;
}

void producer()
{
    int item;

    while (true)
    {
        item = producer_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}

void consumer()
{
    int item;

    while (true)
    {
        down(&full);
        down(&mutex);
        item = remove_item();
        up(&mutex);
        up(&empty);
        consumer_item(item);
    }
}

int main()
{
    //
    //producer();
    //consumer();
    
        std::thread t1(producer);
        std::thread t2(consumer);

        t1.join();
        t2.join(); 
    return 0;
}