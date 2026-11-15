#pragma once

#include "MobileObject.h"

class Balloon : public MobileObject
{

private:

	
	int color;
	bool beenHit=false;
	int col=0;
	float timeAccumulator = 0.0f;
	static constexpr float EXPLOSION_FRAME_TIME = 0.1f;
	static constexpr int LAST_EXPLOSION_FRAME = 5;
	static constexpr int BALLOON_POINTS = 10;



public:

	Balloon(Game* game,const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color);
	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle& obj) override;
	void save(std::ostream& out)const override;
	Balloon(std::istream& in, Game* game);
};
