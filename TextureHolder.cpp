#include "TextureHolder.h"

#include <assert.h>

TextureHolder* TextureHolder::m_spInstance {nullptr};

TextureHolder::TextureHolder()
{
	assert(m_spInstance == nullptr);
	m_spInstance = this;
}

sf::Texture& TextureHolder::getTexture(std::string const& filename)
{

	// Get a reference to m_textureMap using m_spIntance
	auto& m = m_spInstance->m_textureMap;
	auto keyValuePair = m.find(filename);

	//match found?
	if (keyValuePair != m.end())
	{
		//yes 
		return keyValuePair->second;
	}
	else
	{
		// file not loaded into map
		//  create a new key value pair using the filename
		auto& texture = m[filename];
		texture.loadFromFile(filename);

		return texture;

	}
}
