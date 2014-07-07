#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include "RepClassifier.h"
#include "RepClassifier.cpp"

#include <iostream>
#include <atomic>
#include <unistd.h>
#include <string.h>
#include <vector>

using namespace std;

bool busy = false;

int wasLearning = -1;

RepClassifier* rc;
lo::Address* a;

void wait_a_bit()
{
#ifdef WIN32
    Sleep(10);
#else
    usleep(10*1000);
#endif
}

void data_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    std::vector<float> vec;
    vec.push_back(argv[0]->f);
    vec.push_back(argv[1]->f);
    vec.push_back(argv[2]->f);

    while(busy)
    {
        wait_a_bit();
    }
    busy = true;
    rc->infer(vec);
    int g = rc->mostLikeliGesture();
    if (g>=0)
        a->send("/RepClassifier/mostLikeliGesture","i",rc->mostLikeliGesture());
    a->send(lo::Bundle(
    {
        {"/RepClassifier/distance", lo::Message("f",rc->getDistance())},
        {"/RepClassifier/phase", lo::Message("f",rc->getPhase())}
    } ));
    if(wasLearning >= 0 && !rc->isLearning())
    {
        std::cout << "Gesture learned! Length is: " << rc->templateSize(wasLearning) << "\n";
        wasLearning = -1;
    }
    if(rc->isSync())
    {
        a->send("/RepClassifier/repetition","i",rc->repetitionInterval());
    }
    busy = false;
}

void learn_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    while(busy)
    {
        wait_a_bit();
    }
    busy = true;
    rc->learn();
    wasLearning = rc->size()-1;
    cout << "Learning gesture " << rc->size() << "\n";
    busy = false;
}

void clean_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    while(busy)
        wait_a_bit();

    busy = true;
    rc->clear();
    wasLearning = -1;
    cout << "Classifier cleaned\n";
    busy = false;
}

void default_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    std::cout << "Sorry, but " << path << " " << types << " is not a valid command message...\n";
}

int main(int argc, char** argv)
{
    char *myport = "9005";
    char *dest_port = "9006";
    char *dest_ip = "127.0.0.1";

    if ( argc > 3 )
        dest_ip = argv[3];
    if ( argc > 2 )
        myport = argv[2];
    if ( argc > 1 )
        dest_port = argv[1];


    lo::ServerThread st(myport);
    if (!st.is_valid())
    {
        std::cout << "Nope." << std::endl;
        return 1;
    }

    a = new lo::Address("127.0.0.1", dest_port);

    std::cout << "Comming in at: " << st.url() << std::endl;
    std::cout << "Back at you at: " << a->url() << std::endl << std::endl;
    std::cout << "Wanna quit? Hit q and Enter\n\n";

    std::atomic<int> received(0);

    rc = new RepClassifier();

    st.add_method("/data","fff",data_handler);
    st.add_method("/learn","",learn_handler);
    st.add_method("/clean","",clean_handler);
    st.add_method(NULL,NULL,default_handler);

    st.start();

    char s[10];

    while (true)
    {
        std::cin.getline(s,10);
        if(strcmp(s,"q")==0)
            break;
        else
            std::cout << "If you wanna quit, enter 'q'\n";
    }

    std::cout << "\nClosing time...\nYou don't have to go home...\nBut you can't stay here...\n";
}
