#ifndef __TEXTUREHOLDER__
#define __TEXTUREHOLDER__

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
class TextureHolder
{
private:
	// a map container from the STL
	//  that holds related pairs of String and Texture
	std::map<std::string, sf::Texture> m_textureMap;

	//A pointer of the same type as the class
	//   the one and only instance member
	static TextureHolder* m_spInstance;

public:
	TextureHolder();

	static sf::Texture& getTexture(std::string const& filename);


};

#endif
