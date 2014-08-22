/*
 * File:   Template.cpp
 * Author: Bas
 *
 * Created on 26 juni 2014, 17:21
 */

#include <float.h>

#include "Template.h"
#include "vectordtw.h"

Template::Template() {
    tempPhase = 0.;
    templ = vector<Point>();
}

Template::~Template() {
    templ.clear();
}

Template::Template(const Template& orig) {
    tempPhase = orig.tempPhase;
    templ = orig.templ;
    dtw = orig.dtw;
}

double Template::distance(std::deque<Point>& buffer) {
    vector<Point> currentTrial = lastTrial(buffer, templ.size());

    double bestR = DBL_MAX;
    double bestVal = DBL_MAX;

    for (float rotation = 0.; rotation < .975; rotation += .05) {
        vector<Point> rotatedTemplate = rotate(templ, rotation);
        double d = dtw->fastdynamic(rotatedTemplate, currentTrial);
        if (d < bestVal) {
            bestVal = d;
            bestR = rotation;
        }
        rotatedTemplate.clear();
    }
    currentTrial.clear();
    tempPhase = bestR;
    return bestVal;
}

double Template::getPhase() {
    return tempPhase;
}

void Template::add(Point p) {
    templ.push_back(p);
    dtw = new VectorDTW(templ.size(),10.);
}

void Template::clear() {
    templ.clear();
}

int Template::size() {
    return templ.size();
}

std::vector<Point> Template::lastTrial(std::deque<Point>& buf, int length) {
    std::vector<Point> trial;
    for (int i = length; i > 0; i--)
        trial.push_back(buf[buf.size() - i]);
    return trial;
}

vector<Point> Template::rotate(std::vector<Point>& trial, float rotation) {
    vector<Point> newTrial;
    int rotationInt = (int) ((float) trial.size() * rotation);
    //        printf("rotation int = %d\n", rotationInt);
    for (int i = 0; i < trial.size(); i++) {
        newTrial.push_back(trial[(i + rotationInt) % trial.size()]);
    }
    return newTrial;
}
