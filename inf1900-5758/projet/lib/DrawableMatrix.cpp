#include "DrawableMatrix.h"

DrawableMatrix::DrawableMatrix(MatrixProxy& matrix)
	: m_matrix(matrix) {}

void DrawableMatrix::drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const
{
	for (uint8_t i = 0; i < m_matrix.getRowAmount() && row + i < matrix.getRowAmount(); i++) {
		for (uint8_t j = 0; j < m_matrix.getColumnAmount() && col + i <  matrix.getColumnAmount(); j++) {
			matrix.set(row + i, col + j, m_matrix.get(i, j));
		}
	}
}