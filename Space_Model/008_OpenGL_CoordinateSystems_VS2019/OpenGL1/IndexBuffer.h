#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned* data, int size);

	void Bind();
	void Unbind();

private:
	unsigned int id;
};