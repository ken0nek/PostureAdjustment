#include "ofApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    //画面基本設定
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    //カメラを初期化
    cam.initGrabber(1024, 768);
    
    //カメラ映像のフェイストラッカーのセットアップ
    tracker.setup();
    
    debug = false;
    cPressed = false;
    shoda = false;
    
    alert.loadSound("nc105919.mp3");
    alert.setLoop(true);
    // alert.setMultiPlay(true);
    alert.setSpeed(2.0);
}

//--------------------------------------------------------------
void ofApp::update(){
    //カメラ更新
    cam.update();
    if(cam.isFrameNew()) {
        if(shoda) {
            //フェイストラッカーの更新
            imgTracker.update(toCv(faceImage));
        }
        tracker.update(toCv(cam));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //カメラ映像を描画
    ofSetColor(255);
    cam.draw(0, 0);
    //フレームレート表示
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    ofDrawBitmapString("press \"c\" to set base position", 10, 30);
    ofDrawBitmapString("press \"r\" to reset all", 10, 40);
    ofDrawBitmapString("press \"s\" to shoda mode", 10, 50);
    
    //もしカメラの映像に顔が検出されたら以下の処理をする
    if(tracker.getFound()) {
        //カメラで検出された顔の、位置、大きさ、傾きを取得
        ofVec2f position = tracker.getPosition();
        float scale = tracker.getScale();
        ofVec3f orientation = tracker.getOrientation();
        if(debug) {
            ofLog(OF_LOG_NOTICE, "face info\n\tposition: x=%.2f, y=%f\n\tscale: %.2f\n\torientation: x=%.2f, y=%.2f, z=%.2f", position.x, position.y, scale, orientation.x, orientation.y, orientation.z);
            ofLog(OF_LOG_NOTICE, "difference y : %.2f", position.y - y_base);
            ofLog(OF_LOG_NOTICE, "difference scale : %.2f", fabs(scale - scale_base));
        }
        
        if (cPressed) {
            if(position.y - y_base > 100 || fabs(scale - scale_base) > 2.0) {
                ofLog(OF_LOG_WARNING, "------bad posture!!------");
                if(!alert.getIsPlaying()) alert.play();
            } else {
                ofLog(OF_LOG_NOTICE, "------good posture!!------");
                if(alert.getIsPlaying()) alert.stop();
            }
        }
        
        if(shoda) {
            //カメラ映像からメッシュを作成
            ofMesh objectMesh = tracker.getObjectMesh();
            //合成する顔の画像からメッシュを作成
            ofMesh imgMesh = imgTracker.getObjectMesh();
            
            //静止画のメッシュの頂点情報を、カメラから生成したメッシュのものに変換
            //つまり現在の顔の表情を、静止画のメッシュに適用
            for (int i = 0; i < objectMesh.getNumVertices(); i++) {
                imgMesh.setVertex(i, objectMesh.getVertex(i));
            }
            
            //画面の3Dのパースをなしに
            // ofSetupScreenOrtho(1024, 768, OF_ORIENTATION_DEFAULT, true, -1000, 1000);
            
            //静止画のメッシュをカメラの位置、大きさ、傾きにあわせる
            ofPushMatrix();
            ofTranslate(position.x, position.y);
            ofScale(scale, scale, scale);
            ofRotateX(orientation.x * 45.0f);
            ofRotateY(orientation.y * 45.0f);
            ofRotateZ(orientation.z * 45.0f);
            
            //静止画から生成メッシュを配置して、合成する画像をマッピング
            ofSetColor(255, 255, 255);
            faceImage.getTextureReference().bind();
            imgMesh.draw();
            faceImage.getTextureReference().unbind();
            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //トラッカーのリセット
    if(key == 'r') {
        tracker.reset();
        imgTracker.reset();
        debug = false;
        cPressed = false;
        shoda = false;
    }
    if(key == 'c') {
        cPressed = true;
        y_base = tracker.getPosition().y;
        scale_base = tracker.getScale();
        ofLog(OF_LOG_NOTICE, "set   y   : %.2f", y_base);
        ofLog(OF_LOG_NOTICE, "set scale : %.2f", scale_base);
    }
    if(key == 'd') {
        debug = !debug;
    }
    if(key == 's') {
        shoda = true;
        
        //合成するイメージのメモリ領域を確保して、読込み
        faceImage.allocate(1024, 768, OF_IMAGE_COLOR);
        faceImage.loadImage("shoda.jpg");
        
        //合成する顔画像のフェイストラッカーのセットアップ
        imgTracker.setup();
    }
    
    // I want to show danger line
    ofSetColor(255, 0, 0);
    ofSetLineWidth(100);
    ofLine(0, y_base+100, 1024, y_base+100);
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
