#ifndef _roj_fourier_spectrum_
#define _roj_fourier_spectrum_

/**
* @type: class
* @brief: Definition of roj_fourier_spectrum class.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;

/* ************************************************************************************************************************* */
/* fourier spectrum class definition */

class roj_fourier_spectrum{
private:

  /* internal configuration */
  roj_signal_config m_config;

  /* fft shift */
  void fft_shift();

public:

  /* construction */
  roj_fourier_spectrum(roj_complex_signal*);
  ~roj_fourier_spectrum();

  /* config methods */
  roj_signal_config* get_config();

  /* inverse transform */
  roj_complex_signal* get_signal();
  
  /* save to file */
  void save(char *);

  /* analysis */
  double calc_energy();

  /* complex delay spectrum */
  roj_fourier_spectrum* get_complex_group_delay ();

  /**
   * @type: field
   * @brief: This field gives access to spectral lines.
   */
  complex double* m_spectrum; 

};

#endif

