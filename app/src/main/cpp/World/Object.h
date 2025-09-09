#pragma once
#include <cstdint>


class Object {
    public:
    //Function which starts the complete object
    virtual int Init() {return 0;}
    //Function which inits the complete open GL
    virtual int InitGL() {return 0;}
    //Function which updates the complete object (next frame)
    virtual int Update(uint64_t EuS, uint64_t TuS) {return 0;}
    //Function which draws the complete object
    virtual int Draw() {return 0;}
    //Function which Resumes GL
    virtual int ResumeGL() {return 0;}
    //Function which Pauses GL for updates
    virtual int PauseGL() {return 0;}
    //Function which ends everything associated with OpenGL
    virtual int EndGL() {return 0;}
    //Function which destroys the complete object.
    virtual int Destroy() {return 0;}
};