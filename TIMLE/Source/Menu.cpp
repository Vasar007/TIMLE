#include "../Include/Menu.hpp"


void menu(sf::RenderWindow & window)
{
    window.setView(window.getDefaultView());
    sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
    menuTexture1.loadFromFile("Images/Menu/111.png");
    menuTexture2.loadFromFile("Images/Menu/222.png");
    menuTexture3.loadFromFile("Images/Menu/333.png");
    aboutTexture.loadFromFile("Images/Menu/about.png");
    menuBackground.loadFromFile("Images/Menu/Penguins.jpg");
    sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
    bool isMenu = true;
    int menuNum = 0;
    menu1.setPosition(150, 30);
    menu2.setPosition(150, 90);
    menu3.setPosition(150, 150);
    menuBg.setPosition(150, 0);

    // Само меню
    while (isMenu && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }

        menu1.setColor(sf::Color::White);
        menu2.setColor(sf::Color::White);
        menu3.setColor(sf::Color::White);
        menuNum = 0;
        window.clear(sf::Color(129, 181, 221));

        if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) {
            menu1.setColor(sf::Color::Blue);
            menuNum = 1;
        }
        if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(window))) {
            menu2.setColor(sf::Color::Blue);
            menuNum = 2;
        }
        if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) {
            menu3.setColor(sf::Color::Blue);
            menuNum = 3;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (menuNum == 1)
                isMenu = false;    // Если нажали первую кнопку, то выходим из меню 
            if (menuNum == 2) {
                window.draw(about);
                window.display();
                while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
            }
            if (menuNum == 3) {
                window.close();
                isMenu = false;
                break;
            }
        }

        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu3);

        window.display();
    }
}

bool loadResources()
{
    /// Fonts
    if (!font.loadFromFile("Other/CyrilicOld.ttf")) {
        errorType = "Failed to load font 'Other/CyrilicOld.ttf'.";
        return false;
    }
    if (!errorFont.loadFromFile("Other/Sansation.ttf")) {
        errorType = "Failed to load font 'Other/sansation.ttf'.";
        return false;
    }

    /// Units
    // Hero
    if (!heroImage.loadFromFile("Textures/Player/male_light.png")) { //vanilla
        errorType = "Failed to load image 'Textures/Player/male_light.png'.";
        return false;
    }
    if (!heroAttackImage.loadFromFile("Textures/Player/male_longbow.png")) {
        errorType = "Failed to load image 'Textures/Player/male_longbow.png'.";
        return false;
    }
    if (!heroDeathImage.loadFromFile("Textures/Player/imp death.png")) {
        errorType = "Failed to load image 'Textures/Player/imp death.png'.";
        return false;
    }
    // Ghost
    if (!ghostEnemyImage.loadFromFile("Textures/Enemies/ghost.png")) {
        errorType = "Failed to load image 'Textures/Enemies/ghost.png'.";
        return false;
    }
    if (!ghostEnemyImageDeath.loadFromFile("Textures/Enemies/lightning.png")) {
        errorType = "Failed to load image 'Textures/Enemies/lightning.png'.";
        return false;
    }
    // Golem
    if (!golemEnemyImage.loadFromFile("Textures/Enemies/golem-walk.png")) {
        errorType = "Failed to load image 'Textures/Enemies/golem-walk.png'.";
        return false;
    }
    if (!golemEnemyImageAttack.loadFromFile("Textures/Enemies/golem-atk.png")) {
        errorType = "Failed to load image 'Textures/Enemies/golem-atk.png'.";
        return false;
    }
    if (!golemEnemyImageDeath.loadFromFile("Textures/Enemies/golem-die.png")) {
        errorType = "Failed to load image 'Textures/Enemies/golem-die.png'.";
        return false;
    }
    // Dark soldier
    if (!soldierEnemyImage.loadFromFile("Textures/Enemies/darksoldiersheetupdate.png")) {
        errorType = "Failed to load image 'Textures/Enemies/darksoldiersheetupdate.png'.";
        return false;
    }
    // Shadow
    if (!shadowEnemyImage.loadFromFile("Textures/Enemies/shadow-80x70.png")) {
        errorType = "Failed to load image 'Textures/Enemies/shadow-80x70.png'.";
        return false;
    }
    // Goblin
    if (!goblinEnemyImage.loadFromFile("Textures/Enemies/goblin.png")) {
        errorType = "Failed to load image 'Textures/Enemies/goblin.png'.";
        return false;
    }
    // Goblin with sword
    if (!goblinSwordEnemyImage.loadFromFile("Textures/Enemies/goblinsword.png")) {
        errorType = "Failed to load image 'Textures/Enemies/goblinsword.png'.";
        return false;
    }
    // Minotaur mage
    if (!minotaurMageEnemyImage.loadFromFile("Textures/Enemies/gnu-120x100.png")) {
        errorType = "Failed to load image 'Textures/Enemies/gnu-120x100.png'.";
        return false;
    }
    // Dwarves
    if (!dwarfEnemyImage.loadFromFile("Textures/Enemies/dwarves.png")) {
        errorType = "Failed to load image 'Textures/Enemies/dwarves.png'.";
        return false;
    }

    /// Objects
    if (!movePlatformImage.loadFromFile("Textures/Objects/MovingPlatform.png")) {
        errorType = "Failed to load image 'Textures/Objects/MovingPlatform.png'.";
        return false;
    }

    if (!bulletImage.loadFromFile("Textures/Objects/mage-bullet-13x13.png")) {
        errorType = "Failed to load image 'Textures/Objects/mage-bullet-13x13.png'.";
        return false;
    }

    if (!fireDamageImage.loadFromFile("Textures/Objects/Fire01.png")) {
        errorType = "Failed to load image 'Textures/Objects/Fire01.png'.";
        return false;
    }
    ///Interface
    if (!quest_image.loadFromFile("Textures/Interface/missionbg.jpg")) {
        errorType = "Failed to load image 'Textures/Interface/missionbg.jpg'.";
        return false;
    }

    /// Sounds
    if (!shootBuffer.loadFromFile("Sounds/firebol3.ogg")) {    
        errorType = "Failed to load sound 'Sounds/firebol3.ogg'.";
        return false;
    }

    if (!music.openFromFile("Sounds/music.ogg")) {    
        errorType = "Failed to load music 'Sounds/music.ogg'.";
        return false;
    }

    return true;
}