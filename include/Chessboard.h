#include "Chess.h"
#include <array>

namespace Chess{
	
	class ChessController;
	class AbstractBot;

	class Chessboard
	{
		public:

			bool whiteTurn = true; // чий хід
			bool whiteCastling = true; // біла рокіровка 
			bool blackCastling = true; // чорна рокіровка
            std::array<Figure,64> map;
			
            bool makeTurn(Turn turn){return makeTurn(turn,true);};
            bool makeTurn(Turn turn , bool SafeMode);
			
			
			Chessboard();
			Chessboard(const Chessboard&); // конструктор копіювання
				
			bool getCurrentTurns(std::vector<Turn> &turns);
            Figure& at(unsigned x , unsigned y);
            Figure& at(Position p);
			virtual void reset();
			Chessboard& operator= (const Chessboard &str); // оператор копіювання
			Chessboard operator ~() const;
		
		
	};
	
}

