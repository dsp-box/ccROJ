#ifndef _roj_fft_analyzer_
#define _roj_fft_analyzer_

/**
* @type: class
* @brief: Definition of roj_fft_analyzer class.
* @herit: roj_fft_analyzer : roj_xxt_analyzer : roj_analyzer
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-xxt-analyzer.hh"
class tf_xxt_analyzer;

#include "roj-window-gener.hh"
class roj_window_generator;

/* ************************************************************************************************************************* */
/* fft analyzer class definition */

class roj_fft_analyzer
  : public roj_xxt_analyzer{

private:

  /* calc stft */
  complex double ** transforming(int, int);
  
public:

  /* construction */
  roj_fft_analyzer(roj_array_config, roj_window_generator*);
};

#endif

