/* 
 * File:   HistogramBuffer.h
 * Author: Bas
 *
 * Created on 20 februari 2014, 16:34
 */

#include <deque>

using namespace std;

#ifndef HISTOGRAMBUFFER_H
#define	HISTOGRAMBUFFER_H

class HistogramBuffer {
public:
    HistogramBuffer();
    HistogramBuffer(int i);
    HistogramBuffer(const HistogramBuffer& orig);
    virtual ~HistogramBuffer();
    
    void put(int i);
    int putAndGet(int i);
    int get();
    void clear();
    void drop();
    
    void setMaxSize(int i);
    int getMaxSize();
private:
    int maxSize;
    deque<int> buffer;

};

#endif	/* HISTOGRAMBUFFER_H */

