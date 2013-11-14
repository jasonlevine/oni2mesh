#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setupFromONI("claudia chill.oni");;
//    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);
    openNIDevice.start();
    
    // set properties for all user masks and point clouds
    openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    
    openNIDevice.setDepthColoring(COLORING_GREY);
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel
    
// you can alternatively create a 'base' user class
//    ofxOpenNIUser user;
//    user.setUseMaskTexture(true);
//    user.setUsePointCloud(true);
//    user.setPointCloudDrawSize(2);
//    user.setPointCloudResolution(2);
//    openNIDevice.setBaseUserClass(user);
      
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);

    cam.lookAt(ofVec3f(320, 240, 2000));
    cam.setScale(1, -1, 1);
    
    shader.load("shadersGL2/shader");
    img.loadImage("checkerboard.jpeg");//checkerboard.jpeg//    waves69.jpg

}

//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    
    if (ofGetFrameNum() % 30 == 0) shader.load("shadersGL2/shader");
}

//--------------------------------------------------------------
void testApp::draw(){
	
    ofSetColor(255, 255, 255);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
    cam.begin();
//    ofDrawAxis(100);
    
    ofImage depthImage, maskImage;
    
    int numUsers = openNIDevice.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++){
        
        
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
        
        ofPixels depthPix = openNIDevice.getDepthPixels();
        depthImage = depthPix;
        maskImage = user.getMaskPixels();
       
        
        ofMesh depthMesh;
        int width = openNIDevice.getWidth();
        int height = openNIDevice.getHeight();
        int spacer = 1;
        for (float y = 0; y < height; y+=spacer) {//+=10
            for (float x = 0; x < width; x+=spacer) {
                float lightness = depthPix.getColor(x, y).getLightness();
                float z = lightness == 0 ? 255 : lightness;
                depthMesh.addVertex(ofVec3f(x, y, z));//depthPix.getColor(x,y).r * 10
                depthMesh.addTexCoord(ofVec2f(x, y));
            }
        }
        
        
        for (int y = 0; y < height - spacer; y+=spacer){
            for (int x = 0; x < width - spacer; x+=spacer){
                depthMesh.addIndex(x+y*width);       // 0
                depthMesh.addIndex((x+spacer)+y*width);     // 1
                depthMesh.addIndex(x+(y+1)*width);     // 10
                
                depthMesh.addIndex((x+spacer)+y*width);     // 1
                depthMesh.addIndex((x+spacer)+(y+1)*width);   // 11
                depthMesh.addIndex(x+(y+1)*width);     // 10
            }
        }
        
        ofPushMatrix();
        ofTranslate(-320, -240, 0);
        shader.begin();
        shader.setUniformTexture("tex0", img, 0);
        shader.setUniformTexture("tex1", maskImage, 1);
        depthMesh.draw();
        shader.end();
        ofPopMatrix();
        
    }
    cam.end();
    ofDisableBlendMode();
//    ofPopMatrix();
    
    depthImage.draw(0,0,160, 120);
    maskImage.draw(160, 0, 160, 120);
    img.draw(320, 0, 160, 120);
    
    // draw some info regarding frame counts etc
	ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());
    
//	verdana.drawString(msg, 20, openNIDevice.getNumDevices() * 480 - 20);
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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