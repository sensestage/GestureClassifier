/* 
 * File:   AlphaFilter.cpp
 * Author: Bas
 * 
 * Created on 14 maart 2014, 18:23
 */

#include "AlphaFilter.h"

void AlphaFilter::setAlpha(float f) {
    alpha = f > 1. ? 1. : f < 0. ? 0 : f;
}

AlphaFilter::AlphaFilter() {
    alpha = .5;
    val = 0.;
}

AlphaFilter::AlphaFilter(float f) {
    alpha = f;
    val = 0.;
}

AlphaFilter::AlphaFilter(const AlphaFilter& orig) {
    alpha = orig.alpha;
    val = orig.val;
}

AlphaFilter::~AlphaFilter() {
}


float AlphaFilter::value(){
    return val;
}

float AlphaFilter::value(float f){
    val = (f - val) * alpha + val;
    return val;
}