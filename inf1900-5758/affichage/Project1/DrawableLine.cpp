#include "DrawableLine.h"

DrawableLine::DrawableLine(const Direction dir, const uint8_t size, const LineType lineType)
	: m_direction(dir), m_size(size), m_character(getCharacter(dir, lineType)), m_lineType(lineType) {}

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

char16_t DrawableLine::getCharacter(Direction dir, LineType type) const
{
	if(type == LineType::EMPTY) return u' ';

	if (dir == Direction::VERTICAL) {
		switch (type) {
			case LineType::THIN:
				return u'|';
			case LineType::BOLD:
				return u'+';
			case LineType::DOUBLE:
				return u'=';
		}
	}
	else if (dir == Direction::HORIZONTAL) {
		switch (type) {
			case LineType::THIN:
				return u'-';
			case LineType::BOLD:
				return u'+';
			case LineType::DOUBLE:
				return u'=';
		}
	}
}
