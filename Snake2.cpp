#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <vector>
using std::string;
enum true_false { richtig = true, falsig = false };
enum stany { pauza = 0, pomoc = 1, gra = 2, scores = 3, load_game = 4, collision = 5, load_from_file = 6, save_to_file = 7 };
enum sizes { win_size_main = 700, rocksize = 50, button_x_size = 200, button_y_size = 50 };
enum steps { nazwa = 0, czas = 2, pociski = 1, predkosc = 3, end_load = 4 };
enum coordinates { x = 0, y = 1 };
class player {
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f position;
    sf::Vector2i size;
    int boolets;
    float pkt;
public:
    int move(sf::Vector2f wektor, sf::Vector2u win_size) {
        if (position.x + wektor.x >= 0 && position.x + wektor.x + size.x - 5 < win_size.x && position.y + wektor.y >= 0 && position.y + wektor.y + size.y - 5 < win_size.y) {
            m_sprite.move(wektor);
            position = m_sprite.getPosition();
            return 1;
        }
        return -1;
    }
    int set_position(sf::Vector2f new_position) {
        position = new_position;
        return 1;
    }
    int load(float pkt, int boolets) {
        m_texture.loadFromFile("ship.png");
        m_texture.setSmooth(true);
        m_sprite.setTexture(m_texture);
        size.y = 50;
        size.x = 100;
        this->pkt = pkt;
        this->boolets = boolets;
        return 1;
    }
    sf::Sprite ret_sprite() {
        return m_sprite;
    }
    int check_colision(sf::Vector2f rockposition) {
        if (rockposition.x >= position.x && rockposition.y >= position.y && rockposition.x + rocksize)
            return 0;
    }
    void change_pkt(float pkt) {
        this->pkt += pkt;
    }
    float ret_pkt() {
        return pkt;
    }
    sf::Vector2f ret_pos() {
        return position;
    }
    sf::Vector2i ret_size() {
        return size;
    }
    int ret_boolets() {
        return boolets;
    }
    bool check_colision(sf::Vector2f position_rock, sf::Vector2f size_rock) {
        if (position.x + size.x >= position_rock.x && position.x <= position_rock.x + size_rock.x && position.y + size.y >= position_rock.y && position.y <= position_rock.y + size_rock.y) {
            return true;
        }
        return false;
    }
};
class kamien {
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f position;
    sf::Vector2i size;
    int hp;
public:
    int move(sf::Vector2f wektor, sf::Vector2u win_size) {
        if (position.x + wektor.x >= 0 && position.x + wektor.x + size.x - 5 < win_size.x && position.y + wektor.y >= 0 && position.y + wektor.y + size.y - 5 < win_size.y) {
            m_sprite.move(wektor);
            position = m_sprite.getPosition();
            return 1;
        }
        return -1;
    }
    int set_position(sf::Vector2f new_position) {
        position = new_position;
        m_sprite.setPosition(position);
        return 1;
    }
    int load(int hp) {
        m_texture.loadFromFile("rock.png");
        m_texture.setSmooth(true);
        m_sprite.setTexture(m_texture);
        size.y = 50;
        size.x = 50;
        this->hp = hp;
        return 1;
    }
    sf::Sprite ret_sprite() {
        return m_sprite;
    }
};
class bullet {
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f position;
    sf::Vector2i size;
public:
    int move(sf::Vector2f wektor, sf::Vector2u win_size) {
        if (position.x + wektor.x >= 0 && position.x + wektor.x + size.x - 5 < win_size.x && position.y + wektor.y >= 0 && position.y + wektor.y + size.y - 5 < win_size.y) {
            m_sprite.move(wektor);
            position = m_sprite.getPosition();
            return 1;
        }
        return -1;
    }
    int set_position(sf::Vector2f new_position) {
        position = new_position;
        m_sprite.setPosition(position);
        return 1;
    }
    int load() {
        m_texture.loadFromFile("bullet.gif");
        m_texture.setSmooth(true);
        m_sprite.setTexture(m_texture);
        size.y = 5;
        size.x = 10;
        return 1;
    }
    sf::Sprite ret_sprite() {
        return m_sprite;
    }
    bool check_colision(sf::Vector2f position_rock, sf::Vector2f size_rock) {
        if (position.x + size.x >= position_rock.x && position.x <= position_rock.x + size_rock.x && position.y + size.y >= position_rock.y && position.y <= position_rock.y + size_rock.y) {
            return true;
        }
        return false;
    }
};
struct player_score {
    int speed;
    int bullets;
    int points;
    int rocks;
    bool not_null = false;
    string name;
    int pkt;
};
struct file_handler {
public:
    void save_to_file_game(player_score& main_score) {
        FILE* data_file;

        if (data_file = fopen("data.dat", "w")) {
            fwrite(&main_score, sizeof(player_score), 1, data_file);
            fclose(data_file);
        }
    }
    void load_from_file_game(player_score& some_score) {
        FILE* data_file;
        if (data_file = fopen("data.dat", "r")) {

            fread(&some_score, sizeof(player_score), 1, data_file);
            fclose(data_file);
        }

    }
};
int main()
{
    sf::RenderWindow main_win(sf::VideoMode(win_size_main, win_size_main), "main_window", sf::Style::Default);
    sf::RectangleShape menu_buttons[5];
    sf::Vector2f button_locations[5];
    sf::RectangleShape option_buttons[3];
    sf::ConvexShape shields[2];
    sf::Vector2f option_button_locations[3];
    sf::Font main_f;
    sf::Clock timer;
    sf::Clock bullet_clock;
    sf::Time elapsed_bullet;
    sf::Time elapsed;
    sf::String sf_pkt_string;
    sf::Texture help;
    sf::Music soundtrack;
    sf::Vector2i mouse_pos;
    sf::Sprite help_sprite;
    sf::Text menu_buttons_text[5];
    sf::Text option_buttons_text[3];
    sf::Text name_text;
    sf::Text score;
    sf::Event events;
    sf::String input;
    sf::String output;
    sf::Text prompt;
    sf::Text output_text;
    string pkt_string;
    player* main_p = new player;
    player_score* main_score = new player_score;
    file_handler main_filehandler;
    bool utworzono = false;
    bool bullet_reload = false;
    bool reset_timer = true;
    bool game_started = false;
    bool mozna_tekst = true;
    bool num_input = false;
    bool muzyka = true;
    int state = 0;
    int i = 0;
    int current_step = 0;
    int spawn_time = 0;
    int rocks = 0;
    int rock_speed = 0;
    int mouse_position[2] = { 0,0 };
    int option = 0;
    int saved = false;
    std::vector <kamien*> kamienie;
    std::vector <bullet*> bullets;
    soundtrack.openFromFile("muzyka.ogg");
    soundtrack.setLoop(true);
    soundtrack.play();
    for (auto& var : shields) {
        var.setPointCount(5);
        var.setPoint(0, sf::Vector2f(0, 0));
        var.setPoint(1, sf::Vector2f(35, 0));
        var.setPoint(2, sf::Vector2f(35, -35));
        var.setPoint(3, sf::Vector2f(35 / 2, -45));
        var.setPoint(4, sf::Vector2f(0, -35));
        var.setFillColor(sf::Color::Green);
        var.setRotation(180);
    }
    shields[0].setPosition(500, 100);
    shields[1].setPosition(500 - button_x_size - 65, 100);
    main_f.loadFromFile("font.ttf");
    prompt.setFont(main_f);
    output_text.setFont(main_f);
    prompt.setFillColor(sf::Color::White);
    output_text.setFillColor(sf::Color::White);
    prompt.setPosition(sf::Vector2f(0, 0));
    output_text.setPosition(sf::Vector2f(0, 50));
    prompt, output_text.setCharacterSize(30);
    for (auto& var : menu_buttons) {
        var.setOutlineColor(sf::Color::Yellow);
        var.setOutlineThickness(5);
        var.setFillColor(sf::Color::Cyan);
        var.setSize(sf::Vector2f(button_x_size, button_y_size));
        var.setPosition(sf::Vector2f(win_size_main / 2 - button_x_size / 2, 100 + i * button_y_size + i * 50));
        button_locations[i].x = win_size_main / 2 - button_x_size / 2;
        button_locations[i].y = 100 + i * button_y_size + i * 50;
        i++;
    }
    i = 0;
    for (auto& var : option_buttons) {
        var.setOutlineColor(sf::Color::Yellow);
        var.setOutlineThickness(5);
        var.setFillColor(sf::Color::Cyan);
        var.setSize(sf::Vector2f(button_x_size, button_y_size));
        var.setPosition(sf::Vector2f(win_size_main / 2 - button_x_size / 2, 100 + i * button_y_size + i * 50));
        button_locations[i].x = win_size_main / 2 - button_x_size / 2;
        button_locations[i].y = 100 + i * button_y_size + i * 50;
        i++;
    }
    i = 0;
    for (auto& var : menu_buttons_text) {
        var.setFillColor(sf::Color::Magenta);
        var.setFont(main_f);
        var.setCharacterSize(20);
        var.setPosition(sf::Vector2f(win_size_main / 2 - button_x_size / 2, 100 + i * button_y_size + i * 50));
        i++;
    }
    i = 0;
    for (auto& var : option_buttons_text) {
        var.setFillColor(sf::Color::Magenta);
        var.setFont(main_f);
        var.setCharacterSize(20);
        var.setPosition(sf::Vector2f(win_size_main / 2 - button_x_size / 2, 100 + i * button_y_size + i * 50));
        i++;
    }
    i = 0;
    menu_buttons_text[0].setString("New game");
    menu_buttons_text[1].setString("Save");
    menu_buttons_text[2].setString("Load");
    menu_buttons_text[3].setString("Help");
    menu_buttons_text[4].setString("Quit");
    option_buttons_text[0].setString("Low");
    option_buttons_text[1].setString("Medium");
    option_buttons_text[2].setString("High");
    name_text.setFont(main_f);
    name_text.setFillColor(sf::Color::Blue);
    name_text.setCharacterSize(40);
    help.loadFromFile("pomoc.gif");
    help_sprite.setTexture(help);
    score.setFont(main_f);
    score.setFillColor(sf::Color::Blue);
    score.setCharacterSize(30);
    score.setPosition(0, 0);
    main_win.setFramerateLimit(60);
    srand(time(NULL));
    main_p->load(0, 0);
    main_p->set_position(sf::Vector2f(50, 50));
    while (main_win.isOpen()) {
        mozna_tekst = false;
        while (main_win.pollEvent(events)) {
            if (events.type == sf::Event::Closed) {
                main_win.close();
            }
            if (events.type == sf::Event::KeyPressed) {
                if (events.key.code == sf::Keyboard::F1 && state != pomoc) {
                    state = pomoc;
                }
                else if (events.key.code == sf::Keyboard::F1 && state == pomoc) {
                    state = pauza;
                }
                if (events.key.code == sf::Keyboard::P && state != pomoc && muzyka == true) {
                    muzyka = false;
                    soundtrack.pause();
                }
                else if (events.key.code == sf::Keyboard::P && state != pomoc && muzyka == false) {
                    muzyka = true;
                    soundtrack.play();
                }
                if (events.key.code == sf::Keyboard::Escape && state != pauza) {
                    state = pauza;
                }
                else if (events.key.code == sf::Keyboard::Escape && state == pauza && game_started == true) {
                    state = gra;
                }
                if (events.key.code == sf::Keyboard::BackSpace && output.getSize() > 0) {
                    string temp_str = input.toAnsiString();
                    temp_str.pop_back();
                    input = temp_str;
                }
                if (events.key.code == sf::Keyboard::Enter && state == load_game && (option != 0 || current_step == nazwa)) {
                    current_step++;
                    input.clear();
                    option = 0;
                }

            }
            if (events.type == sf::Event::TextEntered) {
                if (events.text.unicode != static_cast<char>(8) && mozna_tekst == true && events.text.unicode != static_cast<char>(13)) {
                    if (num_input == false) {
                        input = input + events.text.unicode;
                    }
                    else {
                        if (isdigit(events.text.unicode)) {
                            input = input + events.text.unicode;
                        }
                    }
                }
            }
            if (events.type == sf::Event::MouseMoved) {
                mouse_position[x] = events.mouseMove.x * win_size_main / main_win.getSize().x;
                mouse_position[y] = events.mouseMove.y * win_size_main / main_win.getSize().y;
            }
        }
        if (state == gra) {
            saved = false;
            if (reset_timer == true) {
                timer.restart();
                reset_timer = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                main_p->move(sf::Vector2f(-5, 0), main_win.getSize());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                main_p->move(sf::Vector2f(5, 0), main_win.getSize());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                main_p->move(sf::Vector2f(0, 5), main_win.getSize());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                main_p->move(sf::Vector2f(0, -5), main_win.getSize());
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && bullet_reload == false && bullets.size() < main_p->ret_boolets()) {
                bullet_clock.restart();
                bullets.push_back(new bullet);
                int temp = bullets.size() - 1;
                bullets[temp]->load();
                bullets[temp]->set_position(sf::Vector2f(main_p->ret_pos().x + (main_p->ret_size().x / 2), main_p->ret_pos().y + (main_p->ret_size().y / 2)));
                bullet_reload = true;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                bullet_reload = false;
            }
            elapsed = timer.getElapsedTime();
            // tego nie tykać jest dobrze
            if (int(elapsed.asMilliseconds()) % spawn_time < 20 && utworzono == false) {
                kamienie.push_back(new kamien);
                int temp = kamienie.size() - 1;
                kamienie[temp]->load(5);
                kamienie[temp]->set_position(sf::Vector2f(win_size_main - 50, rand() % main_win.getSize().y));
                utworzono = true;
            }
            if (int(elapsed.asMilliseconds()) % spawn_time > 20) {
                utworzono = false;
            }
            if (elapsed.asMilliseconds() > 10000) {
                timer.restart();
            }
            int i = 0;
        start_check:
            for (int i = 0; i < kamienie.size(); i++) {
                for (int j = 0; j < bullets.size(); j++) {
                    if (bullets[j]->check_colision(kamienie[i]->ret_sprite().getPosition(), sf::Vector2f(rocksize, rocksize))) {
                        delete bullets[j];
                        bullets[j] = nullptr;
                        delete kamienie[i];
                        kamienie[i] = nullptr;
                        bullets[j] = bullets[bullets.size() - 1];
                        bullets[bullets.size() - 1] = nullptr;
                        bullets.pop_back();
                        kamienie[i] = kamienie[kamienie.size() - 1];
                        kamienie[kamienie.size() - 1] = nullptr;
                        kamienie.pop_back();
                        goto start_check;
                    }
                }
            }
            for (int i = 0; i < kamienie.size(); i++) {
                if (kamienie[i]->move(sf::Vector2f(-rock_speed, 0), sf::Vector2u(win_size_main, win_size_main)) == -1) {
                    delete kamienie[i];
                    kamienie[i] = nullptr;
                    kamienie[i] = kamienie[kamienie.size() - 1];
                    kamienie[kamienie.size() - 1] = nullptr;
                    kamienie.pop_back();
                    i--;
                }
            }
            for (int i = 0; i < bullets.size(); i++) {
                if (bullets[i]->move(sf::Vector2f(10, 0), sf::Vector2u(win_size_main, win_size_main)) == -1) {
                    delete bullets[i];
                    bullets[i] = nullptr;
                    bullets[i] = bullets[bullets.size() - 1];
                    bullets[bullets.size() - 1] = nullptr;
                    bullets.pop_back();
                    i--;
                }
            }
            // do tego momentu
            main_p->change_pkt(1 * ((1 / main_p->ret_boolets()) + rock_speed / 2 + spawn_time / 1000));
            pkt_string = std::to_string(int(main_p->ret_pkt()));
            sf_pkt_string = pkt_string;
            score.setString(sf_pkt_string);
            //drawing   
            main_win.clear(sf::Color(0, 35, 100));
            for (auto& var : kamienie) {
                main_win.draw(var->ret_sprite());
                if (main_p->check_colision(var->ret_sprite().getPosition(), sf::Vector2f(rocksize, rocksize)))
                    state = collision;
            }
            for (auto& var : bullets) {
                main_win.draw(var->ret_sprite());
            }
            main_win.draw(score);
            main_win.draw(main_p->ret_sprite());
            main_win.draw(name_text);
            main_win.display();
        }
        else if (state == pauza) {
            main_win.clear(sf::Color::Blue);
            reset_timer = true;
            if (mouse_position[x] >= menu_buttons[4].getPosition().x && mouse_position[x] <= menu_buttons[4].getPosition().x + button_x_size && mouse_position[y] >= menu_buttons[4].getPosition().y && mouse_position[y] <= menu_buttons[4].getPosition().y + button_y_size && sf::Mouse::isButtonPressed(sf::Mouse::Left)) main_win.close();
            if (mouse_position[x] >= menu_buttons[1].getPosition().x && mouse_position[x] <= menu_buttons[1].getPosition().x + button_x_size && mouse_position[y] >= menu_buttons[1].getPosition().y && mouse_position[y] <= menu_buttons[1].getPosition().y + button_y_size && sf::Mouse::isButtonPressed(sf::Mouse::Left) && saved == falsig) {
                state = save_to_file;
                saved = true;
            }
            if (mouse_position[x] >= menu_buttons[2].getPosition().x && mouse_position[x] <= menu_buttons[2].getPosition().x + button_x_size && mouse_position[y] >= menu_buttons[2].getPosition().y && mouse_position[y] <= menu_buttons[2].getPosition().y + button_y_size && sf::Mouse::isButtonPressed(sf::Mouse::Left)) state = load_from_file;
            if (mouse_position[x] >= menu_buttons[3].getPosition().x && mouse_position[x] <= menu_buttons[3].getPosition().x + button_x_size && mouse_position[y] >= menu_buttons[3].getPosition().y && mouse_position[y] <= menu_buttons[3].getPosition().y + button_y_size && sf::Mouse::isButtonPressed(sf::Mouse::Left)) state = pomoc;
            if (mouse_position[x] >= menu_buttons[0].getPosition().x && mouse_position[x] <= menu_buttons[0].getPosition().x + button_x_size && mouse_position[y] >= menu_buttons[0].getPosition().y && mouse_position[y] <= menu_buttons[0].getPosition().y + button_y_size && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                while (sf::Mouse::isButtonPressed(sf::Mouse::Left)); state = load_game;
            }
            for (auto& var : menu_buttons) { main_win.draw(var); }
            for (auto& var : menu_buttons_text) { main_win.draw(var); }
            for (auto& var : shields) { main_win.draw(var); }
            main_win.display();
        }
        else if (state == load_game) {
            main_win.clear(sf::Color(125, 125, 251));
            if (mouse_position[x] >= option_buttons[0].getPosition().x
                && mouse_position[x] <= option_buttons[0].getPosition().x + button_x_size
                && mouse_position[y] >= option_buttons[0].getPosition().y
                && mouse_position[y] <= option_buttons[0].getPosition().y + button_y_size
                && sf::Mouse::isButtonPressed(sf::Mouse::Left))
                option = 1;
            if (mouse_position[x] >= option_buttons[1].getPosition().x
                && mouse_position[x] <= option_buttons[1].getPosition().x + button_x_size
                && mouse_position[y] >= option_buttons[1].getPosition().y
                && mouse_position[y] <= option_buttons[1].getPosition().y + button_y_size
                && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                option = 2;
            }
            if (mouse_position[x] >= option_buttons[2].getPosition().x
                && mouse_position[x] <= option_buttons[2].getPosition().x + button_x_size
                && mouse_position[y] >= option_buttons[2].getPosition().y
                && mouse_position[y] <= option_buttons[2].getPosition().y + button_y_size
                && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                while (sf::Mouse::isButtonPressed(sf::Mouse::Left));
                option = 3;
            }
            if (input.getSize() > 19) {
                mozna_tekst = false;
            }
            else {
                if (current_step == nazwa) {
                    mozna_tekst = true;
                }
            }
            switch (current_step) {
            case nazwa:
                output = "Enter name (up to 20 characters)";
                main_score->name = input;
                break;
            case pociski:
                mozna_tekst = false;
                output = "Choose number of bullets \n(Low - 1 Normal - 3 High - 5)";
                switch (option) {
                case 1:
                    main_score->bullets = 1;
                    break;
                case 2:
                    main_score->bullets = 3;
                    break;
                case 3:
                    main_score->bullets = 5;
                    break;
                }
                break;
            case czas:
                output = "Choose number of time between rocks \n(Low -300ms Normal - 500ms High - 1000ms)";
                switch (option) {
                case 1:
                    main_score->rocks = 300;
                    break;
                case 2:
                    main_score->rocks = 500;
                    break;
                case 3:
                    main_score->rocks = 1000;
                    break;
                }
                break;
            case predkosc:
                output = "Choose velocity of rocks \n(Low - 3 Normal -5 High - 10)";
                switch (option) {
                case 1:
                    main_score->speed = 3;
                    break;
                case 2:
                    main_score->speed = 5;
                    break;
                case 3:
                    main_score->speed = 10;
                    break;
                }
                break;
            case end_load:
                delete main_p;
                main_p = new player();
                main_p->load(0, main_score->bullets);
                spawn_time = main_score->rocks;
                rock_speed = main_score->speed;
                main_score->pkt = main_p->ret_pkt();
                current_step = 0;
                game_started = richtig;
                mozna_tekst = false;
                main_score->not_null = true;
                name_text.setString(main_score->name);
                name_text.setPosition(sf::Vector2f(0, win_size_main - 40));
                state = gra;

            }
            prompt.setString(output);
            output_text.setString(input);
            if (current_step != nazwa) {
                for (auto& var : option_buttons) {
                    var.setFillColor(sf::Color::Cyan);
                    main_win.draw(var);
                }
                if (option != 0) {
                    option_buttons[option - 1].setFillColor(sf::Color(40, 40, 40));
                    main_win.draw(option_buttons[option - 1]);
                }
                for (auto& var : option_buttons_text) {
                    main_win.draw(var);
                }
            }
            main_win.draw(prompt);
            main_win.draw(output_text);
            main_win.display();
        }
        else if (state == collision) {
            main_win.clear(sf::Color(125, 125, 255));
            output = "GAME OVER\n YOUR SCORE:" + sf_pkt_string;
            prompt.setString(output);
            main_win.draw(prompt);
            delete main_p;
            main_p = nullptr;
            main_win.display();
            for (int i = kamienie.size() - 1; i >= 0; i--) {
                delete kamienie[i];
                kamienie[i] = nullptr;
                kamienie.pop_back();
            }
            for (int i = bullets.size() - 1; i >= 0; i--) {
                delete bullets[i];
                bullets[i] = nullptr;
                bullets.pop_back();
            }
            game_started = falsig;
        }
        else if (state == load_from_file) {
            for (int i = kamienie.size() - 1; i >= 0; i--) {
                delete kamienie[i];
                kamienie[i] = nullptr;
                kamienie.pop_back();
            }
            for (int i = bullets.size() - 1; i >= 0; i--) {
                delete bullets[i];
                bullets[i] = nullptr;
                bullets.pop_back();
            }
            player_score temp_score;
            main_filehandler.load_from_file_game(temp_score);
            if (temp_score.not_null) {
                delete main_score;
                main_score = nullptr;
                main_score = new player_score(temp_score);
                delete main_p;
                main_p = new player();
                main_p->load(0, main_score->bullets);
                spawn_time = main_score->rocks;
                rock_speed = main_score->speed;
                main_p->change_pkt(main_score->pkt);
                current_step = 0;
                game_started = richtig;
                name_text.setString(main_score->name);
                name_text.setPosition(sf::Vector2f(0, win_size_main - 40));
            }
            state = pauza;
        }
        else if (state == save_to_file) {
            if (game_started == richtig) {
                main_score->pkt = main_p->ret_pkt();
                main_filehandler.save_to_file_game(*main_score);
            }
            state = pauza;
        }
        else {
            main_win.clear(sf::Color::Black);
            main_win.draw(help_sprite);
            reset_timer = true;
            main_win.display();
        }

    }
    return 0;
}