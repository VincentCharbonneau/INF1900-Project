#ifndef DRAWABLE_RECTANGLE
#define DRAWABLE_RECTANGLE

#include "AbstractDrawable.h"
#include "DrawableLine.h"
#include "DrawableChar.h"

class DrawableRectangle : public AbstractDrawable {
public:
	enum class SIDE {
		TOP, BOTTOM, LEFT, RIGHT
	};

	enum class REGION {
		INNER, OUTER
	};

	DrawableRectangle(const uint8_t height, const uint8_t length, const DrawableLine::LineType lineType);

	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;

private:
	enum class Corner {
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};

	const uint8_t m_length;
	const uint8_t m_height;
	const DrawableLine m_lines[2];
	const DrawableChar m_corners[4];

	uint16_t getCorner(Corner corner, DrawableLine::LineType lineType);
};

#endif