#ifndef FBO_RENDERER_H
#define FBO_RENDERER_H

#include "../Framebuffer/Framebuffer.h"
#include "../Material/Shader.h"

#include <Math/Matrix.hpp>

namespace Renderer {

	class FBO {

		Shader* shader;
		unsigned VAO;

	public:

		FBO();
		FBO(const std::string& vPath, const std::string& fPath);
		~FBO();

		void Render(const unsigned& texture);
		void Render(const unsigned& texture, const vec2f& position, const vec2f& size);

	};

}

#endif