#pragma once

#include "ofMain.h"
#include "ofxUDPManager.h"
#include "ofxXmlSettings.h"
#include <vector>
#include <map>
#include <istream.h>

using namespace std;


class testApp : public ofBaseApp{
    
    vector <ofVideoPlayer>movies;
    map <string,int>events;
    map <string,int>keys;
    
    int plvid, baselineMovie;
    ofxUDPManager udp;
    ofxXmlSettings xml;
    float currentTime;
    
public:
    
    void setup();
	void update();
	void draw();
    void changeVid(int vid);
    
    void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
};

