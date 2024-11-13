#ifndef DRAWABLE_LINE
#define DRAWABLE_LINE

#include "AbstractDrawable.h" 

class DrawableLine : public AbstractDrawable {
public:
	enum class Direction {
		VERTICAL, HORIZONTAL
	};

	enum class LineType {
		THIN, BOLD, DOUBLE, EMPTY
	};

	DrawableLine(const Direction dir, const uint8_t size, const LineType charac);
	virtual ~DrawableLine() = default;

	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;

	Direction getDirection() const;
	LineType getLineType() const;
	uint8_t getSize() const;
private:
	const LineType m_lineType;
	const Direction m_direction;
	const uint8_t m_size;
	const char16_t m_character;

	void verticalDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;
	void horizontalDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;

	char16_t getCharacter(Direction dir, LineType type) const;
 };

#endif