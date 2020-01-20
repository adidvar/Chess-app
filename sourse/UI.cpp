﻿#include "UI.h"
#include <chrono>
#include <iostream>

namespace Chess{

    InputUI::InputUI(Color c,sf::RenderWindow *win):
        APlayer(c),
        w(win)
    {
        w->setActive(false);
        th = new std::thread(&InputUI::RenderThread,this);
    }

    InputUI::~InputUI()
    {
        th->join();
        delete th;
        w->setActive(true);
    }

    void InputUI::MapEvent(Chessboard board)
    {
         map_mtx.lock();
         map = board;
         map_mtx.unlock();
    }

    void InputUI::FinishEvent(Color c)
    {
        stat_mtx.lock();
        stat = c == Color::White ? MatchStatus::Win : MatchStatus::Lose ;
        stat_mtx.unlock();
    }

    bool InputUI::GetTurn(Chess::Turn &t)
    {
        while(1){
            turnBuffer_mtx.lock();
            if(turnBuffer.size() > 0){
                t = turnBuffer.front();
                turnBuffer.erase(turnBuffer.begin());
                turnBuffer_mtx.unlock();
                break;
            }
            turnBuffer_mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        return true;
    }

    void InputUI::RenderThread()
    {
        sf::Font font;
        font.loadFromFile("sourse/Font.ttf");

        bool FigureChoiseMenu = false;

        std::vector<Position> p;
        sf::Texture textures[2][7];
        sf::Sprite sprites[2][7];
        int twidth = 50;
        int theight = 50;

        //White
        //textures[0][0].loadFromFile("Texture/WEmpty.png");
        textures[color][1].loadFromFile("sourse/WPawn.png");
        textures[color][2].loadFromFile("sourse/WKnight.png");
        textures[color][3].loadFromFile("sourse/WBishop.png");
        textures[color][4].loadFromFile("sourse/WRook.png");
        textures[color][5].loadFromFile("sourse/WQueen.png");
        textures[color][6].loadFromFile("sourse/WKing.png");
        //Black
        //textures[1][0].loadFromFile("sourse/BEmpty.png");
        textures[!color][1].loadFromFile("sourse/BPawn.png");
        textures[!color][2].loadFromFile("sourse/BKnight.png");
        textures[!color][3].loadFromFile("sourse/BBishop.png");
        textures[!color][4].loadFromFile("sourse/BRook.png");
        textures[!color][5].loadFromFile("sourse/BQueen.png");
        textures[!color][6].loadFromFile("sourse/BKing.png");

        for(int i=0;i<2;i++)
        {
            for(int j=0;j<7;j++)
            {
                sprites[i][j].setTexture(textures[i][j]);
            }
        }

        w->setActive(true);

        sf::RenderWindow *window = w;
        while (window->isOpen() )
        {
           sf::Event event;
           while (window->pollEvent(event))
           {
               if( (GetStat()!=Chess::Now ) && (event.type == sf::Event::MouseButtonReleased) )
               {
                   goto end;
               }
               if(event.type == sf::Event::MouseButtonPressed && FigureChoiseMenu == false)
                {
                    p.push_back(Position(event.mouseButton.y / 80,event.mouseButton.x / 80));

                    if(p.size() >= 2)
                    {
                        map_mtx.lock();
                        if(p[0] == Position(7,4) && (p[1] == Position(7,2) || p[1] == Position(7,6)) && this->color == Color::White ) // рокіровка
                        {
                            turnBuffer_mtx.lock();
                            if(p[1] == Position(7,2))
                            {
                                turnBuffer.push_back(Turn(true));
                            } else if (p[1] == Position(7,6))
                            {
                                turnBuffer.push_back(Turn(false));
                            }
                            p.clear();
                            turnBuffer_mtx.unlock();

                        } else if(p[0] == Position(7,3) && (p[1] == Position(7,1) || p[1] == Position(7,5)) && this->color == Color::Black  ) // рокіровка
                        {
                            turnBuffer_mtx.lock();
                            if(p[1] == Position(7,1))
                            {
                                turnBuffer.push_back(Turn(true));
                            } else if (p[1] == Position(7,5))
                            {
                                turnBuffer.push_back(Turn(false));
                            }
                            p.clear();
                            turnBuffer_mtx.unlock();

                        }
                        else if (p[0].x == 1 && map.at(p[0]).type == Pawn )
                        {
                            FigureChoiseMenu = true;
                        } else {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1]));
                            p.clear();
                            turnBuffer_mtx.unlock();
                        }
                        map_mtx.unlock();
                    }
                }
               if(event.type == sf::Event::MouseButtonPressed && FigureChoiseMenu == true)
                {
                    int x = event.mouseButton.x , y = event.mouseButton.y;

                    if( y > 320 && y < 420){
                        if( x > 105 && x < 205)
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Knight));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;
                        } else if ( x > 215 && x < 315 )
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Bishop));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;
                        } else if ( x > 325 && x < 425 )
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Rook));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;

                        }else if( x > 435 && x < 535  )
                        {
                            turnBuffer_mtx.lock();
                            turnBuffer.push_back(Turn(p[0],p[1],Queen));
                            p.clear();
                            turnBuffer_mtx.unlock();
                            FigureChoiseMenu = false;
                        }
                    }
                }
           }
           window->clear(sf::Color(128,128,128));

           if(GetStat() == Now && FigureChoiseMenu == false)
           {

           sf::RectangleShape MapRectangle(sf::Vector2f(80,80));
           bool IsWhite = false;
           for (int x = 0; x < 8; x++)
           {
               for (int y = 0; y < 8; y++)
               {
                   IsWhite = !IsWhite;
                   MapRectangle.setFillColor( IsWhite ? sf::Color(200,200,200) : sf::Color(50,50,50));
                   MapRectangle.setPosition(sf::Vector2f(x*80,y*80));
                   window->draw(MapRectangle);
               }
               IsWhite = !IsWhite;
           }

           map_mtx.lock();
           sf::Sprite figure;
           for (int x = 0; x < 8; x++)
           {
               for (int y = 0; y < 8; y++)
               {
                   auto fgr = map.at(x,y);
                   int color_id = fgr.color ; int figure_id = fgr.type;
                   sprites[color_id][figure_id].setScale(sf::Vector2f((float)70/textures[color_id][figure_id].getSize().x,(float)70/textures[color_id][figure_id].getSize().y));
                   sprites[color_id][figure_id].setPosition(sf::Vector2f(y*80 + 5,x*80+5));
                   if( figure_id != Chess::Emply)
                       window->draw(sprites[color_id][figure_id]);
               }
          }
          map_mtx.unlock();
          }
          else if(GetStat() == Win)
          {
               sf::Text text("You win",font,30);
               text.setPosition(260,300);
               window->draw(text);
               window->display();
          }
          else if(GetStat() == Lose)
          {
               sf::Text text( "You lose" ,font,30);
               text.setPosition(260,300);
               window->draw(text);
               window->display();
          }
          else if(GetStat() == Now && FigureChoiseMenu == true)
          {
            sf::Text text("Choise figure",font,30);
            text.setFont(font);
            text.setPosition(240,260);
            window->draw(text);

            sf::Sprite figure;

            figure.setScale((float)100/textures[!map.WhoTurn()][2].getSize().x,(float)100/textures[!map.WhoTurn()][2].getSize().y);
            figure.setTexture(textures[!map.WhoTurn()][2]);
            figure.setPosition(105,320);
            window->draw(figure);

            figure.setScale((float)100/textures[!map.WhoTurn()][3].getSize().x,(float)100/textures[!map.WhoTurn()][3].getSize().y);
            figure.setTexture(textures[!map.WhoTurn()][3]);
            figure.setPosition(215,320);
            window->draw(figure);

            figure.setScale((float)100/textures[!map.WhoTurn()][4].getSize().x,(float)100/textures[!map.WhoTurn()][4].getSize().y);
            figure.setTexture(textures[!map.WhoTurn()][4]);
            figure.setPosition(325,320);
            window->draw(figure);

            figure.setScale((float)100/textures[!map.WhoTurn()][5].getSize().x,(float)100/textures[!map.WhoTurn()][5].getSize().y);
            figure.setTexture(textures[!map.WhoTurn()][5]);
            figure.setPosition(435,320);
            window->draw(figure);
          }

           window->display();

           std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        end:
        w->setActive(false);
    }

};
