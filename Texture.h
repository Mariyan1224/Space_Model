#pragma once

#include <string>
#include <vector>

class Texture
{
public:
	Texture(const std::string& fileName);
	Texture(const std::vector<std::string> faces);


	void Bind(unsigned textureSlot = 0);
	void SpaceBind(unsigned textureSlot = 0);

	
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

private:
	unsigned _texture;
	unsigned _spaceTex;
	
	int _width = 0;
	int _height = 0;
	int _spaceFaceWidth = 0;
	int _spaceFaceHight = 0;

};

