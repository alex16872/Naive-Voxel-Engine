/*
 * EntityChunk.cpp
 *
 *  Created on: 18 juin 2020
 *      Author: silverly
 */

#include "entity_chunk.h"

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>

#include "../../utils/loaders/ObjLoader.h"
#include "../../utils/loaders/ShaderLoader.h"
#include "../../utils/Location.h"
#include "../../utils/shader/ShaderBase.h"

EntityChunk::EntityChunk(ChunkUtil::BasicChunk *chunk) :
		IEntity::IEntity { } {
	_chunk = chunk;
}

EntityChunk::EntityChunk(ChunkUtil::BasicChunk *chunk, Location loc) :
		IEntity::IEntity { loc } {
	_chunk = chunk;
}

void EntityChunk::preload() {
	std::cout << "Preloading Chunk.\n";

	// Our VAO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// Create and compile our GLSL program from the shaders
	std::cout << "Loading Shader...\n";
	programID = LoadShaders("ressources/shaders/chunkColor/chunkColor.vs",
			"ressources/shaders/chunkColor/chunkColor.fs");

	// Get a handle for our "MVP" uniform
	matrixID = glGetUniformLocation(programID, "MVP");

	// Read our .obj file
	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec3> colors;
	//loadOBJ("ressources/models/example/cube.obj", vertices, uvs, normals);
	std::cout << "Meshing...\n";
	ChunkUtil::Mesh *mesh = ChunkUtil::greedyMesh(*_chunk);

	std::cout << "Creating VAB...\n";
	// First attribute of our VAO, vertices
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3),
			mesh->vertices.data(), GL_STATIC_DRAW);
	verticeBufferSize = (int) mesh->vertices.size();
	std::cout << "Vertice Buffer Size: " << verticeBufferSize << "\n";

	// 2nd attribute, colors
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->colors.size() * sizeof(glm::vec3),
			mesh->colors.data(),
			GL_STATIC_DRAW);

	delete mesh;
	std::cout << "Done preloading Chunk.\n";
}

void EntityChunk::draw(glm::mat4 &base) {
	// Use our shader
	glUseProgram(programID);

	ShaderBase::loadMVP(matrixID, base, _extraPosition, _extraRotation);
	glBindVertexArray(vertexArrayID);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0,	// attribute
			3,	// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,	// stride
			(void*) 0	// array buffer offset
			);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1,	// attribute
			3,	// size 3, rgb
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,	// stride
			(void*) 0	// array buffer offset
			);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, verticeBufferSize);

	// And then we unbind everything
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void EntityChunk::unload() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
}