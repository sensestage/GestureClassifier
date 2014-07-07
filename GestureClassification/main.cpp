/* 
 * File:   main.cpp
 * Author: Bas
 *
 * Created on 17 juni 2014, 18:57
 */

#include <cstdlib>
#include "DtwPsClassifier.h"
#include <string>
#include "vectordtw.h"
#include "RepClassifier.h"
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream> 

using namespace std;

std::map<int, std::vector<std::vector<Point> > >* createAnotatedTemplates(string filename);
std::vector <std::vector <float> > loadData2(string filename, float multiplier = 50, float alpha = 1.f);

/*
 * 
 */
int main(int argc, char** argv) {

    string filename = "../SuperSense/data/maki_0.dat";

    std::vector<std::vector<float> > data = loadData2(filename);

    std::vector<std::vector<float> > vData;
    
    for (int i = 0; i < data.size(); i++) {
        vector<float> v;
        for (int j = 0; j < 3; j++)
            v.push_back(data[i][j]); 
        vData.push_back(v);
    }
    RepClassifier rep = RepClassifier();
    
    // train classifier
    int prevG = -1;
    for(int i = 0; i < vData.size(); i++){
        if(prevG != (int)data[i][3]){
            rep.learn();
            prevG = (int)data[i][3];
        }

        rep.infer(vData[i]);

        printf("%d of %d: #gests = %d, dist = %.3f, GestNr = %.0f, YIN says %s\n", i, data.size(), rep.size(), rep.getDistance(), data[i][3], rep.isSync() ? "sync!" : "no...");
    }
    
    // show gesture set sizes
    for(int i = 0; i < rep.size(); i++){
        printf("size of gest %d = %d\n", i, rep.templateSize(i));
    }
    
//    // evaluate
//    for(int i = 0; i < vData.size(); i++){
//        rep.infer(vData[i]);
//        printf("%d\n",rep.mostLikeliGesture());
//    }
    
    return 0;
}

void testDtwPsClassifier() {

    string filename = "../SuperSense/data/maki_0.dat";

    map<int, vector<vector<Point> > >* templates = createAnotatedTemplates(filename);

    printf("datasize %d\n", templates->size());
    DtwPsClassifier dtw;
    int i = 0;

    for (std::map<int, std::vector<std::vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
        dtw.addTemplate();
        for (int j = 0; j < it->second[0].size(); j++) {
            vector<float> sample;
            sample.push_back(it->second[0][j].x);
            sample.push_back(it->second[0][j].y);
            sample.push_back(it->second[0][j].z);
            dtw.fillTemplate(i, sample);
        }

        i++;
    }

    for (map<int, vector<vector<Point> > >::iterator it = templates->begin(); it != templates->end(); it++) {
        for (i = 0; i < it->second.size(); i++) {
            for (int j = 0; j < it->second[i].size(); j++) {
                vector<float> sample;
                sample.push_back(it->second[0][j].x);
                sample.push_back(it->second[0][j].y);
                sample.push_back(it->second[0][j].z);
                dtw.infer(sample);
                //                printf("%d %f\n", dtw.mostLikeliGesture(), dtw.getPhase());
            }
        }
    }

}

std::vector <std::vector <float> > loadData2(string filename, float multiplier, float alpha) {
    std::vector <std::vector <float> > data;
    ifstream infile(filename.c_str());

    while (infile) {
        string s;
        if (!getline(infile, s))
            break;

        istringstream ss(s);
        vector <float> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ','))
                break;
            record.push_back((float) atof(s.c_str()));
        }
        data.push_back(record);
    }


    infile.close();

    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < 3; j++) {
            data[i][j] = data[i][j] * multiplier - (multiplier / 2.);
            //            printf("%f ", data[i][j]);
        }
        //        printf("\n");
    }
    return data;
}

std::map<int, std::vector<std::vector<Point> > >* createAnotatedTemplates(string filename) {

    std::map<int, std::vector<std::vector<Point> > >* trials;
    trials = new std::map<int, std::vector<std::vector<Point> > >();

    std::vector<std::vector<float> > data = loadData2(filename);

    bool inTrial = false;
    int trialNr = -1;

    // push samples in trials in gestures in gesture set
    for (int sampleNr = 0; sampleNr < data.size(); sampleNr++) {
        vector<float> sample = data[sampleNr];
        bool newTrial = (int) data[sampleNr][4] == 1;
        int sampleGest = (int) sample[3];
        Point p = Point(sample[0], sample[1], sample[2]);
        if (inTrial) {
            if (newTrial) {
                //                printf("newtrial %d\n",sampleGest);
                if (++trialNr < 7) {
                    trials->at(sampleGest).push_back(vector<Point>());
                    trials->at(sampleGest)[trials->at(sampleGest).size() - 1].push_back(p);
                } else {
                    inTrial = false;
                }
            } else {
                trials->at(sampleGest)[trials->at(sampleGest).size() - 1].push_back(p);
            }
        } else {
            if (newTrial) {
                trialNr = 0;
                inTrial = true;
                trials->insert(std::make_pair(sampleGest, vector<vector<Point> >()));
                trials->at(sampleGest).push_back(vector<Point>());
                trials->at(sampleGest)[trials->at(sampleGest).size() - 1].push_back(p);
            }
        }
    }
    return trials;
}