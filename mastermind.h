#pragma once
#include<SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include<algorithm>
#include<cassert>
#include <cstdlib>
#include <ctime>
#include<unordered_set>
#include<iostream>
#include<vector>
#include<random>
#include<iterator>




/// @brief ©2023 Mohammadreza Mirzaee  "All Rights Reserved"


const sf::Vector2f startPoint(72.0, 36.0);





   struct Guess {
    
        std::unordered_set<int>guess;
        int blacks;
        int whites;

    };

   enum class Player
   {
       CPU=0,
       HUMAN
   };

   enum class Colors
   {
       RED = 0,
       GREEN,
       WHITE,
       BLACK,
       BLUE,
       YELLOW
   };
   int randomNumber();
   std::unordered_set<int> randomSet();
   int whiteCounter(const std::unordered_set<int>&, const std::unordered_set<int>&);
   int blackCounter(const std::unordered_set<int>&, const std::unordered_set<int>&);
   Guess attempt(const std::unordered_set<int>&);
   void updateState(const std::unordered_set<int>&,struct State&);
    class MasterMind
    {
    public:
        MasterMind();
        ~MasterMind();
        
        void setTheAnswer();
        std::unordered_set<int> getTheAnswer() const;
     
    private:
        std::unordered_set<int>m_answer;

    };
    struct State
    {
        int roundNumber;
        std::vector<Guess>previous;


    public:
        

        State(int roundNumber = 0, const std::vector<Guess>& previous = {})
            : roundNumber(roundNumber), previous(previous)
        {
        }
    };
    
    

    //Graphics Logics
    
    class Pin : public sf::CircleShape
    {
    public:
        Pin() :lock(false), picked(false),carried(false),ref(true),place_holder(0) {}
        ~Pin(){}
        
        
        bool operator==(const Pin& rhs)
        {
            if (lock == rhs.lock && picked == rhs.picked && getPosition() == rhs.getPosition() && getOrigin() == rhs.getOrigin()
                && getRadius() == rhs.getRadius() && getOutlineColor() == rhs.getOutlineColor() &&getOutlineThickness() == rhs.getOutlineThickness()
                && getFillColor() == rhs.getFillColor() && carried == rhs.carried && ref ==rhs.ref && place_holder==rhs.place_holder)
                return true;
            else
                return false;
        }

        bool mouseOver(sf::Vector2f&)const;
        bool lock;
        bool picked;
        bool carried;
        bool ref;
        int place_holder;
    };
     
    
    class PinCreator
    {
    public:
        Pin create(const sf::Color&, const sf::Vector2f& pos);
        
    };

   

   
        
    
    
    class Game {
    public:

        Game();
        ~Game();
        void draw(std::vector<Pin>&);
         
        
        void loop();
        void pick(Pin&);
        void drag(Pin&);
        void drop(Pin&);
        void delet(Pin&);
        void update(const MasterMind&,State&);
        bool finished();
        int colorToInt(Pin&);
        int vec2fToPlace(const sf::Vector2f&);



        std::vector<Pin> pins;
        Pin gamePins[6];
        sf::VertexArray points{ sf::Points,36 };
        
        
        sf::RenderWindow m_window;
        sf::Event m_event;
        sf::RectangleShape m_board;
        sf::Texture boardtexture;
        std::vector<std::pair<sf::Vector2f,int>> m_active_holes;
        std::vector<sf::Vector2f>scorePoints;
        std::vector<sf::CircleShape> scores;

        int number_of_pins;
        int prev;
        float fps;
        sf::Clock clock;  
        sf::Time previousTime;
        sf::Time currentTime;
        sf::Text text;
        sf::Font font;
        
    };

