#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>


using namespace std;

class MonitorConta{
    private:
        float saldo;
        int cpf;
        mutex lock;
    public:
        MonitorConta(float saldoInicial, int cpfCliente):saldo(saldoInicial), cpf(cpfCliente){

        }

        float getSaldo(){
            float result;

            lock.lock();
            result = saldo;
            lock.unlock();

            return result;
        }

        void credito(float cred){
            if(cred < 0){
                cout << "Credito invalido" << endl;

                return;
            }

            lock.lock();

            saldo += cred;
            cout << "[" << cpf << "]" << "creditando " << cred << " na conta" << endl;

            lock.unlock();

            sleep(1);
        }

        void debito(float deb){

            if(deb < 0){

                cout << "Débito invalido";

                return;
            }

            lock.lock();

            saldo -= deb;
            cout << "[" << cpf << "]" << "debitando " << deb << " da conta" << endl;

            lock.unlock();

            sleep(1);
        }

};


int main(void){

    MonitorConta *monitor_1 = new MonitorConta(100, 1500);
    // MonitorConta *monitor_2 = new MonitorConta(300, 1000); 

    thread t1(MonitorConta::credito, monitor_1, 300);
    thread t2(MonitorConta::debito, monitor_1, 100);
    thread t3(MonitorConta::credito, monitor_1, 500);
    thread t4(MonitorConta::debito, monitor_1, 900);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "[Monitor 1] Saldo final = " << monitor_1->getSaldo() << endl;  
    // cout << "[Monitor 2] Saldo final = " << monitor_2->getSaldo() << endl;

    free(monitor_1);


    return 0;
}