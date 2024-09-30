
#include "mastermind.h"


    MasterMind::MasterMind()
    {
    }
    MasterMind::~MasterMind()
    {
    }
    int randomNumber()
    {
        std::uniform_int_distribution<int> uni(0, 5);
        std::random_device                  rand_dev;
        std::mt19937                        generator(rand_dev());
        return uni(generator);

    }
    std::unordered_set<int> randomSet()
    {
        std::uniform_int_distribution<int> uni(0, 5);
        std::random_device                  rand_dev;
        std::mt19937                        generator(rand_dev());
        std::unordered_set<int>answer;
        while (answer.size() < 4)
        {
            answer.insert(uni(generator));

        }
        return answer;

    }
    void MasterMind::setTheAnswer()
    {

        m_answer = randomSet();
    }
    std::unordered_set<int> MasterMind::getTheAnswer()const
    {
        return m_answer;
    }
    int whiteCounter(const std::unordered_set<int>& s,const std::unordered_set<int>& w)
    {
        std::unordered_set<int>temp{};
        for (auto& item : s)
            temp.insert(item);
        for (auto& item : w)
            temp.insert(item);
        return (int)(8 - temp.size());
    }
    int blackCounter(const std::unordered_set<int>& s, const std::unordered_set<int>& w)
    {
        int blacks{ 0 };
        auto its = s.begin();
        auto itw = w.begin();
        for (; its != s.end() && itw != w.end(); ++its, ++itw)
        {
            if (*its == *itw)
                blacks++;
        }
        return blacks;
     
    }
   
    void updateState(const Guess& s ,State& state)
    {
        state.roundNumber += 1;
        state.previous.push_back(s);
    }
     


     //  Graphics logic
     Game::Game():m_window{ sf::VideoMode(422, 750), "MasterMind" }, m_event{}, pins{},number_of_pins{(int)pins.size()},
         m_active_holes{ {{72.0,566.0},-1},{{127.0,566.0},-1},{{183.0,566.0},-1},{{239.0,566.0},-1} }, text(), fps{}, m_board{}
     {
         
         std::cout << "ctor is called\n";
         scorePoints = {{319.0,578.0},{349.0,578},{319.0,555.0},{349.0,555.0}};//66 up

         // fps measuring
         clock = sf::Clock::Clock();
         text.setPosition(10.0f,10.0f);
         font.loadFromFile("assets/font.otf");
         text.setFont(font);
         previousTime = clock.getElapsedTime();
         
         // Board grapics initiazied

         
         boardtexture.loadFromFile("assets/board.jpeg");
         
         
         m_board.setSize(sf::Vector2f(422.0f, 750.0f));
         m_board.setTexture(&boardtexture);

         // Making game points
         int counter = 0;
         for (int j =(int) startPoint.y; j < 10; j++)
         {
             for (int i = (int)startPoint.x; i < 4; i++)
             {
                 points[counter].position = sf::Vector2f((startPoint.x) + (i * 55), ((startPoint.y) + (j * 67)));
                 counter++;
             }
         }

        
         //Making Game Pins
         PinCreator p;
         sf::Vector2f temp(75.0, 640.0);
         gamePins[0] = p.create(sf::Color::Red, temp);
         temp.x += 52.0;
         gamePins[1] = p.create(sf::Color::Green, temp);
         temp.x += 52.0;
         gamePins[2] = p.create(sf::Color::White, temp);
         temp.x += 52.0;
         gamePins[3] = p.create(sf::Color::Black, temp);
         temp.x += 52.0;
         gamePins[4] = p.create(sf::Color::Blue, temp);
         temp.x += 52.0;
         gamePins[5] = p.create(sf::Color::Yellow, temp);

         //Registering the game pins
         for (const auto& item : gamePins)
             pins.push_back(item);
         
     }//End of Game ctor
     Game::~Game()
     {
         std::cout << "dtor is called\n";
     }


     

     void Game::loop()
     {
         
         //New edition
         MasterMind M;
         State state;
         M.setTheAnswer();
         std::unordered_set<int>theAnswer = M.getTheAnswer();
         for (const auto& item : theAnswer)
             std::cout << item << " ";
         std::cout << std::endl;
         

         while (m_window.isOpen())
         {

             
             
             // fps stuff
             currentTime = clock.getElapsedTime();
             fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
             
             previousTime = currentTime;
             //pin.setPosition((float)sf::Mouse::getPosition(window).x,(float)sf::Mouse::getPosition(window).y); 
             
             while (m_window.pollEvent(m_event))
             {
                 if (m_event.type == sf::Event::Closed)
                     m_window.close();


                 for (auto& item : this->pins)
                 {
                     pick(item);
                     drag(item);
                     drop(item);
                     update(M,state);
                     delet(item);
                 }
             }

             m_window.clear();
             m_window.draw(m_board);

             text.setString(std::to_string(fps));
             m_window.draw(text);
            

             for (const auto& item : this->pins)
                 m_window.draw(item);
             for (const auto& item : scores)
                 m_window.draw(item);

             m_window.display();


             number_of_pins = pins.size();
             if(prev!=number_of_pins)
             std::cout << "Number of pins : "<<number_of_pins << '\n';
             prev = number_of_pins;

             

         }
         
     }
    
     Pin PinCreator::create(const sf::Color& col, const sf::Vector2f& pos)
     {

         Pin pin;
         pin.setPosition(pos);
         pin.setOrigin(20, 20);
         pin.setRadius(21);
         pin.setOutlineColor(sf::Color::Black);
         pin.setOutlineThickness(2);
         pin.setFillColor(col);
         pin.lock = false;
         pin.ref = true;
         return pin;
     }
    
     bool Pin::mouseOver( sf::Vector2f& pos)const
     {
         const sf::Vector2f pinpos = this->getPosition();
         if (((pos.x > pinpos.x - 21.0) &&(pos.x < pinpos.x + 21.0)) && ((pos.y > pinpos.y - 21.0)&& (pos.y < pinpos.y + 21.0)))
             return true;
         return false;
        
     }
     

     void Game::pick(Pin& pin)
     {
         if (m_event.type == sf::Event::EventType::MouseButtonPressed)
         {
             sf::Vector2f p = {(float) sf::Mouse::getPosition(m_window).x,(float)sf::Mouse::getPosition(m_window).y };
             if ((pin.mouseOver(p) == true) && pin.lock == false && pin.ref == true)
             {
                 Pin tempPin(pin);
                 tempPin.picked = true;
                 tempPin.ref = false;
                 tempPin.carried = true;
                 pins.push_back(tempPin);
             }
                 

         }
         
             
     }
     void Game::drag(Pin& pin)
     {
         if (pin.picked == true && sf::Mouse::isButtonPressed(sf::Mouse::Left))
         {
             pin.setPosition((float)sf::Mouse::getPosition(m_window).x, (float)sf::Mouse::getPosition(m_window).y);
             //pin.carried = true;
         }
         
             
     }
    
     void Game::drop(Pin& pin)
     {
        
         if (pin.carried==true && pin.picked == true && m_event.type == sf::Event::MouseButtonReleased)
         {
             
             for (auto& item : m_active_holes)
             {
                 if (pin.mouseOver(item.first)==true)
                 {

                     pin.setPosition(item.first);
                     item.second = colorToInt(pin);
                     pin.picked = false;
                     pin.carried = false;
                     pin.place_holder = vec2fToPlace(item.first);
                     
                 }

             }
           
             
         }
         if (pin.picked == true && m_event.type == sf::Event::MouseButtonReleased)
         {

             pins.erase(std::find(pins.begin(), pins.end(), pin));
         }
         
         
     }
     int Game::vec2fToPlace(const sf::Vector2f& v)
     {
         if (v.x == 72.0)
             return 1;
         if (v.x == 127.0)
             return 2;
         if (v.x == 183.0)
             return 3;
         if (v.x == 239.0)
             return 4;
     }
     int Game::colorToInt(Pin& pin)
     {
         if (pin.getFillColor() == sf::Color::Red)
             return 0;
         if (pin.getFillColor() == sf::Color::Green)
             return 1;
         if (pin.getFillColor() == sf::Color::White)
             return 2;
         if (pin.getFillColor() == sf::Color::Black)
             return 3;
         if (pin.getFillColor() == sf::Color::Blue)
             return 4;
         if (pin.getFillColor() == sf::Color::Yellow)
             return 5;
     }

  
     void Game::delet(Pin& pin)
     {
         if (m_event.type == sf::Event::EventType::MouseButtonPressed)
         {
             sf::Vector2f p = { (float)sf::Mouse::getPosition(m_window).x,(float)sf::Mouse::getPosition(m_window).y };
             if (pin.mouseOver(p) == true && pin.lock == false && pin.ref == false && pin.picked==false && pin.carried==false )
             {
                 pins.erase(std::find(pins.begin(), pins.end(), pin));
                 //std::remove(pins.begin(), pins.end(), pin);
             }
             
             

         }

         
     }
    
   
    
     void Game::update(const MasterMind& M,State& state)
     {
         bool filled{true};
         bool win{ false };
         if (std::any_of(m_active_holes.begin(), m_active_holes.end(), [](std::pair<sf::Vector2f, int>item) {return item.second == -1;}))
             filled = false;
      
         if (filled == true)
         {
             // TODO: add code for show scores
             Guess currentGuess;
             for (auto& item : m_active_holes)
             {
                 currentGuess.guess.insert(item.second);
             }
             currentGuess.whites = whiteCounter(currentGuess.guess, M.getTheAnswer());
             currentGuess.blacks = blackCounter(currentGuess.guess, M.getTheAnswer());
             updateState(currentGuess, state);
             int tempBlack = currentGuess.blacks;
             int tempWhite = currentGuess.whites - tempBlack;
             std::cout << "whites: " << tempWhite << "blacks: " << tempBlack << std::endl;
             std::vector<sf::Vector2f>::iterator iter;
             iter = scorePoints.begin();
             for (int i=0;i<4;i++)
             {
                 while (tempWhite > 0)
                 {
                     sf::CircleShape circle;
                     circle.setPosition(*iter);
                     iter++;
                     circle.setOrigin(10, 10);
                     circle.setRadius(8);
                     circle.setFillColor(sf::Color::White);
                     scores.push_back(circle);
                     tempWhite--;

                 }
                 while (tempBlack > 0)
                 {
                     sf::CircleShape circle;
                     circle.setPosition(*iter);
                     circle.setOrigin(10, 10);
                     circle.setRadius(8);
                     circle.setFillColor(sf::Color::Black);
                     scores.push_back(circle);
                     if ((scorePoints.begin() + 3) > iter)
                         iter++;
                     tempBlack--;
                 }
             }
             for (auto& item : scorePoints)
                 item.y -= 66.0f;
             if (currentGuess.blacks == 4)
                 win = true;


             for (auto& item : m_active_holes)
             {
                 item.first.y -= 67.0f;
                 item.second = -1;
             }
             std::vector<Pin>::reverse_iterator rit = pins.rbegin();
             int i = 4;
             while (i > 0)
             {
                 (*rit).lock = true;
                 ++rit;
                 --i;
             }
                 
         }
         
         if (win == true)
             std::cout << "\nYou win!";
     }
    
         /*
         Pin pin;
         pin.setPosition(pos);
         pin.setOrigin(20, 20);
         pin.setRadius(21);
         pin.setOutlineThickness(4);
         pin.setOutlineColor(sf::Color::Yellow);
         pin.setFillColor(sf::Color::Transparent);
         pin.lock = true;
         return pin;
         */
  



             

            

             
             






