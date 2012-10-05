#include "lib.h"
using namespace RushHour;

Car::Car(int x, int y, int length, int orientation)
{
  this->x = x;
  this->y = y;
  this->length = length;
  this->orientation = orientation;
  this->myCar = false;
}

Car::~Car(){}

bool Car::canMoveAscending(std::vector<Car> &cars, int exception) const
{
  if(this->orientation == HORIZONTAL){
    if(this->y+length == RushHour::parkingWidth-2 && !this->myCar){
      return false;
    }
    for(unsigned int i = 0; i < cars.size(); i++){
      if(i != exception){
        if(cars[i].orientation == HORIZONTAL){
          if(this->y+length == cars[i].y && this->x == cars[i].x){
            return false;
          }
        }
        if(cars[i].orientation == VERTICAL){
          if(this->y + this->length == cars[i].y &&
            this->x >= cars[i].x &&
            this->x < cars[i].x + cars[i].length){
              return false;
          }
        }
      }
    }
  }
  else{
    if(this->x+length == RushHour::parkingHeight-2){
      return false;
    }
    for(unsigned int i = 0; i < cars.size(); i++){
      if(i != exception){
        if(cars[i].orientation == VERTICAL){
          if(this->x+length == cars[i].x && this->y == cars[i].y){
            return false;
          }
        }
        else{
          if(this->x + this->length == cars[i].x &&
            this->y >= cars[i].y &&
            this->y < cars[i].y + cars[i].length){
              return false;
          }
        }
      }
    }
  }
  return true;
}

bool Car::canMoveDescending(std::vector<Car> &cars, int exception) const 
{
  if(this->orientation == HORIZONTAL){
    if(this->y == 0)
      return false;
    for(unsigned int i = 0; i < cars.size(); i++){
      if(i != exception){
        if(cars[i].orientation == HORIZONTAL){
          if(this->y == cars[i].y + cars[i].length && this->x == cars[i].x){
            return false;
          }
        }
        else{
          if(this->y-1 == cars[i].y && 
            this->x >= cars[i].x && 
            this->x < cars[i].x + cars[i].length){
              return false;
          }
        }
      }
    }
  }
  else{
    if(this->x == 0)
      return false;

    for(unsigned int i = 0; i < cars.size(); i++){
      if(i != exception){
        if(cars[i].orientation == VERTICAL){
          if(this->x == cars[i].x + cars[i].length && this->y == cars[i].y){
            return false;
          }
        }
        else{
          if((this->x-1 == cars[i].x) && (this->y >= cars[i].y) && 
            (this->y < cars[i].y + cars[i].length)){
              return false;
          }
        }
      }
    }
  }
  return true;
}

State::State()
{
}

State::~State()
{
}

void State::operator=(State &s)
{
  this->parent = s.parent;
  this->cars = s.cars;
  this->last_moved = s.last_moved;
}

std::vector<State> State::getChildStates()
{
  std::vector<State> childs;
  State dupState = *this;

  for(unsigned int i = 0; i < dupState.cars.size(); i++){
    for(int j = 0; j < std::max(parkingWidth, parkingHeight); j++){
      if(dupState.cars[i].canMoveAscending(dupState.cars, i) && dupState.last_moved !=i){
        dupState.cars[i].moveAscending();
        childs.push_back(dupState);
        childs[childs.size()-1].last_moved = i;
      }
      else break;
    }
    dupState = *this;
    for(int j = 0; j < std::max(parkingWidth, parkingHeight); j++){
      if(dupState.cars[i].canMoveDescending(dupState.cars, i)&& dupState.last_moved !=i){
        dupState.cars[i].moveDescending();
        childs.push_back(dupState);
        childs[childs.size()-1].last_moved = i;
      }
      else break;
    }
    dupState = *this;
  }

  return childs;
}

bool State::operator==(State &s) const
{ 
  for(unsigned int i = 0; i < this->cars.size();i++){
    if(this->cars[i] != s.cars[i])
      return false;
  }
  return true;
}

bool State::operator!=(State &s) const
{
  for(unsigned int i = 0; i < this->cars.size();i++){
    if(this->cars[i] != s.cars[i])
      return true;
  }
  return false;
}

bool State::operator<(State &s) const
{
  for(unsigned int i = 0; i < s.cars.size(); i++){
    if(this->cars[i] < s.cars[i])
      return true;
    if(this->cars[i] < s.cars[i])
      return false;
  }
  return false;
}

bool State::operator>(State &s) const
{
  for(unsigned int i = 0; i < s.cars.size(); i++){
    if(this->cars[i] > s.cars[i])
      return true;
    if(this->cars[i] < s.cars[i])
      return false;
  }
  return false;
}

bool RushHour::loadCars(const char* filename, State &rootState)
{
  int i;
  FILE *f = fopen(filename, "r");
  if(f == NULL){
    return false;
  }
  fscanf_s(f, "%d %d\n", &parkingHeight, &parkingWidth);
  char **inMatrix;
  inMatrix = (char**)malloc(sizeof(char*)*parkingHeight);
  for(i = 0; i < parkingHeight; i++)
    inMatrix[i] = (char*)malloc(sizeof(char)*(parkingWidth*2+1));

  i = 0;
  while(fgets(inMatrix[i++], 2*parkingWidth+1, f));
  fclose(f);
  bool foundMyCar = false;
  for(i = 1; i < parkingHeight-1; i++){
    for(int j = 1; j < parkingWidth-1; j++){
      if(inMatrix[i][j*2] == '^'){
        int length = 0, k = i;
        while(inMatrix[k++][j*2] != 'v');
        length = k-i;
        rootState.cars.push_back(Car(i-1, j-1, length, VERTICAL));
      }
      if(inMatrix[i][j*2] == '<'){
        int length = 0, k = j;
        while(inMatrix[i][(k++)*2] != '>');
        length = k-j;
        rootState.cars.push_back(Car(i-1, j-1, length, HORIZONTAL));
      }
      if(inMatrix[i][j*2] == '?' && !foundMyCar){
        int length = 0, k = j;
        while(inMatrix[i][(k++)*2] == '?');
        length = k-j-1;
        rootState.cars.insert(rootState.cars.begin(), 
          Car(i-1, j-1, length, HORIZONTAL));
        rootState.cars[0].myCar = true;
        foundMyCar = true;
      }
    }
    if(inMatrix[i][(parkingWidth-1)*2] == '*'){
      exit.first = i-1;
      exit.second = parkingWidth-1;
    }
  }
  rootState.last_moved = -1;
  for(i = 0; i < parkingHeight; i++)
    free(inMatrix[i]);
  free(inMatrix);
  return true;
}

bool binInsert(std::vector<State*> &v, State *s, int i, int j)
{
  if(i == j)
    if(*s == *(v[i]))
      return true;
    else
      if(*s > *(v[i])){
        v.insert(v.begin()+i+1, s);
        return false;
      }
      else{
        v.insert(v.begin()+i, s);
        return false;
      }
      int m = (i+j)/2;
      if(*(v[m]) == *s)
        return true;
      if(*(v[m]) > *s)
        return binInsert(v, s, i, m);
      else return binInsert(v, s, m+1, j);
}

std::vector<State> getExitSolutionV2(State rootState)
{
  std::vector<State> exitSolution;
  State end;
  std::queue<State*> q;
  std::vector<State*> visitedStates;
  visitedStates.push_back(&rootState);
  visitedStates[0]->parent = NULL;
  rootState.parent = NULL;//?
  q.push(visitedStates[0]);
  while(!q.empty()){
    State* currentState = q.front();
    q.pop();
    std::vector<State> childState = currentState->getChildStates();
    for(unsigned int i = 0; i < childState.size(); i++){
      State *s = new State();
      *s = childState[i];
      s->parent = currentState;
      if(!binInsert(visitedStates, s, 0, visitedStates.size()-1)){
        q.push(s);
        if(childState[i].isEndState()){
          end = *s;
          goto exitfoundV2;
        }
      }
      else{
        delete s;
      }
    }
  }
exitfoundV2:
  if(end.cars.size() == 0)
    return exitSolution;

  exitSolution.push_back(end);
  while(end.parent){
    end = *end.parent;
    exitSolution.push_back(end);
  }
  return exitSolution;
}

void exportExitSolution(std::vector<State> &exitSolution)
{
  FILE *f = fopen("out.txt", "w");
  if(f == NULL){
    std::cout<<"Error writing file"<<std::endl;
    return;
  }
  else{
    fprintf(f, "%d\n\n", exitSolution.size());
    char **outMatrix;
    outMatrix = (char**)malloc(sizeof(char*)*parkingHeight);
    for(int i = 0; i < parkingHeight; i++){
      outMatrix[i] = (char*)malloc(sizeof(char)*(parkingWidth*2+1));
      for(int k = 0; k < parkingWidth-2; k++){
        outMatrix[i][k] = '-';
      }
    }

    for(int i = exitSolution.size()-1; i >=0; i--){
      for(int j = 0; j < parkingHeight-2; j++){
        for(int k = 0; k < parkingWidth-2; k++){
          outMatrix[j][k] = '-';
        }
      }

      for(unsigned int  j = 1; j < exitSolution[i].cars.size(); j++){
        if(exitSolution[i].cars[j].orientation == HORIZONTAL){
          outMatrix[exitSolution[i].cars[j].x][exitSolution[i].cars[j].y] = '<';
          outMatrix[exitSolution[i].cars[j].x][exitSolution[i].cars[j].y + exitSolution[i].cars[j].length-1] = '>';
        }
        else{
          outMatrix[exitSolution[i].cars[j].x][exitSolution[i].cars[j].y] = '^';
          outMatrix[exitSolution[i].cars[j].x+exitSolution[i].cars[j].length-1][exitSolution[i].cars[j].y] = 'v';
          for(int k = exitSolution[i].cars[j].x+1; k < exitSolution[i].cars[j].x+exitSolution[i].cars[j].length-1; k++){
            outMatrix[k][exitSolution[i].cars[j].y] = '|';
          }
        }
      }
   
      for(int k = 0; k < exitSolution[i].cars[0].length; k++){
        outMatrix[exitSolution[i].cars[0].x][std::min(exitSolution[i].cars[0].y + k, parkingWidth-2)] = '?';
      }
      for(int j = 0; j < parkingWidth; j++)
        fprintf(f, "0 ");
      fprintf(f, "\n");
      for(int i = 0; i < parkingHeight-2; i++){
        fprintf(f, "0 ");
        for(int j = 0; j < parkingWidth-2; j++){
          fprintf(f, "%c ", outMatrix[i][j]);
        }
        if(i == RushHour::exit.first)
          fprintf(f, "* ");
        else fprintf(f, "0 ");
        fprintf(f, "\n");
      }
      for(int j = 0; j < parkingWidth; j++)
        fprintf(f, "0 ");
      fprintf(f, "\n\n");
    }
    for(int i = 0; i < parkingHeight; i++)
      delete outMatrix[i];
    delete outMatrix;
  }
  fclose(f);
}

int main()
{
  clock_t start, finish;
  start = clock();
  State rootState;
  if(loadCars("in.txt", rootState)){
    std::vector<State> exitSol;
    exitSol = getExitSolutionV2(rootState);
    exportExitSolution(exitSol);
  }
  else{
    std::cout<<"Error reading input file"<<std::endl;
    _getch();
    return 0;
  }
  finish = clock();
  std::cout<<std::endl;
#ifdef OUTPUT
  std::cout<<"Time:"<<(float)(finish-start)/CLOCKS_PER_SEC<<std::endl;
  system("notepad out.txt");
  _getch();
#endif
  return 0;
}