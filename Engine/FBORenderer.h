#ifndef FBO_RENDERER_H
#define FBO_RENDERER_H

#include "Framebuffer.h"
#include "Shader.h"

namespace Renderer {

	class FBO {

	protected:

		Shader* shader;
		unsigned VAO;

	public:

		FBO();
		FBO(const std::string& vPath, const std::string& fPath);
		virtual ~FBO();

		void PreRender();
		void PreRender(const vec3f& position, const vec2f& size);

		void Render(const unsigned& texture);

		Shader * const GetShader() const;

	};

}

#endif