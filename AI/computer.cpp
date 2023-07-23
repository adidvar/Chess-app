#include "computer.hpp"
#include "alphabeta.hpp"

/*
Computer::Computer(Match &match, Color color):
    match_(match),
    table_(),
    color_(color)
{
}

Computer::~Computer()
{
}

void Computer::Wait()
{
}

bool Computer::Ready()
{
    return false;
}

Turn Computer::GetBestTurn()
{
    table_.Clear();
    assert(color_ == match_.GetBoard().CurrentColor());
    AlphaBeta<MainAppraiser> ab(color_,stat_, table_);
    return ab.GetBestTurn(match_.GetBoard(), 5);
}

Statistics Computer::GetStatistics()
{
    return stat_;
}
*/

Computer::Computer() {}

Computer::~Computer() {}

void Computer::NewGame() {}

void Computer::Position(const Match &match) { match_ = match; }

void Computer::Go() {
  turn_future_ = std::async(
      std::launch::async,
      [](Match match) {
        Statistics stat_;
        TransPositionTable table_;
        AlphaBeta<MainAppraiser> ab(match.GetBoard().CurrentColor(), stat_,
                                    table_);
        return ab.GetBestTurn(match.GetBoard(), 3);
      },
      match_);
}

void Computer::Wait() { turn_future_.wait(); }

bool Computer::Ready() {
  auto result = turn_future_.wait_for(chrono::seconds{0});

  return result == std::future_status::ready;
}

Turn Computer::GetBestTurn() { return turn_future_.get(); }

Statistics Computer::GetStatistics() { return stat_; }
