/* *************************************************** *
 * This file is a part of ccROJ project (version 0-48) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_iir_analyzer_
#define _roj_iir_analyzer_

/**
 * @type: class
 * @brief: Definition of roj_ode_analyzer class. The class works only for ODE filters at the moment.
 * @herit: roj_ode_analyzer : roj_analyzer
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-analyzer.hh"
class tf_analyzer;

#include "roj-filter-bank.hh"
class roj_filter_bank;

/* ************************************************************************************************************************* */
/* filter analyzer class definition */

class roj_ode_analyzer
  : public roj_analyzer{
private:

  /* filter generator */
  roj_filter_generator* m_filter_gen;

  /* filters and filtered signals slots */
  roj_complex_signal*** m_filtered_signals;
  roj_filter_bank** m_bank_array;

  /* width depends on signal length 
     height on filter number */
  unsigned int get_height();
  unsigned int get_width();

public:

  /* construction */
  roj_ode_analyzer(roj_array_config, roj_filter_generator*);
  ~roj_ode_analyzer();  

  void set_signal(roj_complex_signal*, int =1);
  double get_frequency(unsigned int =0);
  
  roj_image_config get_image_config ();
  roj_real_matrix* create_empty_image();
  roj_stft_transform* get_transform_as_stft ();

  roj_real_matrix* get_spectral_delay();
  roj_real_matrix* get_spectral_energy();

  roj_real_matrix* get_instantaneous_frequency_by_1_estimator();
  roj_real_matrix* get_instantaneous_frequency_by_2_estimator();

  roj_real_matrix* get_chirp_rate_by_k_estimator();
  roj_real_matrix* get_chirp_rate_by_m_estimator();
  
  roj_real_matrix* get_chirp_rate_by_d_estimator();
  roj_real_matrix* get_chirp_rate_by_f_estimator();
};

#endif

