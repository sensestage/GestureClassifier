/*
 * File:   DtwPsClassifier.cpp
 * Author: Bas
 *
 * Created on 26 juni 2014, 14:31
 */

#include "DtwPsClassifier.h"
#include "Template.h"
#include "Template.cpp"

#include <float.h>
#include <vector>
#include <algorithm>
#include <stdio.h>

DtwPsClassifier::DtwPsClassifier() {

}

DtwPsClassifier::DtwPsClassifier(const DtwPsClassifier& orig) {
    templateBank = orig.templateBank;
    buffer = orig.buffer;
}

DtwPsClassifier::~DtwPsClassifier() {

}

void DtwPsClassifier::infer(std::vector<float>& newSample) {

    if (newSample.size() != dimensions)
        return;

    buffer.push_back(Point(newSample[0], newSample[1], newSample[2]));

    if (buffer.size() > maxBufferSize) {
        buffer.pop_front();

        // init vars to find best
        int minIndex = -1;
        float minRot = 0.;
        double minVal = DBL_MAX;

        double secondVal = DBL_MAX;

        // compare buffer with each template
        for (int k = 0; k < templateBank.size(); k++) {
//             double d = templateBank[k].distance(buffer, resolution);
            double d = templateBank[k].distance(buffer);
            double rotation = templateBank[k].getPhase();
            if (d < minVal) {
                secondVal = minVal;
                minVal = d;
                minIndex = k;
                minRot = rotation;
            } else if (d < secondVal) {
                secondVal = d;
            }
        }
        mostLikelyGest = minIndex;
        phase = minRot;
        distance = minVal;
    }
}

void DtwPsClassifier::addTemplate() {
    templateBank.push_back(Template());
}

void DtwPsClassifier::fillTemplate(int i, std::vector<float>& f) {
    if (f.size() != dimensions)
        return;

    templateBank[i].add(Point((double) f[0], (double) f[1], (double) f[2]));
    maxBufferSize = max(maxBufferSize, templateBank[i].size());
}

void DtwPsClassifier::clearTemplate(int i) {
    templateBank[i].clear();
}

void DtwPsClassifier::clear() {
    templateBank.clear();
    phase = 0.;
    mostLikelyGest = -1;
    maxBufferSize = 1;
}

int DtwPsClassifier::mostLikelyGesture() {
    return mostLikelyGest;
}

double DtwPsClassifier::getPhase() {
    return phase;
}

int DtwPsClassifier::templateSize(int i) {
    return templateBank[i].size();
}

int DtwPsClassifier::size() {
    return templateBank.size();
}

double DtwPsClassifier::getDistance() {
    return distance;
}

int DtwPsClassifier::getDimensions() {
    return dimensions;
}

void DtwPsClassifier::setResolution(int r){
    resolution = r;
}

Template DtwPsClassifier::getTemplate(int i){
    return templateBank[i];
}
