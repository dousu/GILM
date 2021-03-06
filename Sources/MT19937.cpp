#include "MT19937.h"

MT19937::MT19937() {
  // TODO Auto-generated constructor stub
}

MT19937::~MT19937() {
  // TODO Auto-generated destructor stub
}

unsigned long long int MT19937::icount = 0LL;
unsigned long long int MT19937::rcount = 0LL;

std::mt19937 MT19937::igen(111);
std::uniform_int_distribution<> MT19937::idist(0, INT_MAX);
// boost::variate_generator<boost::mt19937, boost::uniform_int<int> > MT19937::_irand(MT19937::igen, MT19937::idist);

std::mt19937 MT19937::rgen(111);
std::uniform_real_distribution<> MT19937::rdist(0, 1);
// boost::variate_generator<boost::mt19937, boost::uniform_real<double> > MT19937::_rrand(MT19937::rgen, MT19937::rdist);

int MT19937::irand(void) {
  if (icount < ULLONG_MAX - 1)
    icount++;
  else {
    std::cerr << "cannot use random any more" << std::endl;
    exit(1);
  }

  return idist(igen);
}

int MT19937::irand(int a, int b) {
  if (icount < ULLONG_MAX - 1)
    icount++;
  else {
    std::cerr << "cannot use random any more" << std::endl;
    exit(1);
  }
  std::uniform_int_distribution<> dist(a, b);
  return dist(igen);
}

double MT19937::rrand(void) {
  if (rcount < ULLONG_MAX - 1)
    rcount++;
  else {
    std::cerr << "cannot use random any more" << std::endl;
    exit(1);
  }
  return rdist(igen);
}

double MT19937::rrand(double a, double b) {
  if (rcount < ULLONG_MAX - 1)
    rcount++;
  else {
    std::cerr << "cannot use random any more" << std::endl;
    exit(1);
  }
  std::uniform_real_distribution<> dist(a, b);
  return dist(igen);
}

void MT19937::waste(void) {
  igen.discard(icount);
  rgen.discard(rcount);
}

void MT19937::set_seed(std::size_t seed_value) {
  igen.seed(seed_value);
  rgen.seed(seed_value);
}
