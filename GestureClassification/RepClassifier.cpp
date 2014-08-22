/*
 * File:   RepClassifier.cpp
 * Author: Bas
 *
 * Created on 27 juni 2014, 9:43
 */

#include "RepClassifier.h"
#include "DtwPsClassifier.h"
#include "DtwPsClassifier.cpp"
#include "YIN.h"
#include "YIN.cpp"

#include <float.h>

RepClassifier::RepClassifier()
{
    yin = new YIN(3);
    yin->setDipThreshold(.2);
    //    yin->setAverageThreshold(.2);
    yin->setMaxLength(20);

    dtw = new DtwPsClassifier();
}

RepClassifier::RepClassifier(const RepClassifier& orig)
{
    yin = orig.yin;
    dtw = orig.dtw;
}

RepClassifier::~RepClassifier()
{
    delete yin;
    delete dtw;
}

void RepClassifier::infer(std::vector<float>& newSample)
{
    if (newSample.size() != dtw->getDimensions())
        return;

    if (!recording)
        dtw->infer(newSample);
    yin->process(newSample);

    if (dtw->getDistance() > recognitionThreshold || recording)
    {
        if (learning >= 0)
        {
            if (yin->isSync())
            {
                recording = true;
                dtw->fillTemplate(learning, newSample);
                if (dtw->templateSize(learning) > yin->getLength() && dtw->templateSize(learning) > 20)
                {
                    learning = -1;
                    recording = false;
                }
            }
            else if (dtw->templateSize(learning) > 0)
            {
                dtw->clearTemplate(learning);
                recording = false;
            }
        }
    }
}

void RepClassifier::addTemplate()
{
    dtw->addTemplate();
}

void RepClassifier::fillTemplate(int i, std::vector<float>& t)
{
    dtw->fillTemplate(i, t);
}

void RepClassifier::clearTemplate(int i)
{
    dtw->clearTemplate(i);
}

void RepClassifier::clear()
{
    dtw->clear();
    yin->clear();
}

int RepClassifier::mostLikelyGesture()
{
    if (dtw->getDistance() < recognitionThreshold && yin->isSync())
        return dtw->mostLikelyGesture();
    else
        return -1;
}

double RepClassifier::getPhase()
{
    return dtw->getPhase();
}

int RepClassifier::templateSize(int i)
{
    return dtw->templateSize(i);
}

int RepClassifier::size()
{
    return dtw->size();
}

double RepClassifier::getDistance()
{
    return dtw->getDistance();
}

void RepClassifier::learn()
{
    if (!isLearning())
    {
        addTemplate();
        learning = dtw->size() - 1;
    }
}

void RepClassifier::learn(int i)
{
    dtw->clearTemplate(i);
    learning = i;
}

void RepClassifier::setRecognitionThreshold(double r)
{
    recognitionThreshold = r;
}

bool RepClassifier::isLearning()
{
    return learning >= 0;
}

bool RepClassifier::isSync()
{
    return yin->isSync();
}

int RepClassifier::repetitionInterval()
{
    if(yin->isSync())
        return yin->getLength();
    else
        return -1;
}

    void RepClassifier::stopLearning(){
        learning = false;
        stayLearning = false;
    }
