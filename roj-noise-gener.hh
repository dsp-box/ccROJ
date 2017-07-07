#ifndef _roj_noise_gener_
#define _roj_noise_gener_

/**
* @type: class
* @brief: Definitions of roj_noise_generator class.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;

/* pink macros */
#define PINK_ORDER 5
#define PINK_MAXKEY 0xf
#define PINK_RANGE 6e6

/* ************************************************************************************************************************* */
/* noise generator definition */

class roj_noise_generator{
private:

  /* internal configuration */
  unsigned int* m_white_buffer;
  int m_pink_key;

  /* rnak single sample */
  double rand_pink();

public:

  /* construction */
  roj_noise_generator();
  ~roj_noise_generator();

  /* awgn methods */
  double add_awgn_using_snr(roj_complex_signal*, double);
  double add_awgn_using_sigma(roj_complex_signal*, double);
  double add_awgn_at_exact_snr(roj_complex_signal*, double);

  /* pink noise methods */
  double add_pink_using_range(roj_complex_signal*, double);
  double add_pink_at_exact_snr(roj_complex_signal*, double);
};
  
#endif

