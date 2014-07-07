using namespace std;

#include <list>
#include <vector>
#include <deque>
#include <cfloat>
#include "utils.h"

int maxIndex(list<int> v) {
    int index = -1;
    int value;

    int i = -1;
    for (list<int>::iterator it = v.begin(); it != v.end(); it++) {
        i++;
        if (i > 0) {
            if (*it > value) {
                value = *it;
                index = i;
            }
        } else
            value == *it;
    }
    return index;
}

int maxIndex(vector<int> v) {
    int index = -1;
    int value = FLT_MIN;

    for (int i = 0; i < v.size(); i++) {
        if (v[i] > value) {
            value = v[i];
            index = i;
        }
    }
    return index;
}

int maxIndex(deque<int> v) {
    int index = -1;
    int value;

    int i = -1;
    for (deque<int>::iterator it = v.begin(); it != v.end(); it++) {
        i++;
        if (i > 0) {
            if (*it > value) {
                value = *it;
                index = i;
            }
        } else {
            value == *it;
            index = 0;
        }
    }
    return index;
}

int indexOf(int value, vector<int> list) {
    int i = -1;
    for (vector<int>::iterator it = list.begin(); it != list.end(); it++) {
        i++;
        if (*it == value)
            return i;
    }
    return -1;
}

int indexOf(int value, list<int> l) {
    int i = -1;
    for (list<int>::iterator iter = l.begin(); iter != l.end(); iter++) {
        i++;
        if (*iter == value)
            return i;
    }
    return -1;
}

int maxOccurences(list<int> v) {
    vector<int> elements;
    vector<int> occurences;
    //    for(int i = 0; i < v.size(); i++){
    for (list<int>::iterator it = v.begin(); it != v.end(); it++) {

        int index = indexOf(*it, elements);
        if (index > -1)
            occurences[index]++;
        else {
            elements.push_back(*it);
            occurences.push_back(1);
        }
    }
    return elements[maxIndex(occurences)];
}

float sum(float* array, int length) {
    float sum = 0;
    int i;
    for (i = 0; i < length; i++) {
        sum += array[i];
    }
    return sum;
}

void normalize(float* array, int length) {
    int i;
    float s = sum(array, length);
    for (i = 0; i < length; i++) {
        array[i] = array[i] / s;
    }
}

float average(vector<float> array, int begin, int end) {
    float sum = 0.;
    for (int i = begin; i <= end; i++) {
        sum += array[i];
    }
    return sum / (float) (end - begin + 1);
}

float average(float *array, int begin, int end) {
    float sum = 0.;
    for (int i = begin; i <= end; i++) {
        sum += array[i];
    }
    return sum / (float) (end - begin + 1);
}

vector<float> cmndf(vector<float> array, int length) {
    std::vector<float> cmndf;
    cmndf.push_back(1.f);
    for (int i = 1; i < length; i++)
        cmndf.push_back(array[i] / average(cmndf, 0, i - 1));
    
    return cmndf;
}

void cmndf(float* array, int length) {
    float cmndf[length];
    cmndf[0] = 1.f;
    for (int i = 1; i < length; i++) {
        cmndf[i] = array[i] / average(cmndf, 0, i - 1);
    }
    for (int i = 0; i < length; i++)
        array[i] = cmndf[i];
    //        array[0] = 0.0f;
}

vector<float> takeSample(vector<float> v) {
    vector<float> new_sam;
    for (int k = 0; k < 3; k++) {
        new_sam.push_back(v[k + 1] * 50.0f - 25.0f);
    }
    return new_sam;
}
