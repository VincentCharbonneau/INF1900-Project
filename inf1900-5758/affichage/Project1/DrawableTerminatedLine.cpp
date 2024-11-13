#include "DrawableTerminatedLine.h"

DrawableTerminatedLine::DrawableTerminatedLine(const TERMINATION term, const DrawableLine& line)
	: m_termination(term), m_line(line), m_termChar(getCharacter(m_termination, m_line.getLineType())) {}

void DrawableTerminatedLine::drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	if (m_line.getDirection() == DrawableLine::Direction::VERTICAL) {
		if (m_termination == TERMINATION::START) verticalStartDraw(matrix, row, col);
		else verticalEndDraw(matrix, row, col);
	}
	else {
		if (m_termination == TERMINATION::START) horizontalStartDraw(matrix, row, col);
		else horizontalEndDraw(matrix, row, col);
	}
}

DrawableChar& DrawableTerminatedLine::getCharacter(TERMINATION term, DrawableLine::LineType type) const
{
	char16_t character {};

	if(type == DrawableLine::LineType::EMPTY) character = u' ';
	else if (term == TERMINATION::START) {
		switch (type) {
			case DrawableLine::LineType::THIN:
				character = u'<';
			case DrawableLine::LineType::BOLD:
				character = u'[';
			case DrawableLine::LineType::DOUBLE:
				character = u'[';
		}
	}
	else if (term == TERMINATION::END) {
		switch (type) {
			case DrawableLine::LineType::THIN:
				character = u'>';
			case DrawableLine::LineType::BOLD:
				character = u']';
			case DrawableLine::LineType::DOUBLE:
				character = u']';
		}
	}

	DrawableChar dChar(character);
	return dChar;
}

void DrawableTerminatedLine::verticalStartDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	matrix.draw(m_termChar, row, col);
	matrix.draw(m_line, row + 1, col);
}

void DrawableTerminatedLine::verticalEndDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	matrix.draw(m_line, row, col);
	matrix.draw(m_termChar, row + m_line.getSize(), col);
}

void DrawableTerminatedLine::horizontalStartDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	matrix.draw(m_termChar, row, col);
	matrix.draw(m_line, row, col + 1);
}

void DrawableTerminatedLine::horizontalEndDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	matrix.draw(m_line, row, col);
	matrix.draw(m_termChar, row, col + m_line.getSize());
}