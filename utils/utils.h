/* 
 * File:   utils.h
 * Author: Bas Kooiker
 *
 * Created on 17 januari 2014, 14:14
 */


#ifndef UTILS_H
#define	UTILS_H

using namespace std;

#include <list>
#include <vector>
#include <deque>
#include "utils.h"

int maxIndex(list<int> v);
int maxIndex(vector<int> v);
int maxIndex(deque<int> v);
int indexOf(int value, vector<int> list);
int indexOf(int value, list<int> l);
int maxOccurences(list<int> v);
float sum(float* array, int length);
void normalize(float* array, int length);
float average(vector<float> array, int begin, int end);
float average(float *array, int begin, int end);
vector<float> cmndf(vector<float> array, int length);
void cmndf(float* array, int length);
vector<float> takeSample(vector<float> v);

#endif	/* UTILS_H */

