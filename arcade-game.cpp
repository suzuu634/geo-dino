#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Event.hpp>
#include <optional> 
#include <iostream>
#include <random>

bool checkCollision(const sf::Sprite& spriteA, const sf::Sprite& spriteB) {
    auto boundsA = spriteA.getGlobalBounds();
    auto boundsB = spriteB.getGlobalBounds();

    return (boundsA.position.x < boundsB.position.x + boundsB.size.x &&
            boundsA.position.x + boundsA.size.x > boundsB.position.x &&
            boundsA.position.y < boundsB.position.y + boundsB.size.y &&
            boundsA.position.y + boundsA.size.y > boundsB.position.y);
}


int main() {
    //cree la fenetre
    sf::RenderWindow window(sf::VideoMode({1200, 600}), "arcade game");
    window.setFramerateLimit(60);

    //charger les textures
    sf::Texture texture1;
    if (!texture1.loadFromFile("base.png")) {
        return -1;
    }
    sf::Texture texture2;
    if (!texture2.loadFromFile("perso.png")) {
        return -1;
    }
    sf::Texture texture3;
    if (!texture3.loadFromFile("spike.png")) {
        return -1;
    }
    sf::Texture texture4;
    if (!texture4.loadFromFile("spike2.png")) {
        return -1;
    }
    sf::Texture texture5;
    if ( !texture5.loadFromFile("bdf.png")) {
        return -1;
    }


    sf::Texture background;
    if ( !background.loadFromFile("moon.png")) {
        return -1;
    }
    
    sf::Vector2u windowsize = window.getSize();
    sf::Vector2u texturesize = background.getSize();

    float scaleX = static_cast<float>(windowsize.x) / texturesize.x;
    float scaleY = static_cast<float>(windowsize.y) / texturesize.y;





    sf::Sprite sprite1(texture1);
    sf::Sprite sprite2(texture2);
    sf::Sprite sprite3(texture3);
    sf::Sprite sprite4(texture4);
    sf::Sprite sprite5(texture5);
    sf::Sprite background1(background);
    float moveSpeed1 = 150.f;
    float moveSpeed2 = 150.f;
    float moveSpeed3 = 150.f;

    background1.setScale({scaleX, scaleY});

    int score = 0;

    sprite2.setPosition({300.f, 437.f});

    // Gravité et saut
    float velocityY = 0.f;
    float gravity = 0.3f;
    float jumpforce = -8.f;
    bool isJumping = false;
    float groundY = 437.f;
    int jumpcount = 0;

    // Musique
    sf::Music music2;
    if (!music2.openFromFile("zelda-song2.ogg")) {
        return -1;
    }
    music2.setLooping(true);
    music2.play();

    sf::Music music_death;
    if ( !music_death.openFromFile("death-song.ogg")) {
        return -1;
    }

    sf::Clock clock;

    // Charger la police et le texte Game Over
    sf::Font font;
    if ( !font.openFromFile("arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police 'arial.ttf'." << std::endl;
        return -1;
    }
    
    sf::Text gameOverText(font, "GAME OVER\nPress any key to restart", 50);
    gameOverText.setPosition({300.f, 200.f});
    gameOverText.setFillColor(sf::Color::Red);

    sf::Font font2;
    if ( !font2.openFromFile("arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police 'arial.ttf'." << std::endl;
        return -1;
    }

    sf::Text compteur(font2, "", 25);
    compteur.setFillColor(sf::Color::White); // Couleur du texte
    compteur.setPosition({1100.f, 0.f});
    bool gameOver = false;

    //generation rdm

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 3);

    // Boucle principale
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Sprite spritesToCheck[] = {sprite3, sprite4, sprite5};


        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (!isJumping) {
                    velocityY = jumpforce;
                    isJumping = true;
                }
            }

            if (gameOver) {
                window.clear();
                window.draw(gameOverText);
                window.display();
                
                
                while (1) {                  
                    if ( const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {

                        if ( !keyEvent->control) {
                            sf::sleep(sf::seconds(2));
                            sprite2.setPosition({300.f, 437.f});
                            sprite2.setPosition({300.f, 437.f});
                            sprite3.setPosition({-1000.f, -1000.f});
                            sprite4.setPosition({-1000.f, -1000.f});
                            sprite5.setPosition({-1000.f, -1000.f});
                            score = 0;  // Réinitialiser le score
                            music_death.stop();
                            music2.play();
                            break;
                        }
                    }
                }
            }
            gameOver = false;
        }

        if (!gameOver) {
            // Déplacement du spike et bdf et skin

            velocityY += gravity;
            sprite2.move({0.f, velocityY});



            if (sprite2.getPosition().y >= groundY) {
                sprite2.setPosition({sprite2.getPosition().x, groundY});
                velocityY = 0.f;
                isJumping = false;
            }

 
            float moveDirection = -1.f;
            
            sprite3.move({moveSpeed1 * moveDirection * deltaTime, 0.f});
            sprite4.move({moveSpeed2 * moveDirection * deltaTime, 0.f});
            sprite5.move({moveSpeed3 * moveDirection * deltaTime, 0.f});
            
            int random_number_sprite = distrib(gen);


            switch (random_number_sprite)
            {
            case 1:
                if ( sprite4.getPosition().x <= 500) {
                    sprite3.setPosition({1200.f, 437.f});
                    if (moveSpeed1 < 400 && sprite3.getPosition().x <= 1100) {
                        moveSpeed1 += 30;
                    }
                }
                break;

            case 2:
                if ( sprite5.getPosition().x <= 500) {
                    sprite4.setPosition({1200.f, 437.f});
                    if ( moveSpeed2 < 400 && sprite4.getPosition().x <= 1100) {
                        moveSpeed2 += 30;
                    }
                }
                break;
            case 3:
                if ( sprite3.getPosition().x <= 500) {
                    sprite5.setPosition({1200.f, 410.f});
                    if ( moveSpeed3 < 400 && sprite5.getPosition().x <= 1100) {
                        moveSpeed3 += 30;
                    }
                }
                break;
            }
            
            score++;
            compteur.setString(std::to_string(score));
            window.draw(compteur);


            if (checkCollision(sprite2, sprite3) || 
            checkCollision(sprite2, sprite4) || 
            checkCollision(sprite2, sprite5)) {
            
                gameOver = true;
                music2.stop();
                music_death.play();
            }

        }
        

        window.clear();
        window.draw(background1);
        window.draw(sprite1);
        window.draw(sprite2);
        window.draw(sprite3);
        window.draw(sprite4);
        window.draw(sprite5);
        window.draw(compteur);






        window.display();
    }

    return 0;
}





