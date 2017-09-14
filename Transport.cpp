#include <iostream>
#include <cmath>

#include "Random.h"

//std::vector<double>

class Particle {
  public:
    double x, mu;
    bool   alive;
};

int main() {

  // setup problem
  int    nHistories    = 1e6;
  double total_xs      = 1.0;
  double scatter_ratio = 0.87;
  double thickness     = 4.0;

  double leakage_hist    = 0.0;
  double leakage         = 0.0;
  double leakage_squared = 0.0;

  // loop over histories
  for ( int i = 0 ; i < nHistories ; i++ ) { 
    // generate source particle
    Particle p;
    p.x     = 0.0;
    p.mu    = 1.0;
    p.alive = true;

    leakage_hist = 0.0;

    while ( p.alive ) {
      // do transport

      double dist_to_collision = -log( Urand() ) / total_xs;
      p.x += dist_to_collision * p.mu;

      if ( p.x < 0.0 || p.x > thickness ) {
        // leaked out
        p.alive = false;
        // tally if leaked out of right side
        if ( p.x > thickness ) {
          leakage_hist += 1.0;
        }
      }
      else {
        // have a collision
        if ( Urand() < scatter_ratio ) {
          // particle scatters
          p.mu = 2.0 * Urand() - 1.0;
        }
        else {
          // absorbed :(
          p.alive = false;
        }

      }

    } // end of particle loop

    leakage         += leakage_hist;
    leakage_squared += leakage_hist * leakage_hist;


  } // we are almost done!

  double mean  = leakage / nHistories;
  double stdev = sqrt( ( leakage_squared / nHistories - mean*mean ) / nHistories );

  std::cout << mean << "  " << stdev / mean << std::endl;

  return 0;
}

