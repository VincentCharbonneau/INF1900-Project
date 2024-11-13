#ifndef ABSTRACT_DRAWABLE
#define ABSTRACT_DRAWABLE

#include "MatrixProxy.h"
#include <cstdint>

class MatrixProxy;

class AbstractDrawable {
public:
	AbstractDrawable() = default;
	virtual ~AbstractDrawable() = default;

	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const = 0;
private:
};

#endif