#ifndef DRAWABLE_CHAR
#define DRAWABLE_CHAR

#include "AbstractDrawable.h"

class DrawableChar : public AbstractDrawable {
public:
	DrawableChar(const uint16_t character);
	virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const ;
private:
	const uint16_t m_character;
};

#endif