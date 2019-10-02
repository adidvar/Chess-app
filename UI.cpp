#include "UI.h"

namespace Chess{

	InputUI::InputUI()
	{
		
		
		
	}

	bool InputUI::GetTurn(Chess::Turn &t,Chess::Chessboard &board)
	{
		
	}

	void InputUI::RenderThread()
	{
		/*
		sf::Texture black_textures[7];
		sf::Texture white_textures[7];
		black_textures[1].loadFromFile("img/Black_Pawn.png");
		black_textures[2].loadFromFile("img/Black_Knight.png");
		black_textures[3].loadFromFile("img/Black_Bishop.png");
		black_textures[4].loadFromFile("img/Black_Rook.png");
		black_textures[5].loadFromFile("img/Black_Queen.png");
		black_textures[6].loadFromFile("img/Black_King.png");
		white_textures[1].loadFromFile("img/White_Pawn.png");
		white_textures[2].loadFromFile("img/White_Knight.png");
		white_textures[3].loadFromFile("img/White_Bishop.png");
		white_textures[4].loadFromFile("img/White_Rook.png");
		white_textures[5].loadFromFile("img/White_Queen.png");
		white_textures[6].loadFromFile("img/White_King.png");
		
		sf::RenderWindow w(sf::VideoMode(512,512,32),"Chess");
		
		Chess::MyBot b1;
		//Chess::MyBot b2;
		//Input b2(w);
		PythonConnector b2;
		Chess::ChessController c(b1,b2);
		Chess::Chessboard board;
		
		sf::RectangleShape rect(sf::Vector2f(64,64) );
		sf::Sprite sprite;
		
		
		while(1){
			
			w.clear();

			
			board = c.GetGlobalBoard();
			
			//відмальвка кольору
			bool IsWhite = false;
			for (int x = 0; x < 8; x++) 
			{
				for (int y = 0; y < 8; y++) 
				{
					IsWhite = !IsWhite;
					rect.setFillColor( IsWhite ? sf::Color(200,200,200) : sf::Color(50,50,50));
					rect.setPosition(sf::Vector2f(x*64,y*64));
					w.draw(rect);
				}
				IsWhite = !IsWhite;
			}
			
			for (int x = 0; x < 8; x++) 
			{
				for (int y = 0; y < 8; y++) 
				{
					sprite.setPosition(sf::Vector2f(x*64 + 7,y*64 + 7));
					sf::Texture &t = board.at(y,x).color == Chess::Color::White ? white_textures[board.at(y,x).type] : black_textures[board.at(y,x).type];
					sprite.setTexture(t , false);
					
					if(board.at(y,x).type != Chess::Emply)
						w.draw(sprite);
					
				}
			
			}
			
			w.display();
			
			c.NextMove();

			*/
	}

};