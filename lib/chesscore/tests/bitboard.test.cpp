#include <bitboard.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("BitBoard tests", "[bitboard]") {
  SECTION("Test of TestColor GetFigure GetColor") {
    BitBoard board;
    for (uint8_t i = 0; i < 64; i++) {
      for (uint8_t figure = 1; figure < 7; figure++) {
        for (uint8_t color = 0; color < 2; color++) {
          board.Set(i, {figure, color});

          REQUIRE(board.Test(figure, i));
          REQUIRE(board.TestColor(color, i));
          REQUIRE(board.GetFigure(i) == figure);
          REQUIRE(board.GetColor(i) == color);
          REQUIRE(board.GetCell(i).color == color);
          REQUIRE(board.GetCell(i).type == figure);
        }
      }
    }
  }

  SECTION("Test of TestEmp") {
    BitBoard board;
    for (int i = 0; i < 64; i++) {
      board.Set(i, {Figure::kEmpty, Color::kWhite});
      REQUIRE(board.TestEmp(i));
      REQUIRE(board.GetFigure(i) == (Figure)0);
    }
  }

  SECTION("Test of Swap") {
    BitBoard board;
    for (size_t i = 0; i < 64; i++) {
      for (size_t j = 0; j < 64; j++) {
        if (i == j) continue;

        board.Set(i, {Figure::kPawn, Color::kWhite});
        board.Set(j, {Figure::kKnight, Color::kBlack});
        board.Swap(i, j);
        REQUIRE(board.Test(Figure::kPawn, j));
        REQUIRE(board.Test(Figure::kKnight, i));
        REQUIRE(board.TestColor(Color::kBlack, i));
        REQUIRE(board.TestColor(Color::kWhite, j));
      }
    }
  }
  SECTION("Test of SkipMove CurrentColor OpponentColor") {
    BitBoard board;
    auto copy = board;
    copy.SkipMove();
    REQUIRE(board.CurrentColor() == copy.OpponentColor());
    REQUIRE(copy.CurrentColor() == board.OpponentColor());
  }
  SECTION("Test of ExecuteTurn GenerateTurns TestTurn") {
    BitBoard board;
    auto turns = board.GenerateTurns(board.CurrentColor());
    auto subboards = board.GenerateSubBoards(board.CurrentColor());
    REQUIRE(turns.size() == subboards.size());

    for (size_t i = 0; i < turns.size(); i++) {
      BitBoard sub = board;
      sub.ExecuteTurn(turns[i]);
      REQUIRE(sub.Fen() == subboards[i].Fen());
    }
  }
  SECTION("Test of el passant") {
    BitBoard board;
    REQUIRE(board.ExecuteTurn(Turn(Position(51), Position(35))));
    REQUIRE(board.LastPawnMove() == Position(43));
    REQUIRE(board.ExecuteTurn(Turn(Position(9), Position(17))));
    REQUIRE(board.LastPawnMove() == Position());
    board = BitBoard();
    board.SkipMove();
    REQUIRE(board.ExecuteTurn(Turn(Position(9), Position(25))));
    REQUIRE(board.LastPawnMove() == Position(17));
    REQUIRE(board.ExecuteTurn(Turn(Position(57), Position(42))));
    REQUIRE(board.LastPawnMove() == Position());
  }
  SECTION("Test of castling") {
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(60), Position(62))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(60), Position(58))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(60), Position(52))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(56), Position(48))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(63), Position(55))));
      REQUIRE(board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(56), Position(0))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().black_ooo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(63), Position(7))));
      REQUIRE(!board.RookingFlags().black_oo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(4), Position(6))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(4), Position(2))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(4), Position(12))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(0), Position(8))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(7), Position(15))));
      REQUIRE(board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(0), Position(56))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().white_ooo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.ExecuteTurn(Turn(Position(7), Position(63))));
      REQUIRE(!board.RookingFlags().black_oo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
  }
  SECTION("Test of MateTest End Checkmate WhiteWin BlackWin Tie") {
    {
      BitBoard board;
      REQUIRE(!board.MateTest());
      REQUIRE(!board.End());
      REQUIRE(!board.Checkmate());
      REQUIRE(!board.WhiteWin());
      REQUIRE(!board.BlackWin());
      REQUIRE(!board.Tie());
    }
    {
      BitBoard board(
          "rnb1kbnr/pppppppp/8/4q3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
      REQUIRE(board.MateTest());
      REQUIRE(!board.End());
      REQUIRE(!board.Checkmate());
      REQUIRE(!board.WhiteWin());
      REQUIRE(!board.BlackWin());
      REQUIRE(!board.Tie());
    }
    {
      BitBoard board(
          "rnb1k2r/pppppppp/6n1/4bq2/7K/8/PPPP1PPP/RNBQ1BNR w KQkq - 0 1");
      REQUIRE(board.MateTest());
      REQUIRE(board.End());
      REQUIRE(board.Checkmate());
      REQUIRE(!board.WhiteWin());
      REQUIRE(board.BlackWin());
      REQUIRE(!board.Tie());
    }
    {
      BitBoard board("rnb4r/ppppp3/6N1/4BQ2/7k/8/PPPP1PPP/RNBK3R b KQkq - 0 1");
      REQUIRE(board.MateTest());
      REQUIRE(board.End());
      REQUIRE(board.Checkmate());
      REQUIRE(board.WhiteWin());
      REQUIRE(!board.BlackWin());
      REQUIRE(!board.Tie());
    }
    {
      BitBoard board("k7/8/1Q6/2K5/8/8/8/8 b - - 0 1");
      REQUIRE(!board.MateTest());
      REQUIRE(board.End());
      REQUIRE(!board.Checkmate());
      REQUIRE(board.WhiteWin());
      REQUIRE(!board.BlackWin());
      REQUIRE(board.Tie());
    }
    {
      BitBoard board("K7/8/1q6/2k5/8/8/8/8 w - - 0 1");
      REQUIRE(!board.MateTest());
      REQUIRE(board.End());
      REQUIRE(!board.Checkmate());
      REQUIRE(!board.WhiteWin());
      REQUIRE(board.BlackWin());
      REQUIRE(board.Tie());
    }
  }
}

TEST_CASE("BitBoard fen tests", "[bitboard][fen]") {
  REQUIRE(BitBoard("8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 0").Fen() ==
          "8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 0").Fen() ==
          "8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/2p5/8/2R1P3/8/8/8 w - - 0 0").Fen() ==
          "8/8/2p5/8/2R1P3/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("2q5/8/8/8/8/8/4P3/3K4 w - - 0 0").Fen() ==
          "2q5/8/8/8/8/8/4P3/3K4 w - - 0 0");
  REQUIRE(BitBoard("8/8/8/4pP2/8/8/8/8 w - e6 0 0").Fen() ==
          "8/8/8/4pP2/8/8/8/8 w - e6 0 0");
  REQUIRE(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 0 0").Fen() ==
          "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 0 0");
  REQUIRE(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 0").Fen() ==
          "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 0");
  REQUIRE(BitBoard("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 0")
              .Fen() ==
          "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 0");
  REQUIRE(
      BitBoard(
          "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 0 0")
          .Fen() ==
      "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 0 0");
  REQUIRE(BitBoard(
              "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 0 0")
              .Fen() ==
          "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 0 0");
  REQUIRE(BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 0 0")
              .Fen() ==
          "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 0 0");
  REQUIRE(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4").Fen() ==
          "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 0 0");
  REQUIRE(BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 0 0")
              .Fen() ==
          "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 0 0");
  REQUIRE(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/"
                   "R4RK1 w - - 0 10")
              .Fen() ==
          "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - "
          "0 0");
  REQUIRE(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1").Fen() ==
          "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 0");
  REQUIRE(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1").Fen() ==
          "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1").Fen() ==
          "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 0");
  REQUIRE(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1").Fen() ==
          "5k2/8/8/8/8/8/8/4K2R w K - 0 0");
  REQUIRE(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1").Fen() ==
          "3k4/8/8/8/8/8/8/R3K3 w Q - 0 0");
  REQUIRE(BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1").Fen() ==
          "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 0");
  REQUIRE(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1").Fen() ==
          "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 0");
  REQUIRE(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1").Fen() ==
          "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 0");
  REQUIRE(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1").Fen() ==
          "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 0");
  REQUIRE(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1").Fen() ==
          "4k3/1P6/8/8/8/8/K7/8 w - - 0 0");
  REQUIRE(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1").Fen() ==
          "8/P1k5/K7/8/8/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1").Fen() ==
          "K1k5/8/P7/8/8/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1").Fen() ==
          "8/k1P5/8/1K6/8/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1").Fen() ==
          "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 0");
}

template <typename Board>
static size_t MovesCounterBase(Board board, size_t depth) {
  if (!depth) return 1;
  size_t counter = 0;
  for (auto board : board.GenerateSubBoards(board.CurrentColor())) {
    counter += MovesCounterBase(board, depth - 1);
  }
  return counter;
}

template <typename Board>
static size_t MovesCounterFrom(Board board, size_t depth,
                               uint64_t maskfrom = 1) {
  if (!depth) return 1;
  size_t counter = 0;
  auto boards1 = board.GenerateSubBoards(board.CurrentColor(), maskfrom);
  auto boards2 = board.GenerateSubBoards(board.CurrentColor(), ~maskfrom);
  maskfrom = maskfrom * 3334343 + 1341324;
  for (auto board : boards1) {
    counter += MovesCounterFrom(board, depth - 1, maskfrom);
  }
  for (auto board : boards2) {
    counter += MovesCounterFrom(board, depth - 1, maskfrom);
  }
  return counter;
}

template <typename Board>
static size_t MovesCounterTo(Board board, size_t depth, uint64_t maskto = 1) {
  if (!depth) return 1;
  size_t counter = 0;
  auto boards1 = board.GenerateSubBoards(board.CurrentColor(), kall, maskto);
  auto boards2 = board.GenerateSubBoards(board.CurrentColor(), kall, ~maskto);
  maskto = maskto * 3334343 + 1341324;
  for (auto board : boards1) {
    counter += MovesCounterTo(board, depth - 1, maskto);
  }
  for (auto board : boards2) {
    counter += MovesCounterTo(board, depth - 1, maskto);
  }
  return counter;
}

TEST_CASE("BitBoard generation tests", "[bitboard][generation]") {
  REQUIRE(MovesCounterBase(BitBoard(), 1) == 20);
  REQUIRE(MovesCounterTo(BitBoard(), 1) == 20);
  REQUIRE(MovesCounterFrom(BitBoard(), 1) == 20);

  REQUIRE(MovesCounterBase(BitBoard(), 2) == 400);
  REQUIRE(MovesCounterTo(BitBoard(), 2) == 400);
  REQUIRE(MovesCounterFrom(BitBoard(), 2) == 400);

  REQUIRE(MovesCounterBase(BitBoard(), 3) == 8902);
  REQUIRE(MovesCounterTo(BitBoard(), 3) == 8902);
  REQUIRE(MovesCounterFrom(BitBoard(), 3) == 8902);
}

TEST_CASE("BitBoard generation advanced tests", "[bitboard][generation]") {
  REQUIRE(MovesCounterBase(BitBoard(), 4) == 197281);
  REQUIRE(MovesCounterTo(BitBoard(), 4) == 197281);
  REQUIRE(MovesCounterFrom(BitBoard(), 4) == 197281);

  REQUIRE(MovesCounterBase(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"),
                           1) == 8);
  REQUIRE(MovesCounterTo(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"),
                         1) == 8);
  REQUIRE(MovesCounterFrom(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"),
                           1) == 8);

  REQUIRE(MovesCounterBase(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"), 1) ==
          8);
  REQUIRE(MovesCounterTo(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"), 1) ==
          8);
  REQUIRE(MovesCounterFrom(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"), 1) ==
          8);

  REQUIRE(MovesCounterBase(
              BitBoard(
                  "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"),
              1) == 19);
  REQUIRE(MovesCounterTo(
              BitBoard(
                  "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"),
              1) == 19);
  REQUIRE(MovesCounterFrom(
              BitBoard(
                  "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"),
              1) == 19);

  REQUIRE(MovesCounterBase(BitBoard("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/"
                                    "PPPBBPPP/R3K2R b KQkq - 3 2"),
                           1) == 5);
  REQUIRE(MovesCounterTo(BitBoard("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/"
                                  "PPPBBPPP/R3K2R b KQkq - 3 2"),
                         1) == 5);
  REQUIRE(MovesCounterFrom(BitBoard("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/"
                                    "PPPBBPPP/R3K2R b KQkq - 3 2"),
                           1) == 5);

  REQUIRE(MovesCounterBase(BitBoard("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/"
                                    "PPPBBPPP/R3K2R b KQ - 3 2"),
                           1) == 44);
  REQUIRE(MovesCounterTo(BitBoard("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/"
                                  "PPPBBPPP/R3K2R b KQ - 3 2"),
                         1) == 44);
  REQUIRE(MovesCounterFrom(BitBoard("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/"
                                    "PPPBBPPP/R3K2R b KQ - 3 2"),
                           1) == 44);

  REQUIRE(
      MovesCounterBase(
          BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"),
          1) == 39);
  REQUIRE(
      MovesCounterTo(
          BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"),
          1) == 39);
  REQUIRE(
      MovesCounterFrom(
          BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"),
          1) == 39);

  REQUIRE(MovesCounterBase(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"), 1) ==
          9);
  REQUIRE(MovesCounterTo(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"), 1) ==
          9);
  REQUIRE(MovesCounterFrom(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"), 1) ==
          9);

  REQUIRE(
      MovesCounterBase(
          BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),
          3) == 62379);
  REQUIRE(
      MovesCounterTo(
          BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),
          3) == 62379);
  REQUIRE(
      MovesCounterFrom(
          BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),
          3) == 62379);

  REQUIRE(MovesCounterBase(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/"
                                    "P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"),
                           3) == 89890);
  REQUIRE(MovesCounterTo(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/"
                                  "P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"),
                         3) == 89890);
  REQUIRE(MovesCounterFrom(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/"
                                    "P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"),
                           3) == 89890);

  REQUIRE(MovesCounterBase(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"), 6) ==
          1134888);
  REQUIRE(MovesCounterTo(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"), 6) ==
          1134888);
  REQUIRE(MovesCounterFrom(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"), 6) ==
          1134888);

  REQUIRE(MovesCounterBase(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"), 6) ==
          1015133);
  REQUIRE(MovesCounterTo(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"), 6) ==
          1015133);
  REQUIRE(MovesCounterFrom(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"), 6) ==
          1015133);

  REQUIRE(MovesCounterBase(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"),
                           6) == 1440467);
  REQUIRE(MovesCounterTo(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"), 6) ==
          1440467);
  REQUIRE(MovesCounterFrom(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"),
                           6) == 1440467);

  REQUIRE(MovesCounterBase(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1"), 6) ==
          661072);
  REQUIRE(MovesCounterTo(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1"), 6) ==
          661072);
  REQUIRE(MovesCounterFrom(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1"), 6) ==
          661072);

  REQUIRE(MovesCounterBase(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"), 6) ==
          803711);
  REQUIRE(MovesCounterTo(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"), 6) ==
          803711);
  REQUIRE(MovesCounterFrom(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"), 6) ==
          803711);

  REQUIRE(MovesCounterBase(
              BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"), 4) ==
          1274206);
  REQUIRE(MovesCounterTo(BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"),
                         4) == 1274206);
  REQUIRE(MovesCounterFrom(
              BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"), 4) ==
          1274206);

  REQUIRE(MovesCounterBase(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"),
                           4) == 1720476);
  REQUIRE(MovesCounterTo(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"),
                         4) == 1720476);
  REQUIRE(MovesCounterFrom(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"),
                           4) == 1720476);

  REQUIRE(MovesCounterBase(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"), 6) ==
          3821001);
  REQUIRE(MovesCounterTo(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"), 6) ==
          3821001);
  REQUIRE(MovesCounterFrom(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"), 6) ==
          3821001);

  REQUIRE(MovesCounterBase(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"),
                           5) == 1004658);
  REQUIRE(MovesCounterTo(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"), 5) ==
          1004658);
  REQUIRE(MovesCounterFrom(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"),
                           5) == 1004658);

  REQUIRE(MovesCounterBase(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1"), 6) ==
          217342);
  REQUIRE(MovesCounterTo(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1"), 6) ==
          217342);
  REQUIRE(MovesCounterFrom(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1"), 6) ==
          217342);

  REQUIRE(MovesCounterBase(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1"), 6) ==
          92683);
  REQUIRE(MovesCounterTo(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1"), 6) ==
          92683);
  REQUIRE(MovesCounterFrom(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1"), 6) ==
          92683);

  REQUIRE(MovesCounterBase(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1"), 6) ==
          2217);
  REQUIRE(MovesCounterTo(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1"), 6) == 2217);
  REQUIRE(MovesCounterFrom(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1"), 6) ==
          2217);

  REQUIRE(MovesCounterBase(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"), 7) ==
          567584);
  REQUIRE(MovesCounterTo(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"), 7) ==
          567584);
  REQUIRE(MovesCounterFrom(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"), 7) ==
          567584);

  REQUIRE(MovesCounterBase(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"), 4) ==
          23527);
  REQUIRE(MovesCounterTo(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"), 4) ==
          23527);
  REQUIRE(MovesCounterFrom(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"), 4) ==
          23527);
}

template <typename T>
void TestTurn(T board) {
  auto mysubs = board.GenerateSubBoards(board.CurrentColor());
  auto opsubs = board.GenerateSubBoards(board.OpponentColor());
  auto myturns = board.GenerateTurns(board.CurrentColor());
  auto opturns = board.GenerateTurns(board.OpponentColor());
  REQUIRE(mysubs.size() == myturns.size());
  REQUIRE(opsubs.size() == opturns.size());

  for (size_t i = 0; i < mysubs.size(); ++i) {
    T temp(board);
    temp.ExecuteTurn(myturns[i]);
    REQUIRE(temp.Fen() == mysubs[i].Fen());
  }
}

TEST_CASE("BitBoard turns generation tests", "[bitboard][generation][turn]") {
  (TestTurn(BitBoard()));
  (TestTurn(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2")));
  (TestTurn(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3")));
  (TestTurn(
      BitBoard("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2")));
  (TestTurn(BitBoard(
      "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2")));
  (TestTurn(BitBoard(
      "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2")));
  (TestTurn(
      BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9")));
  (TestTurn(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4")));
  (TestTurn(
      BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8")));
  (
      TestTurn(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/"
                        "1PP1QPPP/R4RK1 w - - 0 10")));
  (TestTurn(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1")));
  (TestTurn(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1")));
  (TestTurn(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1")));
  (TestTurn(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1")));
  (TestTurn(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1")));
  (TestTurn(BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1")));
  (TestTurn(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1")));
  (TestTurn(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1")));
  (TestTurn(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1")));
  (TestTurn(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1")));
  (TestTurn(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1")));
  (TestTurn(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1")));
  (TestTurn(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1")));
  (TestTurn(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1")));
}
