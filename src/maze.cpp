/* Your code here! */
#include "maze.h"
#include <iostream>

//directions: right = 0, down = 1, left = 2, up = 3

SquareMaze::SquareMaze(){
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if(exists){
        if(dir == 0){ //right
          rightval[x][y] = true;
        }else if (dir == 1) { //down wall
          downval[x][y] = true; 
        }
    }else{ //delete wall
        if(dir == 0){ //right wall
          rightval[x][y] = false; //down wall
        } else if (dir == 1){ //down wall
          downval[x][y] = false; //right wall
        }   
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const{
    if(dir == 2 && x!=0 && (rightval[x-1][y] == false)){ //left, can be none or down
      return true;
    } else if (dir == 0 && x<widtha-1 && (rightval[x][y] == false)){ //right, can be none or down
      return true;
    } else if (dir == 1 && y<heighta-1 && (downval[x][y] == false)){ //down, can be none or right
      return true;
    } else if (dir == 3 && y!=0 && (downval[x][y-1] == false)){ //up, can be none or right
      return true;
    }
    return false;
}

void SquareMaze::makeMaze(int width, int height){
    widtha = width;
    heighta = height;
    int newsize = width*height;
    disjsets.addelements(newsize);
    rightval.resize(height,std::vector<bool>(width,true));
    downval.resize(height,std::vector<bool>(width,true));

    for(int i = 0; i < width; i++){
      for(int j = 0; j < height; j++){
        int randgen = rand() % 2; //random generator that gives 0 or 1, 0 being down, 1 being right
        if(randgen == 0){
          if(i != width-1 && disjsets.find(width*j+i) != disjsets.find(width*j+(i+1))){
            setWall(i,j,0,false); //delete right wall
            disjsets.setunion(width*j+i, width*j+(i+1));
          }
          else if(j != height-1 && disjsets.find(width*j+i) != disjsets.find(width*(j+1)+i)){
            setWall(i,j,1,false); //delete down wall
            disjsets.setunion(width*j+i, width*(j+1)+i);
          }
        } else if(randgen == 1){
          if(j != height-1 && disjsets.find(width*j+i) != disjsets.find(width*(j+1)+i)){
            setWall(i,j,1,false); //delete down wall
            disjsets.setunion(width*j+i, width*(j+1)+i);
          }
          else if(i != width-1 && disjsets.find(width*j+i) != disjsets.find(width*j+(i+1))){
            setWall(i,j,0,false); //delete right wall
            disjsets.setunion(width*j+i, width*j+(i+1));
          }
        }
      }
    }
}

cs225::PNG* SquareMaze::drawMaze() const{
    cs225::PNG* rv = new cs225::PNG(widtha*10+1,heighta*10+1); //(width*10+1,height*10+1)
    //Blacken the entire topmost row and leftmost column of pixels, except the entrance (1,0) through (9,0)
    for(int i = 0; i<widtha*10+1; i++){ //topmost row
      if(i == 0 || i > 9){ //except the entrance (1,0) through (9,0)
        (rv->getPixel(i,0)).l = 0; //darkening the row
      }
    }
    for(int j = 0; j<heighta*10+1; j++){ //leftmost col
      (rv->getPixel(0,j)).l = 0; //darkening the col
    }
    //For each square in the maze, call its maze coordinates (x,y)
    //If the right wall exists, then blacken the pixels with coordinates 
    //((x+1)*10,y*10+k) for k from 0 to 10.
    //If the bottom wall exists, then blacken the pixels with coordinates 
    //(x*10+k, (y+1)*10) for k from 0 to 10.
    for(int i=0; i<widtha; i++){
      for(int j=0; j<heighta; j++){
        if(rightval[i][j]){ //right wall
          for(int k = 0; k<=10; k++){
            (rv->getPixel((i+1)*10, j*10+k)).l = 0; //blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from 0 to 10.
          }
        }
        if(downval[i][j]){ //down wall
          for(int k = 0; k<=10; k++){
            (rv->getPixel(i*10+k, (j+1)*10)).l = 0; //blacken the pixels with coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
          }
        }
      }
    }
    return rv;
}

std::vector<int> SquareMaze::solveMaze(){
    std::vector<std::vector<bool>> visited(heighta,std::vector<bool>(widtha,false)); //Need a way to keep track of which cells have already been visited
    std::vector<int> potential(widtha*heighta,-1); //Need a way to keep track of potential paths
    //use bfs
    std::deque<int> bfs;
    bfs.push_front(0);
    visited[0][0] = true;
    int index = bfs.front();
    while(!bfs.empty()){
      index = bfs.front();
      int x = index%widtha;
      int y = index/widtha;
      bfs.pop_front();
      if(canTravel(x,y,0) && !visited[x+1][y]){
        potential[index+1] = index;
        visited[x+1][y] = true;
        bfs.push_front(index+1);
      }
      if(canTravel(x,y,1) && !visited[x][y+1]){
        potential[index+widtha] = index;
        visited[x][y+1] = true;
        bfs.push_front(index+widtha);
      }
      if(canTravel(x,y,2) && !visited[x-1][y]){
        potential[index-1] = index;
        visited[x-1][y] = true;
        bfs.push_front(index-1);
      }
      if(canTravel(x,y,3) && !visited[x][y-1]){
        potential[index-widtha] = index;
        visited[x][y-1] = true;
        bfs.push_front(index-widtha);
      }
    }
    int maxval = -1;
    index = -1;
    for(int i = widtha*heighta-1; i>=widtha*(heighta-1); i--){
      int distance = 0;
      int temp = i;
      while(potential[temp] != -1){
        distance++;
        temp = potential[temp];
      }
      if(distance >= maxval){
        index = i;
        if(distance != maxval){
          maxval = distance;
        }
      }
    }
    std::deque<int> rvbutqueue;
    for(int i=0; i<maxval; i++){
      if(potential[index] == index-1) rvbutqueue.push_front(0);
      if(potential[index] == index-widtha) rvbutqueue.push_front(1);
      if(potential[index] == index+1) rvbutqueue.push_front(2);
      if(potential[index] == index+widtha) rvbutqueue.push_front(3);
      index = potential[index];
    }
    std::vector<int> rv; 
    for(unsigned long i=0; i<rvbutqueue.size(); i++){
      rv.push_back(rvbutqueue[i]);
    }
    return rv;
}
    
cs225::PNG* SquareMaze::drawMazeWithSolution(){
    cs225::PNG* cur = drawMaze();
    std::vector<int> sol = solveMaze();
    cs225::HSLAPixel red(0, 1, 0.5, 1);
    //Start at pixel (5,5)
    int x = 5;
    int y = 5;
    //downward, color pixels (5,5) through (5,15) red(0,1,0.5,1)
    //right, color pixels (5,15) through (15,15) red
    //up, color pixels (15,15) through (15,5) red
    for(unsigned long i=0; i<sol.size(); i++){
      for(int j = 0; j<10; j++){
        cur->getPixel(x,y) = red;
        if(sol[i] == 0) x++; //right
        else if(sol[i] == 1) y++; //down
        else if(sol[i] == 2) x--; //left
        else if(sol[i] == 3) y--; //up
      }
    }

    cur->getPixel(x,y) = red;

    //call the destination maze coordinates (x,y), and whiten the pixels with coordinates (x*10+k, (y+1)*10) for k from 1 to 9
    x -= 4;
    y += 5;
    for (int i = 0; i < 9; i++) {
      (cur->getPixel(x+i,y)).l = 1;
    }

    return cur;
}
