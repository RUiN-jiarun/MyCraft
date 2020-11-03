#include "ballRender.h"
const float PI = acos(-1);

Ball::Ball()
{
	this->radius = 3.0f;
	this->slices = 36;
	this->stacks = 18;
	this->interleavedStride = 32;
	buildVertices();
	initRenderData();
}

void Ball::buildVertices()
{

	// clear memory of prev arrays
	vector<float>().swap(vertices);
	vector<float>().swap(normals);
	vector<float>().swap(texCoords);
	vector<unsigned int>().swap(indices);

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float s, t;                                     // texCoord

	float sectorStep = 2 * PI / slices;
	float stackStep = PI / stacks;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		for (int j = 0; j <= slices; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord between [0, 1]
			s = (float)j / slices;
			t = (float)i / stacks;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (slices + 1);     // beginning of current stack
		k2 = k1 + slices + 1;      // beginning of next stack

		for (int j = 0; j < slices; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
				// k1---k2---k1+1
			}

			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
				// k1+1---k2---k2+1
			}

			// vertical lines for all stacks
			/*lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}*/
		}
	}

	// generate interleaved vertex array as well
	buildInterleavedVertices();
}

void Ball::buildInterleavedVertices()
{
	vector<float>().swap(interleavedVertices);

	std::size_t i, j;
	std::size_t count = vertices.size();
	for (i = 0, j = 0; i < count; i += 3, j += 2)
	{
		interleavedVertices.push_back(vertices[i]);
		interleavedVertices.push_back(vertices[i + 1]);
		interleavedVertices.push_back(vertices[i + 2]);

		interleavedVertices.push_back(normals[i]);
		interleavedVertices.push_back(normals[i + 1]);
		interleavedVertices.push_back(normals[i + 2]);

		interleavedVertices.push_back(texCoords[j]);
		interleavedVertices.push_back(texCoords[j + 1]);
	}
}

void Ball::Draw_Ball()
{
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
	glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
	glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[6]);

	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}

void Ball::initRenderData()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int stride = interleavedStride;
	glBufferData(GL_ARRAY_BUFFER, interleavedVertices.size() * sizeof(float), interleavedVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Ball::Ball_Render(Shader& shader, Texture2D& texture)
{
	shader.Use();
	texture.Bind();
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES,  // primitive type
		indices.size(), // # of indices
		GL_UNSIGNED_INT, // data type
		(void*)0); // ptr to indices
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}