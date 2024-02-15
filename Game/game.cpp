#include "game.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>


static std::vector<std::tuple<std::string, glm::vec4>> hitObjects;
static std::vector<glm::vec4> eyeCoords;
static std::vector<glm::vec4> spotLights;
static std::vector<glm::vec4> directLights;
static std::vector<glm::vec4> lightIntens;
static std::vector<glm::vec4> ambientColor;
static std::vector<glm::vec4> objColors;


static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

void initVec(glm::vec4& vec, std::vector<std::string> coords)
{
	vec.x = std::stof(coords[0]);
	vec.y = std::stof(coords[1]);
	vec.z = std::stof(coords[2]);
	vec.w = std::stof(coords[3]);
}

void processLine(const std::string& line) {
	if (line.empty()) return; // Skip empty lines

	char type = line[0]; // First character indicates the type
	std::string data = line.substr(2); // The rest of the line is data
	std::vector<std::string> coords;
	std::string typeVal;
	glm::vec4 vec;
	switch (type) {
	case 'e': // EYE VEC
		coords = split(data, " ");
		initVec(vec, coords);
		eyeCoords.push_back(vec);
		break;
	case 'a': // AMBIENT VEC
		coords = split(data, " ");
		initVec(vec, coords);
		ambientColor.push_back(vec);
		break;
	case 'o': // OBJECT VEC
	case 'r': // REFLECTIVE OBJECT VEC
	case 't': // TRANSPARENT OBJECT VEC
		type == 'o' ? typeVal = "object" : (type == 'r' ? typeVal = "reflective" : typeVal = "transparent");
		coords = split(data, " ");
		initVec(vec, coords);
		hitObjects.push_back(std::make_tuple(typeVal, vec));
		break;
	case 'c': // COLOR OF OBJECT VEC
		coords = split(data, " ");
		initVec(vec, coords);
		objColors.push_back(vec);
		break;
	case 'd': // DIRECT LIGHT VEC
		coords = split(data, " ");
		initVec(vec, coords);
		directLights.push_back(vec);
		break;
	case 'p': // SPOTLIGHT VEC
		coords = split(data, " ");
		initVec(vec, coords);
		spotLights.push_back(vec);
		break;
	case 'i': // LIGHT INTENSITY VEC
		coords = split(data, " ");
		initVec(vec, coords);
		lightIntens.push_back(vec);
		break;
	default:
		std::cerr << "Unknown line type: " << type << std::endl;
		break;
	}
}

void parseFile()
{
	std::string currLine;
	std::ifstream inputFile("../res/scene1.txt", std::ios::in);
	if (!inputFile)
	{
		std::cerr << "could not open the specify file" << std::endl;
		exit(1);
	}
	while (std::getline(inputFile, currLine))
	{
		processLine(currLine);
	}

}


void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	AddTexture("../res/textures/box0.bmp",false);

	AddShape(Plane,-1,TRIANGLES);
	
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
