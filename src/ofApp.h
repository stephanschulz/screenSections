#pragma once

#include "ofMain.h"

class Section
{
public:
    int myID;
    float width;
    bool active; // true if width == min_width or if Section is being expanded at this frame.
    float activeAge;
    float activeStartTime;
    
    void update(){
        
        if(active == true){
            activeAge = ofGetElapsedTimef() - activeStartTime;
            if(activeAge > 5) active = false;
        }
    }
    
    void draw(){
        
    }
    void switchActive(){
        active = !active;
        if(active == true) activeStartTime = ofGetElapsedTimef();
    }
};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<Section> sections;
    
    void expand(vector<int>expand_sections, float total_expansion_amount);
    void shrink(vector<int>shrink_sections, float total_shrink_amount);
    
    float maxWidth;
    float defaultWidth;

};
