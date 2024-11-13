#include "DrawableLine.h"

uint8_t getLineLength(LineLength length) {
	switch(length) {
		case LineLength::LONG:
			return 4;
		case LineLength::MEDIUM:
			return 3;
		case LineLength::SMALL:
			return 2;
		default:
			return 0;
	}
}

DrawableLine::DrawableLine(const Direction dir, const uint8_t size, const LineType lineType)
	: m_direction(dir), m_size(size), m_character(getCharacter(dir, lineType)), m_lineType(lineType) {}

DrawableLine::DrawableLine(Line& line)
	: m_direction(Direction::VERTICAL), m_size(getLineLength(line.getRawLineType())),
	  m_lineType(DrawableLine::LineType::DOUBLE), m_character(getCharacter(DrawableLine::Direction::VERTICAL, DrawableLine::LineType::DOUBLE)) {}

void DrawableLine::drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	if (m_direction == Direction::VERTICAL) verticalDraw(matrix, row, col);
	else if(m_direction == Direction::HORIZONTAL) horizontalDraw(matrix, row, col);
}

DrawableLine::Direction DrawableLine::getDirection() const
{
	return m_direction;
}

DrawableLine::LineType DrawableLine::getLineType() const
{
	return m_lineType;
}

uint8_t DrawableLine::getSize() const
{
	return m_size;
}

void DrawableLine::verticalDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	for (uint8_t i = 0; i < m_size && row + i < matrix.getRowAmount(); i++) {
		matrix.set(row + i, col, m_character);
	}
}

void DrawableLine::horizontalDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	for (uint8_t i = 0; i < m_size && col + i < matrix.getColumnAmount(); i++) {
		matrix.set(row, col + i, m_character);
	}
}

uint16_t DrawableLine::getCharacter(Direction dir, LineType type) const
{
	if(type == LineType::EMPTY) return u' ';

	if (dir == Direction::VERTICAL) {
		switch (type) {
			case LineType::THIN:
				return 0x2502;
			case LineType::BOLD:
				return 0x2503;
			case LineType::DOUBLE:
				return 0x2551;
		}
	}
	else if (dir == Direction::HORIZONTAL) {
		switch (type) {
			case LineType::THIN:
				return 0x2500;
			case LineType::BOLD:
				return 0x2501;
			case LineType::DOUBLE:
				return 0x2550;
		}
	}
}
