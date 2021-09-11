/*
 * TextRenderer.h
 *
 *  Created on: 21 mai 2020
 *      Author: silverly
 */

#ifndef UTILS_SHADER_TEXTRENDERER_H_
#define UTILS_SHADER_TEXTRENDERER_H_

#include <GL/glew.h>
#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../../libs/freetype-2.10.1/include/freetype/ftimage.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
 *
 */
class TextRenderer {
public:
	TextRenderer(std::string path, int fontWidth, int fontHeight,
			int windowWidth, int windowHeight);
	void setRatio(int windowWidth, int windowHeight);
	void bind();
	void unbind();
	void renderText(std::string text, float x, float y, float scale,
			glm::vec3 color);
private:
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2 Size;      // Size of glyph
		glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
		FT_Pos Advance;   // Horizontal offset to advance to next glyph
	};

	static GLuint _programId;
	std::map<GLchar, Character> _characters;
	GLuint _VAO, _VBO;
	int _windowWidth, _windowHeight;
};

#endif /* UTILS_SHADER_TEXTRENDERER_H_ */