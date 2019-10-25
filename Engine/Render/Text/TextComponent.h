#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "../../Components/Component.h"
#include "../Font/Font.h"

#include <string>

enum PARAGRAPH { 
	LEFT,
	CENTER,
	RIGHT
};

enum ALIGNMENT { 
	TOP,
	MIDDLE,
	BOTTOM
};

struct TextComponent : Component {

	std::string text;

	Font* font;
	float scale;
	float lineSpacing;
	float characterSpacing;

	PARAGRAPH paragraphAlignment;
	ALIGNMENT verticalAligment;

	TextComponent();
	~TextComponent() override;

};

#endif
