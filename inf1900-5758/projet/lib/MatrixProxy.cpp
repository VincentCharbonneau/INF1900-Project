#include "MatrixProxy.h"

MatrixProxy::MatrixProxy(const uint8_t nRows, const uint8_t nColumns, uint16_t defaultChar)
	: m_nRows(nRows), m_nColumns(nColumns) {}

uint8_t MatrixProxy::getRowAmount() const {
	return m_nRows;
}

uint8_t MatrixProxy::getColumnAmount() const {
	return m_nColumns;
}

void MatrixProxy::draw(const AbstractDrawable& drawable, const uint8_t row, const uint8_t col)
{
	drawable.drawOnto(*this, row, col);
}
