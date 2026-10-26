#pragma once
#include "BoardLayout.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

// Central place for window size, layout and colours.
namespace ttt::ui {

	inline constexpr unsigned kWindowWidth  = 600;
	inline constexpr unsigned kWindowHeight = 720;

	inline constexpr BoardLayout kBoardLayout{50.f, 110.f, 500.f};

	inline constexpr sf::Color kBackground  {28, 32, 44};
	inline constexpr sf::Color kGrid        {88, 96, 122};
	inline constexpr sf::Color kMarkX       {235, 99, 99};
	inline constexpr sf::Color kMarkO       {99, 181, 235};
	inline constexpr sf::Color kText        {235, 238, 245};
	inline constexpr sf::Color kButton      {52, 60, 84};
	inline constexpr sf::Color kButtonHover {74, 85, 120};

}
