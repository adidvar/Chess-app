#include "turn.h"

Turn::Turn():
    type(ErrorMove)
{}

Turn::Turn(Pos p1, Pos p2):
    type(Move),
    position_1(p1),
    position_2(p2)
{}

Turn::Turn(Pos p1, Pos p2, Figures figure):
    type(PawnMove),
    position_1(p1),
    position_2(p2),
    figure_type(figure)
{}

Turn::Turn(bool rooking):
    type(Rooking),
    rooking_type(rooking)
{}

#define N(p)  '(' <<p / 8  << ':' << p%8 << ')'

std::string Turn::toStr()
{
    std::stringstream ss;
    switch (type) {
    case Move:
        ss << '<' << N(position_1) << ':' << N(position_2) << '>';
        break;
    case PawnMove:

        ss << '<' << N(position_1) << ':' << N(position_2) << ':' << figure_type << '>';
        break;


        break;
    case Rooking:
        ss << '<' << (rooking_type ? "l" : "r" )<< '>';
        break;
    case ErrorMove:
        ss << '<' << "Error" << '>';
    default:
        break;
    }
    return ss.str();
}

bool Turn::operator ==(const Turn &t) const
{
    if(this->type == t.type)
    {
        switch (this->type) {
        case ErrorMove:
            return true;
            break;
        case PawnMove:
            if(this->position_1 == t.position_1 && this->position_2 == t.position_2 && this->figure_type == t.figure_type)return true;
            break;
        case Move:
            if(this->position_1 == t.position_1 && this->position_2 == t.position_2)return true;
            break;
        case Rooking:
            if(this->rooking_type == t.rooking_type)return true;
            break;

        }
    }
    return false;
}
