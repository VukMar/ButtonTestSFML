#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

class DisplayText
{
    private:
    
        sf::Text MainTxt;
        sf::Font f;
        
        int txtSize;
        
        sf::Vector2f TxtPos;
        sf::Color TxtColor;

        std::string TextText;

        float width;
        float height;
        
        void Calibrate()
        {
            MainTxt.setFillColor(TxtColor);
            MainTxt.setString(TextText);
            MainTxt.setCharacterSize(txtSize);
            sf::Text Temp;
            Temp.setFont(f);
            Temp.setString(TextText);
            Temp.setCharacterSize(txtSize);
            sf::FloatRect fRect = Temp.getGlobalBounds();
            height = fRect.height + fRect.top;
            width = fRect.width + fRect.left;
            MainTxt.setOrigin(width/2, height/2);
            MainTxt.setPosition(TxtPos);
        }

    public:

        DisplayText(std::string Text, sf::Color Color, int size, float posx, float posy)
        {
            f.loadFromFile("arial.ttf");
            MainTxt.setFont(f);
            TxtColor = Color;
            TextText = Text;
            txtSize = size;
            TxtPos = sf::Vector2f(posx, posy);
            Calibrate();
        }
        void ChangeText(std::string Text)
        {
            TextText = Text;
            Calibrate();
        }
        std::string getText()
        {
            return TextText;
        }
        sf::Text Text()
        {
            return MainTxt;
        }
};


class Button
{
    private:
    
        sf::Font font;
        sf::Text text;
        sf::RectangleShape shape;
        sf::Vector2f textPos;
        sf::Vector2f textBounds;
        sf::Vector2f pos;
        sf::Vector2f size;
        sf::Color DefaultColor;
        sf::Color hColor;
        sf::SoundBuffer bf1;
        sf::SoundBuffer bf2;
        sf::Sound HoverSound;
        sf::Sound ClickSound;

        bool wFocus = false;
        bool ButtonClick = false;
        bool CursorOverButton(sf::RenderWindow &window)
        {
            sf::Vector2f CursorPos = (sf::Vector2f)sf::Mouse::getPosition(window);

            if((CursorPos.x < (pos.x + size.x) && CursorPos.x > pos.x) && (CursorPos.y < (pos.y + size.y) && CursorPos.y > pos.y))
            {

                if(shape.getFillColor() != hColor){
                    shape.setFillColor(hColor);
                    HoverSound.play();
                }
                else if(shape.getFillColor() == hColor)
                {

                }
                return true;
            }
            else 
            {
                shape.setFillColor(DefaultColor);
                return false;
            }
        }

    public:

        Button()
        {
            font.loadFromFile("arial.ttf");
        }
        
        void create(std::string ButtonText, int TextSize,  sf::Color Color, float buttonWidth, float buttonHeight, float x, float y, sf::Color HoverColor)
        {
            pos.x = x;
            pos.y = y;
            size.x = buttonWidth;
            size.y = buttonHeight;
            DefaultColor = Color;
            hColor = HoverColor;
            
            text.setFont(font);
            text.setCharacterSize(TextSize);
            text.setString(ButtonText);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(2);
            sf::FloatRect fr =  text.getGlobalBounds();
            textBounds.x = fr.width + fr.left;
            textBounds.y = fr.height + fr.top;
            
            shape.setSize(size);
            shape.setFillColor(DefaultColor);
            shape.setPosition(pos);
            
            text.setOrigin(textBounds.x/2, textBounds.y/2);
            text.setPosition(pos.x + (size.x/2), pos.y + (size.y/2) - 1);
        }
        
        
        bool isButtonClicked(sf::RenderWindow &window)
        {
            if(!CursorOverButton(window))
            {
                wFocus = (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))? true : false;
            }
            if(CursorOverButton(window) && !wFocus && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if(!ButtonClick) ClickSound.play();
                ButtonClick = true;
                return true;
            }
            else ButtonClick = false;
            return false;
        }
        
        bool isButtonReleased(sf::RenderWindow &window)
        {
            return (isButtonClicked(window))? false : true;
        }

        void HoverClickSound(std::string HoverSoundPath1, std::string ClickSoundPath2)
        {
            bf1.loadFromFile(HoverSoundPath1);
            HoverSound.setBuffer(bf1);
            bf2.loadFromFile(ClickSoundPath2);
            ClickSound.setBuffer(bf2);
        }
        
        void draw(sf::RenderWindow &window)
        {
            window.draw(shape);
            if(textBounds.x < size.x || textBounds.y < size.y) window.draw(text);
        }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280,720), "SFML + CPP-CLASS");

    //Main Menu Utiils:
    Button TestClick;
    TestClick.create("CLICK ME!", 40, sf::Color::Green, 200, 80, 540, 600, sf::Color::Blue);
    TestClick.HoverClickSound("AppOpen.wav", "IncomeSound.wav");

    Button Test;
    Test.create("PLAY", 40, sf::Color::Red, 150, 60, 565, 200, sf::Color::Blue);
    Test.HoverClickSound("AppOpen.wav", "IncomeSound.wav");

    Button Test2;
    Test2.create("QUIT", 40, sf::Color::Green, 150, 60, 565, 300, sf::Color::Red);
    Test2.HoverClickSound("AppOpen.wav", "IncomeSound.wav");

    DisplayText Wellcome("Wellcome to SFML + CPP-CLASS practice!", sf::Color::Green, 50, 640, 100);
    
    //Back Button(is used in game and pregame):
    Button Back;
    Back.create("BACK", 35, sf::Color::Red, 150, 60, 50, 50, sf::Color::Green);
    Back.HoverClickSound("AppOpen.wav", "IncomeSound.wav");

    //PreGame Utiils:
    Button Clear;
    Clear.create("CLEAR", 35, sf::Color::Red, 200, 60, 650, 640, sf::Color::Green);
    Clear.HoverClickSound("AppOpen.wav", "IncomeSound.wav");

    Button Continue;
    Continue.create("CONTINUE", 35, sf::Color::Red, 200, 60, 440, 640, sf::Color::Green);
    Continue.HoverClickSound("AppOpen.wav", "IncomeSound.wav");
    
    DisplayText Works("WORKS!", sf::Color::Red, 100, 640, 360);

    std::string input = "Enter name to continue!";
    DisplayText PlayerInput(input, sf::Color::Green, 40, 640, 580);

    //Game  Utills:
    DisplayText WelcomeGame("Game Screen Finaly!", sf::Color::Green, 40, 640, 100);
    DisplayText Player1("Player One: ", sf::Color::White, 30, 960, 150);
    DisplayText Player2("Player Two: ", sf::Color::White, 30, 960, 200);
    
    bool MainMenu = true;
    bool PreGame = false;
    bool Game = false;
    int NameTag = 1;

    int TestCounter = 0;

    while(window.isOpen())
    {
        sf::Vector2f MousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
        
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::TextEntered)
            {
                if(PreGame && event.text.unicode < 128)
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                    {
                        if(!input.empty())input.pop_back();
                    }
                    else if(input == "Enter name to continue!")
                    {
                        input.clear();
                        input += event.text.unicode;
                    }else input += event.text.unicode;
                    PlayerInput.ChangeText(input);
                }
            }
        }
        
        if(MainMenu)
        {
            if(Test.isButtonClicked(window))
            {
                TestCounter = 0;
                std::stringstream ss;
                ss << TestCounter;
                Wellcome.ChangeText(ss.str());
                /*MainMenu = false;
                Game = false;
                PreGame = true;*/
            }
            if(Test2.isButtonClicked(window))
            {
                window.close();
            }
            if(TestClick.isButtonClicked(window))
            {
                TestCounter++;
                std::stringstream ss;
                ss << TestCounter;
                Wellcome.ChangeText(ss.str());
            }
            window.clear();
            Test.draw(window);
            Test2.draw(window);
            TestClick.draw(window);
            window.draw(Wellcome.Text());
        }
        
        if(PreGame)
        {
            if(Back.isButtonClicked(window))
            {
               if(Back.isButtonReleased(window))
                {
                    MainMenu = true;
                    PreGame = false;
                    Game = false;
                }
            }
            if(Clear.isButtonClicked(window))
            {
                input = "Enter name to continue!";
                PlayerInput.ChangeText(input);
            }
            if(Continue.isButtonClicked(window))
            {
                if(NameTag == 2 && input != "Enter name to continue!")
                {
                    Player2.ChangeText(Player2.getText() += input);
                    Game = true;
                    PreGame = false;
                    MainMenu = false;
                    input = "Enter name to continue!";
                    PlayerInput.ChangeText(input);
                }
                else if(NameTag == 1 && input != "Enter name to continue!")
                {
                    NameTag = 2;
                    Player1.ChangeText(Player1.getText() += input);
                    input = "Enter name to continue!";
                    PlayerInput.ChangeText(input);
                }
            }
            window.clear();
            Clear.draw(window);
            Continue.draw(window);
            window.draw(PlayerInput.Text());
            window.draw(Works.Text());
            Back.draw(window);
        }
        if(Game)
        {
            if(Back.isButtonClicked(window))
            {
                if(Back.isButtonReleased(window))
                {
                    Player1.ChangeText("Player One: ");
                    Player2.ChangeText("Player Two: ");
                    MainMenu = false;
                    PreGame = true;
                    Game = false;
                    NameTag = 1;
                }
            }
            window.clear();
            Back.draw(window);
            window.draw(Player1.Text());
            window.draw(Player2.Text());
            window.draw(WelcomeGame.Text());
        }
        window.display();
    }
    return 0;
}