#include <iostream>
#include <fstream>
#include <string>
#include "SDLGraphicsProgram.hpp"

using namespace std;
int main(int argc, char* argv[]) {
    // allow for multiple .obj files to be passed in as args
    vector<string> objArgs; 
    if(argc != 3){
        cout << "Please specify a .obj model and a .ppm texture" << std::endl;
        return 0;
    }
    else {
        string filePath(argv[1]);
        string texturePath(argv[2]);
        objArgs.push_back(filePath);   
         // Create an instance of an object for a SDLGraphicsProgram
        SDLGraphicsProgram mySDLGraphicsProgram(1280,720,objArgs,texturePath);
        // Run our program forever
        mySDLGraphicsProgram.Loop();
        objArgs.clear();
    }

	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}