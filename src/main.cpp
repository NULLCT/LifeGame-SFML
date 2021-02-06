#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <ctime>

using namespace std;

const vector<sf::Vector2f> mooreneighbors = {
  sf::Vector2f(0,1),
  sf::Vector2f(1,1),
  sf::Vector2f(1,0),
  sf::Vector2f(1,-1),
  sf::Vector2f(0,-1),
  sf::Vector2f(-1,-1),
  sf::Vector2f(-1,0),
  sf::Vector2f(-1,1)
};

void updateCells(vector<vector<pair<bool, sf::RectangleShape>>>& _begin,
                 vector<vector<pair<bool, sf::RectangleShape>>>& _end){

  //update
  for(int i=0;i<_begin.size();i++){
    for(int j=0;j<_begin.size();j++){
      int cnt=0;
      for(auto &moore:mooreneighbors){
        sf::Vector2 pos = moore + sf::Vector2f(j,i);
        if(0 <= pos.x and 0 <= pos.y and pos.x < _begin.size() and pos.y < _begin.size()){
          if(_begin[pos.y][pos.x].first)
            cnt++;
        }
      }

      if(_begin[i][j].first){ // alive
        if(not (cnt == 2 or cnt == 3)){
          _end[i][j].first = false;
        }else{
          _end[i][j].first = true;
        }
      }else{ //dead
        if(cnt == 3){
          _end[i][j].first = true;
        }else{
          _end[i][j].first = false;
        }
      }
    }
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "LifeGame-SFML");
  window.setFramerateLimit(60);

  srand(time(NULL));

  constexpr int mpsize = 100;

  // mp0 -> mp1 -> mp0 -> ...
  vector<vector<pair<bool, sf::RectangleShape>>> mp0(mpsize,vector<pair<bool,sf::RectangleShape>>(mpsize));
  vector<vector<pair<bool, sf::RectangleShape>>> mp1(mpsize,vector<pair<bool,sf::RectangleShape>>(mpsize));

ST:

  //init mp
  for(int i=0;i<mpsize;i++){
    for(int j=0;j<mpsize;j++){
      mp0[i][j].first = false;
      mp0[i][j].second.setSize(sf::Vector2f(window.getSize().x/float(mpsize),window.getSize().y/float(mpsize)));
      mp0[i][j].second.setPosition(sf::Vector2f(i*(window.getSize().x/float(mpsize)),j*(window.getSize().x/float(mpsize))));
    }
  }
  for(int i=0;i<mpsize;i++){
    for(int j=0;j<mpsize;j++){
      mp1[i][j].first = false;
      mp1[i][j].second.setSize(sf::Vector2f(window.getSize().x/float(mpsize),window.getSize().y/float(mpsize)));
      mp1[i][j].second.setPosition(sf::Vector2f(i*(window.getSize().x/float(mpsize)),j*(window.getSize().x/float(mpsize))));
    }
  }

  //Glider
  mp0[1][0].first = mp0[2][1].first = mp0[0][2].first = mp0[1][2].first = mp0[2][2].first = true;

  //Nebra
  {
    sf::Vector2i pos(10,50);

    for(int i=0;i<6;i++)
      mp0[pos.y][pos.x+i].first = mp0[pos.y+1][pos.x+i].first = true;
    for(int i=0;i<6;i++)
      mp0[pos.y+7][pos.x+i+3].first = mp0[pos.y+1+7][pos.x+i+3].first = true;

    for(int i=0;i<6;i++)
      mp0[pos.y+i][pos.x+7].first = mp0[pos.y+i][pos.x+7+1].first = true;
    for(int i=0;i<6;i++)
      mp0[pos.y+i+3][pos.x].first = mp0[pos.y+i+3][pos.x+1].first = true;
  }
  {
    sf::Vector2i pos(50,10);

    for(int i=0;i<6;i++)
      mp0[pos.y][pos.x+i].first = mp0[pos.y+1][pos.x+i].first = true;
    for(int i=0;i<6;i++)
      mp0[pos.y+7][pos.x+i+3].first = mp0[pos.y+1+7][pos.x+i+3].first = true;

    for(int i=0;i<6;i++)
      mp0[pos.y+i][pos.x+7].first = mp0[pos.y+i][pos.x+7+1].first = true;
    for(int i=0;i<6;i++)
      mp0[pos.y+i+3][pos.x].first = mp0[pos.y+i+3][pos.x+1].first = true;
  }

  for(auto && i: mp0) for(auto && j: i){
    j.first = rand()%2;
  }

  bool tr=false;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
      goto ST;
    }
    window.clear();

    if(tr){
      updateCells(mp1,mp0);
    }else{
      updateCells(mp0,mp1);
    }

    //draw
    for(int i=0;i<mpsize;i++){
      for(int j=0;j<mpsize;j++){
        if((tr ? mp1[i][j].first : mp0[i][j].first)){
          window.draw((tr ? mp1[i][j].second : mp0[i][j].second));
        }
      }
    }

    tr = not tr;
    window.display();
  }
}
