#pragma once



class Object{
public:
	virtual ~Object() {}
	virtual void InitGL() {};
	virtual void Init() {};
	virtual void Draw() {};
	virtual void Update() {};
}
