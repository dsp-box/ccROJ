#ifndef _roj_hilbert_equiv_
#define _roj_hilbert_equiv_

/**
* @type: class
* @brief: Definition of roj_hilbert_equiv class.
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-complex-signal.hh"
class roj_complex_signal;
struct roj_signal_config;

#include "roj-fourier-spectr.hh"
class roj_fourier_spectrum;

/* ************************************************************************************************************************* */
/* Hilbert equivalent class definition */

class roj_hilbert_equiv{
private:

  /* internal data */
  roj_complex_signal* m_signal;
  roj_complex_signal* m_transform;
  roj_complex_signal* m_equivalent;

public:

  /* construction */
  roj_hilbert_equiv(roj_complex_signal*);
  ~roj_hilbert_equiv();

  /* configuration */
  roj_signal_config get_config();

  /* analytic equivalent */
  roj_complex_signal* get_equivalent ();
};

#endif

