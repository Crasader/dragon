#ifndef COCOS2D_PLAT_DIFF__
#define COCOS2D_PLAT_DIFF__

#ifdef WONPEE_SERVER
namespace cocos2d
{
	class CCPoint
	{
	public:
		float x;
		float y;
	public:
		CCPoint() {
			 x = 0; y = 0; 
		}
		CCPoint(float _x, float _y) {
			x = _x; y = _y; 
		}
		CCPoint(const CCPoint& other)
		{
			x = other.x;
			y = other.y;
		}
		CCPoint& operator=(const CCPoint& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}
	};

	class CCSize
	{
	public:
		float width;
		float height;

	public:
		CCSize() {
			width = 0;
			height = 0;
		}
		CCSize(float _width, float _height) {
			width = _width;
			height = _height;
		}
		CCSize(const CCSize& other) {
			width = other.width;
			height = other.height;
		}
		CCSize& operator=(const CCSize& other) {
			width = other.width;
			height = other.height;
			return *this;
		}
	};
};
#endif

#endif
