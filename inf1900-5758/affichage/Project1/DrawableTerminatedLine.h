#pragma once
#include "AbstractDrawable.h"
#include "DrawableLine.h"
#include "DrawableChar.h"

class DrawableTerminatedLine :
    public AbstractDrawable
{
public:
    enum class TERMINATION {
        START, END
    };

    DrawableTerminatedLine(const TERMINATION term, const DrawableLine& line);
    virtual ~DrawableTerminatedLine() = default;

    virtual void drawOnto(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const override;
private:
    const TERMINATION m_termination;
    const DrawableLine& m_line;
    const DrawableChar m_termChar;

    DrawableChar& getCharacter(TERMINATION term, DrawableLine::LineType type) const;
    void verticalStartDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;
    void verticalEndDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;
    void horizontalStartDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;
    void horizontalEndDraw(MatrixProxy& matrix, const uint8_t row, const uint8_t col) const;
};