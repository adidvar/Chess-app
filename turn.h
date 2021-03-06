#ifndef TURN_H
#define TURN_H
#include "board.h"
#include <string>
#include <sstream>

enum TurnType
{
    Move = 0,
    PawnMove = 1,
    Rooking = 2,
    ErrorMove = 3
};

class Turn
{
    uint8_t type;
    union
    {
        struct
        {
            Pos position_1;
            Pos position_2;
            Figures figure_type;
        };
        bool rooking_type;
    };



public:
    Turn();
    Turn(Pos p1 , Pos p2);
    Turn(Pos p1 , Pos p2 , Figures figure);
    Turn(bool rooking);

    bool Normal(){return type != ErrorMove;}

    std::string toStr();

    bool operator ==(const Turn &t) const;

    friend class TurnExecutor;
    friend class Map_preview;
};

#endif // TURN_H
