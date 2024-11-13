#include "DrawableChar.h"

DrawableChar::DrawableChar(uint16_t character): m_character(character) {}

void DrawableChar::drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	matrix.set(row, col, m_character);
}
