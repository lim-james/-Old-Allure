#ifndef ADDITIVE_RENDERER_H
#define ADDITIVE_RENDERER_H

#include "FBORenderer.h"

namespace Renderer {

	class Additive : public FBO {
	public:

		Additive();
		void Render(const unsigned& t0, const unsigned& t1);

	};

}

#endif
