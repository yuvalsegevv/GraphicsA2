#pragma once
#include "scene.h"
#include "HitObject.h"
#include "sphere.h"
#include "plane.h"
#include "spotlight.h"
#include "dirlight.h"

class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
};

