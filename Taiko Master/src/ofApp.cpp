#include "ofApp.h"
#include <vector>
#include <iostream>

using namespace taikomaster;

//--------------------------------------------------------------
void taikoGame::setup(){
    ofSetWindowTitle("Taiko drum master");
    ofSetCircleResolution(60);
    ofBackground(255,255,255);
    ofSetFrameRate(120);
    score = 0;
    player.load("song1.mp3");
    myFont.load("mikiyu-mokomori-kuro.ttf", 60);
    timer_crr = ofGetElapsedTimef();
    
    sampleRate = 44100;
    bufferSize = 512;
    int channels = 2;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);

}

//--------------------------------------------------------------
void taikoGame::update(){
    
    if (current_state_ == IN_MENU) {
        score = 0;
        if (song_state_ == SONG_1) {
            player.load("song1.mp3");
        }
        if (song_state_ == SONG_2) {
            player.load("song2.mp3");
        }
        if (song_state_ == SONG_3) {
            player.load("song3.mp3");
        }
    }
    
    if (player.getPosition() >= 1.0 && current_state_ == IN_PROGRESS) {
        current_state_ = END;
        isPlaying = false;
        notes.clear();
    }
    
    if (current_state_ != IN_MENU && current_state_ != END) {
        if (!isPlaying) {
            player.play();
            isPlaying = true;
        }
    for (int i = 0; i < notes.size(); i++) {
        notes[i].dist_x_ -= 10;
    }
    
    soundBuffer = player.getCurrentSoundBuffer(bufferSize);
    
    audioAnalyzer.analyze(soundBuffer);
    
    onset = audioAnalyzer.getOnsetValue(0);
    
    strongDecay = audioAnalyzer.getValue(STRONG_DECAY, 0, 0);
    
    pitchSalience  = audioAnalyzer.getValue(PITCH_SALIENCE, 0, 0);
    
    strongPeak = audioAnalyzer.getValue(STRONG_PEAK, 0, 0);
    
    pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, 0, 0);
    
    }
}

//--------------------------------------------------------------
void taikoGame::draw(){
    
    if (current_state_ == IN_MENU) {
        ofSetColor(255,130,10);
        menuFont.load("mikiyu-mokomori-kuro.ttf", 80);
        menuFont.drawString("Drum Master", 0.17 * ofGetWindowWidth(), 0.3 * ofGetWindowHeight());
        
        ofSetColor(255,130,10);
        menuFont.load("mikiyu-mokomori-kuro.ttf", 35);
        menuFont.drawString("choose a song to play", 0.23 * ofGetWindowWidth(), 0.50 * ofGetWindowHeight());
        
        ofSetColor(255,255,255);
        left.load("left.jpeg");
        left.resize(60, 70);
        left.draw(0.13 * ofGetWindowWidth(), 0.65 * ofGetWindowHeight());
        
        ofSetColor(255,255,255);
        right.load("right.jpeg");
        right.resize(60, 70);
        right.draw(0.87 * ofGetWindowWidth(), 0.65 * ofGetWindowHeight());
        
        if (song_state_ == SONG_1) {
            song_1.load("song1.jpeg");
            song_1.resize(250, 250);
            song_1.draw(0.4 * ofGetWindowWidth(), 0.55 * ofGetWindowHeight());
        }
        
        if (song_state_ == SONG_2) {
            song_2.load("song2.jpeg");
            song_2.resize(250, 250);
            song_2.draw(0.4 * ofGetWindowWidth(), 0.55 * ofGetWindowHeight());
        }
        
        if (song_state_ == SONG_3) {
            song_3.load("song3.jpeg");
            song_3.resize(250, 250);
            song_3.draw(0.4 * ofGetWindowWidth(), 0.55 * ofGetWindowHeight());
        }
        
    }
    
    if (current_state_ == END) {
        ofSetColor(255,130,10);
        endFont.load("mikiyu-mokomori-kuro.ttf", 80);
        endFont.drawString("Game Over", 0.17 * ofGetWindowWidth(), 0.3 * ofGetWindowHeight());
        
        ofSetColor(51,255,255);
        endFont.load("mikiyu-mokomori-kuro.ttf", 35);
        endFont.drawString("Your Score is " + to_string(score), 0.23 * ofGetWindowWidth(), 0.50 * ofGetWindowHeight());
        
        ofSetColor(255,130,10);
        endFont.load("mikiyu-mokomori-kuro.ttf", 35);
        endFont.drawString("Press 'r' to get back to menu", 0.13 * ofGetWindowWidth(), 0.65 * ofGetWindowHeight());
    }
    
    if (current_state_ != IN_MENU && current_state_ != END) {
    myFont.drawString(to_string(score), 650, 200);
    myFont.drawString("score:", 350, 200);
    float timer = ofGetElapsedTimef();
        
    if (onset) {
        if (strongDecay >= 75 || strongPeak >= 1) {
            Note note = *new Note();
            note.time_out_ = timer;
            note.color_ = true;
            notes.push_back(note);
        } else if (pitchSalience >= 0.85 && pitchFreq >= 700) {
            Note note = *new Note();
            note.time_out_ = timer;
            note.color_ = false;
            notes.push_back(note);
        }
    }
    
    for (int i = 0; i < notes.size(); i++) {
        if (notes[i].drum_state_ == NOT_PRESSED && timer >= notes[i].time_out_) {
            if (notes[i].color_ == true) {
                drawRedDot(notes[i].dist_x_);
            } else {
                drawBlueDot(notes[i].dist_x_);
            }
        }
    }
    
    //the drum head
    ofSetHexColor(0xCCCCCC);
    ofNoFill();
    ofDrawCircle(100,400,50);
    
    //an outline
    ofNoFill();
    ofSetHexColor(0xCCCCCC);
    ofDrawCircle(100,400,80);
    
    if (current_state_ == PLAY_RED) {
        drawRedDrum();
    } else if (current_state_ == PLAY_BLUE) {
        drawBlueDrum();
    }
    
    for (int i = 0; i < notes.size(); i++) {
        if (notes[i].dist_x_ < 0 && notes[i].dist_x_ > -100 && notes[i].drum_state_ == NOT_PRESSED) {
            drawOHHH();
        }
    }
    }
}

//--------------------------------------------------------------
void taikoGame::keyPressed(int key){
    int upper_key = toupper(key); // Standardize on upper case
    
    if (upper_key == OF_KEY_LEFT && current_state_ == IN_MENU) {
        if (song_state_ == SONG_2) {
            song_state_ = SONG_1;
            player.load("song1.mp3");
        } else if (song_state_ == SONG_3) {
            song_state_ = SONG_2;
            player.load("song2.mp3");
        }
    }
    
    if (upper_key == OF_KEY_RIGHT && current_state_ == IN_MENU) {
        if (song_state_ == SONG_1) {
            song_state_ = SONG_2;
            player.load("song2.mp3");
        } else if (song_state_ == SONG_2) {
            song_state_ = SONG_3;
            player.load("song3.mp3");
        }
    }
    
    if (current_state_ == IN_MENU && upper_key == OF_KEY_RETURN) {
        current_state_ = IN_PROGRESS;
    }
    
    if (upper_key == 'R' && current_state_ == END) {
        current_state_ = IN_MENU;
    }
    

    
    if (upper_key == ' ' && current_state_ != FINISHED) {
        mySound.load("Drums_Drum_Short_Shot_04.wav");
        mySound.play();
        current_state_ = PLAY_RED;
        for (int i = 0; i < notes.size(); i++) {
            if (notes[i].dist_x_ >= 80 && notes[i].dist_x_ <= 120 &&
                notes[i].color_ == true) {
                notes[i].drum_state_ = PRESSED;
                font_state_ = GOOD;
                score += 640;
            } else if (notes[i].dist_x_ >= 50 && notes[i].dist_x_ <= 150 &&
                       notes[i].color_ == true) {
                notes[i].drum_state_ = PRESSED;
                font_state_ = OKAY;
                score += 320;
            }
        }
    } else if (upper_key == 'I' && current_state_ != FINISHED) {
        mySound.load("Drums_Drum_Short_Shot_02.wav");
        mySound.play();
        current_state_ = PLAY_BLUE;
        for (int i = 0; i < notes.size(); i++) {
            if (notes[i].dist_x_ >= 80 && notes[i].dist_x_ <= 120 &&
                notes[i].color_ == false) {
                notes[i].drum_state_ = PRESSED;
                font_state_ = GOOD;
                score += 640;
            } else if (notes[i].dist_x_ >= 50 && notes[i].dist_x_ <= 150 &&
                       notes[i].color_ == false) {
                notes[i].drum_state_ = PRESSED;
                font_state_ = OKAY;
                score += 320;
            }
            
        }
    }
}

void taikoGame::drawRedDrum() {
    ofSetColor(255,130,10,100);
    ofFill();        // draw "filled shapes"
    ofDrawCircle(100,400,50);
    if (font_state_ == NOTHING) {
        // do nothing
    } else if (font_state_ == OKAY) {
        drawOkay();
    } else if (font_state_ == GOOD) {
        drawGood();
    }
}

void taikoGame::drawGood() {
    drumFont.load("mikiyu-mokomori-kuro.ttf", 20);
    myFont.drawString("Good", 60, 400);
}

void taikoGame::drawOkay() {
    drumFont.load("mikiyu-mokomori-kuro.ttf", 20);
    myFont.drawString("Okay", 60, 400);
}

void taikoGame::drawOHHH() {
    drumFont.load("mikiyu-mokomori-kuro.ttf", 20);
    myFont.drawString("OHHH", 60, 400);
}

void taikoGame::drawRedDot(int dot_position) {
    ofSetColor(0,0,0);
    ofFill();
    ofDrawCircle(dot_position,400,50);
    
    ofSetColor(255,255,255);
    ofFill();        // draw "filled shapes"
    ofDrawCircle(dot_position,400,47);
    
    ofSetColor(255,130,10);
    ofFill();        // draw "filled shapes"
    ofDrawCircle(dot_position,400,40);
}

void taikoGame::drawBlueDrum() {
    ofSetColor(51,255,255,100);
    ofFill();        // draw "filled shapes"
    ofDrawCircle(100,400,50);
    if (font_state_ == NOTHING) {
        // do nothing
    } else if (font_state_ == OKAY) {
        drawOkay();
    } else if (font_state_ == GOOD) {
        drawGood();
    } else if (font_state_ == OHHH) {
        drawOHHH();
    }
}

void taikoGame::drawBlueDot(int dot_position) {
    ofSetColor(0,0,0);
    ofFill();
    ofDrawCircle(dot_position,400,50);
    
    ofSetColor(255,255,255);
    ofFill();        // draw "filled shapes"
    ofDrawCircle(dot_position,400,47);
    
    ofSetColor(51,255,255);
    ofFill();        // draw "filled shapes"
    ofDrawCircle(dot_position,400,40);
}

//--------------------------------------------------------------
void taikoGame::keyReleased(int key){
    int upper_key = toupper(key); // Standardize on upper case
    
    if (upper_key == ' ' && current_state_ != FINISHED) {
        draw();
        current_state_ = IN_PROGRESS;
        font_state_ = NOTHING;
    } else if (upper_key == 'I' && current_state_ != FINISHED) {
        draw();
        current_state_ = IN_PROGRESS;
        font_state_ = NOTHING;
    }
}

//--------------------------------------------------------------
void taikoGame::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void taikoGame::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void taikoGame::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void taikoGame::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void taikoGame::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void taikoGame::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void taikoGame::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void taikoGame::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void taikoGame::dragEvent(ofDragInfo dragInfo){
    
}

