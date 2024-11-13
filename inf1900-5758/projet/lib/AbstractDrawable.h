#ifndef ABSTRACT_DRAWABLE
#define ABSTRACT_DRAWABLE

#include "MatrixProxy.h"
#include <stdint.h>

class MatrixProxy;

class AbstractDrawable {
public:
	AbstractDrawable() = default;

	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const = 0;
private:
};

#endif