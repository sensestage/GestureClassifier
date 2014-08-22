/*
 * File:   DtwPsClassifier.h
 * Author: Bas
 *
 * Created on 26 juni 2014, 14:31
 */

#ifndef DTWPSCLASSIFIER_H
#define	DTWPSCLASSIFIER_H

#include "Template.h"
#include "AbstractGestureClassifier.h"

#include "VectorDTW.h"
#include <deque>
#include <float.h>

class DtwPsClassifier : public AbstractGestureClassifier {
protected:
    std::vector<Template> templateBank;
    std::deque<Point> buffer;

    int maxBufferSize = 1;
    int dimensions = 3;
    int mostLikelyGest = -1;
    double phase = 0.;
    double distance = DBL_MAX;

public:
    DtwPsClassifier();
    DtwPsClassifier(const DtwPsClassifier& orig);
    virtual ~DtwPsClassifier();

    void infer(std::vector<float>&);
    void addTemplate();
    void fillTemplate(int, std::vector<float>&);
    void clearTemplate(int);
    void clear();
    int mostLikelyGesture();
    double getPhase();
    int templateSize(int i);
    int size();
    double getDistance();
    int getDimensions();

};

#endif	/* DTWPSCLASSIFIER_H */

