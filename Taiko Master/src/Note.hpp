//
//  Note.hpp
//  myFinalProject
//
//  Created by 杨航一 on 4/27/18.
//

#ifndef Note_hpp
#define Note_hpp
#include "ofMain.h"
#include "ofxGui.h"

#include <stdio.h>

namespace taikomaster {
    enum DrumState {
        NOT_PRESSED = 0,
        PRESSED
    };
    
    class Note {
        public:
        bool color_; // the color of the note, true if it is red, false if it is blue
        
        DrumState drum_state_ = NOT_PRESSED;
        Note(); // Default constructor
        int dist_x_; // the x position in window
        
        int dist_y_; // the y position in window
        
        float time_out_;// the time that this note is popping out
        
        bool getColor(); // get the color of the note
    };
    
    
}

#endif /* Note_hpp */
