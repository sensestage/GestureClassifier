/* 
 * File:   AbstractGestureClassifier.h
 * Author: Bas
 *
 * Created on 17 juni 2014, 18:57
 */

#ifndef ABSTRACTGESTURECLASSIFIER_H
#define	ABSTRACTGESTURECLASSIFIER_H

#include <vector>

class AbstractGestureClassifier {
public:
    void infer(std::vector<float>);
    void addTemplate();
    void fillTemplate(int, std::vector<float>);
    void clearTemplate(int);
    void clear();
    int mostLikeliGesture();
    
protected:
    
    
};

#endif	/* ABSTRACTGESTURECLASSIFIER_H */

