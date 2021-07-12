#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, vector<string> &fileNames, string &texturePath):m_screenWidth(w),m_screenHeight(h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;
	// Render flag

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Final Project",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth,
                                m_screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();

    m_object = new Object[fileNames.size()];
	int ii = 0;
	for (auto& it: fileNames) {
   		m_object[ii].CreateObject(it);
		ii++;
	}
	m_objIdx = 0;
	m_numObjects = fileNames.size();
	m_xtoon = false;
	m_object[0].LoadTexture(texturePath);
	m_dindex = 0;
	m_outline = true;
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    // Reclaim all of our objects
    delete[] m_object;

    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
	return success;
}


// Update OpenGL
void SDLGraphicsProgram::Update(){
	// make sure that the object index makes sense and that it does not exceed
	// the total number of objects that are in this program.

	if (m_objIdx != -1 && m_objIdx < m_numObjects) {
		m_object[m_objIdx].GetTransform().LoadIdentity();
		m_object[m_objIdx].m_shader.SetUniform1i("d_index", m_dindex);
		// the light associated with the object
		m_object[m_objIdx].m_shader.SetUniform1i("xtoon", (int)m_xtoon);
		m_object[m_objIdx].m_shader.SetUniform1f("camera_z", Camera::Instance().GetEyeZPosition());
		m_object[m_objIdx].Update(m_screenWidth,m_screenHeight);


	}
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::Render(){
    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
   
	/* Draw the object twice: once with the offset to get an outline, and another
	   normally */
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset(-20.0f, -20.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(9.0f);

	// Render the object
	if(m_outline) {
		RenderObject(0.0f, 0.0f, 0.0f);	
	}
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_LIGHTING);
	// Render the object	
	RenderObject(1.0f, 1.0f, 1.0f);
	glPopAttrib();

    SDL_Delay(50); 
}

void SDLGraphicsProgram::RenderObject(float R, float G, float B) {
	if (m_objIdx != -1 && m_objIdx < m_numObjects) {
		m_object[m_objIdx].m_shader.SetUniform3f("color", R, G, B);
		m_object[m_objIdx].Render();
	}
}

//Loops forever!
void SDLGraphicsProgram::Loop(){
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();
	const float speed = 0.5f;
    // While application is running

    while(!quit){
     	 //Handle events on queue
		  while (SDL_PollEvent(&e) != 0)
        {
            // If the user hits the 'x' in the top right corner,
            // that signals an 'exit' event, and we terminate our 
            // application.
            if(e.type == SDL_QUIT){
                quit = true;
            }            
            else if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                        quit = true;
                        break;
					case SDLK_r:
						m_object[m_objIdx].m_rotate = !m_object[m_objIdx].m_rotate;
						break;
					case SDLK_w:
						Camera::Instance().MoveUp(speed);
						break;
					case SDLK_s:
						Camera::Instance().MoveDown(speed);
						break;
					case SDLK_o:
						m_outline = !m_outline;
						break;
					case SDLK_d:
						m_dindex = (m_dindex + 1) % 3;
						switch(m_dindex) {
							case 0:
								cout << "Calculating texture coords for Depth of Field" << endl;
								break;
							case 1:
								cout << "Calculating texture coords for Backlighting" << endl;
								break;
							case 2:
								cout << "Calculating texture coords for Specular Lighting" << endl;
								break;
						}
						break;
					case SDLK_x:
						m_xtoon = !m_xtoon;
						break;
                }
            }
			else if (e.type == SDL_MOUSEWHEEL) {
				if(e.wheel.y > 0) { // scroll up
					Camera::Instance().MoveForward(speed);
				}
				else if(e.wheel.y < 0) { // scroll down
					Camera::Instance().MoveBackward(speed);
				}

			}
        }

		// Update our scene
		Update();
		// Render using OpenGL
	    Render(); 	

      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
    }
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}