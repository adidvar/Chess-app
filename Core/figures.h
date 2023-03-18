#ifndef FIGURES_H
#define FIGURES_H
#include <inttypes.h>

///  Тип данних для зберігання фігур
/// \brief Типи фігур
class Figure
{
    uint8_t value_;
public:
    static constexpr uint8_t kEmpty = 0; ///<Пуста клітинка
    static constexpr uint8_t kPawn = 1; ///< Пішка
    static constexpr uint8_t kKnight = 2; ///< Кінь
    static constexpr uint8_t kBishop = 3;///< Офіцер
    static constexpr uint8_t kRook = 4; ///< Тура
    static constexpr uint8_t kQueen = 5;///< Королева
    static constexpr uint8_t kKing = 6; ///< Король
    Figure(uint8_t value) noexcept:
        value_(value)
    {
    }
    Figure() noexcept:
        value_(kEmpty)
    {
    }
    operator uint8_t() const noexcept{
        return value_;
    }
};

/// \brief Тип данних для зберігання кольору фігури
/// Описує кольори фігур

class Color
{
    uint8_t value_;
public:
    static constexpr uint8_t kWhite = 0; ///<Пуста клітинка
    static constexpr uint8_t kBlack = 1; ///< Пішка
    Color(uint8_t value) noexcept:
        value_(value)
    {
    }
    Color() noexcept:
        value_(kWhite)
    {
    }
    operator uint8_t() const noexcept{
        return value_;
    }
};

#endif // FIGURES_H
