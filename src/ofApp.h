#pragma once
#include <iostream>
#include "ofMain.h"
#include "ofxTSNE.h"
#include "ofxGui.h"
#include "ofxCsv.h"

#define MODE_RAND 0
#define MODE_BYTE 1
#define MODE_CSV 2

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void setupFakeData();
        void readInData(string, int, int);
//        void csvData();
        void setupTsne();
        vector<vector<float>> fakeData(int N, int D);
        vector<vector<float>> byteData(string, int numpts, int numdims);
        vector<vector<float>> csvData(string);
    
    
    struct TestPoint {
        int class_;
        ofColor color;
        vector<float> point;
        ofPoint tsnePoint;
    };
    ofxTSNE tsne;
    ofEasyCam cam;
    vector<TestPoint> testPoints;
    vector<vector<double> > tsnePoints;
    vector<ofPolyline> paths;

    ofParameter<int> mode;
    ofParameter<float> perplexity;
    ofParameter<float> theta;
    ofParameter<bool> b_useDiffColors;
    ofParameter<int> startingPoint;
    ofParameter<bool> showTrails;
    ofParameter<bool> showConnections;
    ofParameter<int> scrubber;
    ofParameter<bool> playMotion;
    int iter_counter = 0;
    ofxPanel gui;
    ofxButton runAgain;
    string csvFileName = "samp_embed.csv";
    bool runManually = true;
    bool b_showGUI = true;
    bool b_useFakeData = true;
    string dataFileName = "word2vec_10000_200d_tensors.bytes";
    ofParameter<float> threshold;
    
};
