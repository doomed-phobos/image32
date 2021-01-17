#ifndef TIME_H
#define TIME_H
#include <cstdio>
#include <ctime>
//TODO: Muestra el tiempo de una rutina, ELIMINAR CUANDO NO SE NECESITE
class Timer
{
public:
   Timer() {m_start = clock();}
   ~Timer() {
      m_end = clock();
      double secs = (double)(m_end - m_start) / CLOCKS_PER_SEC;

      printf("Tiempo: %f\n", secs);
   }
private:
   std::clock_t m_start, m_end;
};

#endif