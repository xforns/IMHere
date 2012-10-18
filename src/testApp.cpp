#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
    
    baselineMovie = -1;
    
    // XML settings
    if(!xml.loadFile("settings.xml")) {
        cerr << "Couldn't read 'settings.xml' file. Exiting.." << endl;
        exit();
    }
    
    int port = xml.getValue("port",10552);
    
    if(xml.getNumTags("movies")==0) {
        cerr << "No movies defined in 'settings.xml'. Exiting.." << endl;
        exit();
    }
    
    xml.pushTag("movies");
    
    int cmovies = xml.getNumTags("movie");
    if(cmovies==0) {
        cerr << "No movies defined in 'settings.xml'. Exiting.." << endl;
        exit();
    }
    
    cout << "Loading movies" << endl;
    for(int i=0;i<cmovies;i++) {
        
        string name = xml.getValue("movie:name","",i);
        string event = xml.getValue("movie:event","",i);
        if( (name.compare("")==0) || (event.compare("")==0) )
            continue;
        
        // Load movie
        ofVideoPlayer foo;
        if(!foo.loadMovie(name)) {
            cout << "Could not load movie with name: " << name << endl;
            continue;
        }
        movies.push_back(foo);
        
        // UDP message that fires the movie
        events.insert(pair<string,int>(event,i));
        
        // Associate a key event for this movie
        string key = xml.getValue("movie:key","",i);
        if(key.compare("")!=0)
            keys.insert(pair<string,int>(key,i));
        
        // Check which one is the baseline movie
        int isbaseline = xml.getValue("movie:baseline",0,i);
        if(isbaseline)
            baselineMovie = i;
    }
    
    if(baselineMovie==-1)
        baselineMovie = 0;
    plvid = baselineMovie;
    
    // Create udp connection
    udp.Create();
    udp.SetNonBlocking(true);
    udp.Bind(port);
    
    // Start playing the baseline movie
    movies[plvid].play();
    currentTime = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void testApp::update(){
    
    // UDP messages
    char msg[100000];
	udp.Receive(msg,100000);
	string message = msg;
    if(message.length()>0)
        cout << "Received UDP message: " << message << endl;
    
    if(events.find(message)!=events.end()) {
        plvid = events[message];
        changeVid(plvid);
    }
    
    // Current video play
    movies[plvid].idleMovie();
    if(ofGetElapsedTimef()-currentTime>=movies[plvid].getDuration()) {
        if(plvid!=baselineMovie)
            plvid = baselineMovie;
        changeVid(plvid);
    }
}


//--------------------------------------------------------------
void testApp::draw(){
    
	ofSetHexColor(0xFFFFFF);
    
    movies[plvid].draw(0,0,ofGetWidth(),ofGetHeight());
}


void testApp::changeVid(int vid) {
    
    // Stop all movies (just in case..)
    for(int i=0;i<movies.size();i++)
        movies[i].stop();
    
    currentTime = ofGetElapsedTimef();
    
    movies[plvid].firstFrame();
    movies[plvid].play();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    char ckey = (char)key;
    stringstream ss;
    ss << ckey;
    if(keys.find(ss.str())!=keys.end()) {
        plvid = keys[ss.str()];
        changeVid(plvid);
    }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
