#ifndef MATRIX_PROXY
#define MATRIX_PROXY

#include <cstdint>
#include "AbstractDrawable.h"

class AbstractDrawable;

class MatrixProxy {
protected:
	const uint8_t m_nRows;
	const uint8_t m_nColumns;

public:
	MatrixProxy(const uint8_t nRows, const uint8_t nColumns, char16_t defaultChar);
	virtual ~MatrixProxy() = default;
	virtual char16_t get(const uint8_t row, const uint8_t column) const = 0;
	virtual void set(const uint8_t row, const uint8_t column, char16_t val) = 0;

	uint8_t getRowAmount() const;
	uint8_t getColumnAmount() const;

	virtual void draw(const AbstractDrawable& drawable, const uint8_t row, const uint8_t col);
};

#endif