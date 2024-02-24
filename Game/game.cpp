#include "game.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>


static std::vector<std::tuple<std::string, glm::vec4>> hitObjectsShapes;
static std::vector<glm::vec4> eyeCoords;
static std::vector<glm::vec4> spotLights;
static std::vector<glm::vec4> Lights;
static std::vector<glm::vec4> lightIntens;
static std::vector<glm::vec4> ambientColor;
static std::vector<glm::vec4> objColors;
static std::vector<HitObject> hitObjects;
static std::vector<spotlight> spotlights;
static std::vector<dirlight> dirlights;

static int PICTURE_SIZE = 512; //increasing this value will result in higher resolution but slower runtime
static int DEAPTH = 5;
static std::string INPUT_FILE = "scene";
static bool DO_MULTI_SMAPLING = true;

static glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);
static float infinity = std::numeric_limits<float>::max();
static float epsilon = 0.001f;
static float MAT_REFLECTION = 1.5f;

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
		hitObjectsShapes.push_back(std::make_tuple(typeVal, vec));
		break;
	case 'c': // COLOR OF OBJECT VEC
		coords = split(data, " ");
		initVec(vec, coords);
		objColors.push_back(vec);
		break;
	case 'd': // DIRECT LIGHT VEC
		coords = split(data, " ");
		initVec(vec, coords);
		Lights.push_back(vec);
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
	std::ifstream inputFile("../res/" + INPUT_FILE + ".txt", std::ios::in);
	if (!inputFile)
	{
		std::cerr << "could not open the specify file" << std::endl;
		exit(1);
	}
	while (std::getline(inputFile, currLine))
	{
		processLine(currLine);
	}
	std::vector<std::tuple<std::string, glm::vec4>>::iterator it1 = hitObjectsShapes.begin();
	std::vector<glm::vec4>::iterator it2 = objColors.begin();
	while (it1 != hitObjectsShapes.end() || it2 != objColors.end())
	{
		std::tuple<std::string, glm::vec4> shape = *it1;
		glm::vec4 color = *it2;
		std::string shape_behavior = std::get<0>(shape);
		glm::vec4 shapeCoords = std::get<1>(shape);
		glm::vec3 color3 = glm::vec3(color.x, color.y, color.z);
		if (shape_behavior == "object")
		{
			hitObjects.push_back(HitObject(color3, shapeCoords, color.w, false, false));
		}
		if (shape_behavior == "reflective")
		{
			hitObjects.push_back(HitObject(color3, shapeCoords, color.w, true, false));
		}
		if (shape_behavior == "transparent")
		{
			hitObjects.push_back(HitObject(color3, shapeCoords, color.w, false, true));
		}
		++it1;
		++it2;
	}

	std::vector<glm::vec4>::iterator it3 = Lights.begin();
	std::vector<glm::vec4>::iterator it4 = spotLights.begin();
	std::vector<glm::vec4>::iterator it5 = lightIntens.begin();
	while (it3 != Lights.end() || it4 != spotLights.end() || it5 != lightIntens.end())
	{
		glm::vec4 light = *it3;
		glm::vec4 light_intern = *it5;
		if (light.w == 1.0f)
		{
			glm::vec4 light_dir = *it4;
			spotlights.push_back(spotlight(
				glm::vec3(light.x, light.y, light.z),
				glm::vec3(light_dir.x, light_dir.y, light_dir.z),
				light_dir.w,
				glm::vec3(light_intern.x, light_intern.y, light_intern.z)));
			++it3;
			++it4;
			++it5;
		}
		else
		{
			dirlights.push_back(dirlight(glm::vec3(light.x, light.y, light.z), glm::vec3(light_intern.x, light_intern.y, light_intern.z)));
			++it3;
			++it5;
		}
	}

}

glm::vec3 getLightIllumination(ray reflected, hit_rec obj, ray camera, glm::vec3 base_illumination) {
	if (glm::length(reflected.dir) == 0)
		return BLACK;
	glm::vec3 L = glm::normalize(reflected.dir);
	glm::vec3 N = glm::normalize(obj.normal);
	glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L);
	glm::vec3 V = glm::normalize(camera.dir);
	glm::vec3 diffuse = obj.mat.Kd * std::abs(glm::dot(N, L)) * base_illumination;
	glm::vec3 specular = obj.mat.Kd * std::pow(glm::dot(R, V), obj.mat.shininess) * base_illumination;
	if (specular.x < 0)
		specular.x = 0;
	if (specular.y < 0)
		specular.y = 0;
	if (specular.z < 0)
		specular.z = 0;
	return diffuse + specular;

}

glm::vec3 check_spotlights(ray r, hit_rec hr){
	glm::vec3 illumination = BLACK;
	for (spotlight sl : spotlights)
	{
		if (!std::isnan(hr.t)) {
			bool blocked = false;
			ray lr = ray(hr.point, sl.getRay(hr.point));
			for (HitObject h : hitObjects) {
				hit_rec hrl;
				hrl = h.get_hit(lr, epsilon, sl.getT(hr.point));
				if (!std::isnan(hrl.t) && hrl.t > epsilon && hrl.t < sl.getT(hr.point))
					blocked = true;
			}
			if (!blocked)
				illumination += getLightIllumination(lr, hr, r, sl.illumination);
		}
	}
	return illumination;
}

glm::vec3 check_dirlights(ray r, hit_rec hr) {
	glm::vec3 illumination = BLACK;
	for (dirlight dl : dirlights)
	{
		if (!std::isnan(hr.t)) {
			bool blocked = false;
			ray lr = ray(hr.point, dl.getRay(hr.point));
			for (HitObject h : hitObjects) {
				hit_rec hrl;
				hrl = h.get_hit(lr, epsilon, dl.getT(hr.point));
				if (!std::isnan(hrl.t) && hrl.t > epsilon && hrl.t < dl.getT(hr.point))
					blocked = true;
			}
			if (!blocked)
				illumination += getLightIllumination(lr, hr, r, dl.illumination);
		}
	}
	return illumination;
}


glm::vec3 ray_color(ray r, int depth) {
	if (depth == 0)
		return BLACK;
	float min_t = infinity;
	hit_rec hr;
	glm::vec3 result = BLACK;
	for (HitObject h : hitObjects) {
		hit_rec nhr = h.get_hit(r, epsilon, infinity);
		if (nhr.t < min_t && !isnan(nhr.t))
		{
			min_t = nhr.t;
			hr = nhr;
		}
	}
	glm::vec3 illumination = glm::vec3(ambientColor[0].x * hr.mat.Kd, ambientColor[0].y * hr.mat.Kd, ambientColor[0].z * hr.mat.Kd);
	
	if (hr.mat.reflective)
	{
		glm::vec3 N = glm::normalize(hr.normal);
		glm::vec3 dir = 2.0f * N * (glm::dot(glm::normalize(r.dir), N)) - glm::normalize(r.dir);
		ray ray_ref = ray(hr.point - dir * epsilon, -dir);
		result = ray_color(ray_ref, depth - 1);
	}
	else if (hr.mat.transparent)
	{
		
		float n1_div_n2 = glm::dot(r.dir, hr.normal) > 0.0f ? MAT_REFLECTION : 1 / MAT_REFLECTION;
		glm::vec3 S1 = -glm::normalize(r.dir);
		//float theta1 = glm::dot(S1, glm::normalize(hr.normal));
		//float theta2 = asin(glm::clamp(n1_div_n2 * sin(theta1), -1.f, 1.f));
		//glm::vec3 S2 = (n1_div_n2 * cos(theta1) - cos(theta2)) * glm::normalize(hr.normal) - n1_div_n2 * S1;
		glm::vec3 S2 = glm::refract(S1, glm::normalize(hr.normal), n1_div_n2);
		result = ray_color(ray(S2, hr.point), depth - 1);
	}
	else
		result = hr.mat.base_color * (illumination + check_spotlights(r, hr) + check_dirlights(r, hr));
	
	if (result.x > 255.0f)
		result.x = 255.0f;
	if (result.y > 255.0f)
		result.y = 255.0f;
	if (result.z > 255.0f)
		result.z = 255.0f;
	return result;
}

void tracePixel(unsigned char* image,int x,int y, int size_x, int size_y)
{
	glm::vec3 eye = glm::vec3(eyeCoords[0].x, eyeCoords[0].y, eyeCoords[0].z); //origin
	glm::vec3 horizontal = glm::vec3(2.0, 0, 0); // X axis
	glm::vec3 vertical = glm::vec3(0, 2.0, 0); // Y axis
	glm::vec3 focal_length = glm::vec3(0, 0, eyeCoords[0].z); // distance from camera to screen
	glm::vec3 lower_left_corner = 
		eye - horizontal / 2.0f - vertical / 2.0f - focal_length;

	glm::vec3 pixel_color = glm::vec3(0.0f, 0.0f, 0.0f);
	float x_ = (float(x) / (size_x - 1));
	float y_ = (float(y) / (size_y - 1));
	glm::vec3 xDir = glm::vec3(2 * x_, 0, 0);
	glm::vec3 yDir = glm::vec3(0, 2 * y_, 0);
	glm::vec3 x_smaple_movement = glm::vec3(0.5f / (size_x - 1), 0, 0);
	glm::vec3 y_smaple_movement = glm::vec3(0, 0.5f / (size_x - 1), 0);

	ray r(eye, lower_left_corner + xDir + yDir - eye);
	ray r1(eye, lower_left_corner + xDir + yDir - eye - x_smaple_movement - y_smaple_movement);
	ray r2(eye, lower_left_corner + xDir + yDir - eye + x_smaple_movement - y_smaple_movement);
	ray r3(eye, lower_left_corner + xDir + yDir - eye - x_smaple_movement + y_smaple_movement);
	ray r4(eye, lower_left_corner + xDir + yDir - eye + x_smaple_movement + y_smaple_movement);

	if (DO_MULTI_SMAPLING)
		pixel_color = (ray_color(r1, DEAPTH) + ray_color(r2, DEAPTH) + ray_color(r2, DEAPTH) + ray_color(r2, DEAPTH)) * 0.25f;
	else
		pixel_color = ray_color(r, DEAPTH);

	image[(size_y - 1) * size_x * 4 - y * size_x * 4 + x * 4] = pixel_color.x;
	image[(size_y - 1) * size_x * 4 - y * size_x * 4 + x * 4 + 1] = pixel_color.y;
	image[(size_y - 1) * size_x * 4 - y * size_x * 4 + x * 4 + 2] = pixel_color.z;
	image[(size_y - 1) * size_x * 4 - y * size_x * 4 + x * 4 + 3] = 255;
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	//AddTexture("../res/textures/box0.bmp",false);

	AddShape(Plane,-1,TRIANGLES);
	
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;


	parseFile();
	int w = PICTURE_SIZE;
	int h = PICTURE_SIZE;

	unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char)* w * h *4);
	for (int i = 0; i < w; i++)
		for (int j = 0; j < w; j++)
			tracePixel(image, j, i, h, w);
	AddTexture(w, h, image);
	delete image;
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
