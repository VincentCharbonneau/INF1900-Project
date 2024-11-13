#include "DrawableRectangle.h"
#include <iostream>

DrawableRectangle::DrawableRectangle(const uint8_t height, const uint8_t length, const DrawableLine::LineType lineType)
	: m_length(length), m_height(height),
	m_lines{ DrawableLine(DrawableLine::Direction::HORIZONTAL, length - 2, lineType),
			 DrawableLine(DrawableLine::Direction::VERTICAL, height - 2, lineType) },
	m_corners { DrawableChar(getCorner(Corner::TOP_LEFT, lineType)),
				DrawableChar(getCorner(Corner::TOP_RIGHT, lineType)),
				DrawableChar(getCorner(Corner::BOTTOM_LEFT, lineType)),
				DrawableChar(getCorner(Corner::BOTTOM_RIGHT, lineType))}
{
}

void DrawableRectangle::drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	m_lines[0].drawOnto(matrix, row, col + 1);
	m_lines[0].drawOnto(matrix, row + m_height - 1, col + 1);
	m_lines[1].drawOnto(matrix, row + 1, col);
	m_lines[1].drawOnto(matrix, row + 1, col + m_length - 1);
	m_corners[0].drawOnto(matrix, row, col);
	m_corners[1].drawOnto(matrix, row, col + m_length - 1);
	m_corners[2].drawOnto(matrix, row + m_height - 1, col);
	m_corners[3].drawOnto(matrix, row + m_height - 1, col + m_length - 1);
}

char16_t DrawableRectangle::getCorner(Corner corner, DrawableLine::LineType lineType)
{
	if(lineType == DrawableLine::LineType::EMPTY) return u' ';

	if (corner == Corner::TOP_LEFT) {
		switch (lineType) {
			case DrawableLine::LineType::THIN:
				return u'T';
			case DrawableLine::LineType::BOLD:
				return u'T';
			case DrawableLine::LineType::DOUBLE:
				return u'T';
		}
	}
	else if (corner == Corner::TOP_RIGHT) {
		switch (lineType) {
			case DrawableLine::LineType::THIN:
				return u'T';
			case DrawableLine::LineType::BOLD:
				return u'T';
			case DrawableLine::LineType::DOUBLE:
				return u'T';
		}
	}
	else if (corner == Corner::BOTTOM_LEFT) {
		switch (lineType) {
			case DrawableLine::LineType::THIN:
				return u'T';
			case DrawableLine::LineType::BOLD:
				return u'T';
			case DrawableLine::LineType::DOUBLE:
				return u'T';
		}
	}
	else if (corner == Corner::BOTTOM_RIGHT) {
		switch (lineType) {
			case DrawableLine::LineType::THIN:
				return u'T';
			case DrawableLine::LineType::BOLD:
				return u'T';
			case DrawableLine::LineType::DOUBLE:
				return u'T';
		}	
	}
}


