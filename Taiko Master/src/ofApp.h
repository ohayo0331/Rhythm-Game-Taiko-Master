#pragma once

#include "ofMain.h"
#include <ctime>
#include <cstdlib>
#include <utility>
#include <vector>
#include "ofMain.h"
#include "Note.hpp"
#include "vector"
#include "ofxAudioAnalyzer.h"
#include "ofxAudioDecoder.h"
#include "ofSoundPlayerExtended.h"

using namespace taikomaster;

enum GameState {
    IN_PROGRESS = 0,
    IN_MENU,
    END,
    PLAY_RED,
    PLAY_BLUE,
    PAUSED,
    FINISHED
};

enum FontState {
    NOTHING = 0,
    GOOD,
    OKAY,
    OHHH
};

enum SongState {
    SONG_1 = 0,
    SONG_2,
    SONG_3
};


class taikoGame : public ofBaseApp{
    
public:
    int score;
    float * fftSmooth;
    int bands;
    bool isPlaying = false;
    bool onset;
    ofTrueTypeFont myFont;
    ofTrueTypeFont drumFont;
    ofTrueTypeFont menuFont;
    ofTrueTypeFont endFont;
    ofImage left;
    ofImage right;
    ofImage song_1;
    ofImage song_2;
    ofImage song_3;
    GameState current_state_ = IN_MENU;
    FontState font_state_ = NOTHING;
    SongState song_state_ = SONG_1;
    ofSoundPlayer song;
    ofSoundPlayer mySound;
    vector<Note> notes;
    vector<int> time_pop_;
    float timer_crr;
    float timer;
    void setup();
    void update();
    void draw();
    void drawRedDrum();
    void drawBlueDrum();
    void drawRedDot(int dot_position);
    void drawBlueDot(int dot_position);
    void drawGood();
    void drawOkay();
    void drawOHHH();
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
    

    ofxAudioAnalyzer audioAnalyzer;
    ofSoundPlayerExtended player;
    
    int sampleRate;
    int bufferSize;
    float strongDecay;
    float strongPeak;
    float pitchSalience;
    float pitchFreq;
    
        ofSoundBuffer soundBuffer;
    
    
    
};
