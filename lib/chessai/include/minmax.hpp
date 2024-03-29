#ifndef MINIMAX_HPP
#define MINIMAX_HPP

#include "bitboard.hpp"
#include "bitboardtuple.hpp"
#include "score.hpp"
#include "search.hpp"
#include "statistics.hpp"

class MinMax : public Search {
  using T = Score;

  Statistics m_stat;

  T minimax(const BitBoard &bitboard, size_t depth, size_t max_depth) {
    if (depth == 0) {
      auto approx = T::GetStaticValue(bitboard, bitboard.CurrentColor(),
                                      GetSearchSettings().GetStage());
      return approx;
    }

    CheckStopFlag();

    auto nodes = bitboard.GenerateSubBoards(bitboard.CurrentColor());

    if (nodes.empty()) {
      if (bitboard.Checkmate()) return T::CheckMate(depth, max_depth);
      return T::Tie();
    }

    T bestscore = T::Min();
    for (auto &sub : nodes) {
      auto score = -minimax(sub, depth - 1, max_depth);
      bestscore = std::max(score, bestscore);
    }
    return bestscore;
  }
  std::vector<Turn> findpv(BitBoard board, int depth) {
    std::vector<Turn> turns_;

    for (size_t i = 0; i < depth; i++) {
      auto turn = minimaxturn({board}, depth - i, depth);
      if (turn.second == Turn()) break;
      turns_.push_back(turn.second);
      board.ExecuteTurn(turn.second);
    }

    return turns_;
  }

  std::pair<T, Turn> minimaxturn(const BitBoardTuple &tuple, int depthleft,
                                 int depthmax) {
    m_stat.MainNode();

    auto moves =
        BitBoardTuple::GenerateTuplesFast(tuple, tuple.board.CurrentColor());

    if (moves.empty()) {
      return {T(), Turn()};
    }

    T bestscore = T::Min();
    Turn turn = Turn();
    for (auto &sub : moves) {
      auto score = -minimax(sub.board, depthleft - 1, depthmax);
      if (score > bestscore) {
        bestscore = score;
        turn = sub.turn;
      }
    }
    return {bestscore, turn};
  }

 public:
  explicit MinMax(const BitBoard &board, Color color) : Search(board, color) {}

  T GetValue(int depth) {
    m_stat.Clear();
    return minimax(GetBoard(), depth, depth);
  }

  Turn GetTurn(int depth) {
    m_stat.Clear();
    return minimaxturn({GetBoard()}, depth, depth).second;
  }

  std::vector<Turn> FindPV(int depth) {
    m_stat.Clear();
    return findpv(GetBoard(), depth);
  }
};

#endif
