/* 
 * File:   Template.h
 * Author: Bas
 *
 * Created on 26 juni 2014, 17:21
 */

#ifndef TEMPLATE_H
#define	TEMPLATE_H

#include <vector>
#include "vectordtw.h"
#include <deque>

class Template {
private:
    VectorDTW* dtw;
    std::vector<Point> templ;
    double tempPhase;

public:
    Template();
    Template(const Template& orig);
    virtual ~Template();
    
    double distance(std::deque<Point>& buffer);
    double getPhase();
    void add(Point p);
    void clear();
    int size();
    
private:
    std::vector<Point> lastTrial(std::deque<Point>& buf, int length);
    std::vector<Point> rotate(std::vector<Point>& trial, float rotation);
};

#endif	/* TEMPLATE_H */

