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

        //Text bounds:
        float width;
        float height;
        
        void Calibrate() //Sets text params and applies changes when done with change text function
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
        } //Calibate function is called on creation and on text update

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
        } //On text construction the given parameters are stored and "Calibrate()" is called
        
        void ChangeText(std::string Text)//Updates text change
        {
            TextText = Text;
            Calibrate();
        }
        std::string getText() //Returns text as form of string
        {
            return TextText;
        }
        sf::Text Text() //retruns sf::Text so it can be drawn to window
        {
            return MainTxt;
        }
};


class Button
{
    private:
    
        //Button Text Params:
        sf::Font font;                  
        sf::Text text;                  
        sf::Vector2f textPos;           
        sf::Vector2f textBounds;        
        //Button Shape Params:
        sf::RectangleShape shape;
        sf::Vector2f pos;               
        sf::Vector2f size;              
        //Button colors(Hover and Default)
        sf::Color DefaultColor;         
        sf::Color hColor;     
        //Button Sounds:
        sf::SoundBuffer bf1;            
        sf::SoundBuffer bf2;            
        sf::Sound HoverSound;           
        sf::Sound ClickSound;           

        bool wFocus = false;            //Returns focus on main window
        
        bool ButtonClick = false;       //Controls the code on button click
        
        bool CursorOverButton(sf::RenderWindow &window) //Detect cursor pos on window
        {
            sf::Vector2f CursorPos = (sf::Vector2f)sf::Mouse::getPosition(window);

            if((CursorPos.x < (pos.x + size.x) && CursorPos.x > pos.x) && (CursorPos.y < (pos.y + size.y) && CursorPos.y > pos.y))
            {

                if(shape.getFillColor() != hColor){     //if cursor passes over window 
                    shape.setFillColor(hColor);         //all of this will execute
                    HoverSound.play();
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
            font.loadFromFile("arial.ttf"); //Loads font for Button text default is ariel(change to what you want!)
        }
        //Button Creation:
        void create(std::string ButtonText, int TextSize,  sf::Color Color, float buttonWidth, float buttonHeight, float x, float y, sf::Color HoverColor)
        {
            //Create Button size and position:
            pos.x = x;
            pos.y = y;
            size.x = buttonWidth;
            size.y = buttonHeight;
            DefaultColor = Color;
            hColor = HoverColor;
            
            //Load text for button:
            text.setFont(font);
            text.setCharacterSize(TextSize);
            text.setString(ButtonText);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(2);
            sf::FloatRect fr =  text.getGlobalBounds();
            textBounds.x = fr.width + fr.left;
            textBounds.y = fr.height + fr.top;
            
            //Apply paramaters to shape:
            shape.setSize(size);
            shape.setFillColor(DefaultColor);
            shape.setPosition(pos);
            
            //Fit text to button:
            text.setOrigin(textBounds.x/2, textBounds.y/2);
            text.setPosition(pos.x + (size.x/2), pos.y + (size.y/2) - 1);
        }
        
        
        bool isButtonClicked(sf::RenderWindow &window) //When button is pressed down whis executes
        {
            if(!CursorOverButton(window)) //Tests focus on window
            {
                wFocus = (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))? true : false;
            }
            if(CursorOverButton(window) && !wFocus && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if(!ButtonClick) ClickSound.play(); //Plays only on first click
                ButtonClick = true;
                return true;
            }
            else ButtonClick = false; //Return clicked state
            return false;
        }

        void HoverClickSound(std::string HoverSoundPath1, std::string ClickSoundPath2) //add sounds to buffer and load into
        {                                                                              //playable sound
            bf1.loadFromFile(HoverSoundPath1);
            HoverSound.setBuffer(bf1);
            bf2.loadFromFile(ClickSoundPath2);
            ClickSound.setBuffer(bf2);
        }
        
        void draw(sf::RenderWindow &window) //draws button on window(with text)
        {
            window.draw(shape);
            if(textBounds.x < size.x || textBounds.y < size.y) window.draw(text);
        }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280,720), "SFML + CPP-CLASS");

    Button TestClick;
    TestClick.create("CLICK ME!", 40, sf::Color::Green, 220, 80, 50, 600, sf::Color::Blue); //Button creation
                                                                                            //with all needed parameters
    TestClick.HoverClickSound("Hover.wav", "Click.wav"); //Loads sounds the button makes (FROM FILE)

    Button ClearAll;
    ClearAll.create("CLEAR ALL", 40, sf::Color::Green, 330, 80, 300, 600, sf::Color::Blue);
    ClearAll.HoverClickSound("Hover.wav", "Click.wav");

    Button Quit;
    Quit.create("QUIT", 40, sf::Color::Green, 220, 80, 660, 600, sf::Color::Blue);
    Quit.HoverClickSound("Hover.wav", "Click.wav");

    DisplayText Welcome("WELCOME TO BUTTON AND TEXT TEST!", sf::Color::White, 50, 640, 80); //Constructs text with all 
                                                                                            //needed parameters
    DisplayText Counter("Counter: 0", sf::Color::White, 30, 320, 200);
    int count = 0;

    std::string input = "Write something...";
    DisplayText PlayerInput(input, sf::Color::Green, 30, 320, 300);
    
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::TextEntered)
            {
                if(event.text.unicode < 128)
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                    {
                        if(!input.empty())input.pop_back();  //This erases last character from input(until input is empty)
                    }
                    else if(input == "Write something...")
                    {
                        input.clear();                  //Clear input whne start typing
                        input += event.text.unicode;    //Detect pressed char and add it to input
                    }else input += event.text.unicode;  //Or if input has changed add it to changed input
                    PlayerInput.ChangeText(input);      //Add modified input to Player input text
                }
            }
        }
        if(TestClick.isButtonClicked(window)) 
        {
            count++;                            //This will count how many times
            std::stringstream ss;               //button will execute click(This is the problem)
            ss << "Counter: " << count;         //I can't make it work on release
            Counter.ChangeText(ss.str());       //It detectes only when pressed and
        }                                       //executes this code while pressed!
        
        if(ClearAll.isButtonClicked(window))    //When this button is clicked
        {                                       //everything is reset
            count = 0;
            Counter.ChangeText("Counter: 0");
            input = "Write something...";
            PlayerInput.ChangeText(input);
        }
        
        if(Quit.isButtonClicked(window))
        {
            window.close();
        }
        window.clear();
        TestClick.draw(window);//draws button on window
        ClearAll.draw(window);
        Quit.draw(window);
        window.draw(Welcome.Text()); // draws text on window
        window.draw(Counter.Text());
        window.draw(PlayerInput.Text());
        window.display();
    }
    return 0;
}
