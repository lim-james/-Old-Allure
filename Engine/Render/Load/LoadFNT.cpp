#include "LoadFnt.h"

#include "LoadTGA.h"

#include <Helpers/String/StringHelpers.h>

#include <GL/glew.h>

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
	auto common = Helpers::GetDictionary<float>(buffer, ' ', '=');

	const float baseUnit = 1.f / common["base"];
	font->lineHeight = common["lineHeight"] * baseUnit;

	font->scale.Set(common["scaleW"], common["scaleH"]);
	const vec2f texSize(font->scale);

	// skip line
	std::getline(ifs, buffer);

	// characters

	std::getline(ifs, buffer);
	auto chars = Helpers::GetDictionary<unsigned>(buffer, ' ', '=');

	font->count = chars["count"];
	
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	int offset = 0;
	for (unsigned i = 0; i < font->count; ++i) {
		std::getline(ifs, buffer);
		auto charData = Helpers::GetDictionary<float>(buffer, ' ', '=');

		Character character;
		character.index = i;

		// its rectangle in the texture
		const vec4f texRect(
			charData["x"], charData["y"],
			charData["width"], charData["height"]
		);

		const vec2f texOffset(
			charData["xoffset"], 
			-charData["yoffset"]
		);

		const float xAdvance = charData["xadvance"];

		character.rect.size = texRect.size * baseUnit;
		character.rect.origin = texOffset * baseUnit;
		character.xAdvance = xAdvance * baseUnit;

		font->characters[static_cast<int>(charData["id"])] = character;

		vec4f uvRect = texRect / vec4f(texSize, texSize);
		uvRect.y = 1.f - uvRect.y;

		Vertex v;
		v.position.Set(0.f, -character.rect.size.h, 0.f);
		v.texCoords.Set(
			uvRect.origin.u,
			uvRect.origin.v - uvRect.size.h
		);
		vertices.push_back(v);

		v.position.Set(character.rect.size.w, -character.rect.size.h, 0.f);
		v.texCoords.Set(
			uvRect.origin.u + uvRect.size.w, 
			uvRect.origin.v - uvRect.size.h
		);
		vertices.push_back(v);

		v.position.Set(character.rect.size.w, 0.f, 0.f);
		v.texCoords.Set(
			uvRect.origin.u + uvRect.size.w, 
			uvRect.origin.v
		);		
		vertices.push_back(v);

		v.position.Set(0.f);
		v.texCoords.Set(uvRect.origin);		
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