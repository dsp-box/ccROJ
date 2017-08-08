/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_xxt_analyzer_
#define _roj_xxt_analyzer_

/**
* @type: class
* @brief: Definition of roj_xxt_analyzer abstract class
* @herit: roj_xxt_analyzer : roj_analyzer
*/

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-analyzer.hh"
class tf_analyzer;

#include "roj-window-gener.hh"
class roj_window_generator;

#include "roj-stft-transform.hh"
class roj_stft_transform;

/* macros */

#define CODE_WIN_D2 0
#define CODE_WIN_D 1
#define CODE_WIN_ZERO 2
#define CODE_WIN_T 3
#define CODE_WIN_T2 4
#define CODE_WIN_DT 5

/* ************************************************************************************************************************* */
/* fft analyzer class definition */

class roj_xxt_analyzer
  : public roj_analyzer{

private:

  /* calc stft */
  virtual complex double ** transforming(int, int) =0;

protected:
  
  /* window generator */
  roj_window_generator* m_window_gen;
  
  /* STFT for various windows */
  complex double *** m_fourier_spectra;

  /* allocate memory for stft */
  complex double ** allocate_stft();
  
  /* width depends on signal length, then height 
     on fft length, as well as on frequency bandwidth */
  unsigned int get_initial();
  unsigned int get_height();
  unsigned int get_width();
  unsigned int m_height;
  unsigned int m_width;
  
public:

  /* construction */
  roj_xxt_analyzer(roj_array_config, roj_window_generator*);
  ~roj_xxt_analyzer(); 
  
  void set_signal(roj_complex_signal*, int =1);

  /* methods for produce distributions */  

  roj_image_config get_image_config ();
  roj_real_matrix* create_empty_image ();
  roj_stft_transform* get_stft_transform ();

  roj_real_matrix* get_spectral_energy ();
  roj_real_matrix* get_spectral_delay ();
  roj_real_matrix* get_dof_density ();
  
  roj_real_matrix* get_instantaneous_frequency_by_1_estimator ();
  roj_real_matrix* get_instantaneous_frequency_by_2_estimator ();
  
  roj_real_matrix* get_chirp_rate_by_k_estimator ();
  roj_real_matrix* get_chirp_rate_by_d_estimator ();
  roj_real_matrix* get_chirp_rate_by_f_estimator ();
};

#endif

