#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <bitboard.hpp>
#include <statistics.hpp>

template<typename T, typename S = NoStatistics>
class AlphaBeta
{
    Color color_;
    S &stat_;

    //T minimax(const BitBoard& bitboard, size_t depth);
    T alphabeta(const BitBoard& bitboard, size_t depth, T a, T b)
    {
        stat_.NewNodeEvent();
        stat_.GenerationTimerBegin();
        auto nodes = bitboard.GenerateSubBoards();
        stat_.GenerationTimerEnd();

        bool zero_moves = (nodes.size() == 0);
        bool mate = bitboard.MateTest();

        if(zero_moves && !mate){
            return T::Tie();
        } else if(zero_moves && mate){
            return bitboard.CurrentColor() == color_ ? T::CheckMateLose() : T::CheckMateWin();
        } else if(depth == 0){

            stat_.NewApproximationEvent();
            stat_.ApproximationTimerBegin();
            auto approx = T::Approximate(bitboard, color_);
            stat_.ApproximationTimerEnd();

            return approx;
        }

        if(bitboard.CurrentColor() == color_){
            T value = T::Min();
            for( const auto&node : nodes)
            {
                value = std::max(value, alphabeta(node, depth - 1,a ,b).Process());
                a = std::max(a,value);
                if(b <= a)
                    break;
            }
            return value;
        } else {
            T value = T::Max();
            for( const auto&node : nodes)
            {
                value = std::min(value, alphabeta(node, depth - 1,a ,b).Process());
                b = std::min(b,value);
                if(b <= a)
                    break;
            }
            return value;
        }
    }

public:

    AlphaBeta(Color color, S &s):
    color_(color),
      stat_(s)
    {}

    T Evaluate(BitBoard board, int depth)
    {
        stat_.Clear();
        stat_.AllTimerBegin();
        auto value = alphabeta(board,depth, T::Min() , T::Max());
        stat_.AllTimerEnd();
        return value;
    }

    static T Evaluate(BitBoard board, Color color, int depth, S &stat ){
         AlphaBeta core(color,stat);
         return core.Evaluate(board,depth);
    }
};

#endif
