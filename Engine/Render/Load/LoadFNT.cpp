#include "LoadFnt.h"

#include "LoadTGA.h"

#include <Helpers/String/StringHelpers.h>

#include <GL/glew.h>

#include <iostream>
#include <fstream>

Font* Load::FNT(const std::string& fntPath, const std::string& tgaPath) {
	if (fontCache[fntPath])
		return fontCache[fntPath];

	std::ifstream ifs(fntPath);
	std::string buffer;

	// info

	std::getline(ifs, buffer);

	Font* font = new Font;
	font->name = Helpers::Pair(Helpers::Split(buffer, ' ')[1], '=').second;

	// common
	
	std::getline(ifs, buffer);
	auto common = Helpers::GetDictionary<unsigned>(buffer, ' ', '=');

	font->lineHeight = common["lineHeight"];
	font->base = common["base"];
	const float base = font->base;

	font->scale.Set(common["scaleW"], common["scaleH"]);
	const vec2f texSize(font->scale);

	// skip line
	std::getline(ifs, buffer);

	// characters

	std::getline(ifs, buffer);
	auto chars = Helpers::GetDictionary<unsigned>(buffer, ' ', '=');

	font->count = chars["count"];

	Character charBuffer;
	std::map<std::string, int> charData;
	
	Vertex v;
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	int offset = 0;
	for (unsigned i = 0; i < font->count; ++i) {
		std::getline(ifs, buffer);
		charData = Helpers::GetDictionary<int>(buffer, ' ', '=');

		charBuffer.index = i;

		charBuffer.rect.Set(
			charData["x"], charData["y"],
			charData["width"], charData["height"]
		);
		const vec4f originalRect = vec4f(charBuffer.rect);
		vec4f rect = originalRect / vec4f(texSize, texSize);
		rect.y = 1.f - rect.y;

		vec2f halfSize; 
		halfSize.h = (originalRect.size.h / base) * 0.5f;
		halfSize.w = (originalRect.size.w / originalRect.size.h) * halfSize.h;

		charBuffer.offset.Set(
			charData["xoffset"], charData["yoffset"]
		);

		charBuffer.xAdvance = charData["xadvance"];

		font->characters[charData["id"]] = charBuffer;

		v.position.Set(-halfSize.w, -halfSize.h, 0);
		v.texCoords.Set(rect.origin);
		vertices.push_back(v);

		v.position.Set(halfSize.w, -halfSize.h, 0);
		v.texCoords.Set(
			rect.origin.u + rect.size.w, 
			rect.origin.v
		);
		vertices.push_back(v);

		v.position.Set(halfSize.w, halfSize.h, 0);
		v.texCoords.Set(rect.origin + rect.size);
		vertices.push_back(v);

		v.position.Set(-halfSize.w, halfSize.h, 0);
		v.texCoords.Set(
			rect.origin.u, 
			rect.origin.v + rect.size.h
		);		
		vertices.push_back(v);

		indices.push_back(offset + 0);
		indices.push_back(offset + 1);
		indices.push_back(offset + 2);
		indices.push_back(offset + 0);
		indices.push_back(offset + 2);
		indices.push_back(offset + 3);
		offset += 4;
	}

	ifs.close();

	font->texture = Load::TGA(tgaPath);
	font->mesh = new Mesh(vertices, indices); 	
	fontCache[fntPath] = font;

	return font;
}

void Load::ClearFontCache() {
	for (const auto& item : fontCache)
		delete item.second;

	fontCache.clear();
}