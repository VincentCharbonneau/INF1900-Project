#ifndef MATRIX_PROXY
#define MATRIX_PROXY

#include <stdint.h>
#include "AbstractDrawable.h"

class AbstractDrawable;

class MatrixProxy {
protected:
	const uint8_t m_nRows;
	const uint8_t m_nColumns;

public:
	MatrixProxy(const uint8_t nRows, const uint8_t nColumns, uint16_t defaultChar);
	
	virtual uint16_t get(const uint8_t row, const uint8_t column) const = 0;
	virtual void set(const uint8_t row, const uint8_t column, uint16_t val) = 0;

	uint8_t getRowAmount() const;
	uint8_t getColumnAmount() const;

	virtual void draw(const AbstractDrawable& drawable, const uint8_t row, const uint8_t col);
};

#endif