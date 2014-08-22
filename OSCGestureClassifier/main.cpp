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
bool wasSync = false;
bool wasDetected = false;

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
        wait_a_bit();

    busy = true;
    rc->infer(vec);
    int g = rc->mostLikeliGesture();
    if (g>=0)
    {
        if(!wasDetected)
        {
            wasDetected = true;
            a->send("/OSCGestureClassifier/detected","i",1);
        }
        a->send(lo::Bundle(
        {
            {"/OSCGestureClassifier/index",lo::Message("i",rc->mostLikeliGesture())},
            {"/OSCGestureClassifier/distance", lo::Message("f",rc->getDistance())},
            {"/OSCGestureClassifier/phase", lo::Message("f",rc->getPhase())}
        } ));
    }
    else if(wasDetected)
    {
        wasDetected = false;
        a->send("/OSCGestureClassifier/detected","i",0);
    }


    if(wasLearning >= 0 && !rc->isLearning())
    {
        std::cout << "Gesture learned! Length is: " << rc->templateSize(wasLearning) << "\n";
        a->send("/OSCGestureClassifier/learned", "i", wasLearning);
        wasLearning = -1;
//        rc->learn();
    }


    if(rc->isSync())
    {
        if(!wasSync)
        {
            wasSync = true;
            a->send("/OSCGestureClassifier/repetition", "i", 1);
        }
        a->send("/OSCGestureClassifier/interval","i",rc->repetitionInterval());
    }
    else if(wasSync)
    {
        wasSync = false;
        a->send("/OSCGestureClassifier/repetition", "i", 0);
    }

    busy = false;
}

void learn_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    while(busy)
        wait_a_bit();

    busy = true;
    rc->learn();
    wasLearning = rc->size()-1;
    cout << "Learning gesture " << rc->size() << "\n";
    busy = false;
}

void recognize_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    while(busy)
        wait_a_bit();

    busy = true;
    rc->stopLearning();
    cout << "Stopped learning\n";
    busy = false;
}

void clear_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    while(busy)
        wait_a_bit();

    busy = true;
    rc->clear();
    wasLearning = -1;
    cout << "Classifier cleared\n";
    busy = false;
}

void threshold_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    while(busy)
        wait_a_bit();
    busy = true;
    rc->setRecognitionThreshold((double)argv[0]->f);
    cout << "threshold set to: " << argv[0]->f << "\n";
    busy = false;
}

void default_handler(const char *path, const char *types, lo_arg **argv, int argc)
{
    std::cout << "Sorry, but " << path << " " << types << " is not a valid command message...\n";
}

void help()
{
    std::cout << "+==============================================================================\n";
    std::cout << "+====================          OSCGestureClassifier          ==================\n";
    std::cout << "+==============================================================================\n";
    std::cout << "\n";
    std::cout << "Send messages:\n";
    std::cout << "/data f f f \t3-dimensional data such as accelerometer or Kinect data\n";
    std::cout << "/learn \t\tto start recording a new template on repeated movement\n";
    std::cout << "/clear \t\tto clear the classifier\n";
    std::cout << "/theshold f \tto set the recognition threshold\n";
    std::cout << "\n";
    std::cout << "When learning, the algorithm will still try to recognize the gesures.";
    std::cout << "If the distance between the performed gesture and a template is below \n";
    std::cout << "the threshold, the gesture is recognized. If that distance is above the \n";
    std::cout << "threshold, a new gesture is learned.\n";
    std::cout << "\n";
    std::cout << "Messages back:\n";
    std::cout << "/OSCGestureClassifier/repetition i \t1 or 0 whether repetition is detected\n";
    std::cout << "/OSCGestureClassifier/interval i \tlength of the detected repetition \n";
    std::cout << "/OSCGestureClassifier/detected i \t1 or 0 whether template is detected \n";
    std::cout << "/OSCGestureClassifier/index i \t\tindex of detected template\n";
    std::cout << "/OSCGestureClassifier/distance f \tdistace from gesture to detected template\n";
    std::cout << "/OSCGestureClassifier/phase f \t\tcurrent phase of performed gesture\n";
    std::cout << "/OSCGestureClassifier/learned i \tsize of vocabulary\n";
    std::cout << "\n";
    std::cout << "+==============================================================================\n";
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
    std::cout << "Press h for Help\n";
    std::cout << "Wanna quit? Hit q and Enter\n\n";

    std::atomic<int> received(0);

    rc = new RepClassifier();

    st.add_method("/data","fff",data_handler);
    st.add_method("/learn","",learn_handler);
    st.add_method("/recognize","",recognize_handler);
    st.add_method("/threshold","f",threshold_handler);
    st.add_method("/clear","",clear_handler);
    st.add_method(NULL,NULL,default_handler);

    st.start();

    char s[10];

    while (true)
    {
        std::cin.getline(s,10);
        if(strcmp(s,"q")==0)
            break;
        else if(strcmp(s, "h")==0)
            help();
        else
            std::cout << "If you wanna quit, enter 'q'\n";
    }

    std::cout << "\nClosing time...\nYou don't have to go home...\nBut you can't stay here...\n";
}
