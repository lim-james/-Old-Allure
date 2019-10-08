#ifndef FBO_RENDERER_H
#define FBO_RENDERER_H

#include "../Framebuffer/Framebuffer.h"
#include "../Material/Shader.h"

namespace Renderer {

	class FBO {

		Shader* shader;
		unsigned VAO;

	public:

		FBO();
		FBO(const std::string& vPath, const std::string& fPath);
		~FBO();

		void Render(const unsigned& texture);

	};

}

#endif