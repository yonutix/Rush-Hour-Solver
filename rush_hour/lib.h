#include <iostream>
#include <vector>
#include <queue>
#include <conio.h>
#include <time.h>

//fopen warning
#pragma warning(disable : 4996)

//#define OUTPUT

namespace RushHour{

  /**
  * \brief Pozitiile in care se poate afla masina
  */
  enum{VERTICAL, HORIZONTAL};

  /**
  * \param parkingWidth Latimea parcarii plus marginile
  */
  static int parkingWidth;

  /**
  * \param parkingWidth Lungimea parcarii plus marginile
  */
  static int parkingHeight;

  /**
  * \param exit Pozitia iesirii
  */
  static std::pair<int, int> exit;


  /**
  * \class Car este clasa pentru o masina
  */
  class Car{
  public:
    /**
    * \param x Linia pe care se afla masina
    * \param y Coloana pe care se afla masina
    */
    int y, x;
    /**
    * \param length Lungimea masinii
    */
    int length;

    /**
    * \param orientation Orientarea masinii
    */
    int orientation;

    /**
    * \param myCar Este true daca obiectul este masina rosie
    */
    bool myCar;

    Car(int x, int y, int length, int orientation);

    ~Car();

    /**
    * \return Returneaza true daca masina poate avansa cu o casuta
    * \param i Masina a carei mutare este incercata
    * \param cars Toate masinile inclusiv cea a carei mutare este
    * incercata
    */
    bool canMoveAscending(std::vector<Car> &cars, int exception) const;

    /**
    * \return Returneaza true daca masina i din vectorul cars poate
    * fi mutata inapoi cu o casuta;
    */
    bool canMoveDescending(std::vector<Car> &cars, int exception) const;

    /**
    * \brief Muta masina inainte cu o casuta
    */
    inline void moveAscending() {(this->orientation == HORIZONTAL)?y++:x++;}

    /**
    * \brief Muta masina inapoi cu o casuta
    */
    inline void moveDescending() {(this->orientation == HORIZONTAL)?y--:x--;}

    inline bool operator==(Car &c) const {return (memcmp(this, &c, 8) == 0);}

    inline bool operator!=(Car &c) const {return (memcmp(this, &c, 8) != 0);}


    inline bool operator<(Car &c) const {
      return (this->x < c.x)?true:(this->x > c.x)?false:(this->y < c.y);
    }

    inline bool operator>(Car &c) const {
      return (this->x > c.x)?true:(this->x < c.x)?false:this->y > c.y;
    }

  };

  /**
  * \class State Este o configuratie a tablei de joc
  */
  class State{
  public:
    /**
    * \param cars Masinile de pe tabla
    */
    std::vector<Car> cars;

    /**
    * \param parent Parintele starii curente, NULL daca 
    * starea este cea de inceput
    */
    State *parent;

    /**
    * \param last_moved Indexul ultimei masini mutate
    * -1 daca este starea de inceput
    */
    int last_moved;

    State();

    ~State();
    /**
    * \return Returneaza true daca masina a iesit
    */
    inline bool isEndState() const {return cars[0].y == parkingWidth-2;}

    /**
    * \return Returneaza un vector cu starile in care
    * poate avansa plansa
    */
    std::vector<State> getChildStates();

    bool operator==(State &s) const;

    bool operator!=(State &s) const;

    bool operator<(State &s) const;

    bool operator>(State &s) const;

    void operator=(State &s);
  };

  /**
  * \brief incarca configuratia initiala din fisier
  */
  bool loadCars(const char* filename, State &rootState); 
};
