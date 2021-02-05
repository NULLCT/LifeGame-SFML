#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <tuple>

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

  constexpr int mpsize = 100;

  // mp0 -> mp1 -> mp0 -> ...
  vector<vector<pair<bool, sf::RectangleShape>>> mp0(mpsize,vector<pair<bool,sf::RectangleShape>>(mpsize));
  vector<vector<pair<bool, sf::RectangleShape>>> mp1(mpsize,vector<pair<bool,sf::RectangleShape>>(mpsize));

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

  mp0[1][0].first = mp0[2][1].first = mp0[0][2].first = mp0[1][2].first = mp0[2][2].first = true;

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
