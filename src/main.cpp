#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>

using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "LifeGame-SFML");
  window.setFramerateLimit(60);

  constexpr int mpsize = 100;
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

  vector<vector<pair<bool, sf::RectangleShape>>> mp(mpsize,vector<pair<bool,sf::RectangleShape>>(mpsize));

  for(int i=0;i<mpsize;i++){
    for(int j=0;j<mpsize;j++){
      mp[i][j].first = false;
      mp[i][j].second.setSize(sf::Vector2f(window.getSize().x/float(mpsize),window.getSize().y/float(mpsize)));
      mp[i][j].second.setPosition(sf::Vector2f(i*(window.getSize().x/float(mpsize)),j*(window.getSize().x/float(mpsize))));
    }
  }

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

    //update
    for(int i=0;i<mpsize;i++){
      for(int j=0;j<mpsize;j++){
        int cnt=0;
        for(auto &moore:mooreneighbors){
          sf::Vector2 pos(moore + mp[i][j].second.getPosition());
          if(mp[pos.y][pos.x].first){
            cnt++;
          }
        }

        if(mp[i][j].first){ // alive
          if(cnt != 2 and cnt != 3){
            mp[i][j].first = false;
          }
        }else{ //dead
          if(cnt == 3){
            mp[i][j].first = true;
          }
        }
      }
    }

    //draw
    for(int i=0;i<mpsize;i++){
      for(int j=0;j<mpsize;j++){
        if(mp[i][j].first){
          window.draw(mp[i][j].second);
        }
      }
    }

    window.display();
  }
}
