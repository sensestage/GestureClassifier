/* 
 * File:   AlphaFilter.h
 * Author: Bas
 *
 * Created on 14 maart 2014, 18:23
 */

#ifndef ALPHAFILTER_H
#define	ALPHAFILTER_H

class AlphaFilter {
public:
    AlphaFilter();
    AlphaFilter(float f);
    AlphaFilter(const AlphaFilter& orig);
    virtual ~AlphaFilter();
    
    void setAlpha(float f);
    float value();
    float value(float f);
    
protected:
    float alpha;
    float val;
};

#endif	/* ALPHAFILTER_H */

