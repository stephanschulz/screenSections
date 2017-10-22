#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofSetBackgroundColor(200);
    //    Section[] screen = new Section[N];
    sections.resize(20);
    
    float screen_w = ofGetWidth()-2;
    defaultWidth = screen_w/float(sections.size());
    ofLog()<<"defaultWidth "<<defaultWidth;
    // Sections from x value 0 to screen_w, where section[i] spans section[i - 1].x to section[i - 1].x + section[i].width and section[0].x = 0.
    // section[N - 1] spans section[N-2].x + section[N-2].wdith to screen_width.
    
    for(int i = 0; i < sections.size(); i++){
        sections[i].myID = i;
        sections[i].width = defaultWidth;
        sections[i].active = false;
    }
    
    maxWidth = 200;
}



//--------------------------------------------------------------
void ofApp::update(){
    
    float expandStep = 1;
    vector<int>expand_sections;
    vector<int>shrink_sections;
    for(int i = 0; i < sections.size(); i++){
        sections[i].update();
        if(sections[i].active == true){
            if(sections[i].width <= maxWidth) expand_sections.push_back(i);
        } else {
             if(sections[i].width > defaultWidth) shrink_sections.push_back(i);
        }
    }
    
    float expandAmount = expand_sections.size()*expandStep;
    expand(expand_sections, expandAmount);
    expandAmount = shrink_sections.size()*expandStep;
    shrink(shrink_sections, expandAmount);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float x = 1;
    
    ofNoFill();
    ofSetColor(0);
    for(int i = 0; i < sections.size(); i++)
    {
        float x2 = x + sections[i].width;
        
        ofFill();
        if(sections[i].active) ofSetColor(255,0,0);
        else ofSetColor(200);
        ofDrawRectangle(ofPoint(x,0), sections[i].width,300);
        
        ofNoFill();
        ofSetColor(255);
        ofDrawRectangle(ofPoint(x,0), sections[i].width,300);
        
        ofDrawBitmapString(ofToString(sections[i].myID), x+sections[i].width/2, 150);
        x = x2;
    }
    
    //    drawImage(x, screen_width);
    ofSetColor(255);
    ofDrawBitmapString(ofToString("press 1-9 keys to expand those sections"), 10, ofGetHeight() - 100);
    ofDrawBitmapString(ofToString("red == active age < 5 secs "), 10, ofGetHeight() - 80);

    
}

void ofApp::shrink(vector<int>shrink_sections, float total_shrink_amount)
{
    if(shrink_sections.size() <= 0) return;
    ofLog()<<"---------shrink.  shrink_sections.size() "<<shrink_sections.size()<<" total_shrink_amount "<<total_shrink_amount;
    
     for(int i = 0; i < shrink_sections.size(); i++){
         ofLog()<<"shrink_sections[i] "<<shrink_sections[i]<<" width "<<sections[shrink_sections[i]].width;
     }
    // Uniformly shrink areas to accommodate the growth.
    bool changed = true;
    float amount_unshrinked = total_shrink_amount;
    float epsilon = .1;
//    float minimum_width = 2;
    
    while(amount_unshrinked >= epsilon && changed == true) //epsilon == around 2.
    {
        changed = false;
        
        //--calculate the width of all sections that are not active
        float total_available_width = 0;
        for(int i = 0; i < sections.size(); i++)
        {
            if ( sections[i].active == false && sections[i].width < defaultWidth){
                total_available_width += sections[i].width;
            }
        }
        ofLog()<<"total_available_width "<<total_available_width<<" - total_shrink_amount "<<total_shrink_amount;

        
        float desired_width = total_available_width + total_shrink_amount;
        ofLog()<<"desired_width "<<desired_width;
        
        float percentage_change = desired_width/float(total_available_width);
        ofLog()<<"percentage_change "<<percentage_change;

        for(int i = 0; i < sections.size(); i++)
        {
            if(sections[i].active == false && sections[i].width < defaultWidth){
                //                ofLog()<<i<<" sections[i].width "<<sections[i].width;
                float original_width = sections[i].width;
                float new_width = sections[i].width*percentage_change;
                
                new_width = MAX(new_width, defaultWidth);
                
                sections[i].width = new_width;
                amount_unshrinked -= original_width - new_width;
                changed = true;
            }
        }
        
    }//end while
    
    ofLog()<<"amount_unshrinked "<<amount_unshrinked;
    
//    total_shrink_amount -= amount_unshrinked;
    
    ofLog()<<"total_shrink_amount "<<total_shrink_amount;
    
    float shrink_per_section = total_shrink_amount / shrink_sections.size();
    
    ofLog()<<"shrink_per_section "<<shrink_per_section;
    
    for(int i = 0; i < shrink_sections.size(); i++)
    {
        sections[shrink_sections[i]].width -= shrink_per_section;
    }
}

// Indices of sections to expand, section structures.
// all screen.active should = true only if they are at the minimum width.
//void ofApp::expand(vector<int>expand_sections, vector<Section> screen, int total_expansion_amount)
void ofApp::expand(vector<int>expand_sections, float total_expansion_amount)
{
    if(expand_sections.size() <= 0) return;
    ofLog()<<"expand.  expand_sections.size() "<<expand_sections.size();
    
    
    // Uniformly shrink areas to accommodate the growth.
    bool changed = true;
    float amount_unexpanded = total_expansion_amount;
    float epsilon = .1;
    float minimum_width = 2;
    
    //shrink each non-active section often enough until we made enough space for the active section to take up that empty space
    while(amount_unexpanded >= epsilon && changed == true) //epsilon == around 2.
    {
        changed = false;
        
        //--calculate the width of all sections that are not active
        float total_available_width = 0;
        for(int i = 0; i < sections.size(); i++)
        {
            if ( sections[i].active == false && sections[i].width > minimum_width){
                total_available_width += sections[i].width;
            }
        }
        ofLog()<<"total_available_width "<<total_available_width<<" - total_expansion_amount "<<total_expansion_amount;
        
        //--the width that all non-active sections will have to occupy
        float desired_width = total_available_width - total_expansion_amount;
        ofLog()<<"desired_width "<<desired_width;
        
        //--the percentage each of the non-active sections will have to change to make space
        float percentage_change = desired_width/float(total_available_width);
        ofLog()<<"percentage_change "<<percentage_change;
        
        //shrink all non-active sections if they are not too small
        for(int i = 0; i < sections.size(); i++)
        {
            if(sections[i].active == false && sections[i].width > minimum_width){
                //                ofLog()<<i<<" sections[i].width "<<sections[i].width;
                float original_width = sections[i].width;
                float new_width = sections[i].width*percentage_change; //will be smaller than before
                
                new_width = MAX(new_width, minimum_width);
                
                sections[i].width = new_width;
                amount_unexpanded -= original_width - new_width;
                changed = true;
            }
        }
    }//end while
    
    ofLog()<<"amount_unexpanded "<<amount_unexpanded;
    total_expansion_amount -= amount_unexpanded; //incase we coould not fully shrink other sections 
    
    ofLog()<<"total_expansion_amount "<<total_expansion_amount;
    
    float expand_per_section = total_expansion_amount / expand_sections.size();
    
    ofLog()<<"expand_per_section "<<expand_per_section;
    
    for(int i = 0; i < expand_sections.size(); i++)
    {
        sections[expand_sections[i]].width += expand_per_section;
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key >='1' && key <= '9'){
        int id = key - '1';
        sections[id].switchActive();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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
