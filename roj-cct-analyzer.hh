#ifndef _roj_cct_analyzer_
#define _roj_cct_analyzer_

/**
 * @type: class
 * @brief: Definition of roj_cct_analyzer class.
 * @herit: roj_cct_analyzer : roj_xxt_analyzer : roj_analyzer
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-xxt-analyzer.hh"
class roj_xxt_analyzer;

#include "roj-window-gener.hh"
class roj_window_generator;

/* ************************************************************************************************************************* */
/* changeable chirplet analyzer class definition */

class roj_cct_analyzer
  : public roj_xxt_analyzer{

private:
  
  /* chirprate can be changed for each instant */
  complex double ** transforming(int, int);

  /* external information about chirprate */
  roj_real_array *m_chirprate;
  
public:

  /* construction */
  roj_cct_analyzer(roj_array_config, roj_window_generator*);
  ~roj_cct_analyzer(); 

  /* instantaneous chirp-rate setter */
  void set_chirp_rate(roj_real_array*);
};

#endif

