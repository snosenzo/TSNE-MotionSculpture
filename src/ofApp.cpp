#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableDepthTest();
    runAgain.addListener(this, &ofApp::setupTsne);
    gui.setup("FakeData");
//    gui.add(b_useFakeData.set("fake_data", true));
    gui.add(mode.set("mode", 0, 0, 2));
    gui.add(showTrails.set("Trails", true));
    gui.add(showConnections.set("Connections", false));
    gui.add(startingPoint.set("startPt", 700, 0, 1000));
    gui.add(perplexity.set("Perplexity", 10, 5, 50));
    gui.add(theta.set("theta", .5, .01, 1));
    gui.add(scrubber.set("scrubber", 700, 0, 999));
    gui.add(threshold.set("Threshold", 100, 10, 700));
    gui.add(playMotion.set("Play Motion", true));
    gui.add(runAgain.setup("re-run"));
    
//        readInData(dataFileName, 10000, 200);
//        exit();
//        setupFakeData();
//        csvData();
        setupTsne();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // if we are running our t-SNE manually, we need to run tsne.iterate() to
        // go through each iteration and collect the points where they currently are
    if(scrubber >= 1000) {
        playMotion = false;
    }
    if(playMotion) {
        scrubber++;
    }
//    
//    if (runManually) {
//        vector<TestPoint> newHist;
//        tsnePoints = tsne.iterate();
//        iter_counter++;
//        for (int i=0; i<testPoints.size(); i++) {
//            testPoints[i].tsnePoint = ofPoint(tsnePoints[i][0], tsnePoints[i][1], tsnePoints[i][2]);
//            if(iter_counter < 1000) {
//                paths[i].curveTo(ofVec3f(testPoints[i].tsnePoint.x * ofGetHeight(), testPoints[i].tsnePoint.y * ofGetHeight(), testPoints[i].tsnePoint.z * ofGetHeight()));
//            }
//        }
//    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100);
    //    cam.setTarget(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, ofGetFrameNum()-300));
    cam.begin();
    ofPushMatrix();
    ofTranslate(-1 * ofGetHeight()/2, -1 * ofGetHeight()/2, -1 * ofGetHeight()/2);
    
    // this goes back along the path to trace the last few steps of the path
//    for(float k = .99; k <= 1.0; k+=.001){
//        for(int i = 0; i < paths.size(); i++){
//            if(showConnections) {
//                for(float j = i+1; j < paths.size()-1; j++) {
//                    ofPoint p1 = paths[i].getPointAtPercent(k);
//                    ofPoint p2 = paths[j].getPointAtPercent(k);
//                    ofSetColor(255);
//                    if(ofDist(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z) < threshold){
//                        ofDrawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
//                    }
//                }
//            }
//        }
//    }
    
    
    // Draws all of the paths for the points
    for(int i = 0; i < paths.size(); i++){
        ofSetLineWidth(1);
        ofSetColor(255);
        if(showConnections) {
            for(int j = i-1; j < paths.size()-1; j++) {
                ofPoint p1 = paths[i].getPointAtPercent(scrubber/1000.0);
                ofPoint p2 = paths[j].getPointAtPercent(scrubber/1000.0);
                ofSetColor(255);
                if(ofDist(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z) < threshold){
                    ofDrawLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
                }
            }
        }
//        ofSetColor(testPoints[i].color, 200);
        ofSetColor(testPoints[i].color);
        if(showTrails){
            paths[i].draw();
            ofDrawSphere(paths[i].getPointAtPercent(scrubber/1000.0), 8);
        }
        
    }

    ofPopMatrix();
    
    cam.end();
    if(b_showGUI) {
        ofDisableDepthTest();
        ofSetColor(255, 255, 255);
        gui.draw();
        ofEnableDepthTest();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h') {
        b_showGUI = !b_showGUI;
        if(!b_showGUI) {
            ofHideCursor();
        }
    }

}

void ofApp::setupTsne() {
    vector<vector<float>> data;
    if(mode == MODE_RAND) {
        data = fakeData(100, 100);
    } else if(mode == MODE_BYTE) {
        data = byteData(dataFileName, 10000, 200);
    } else if (mode == MODE_CSV) {
        data = csvData(csvFileName);
    }
    
    testPoints.clear();
    iter_counter = 0;
    tsnePoints.clear();
    paths.clear();
    
    for (int i = 0; i < data.size(); i++) {
        TestPoint tp;
        tp.class_ = 0;
        tp.color = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        tp.point = data[i];
        testPoints.push_back(tp);
        ofPolyline p;
        paths.push_back(p);
    }
    
    
    // ofxTSNE takes four parameters:
    //
    // dims = number of dimensions to embed our points into. we will
    //   use 2 points to visualize along (x, y), although t-SNE is commonly
    //   used for 3d as well. since t-SNE is best known for visualization,
    //   it's most commonly run with 2 or 3 dims, but more can be used.
    // perplexity = a measure of the dataset's Shannon entropy, this can be
    //   loosely interpreted as how uncertain the data is / how difficult it is
    //   to be modeled by a probability distribution. generally speaking, larger
    //   datasets need a higher perplexity, but if your points end up looking
    //   like a poorly-arranged uniformly-distributed ball, your perplexity is
    //   too high. this value is usually set between 5 and 50 with 30 being
    //   a good default. t-SNE is fairly insensitive to this parameter so
    //   in most cases the default should work fine.
    // theta / angle = this parameter controls the accuracy/speed tradeoff
    //   of the Barnes-Hut simulation used to solve the t-SNE embedding. low
    //   theta is more accurate but slower, and high theta is faster but
    //   maybe less optimal solution. it should be between 0 and 1. in practice
    //   you can usually get away with higher theta (say 0.5) without sacrificing
    //   noticeable accuracy.
    // normalize = this will automatically remap all tsne points to range {0, 1}
    //   if false, you'll get the original points.
    
    int dims = 3;
    bool normalize = true;
    
    // finally let's run ofxTSNE! this may take a while depending on your
    // data, and it will return a set of embedded points, structured as
    // a vector<vector<float> > where the inner vector contains (dims) elements.
    // We will unpack these points and assign them back to our testPoints dataset.
    
    tsnePoints = tsne.run(data, dims, perplexity, theta, normalize, runManually);
    iter_counter=0;
    for( int i = 0; i < startingPoint; i++) {
        tsne.iterate();
        iter_counter++;
        
    }
    
    for( int i = startingPoint; i <= 1000; i++) {
        vector<TestPoint> newHist;
        tsnePoints = tsne.iterate();
        iter_counter++;
        for (int i=0; i<testPoints.size(); i++) {
            testPoints[i].tsnePoint = ofPoint(tsnePoints[i][0], tsnePoints[i][1], tsnePoints[i][2]);
            if(iter_counter < 1000) {
                paths[i].curveTo(ofVec3f(testPoints[i].tsnePoint.x * ofGetHeight(), testPoints[i].tsnePoint.y * ofGetHeight(), testPoints[i].tsnePoint.z * ofGetHeight()));
            } else {
                break;
            }
        }
    }
    
    
}

// returns vector<vector<float>>
vector<vector<float>> ofApp::fakeData(int N, int D) {
    // This method was mostly taken from Gene Kogan's TSNE example
    int numClasses = 10;
    
    vector<vector<float>> classCenters;
    classCenters.resize(numClasses);
    
    for (int i = 0; i < numClasses; i++) {
        // pick a random center point for this class
        vector<float> classCenter;
        classCenter.resize(D);
        for (int j=0; j<D; j++) {
            classCenter[j] = ofRandom(1);
        }
        classCenters[i] = classCenter;
    }
    
    vector<vector<float>> data;
    for (int i = 0; i < N; i++) {
        // choose a random class
        int class_ = ofRandom(numClasses);
        vector<float> point;
        point.resize(D);
        
        // the TestPoint will be located with some (fairly large) random
        // deviation from the center point of its class
        for (int j=0; j<D; j++) {
            point[j] = classCenters[class_][j] + ofRandom(-1.5, 1.5);
        }
        data.push_back(point);
        // save the point and class information in a TestPoint
    }
    return data;
}

vector<vector<float>> ofApp::byteData(string filename, int numpts, int numdims) {
    ofFile file(filename, ofFile::ReadOnly, true);
    
    // open buffer to read file
    ofBuffer buffer(file);
    cout << "Beginning to read" << endl;
    
    
    cout << "Size of Buffer: " << buffer.size() << endl;
    int buffsize = buffer.size();
    
    vector<vector<float>> data;
    // I did this just to reduce the size of the total number of points going into the algorithm
    // Originally I think it's ~10K, which would be too slow to do in real-time so I only take a fraction
    data.resize(numpts/20);
    int ptcounter = 0;
    for(int i = 0; i < data.size()*4; i+=4*numdims) {
        // embedding vector
        vector<float> v;
        v.resize(numdims);
        for(int j = 0; j < numdims; j++) {
            // each unsigned int is 4 bytes
            // concatenates bytes
            int res = buffer.getData()[i+j];
            res = (res << 8) | buffer.getData()[i+1];
            res = (res << 8) | buffer.getData()[i+2];
            res = (res << 8) | buffer.getData()[i+3];
            float dt = ((unsigned int)(res))/4294967295.0;
            v.push_back(dt);
        }
        data.push_back(v);
    }
    return data;
}

vector<vector<float>> ofApp::csvData(string fileName) {
    ofxCsv csv;
    cout << csv.load(fileName, ",");
    vector<vector<float>> data;
    int numdims = csv.getNumCols();
    data.resize(csv.getNumRows());
    int ptcounter = 0;
    for(int i = 0; i < csv.getNumRows(); i++) {
        vector<float> v;
        v.resize(numdims);
        for(int j = 0; j < csv.getNumCols(); j++) {
            float dt = ofToFloat(csv[i][j])/10.0;
            v.push_back(dt);
        }
        data.push_back(v);
    }
    return data;
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
