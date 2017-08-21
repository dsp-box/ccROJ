/* *************************************************** *
 * This file is a part of ccROJ project (version 0-45) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#ifndef _roj_analyzer_
#define _roj_analyzer_

/**
 * @type: class
 * @brief: Definition of roj_analyzer abstract class.
 */

/* ************************************************************************************************************************* */
/* headers and declarations */

#include "roj-external.hh"
#include "roj-misc.hh"

#include "roj-real-array.hh"
struct roj_array_config;

#include "roj-complex-signal.hh"
class roj_complex_signal;

#include "roj-filter-bank.hh"
class roj_filter_bank;

#include "roj-real-matrix.hh"
class roj_real_matrix;

#include "roj-stft-transform.hh"
struct roj_stft_transform;

/* ************************************************************************************************************************* */
/* macros */

/**
* @type: define
* @brief: This const defines the code of chirp-rate K estimator.
*/
#define CR_K_ESTIMATOR 0

/**
* @type: define
* @brief: This const defines the code of chirp-rate D estimator.
*/
#define CR_D_ESTIMATOR 1

/**
* @type: define
* @brief: This const defines the code of chirp-rate F estimator.
*/
#define CR_F_ESTIMATOR 2

/**
* @type: define
* @brief: This const defines the code of chirp-rate K1 estimator.
*/
#define CR_K1_ESTIMATOR 3

/**
* @type: define
* @brief: This const defines the code of first instantaneous frequency estimator.
*/
#define IF_1_ESTIMATOR 0

/**
* @type: define
* @brief: This const defines the code of second instantaneous frequency estimator.
*/
#define IF_2_ESTIMATOR 1

/* ************************************************************************************************************************* */
/* analyzer class definition */

class roj_analyzer{
protected:
 
  /* configuration */
  roj_array_config m_bank_config;

  /* analyzed signal */
  roj_complex_signal *m_input_signal;
  int m_hop;

  virtual unsigned int get_width() =0;
  virtual unsigned int get_height() =0;
  
public:
 
  ~roj_analyzer();

  void set_signal(roj_complex_signal*, int =1);

  virtual roj_image_config get_image_config () = 0;
  virtual roj_real_matrix* create_empty_image() = 0;
  
  virtual roj_real_matrix* get_spectral_energy() = 0;
  virtual roj_real_matrix* get_spectral_delay() = 0;

  roj_real_matrix* get_instantaneous_frequency(int=0);

  virtual roj_real_matrix* get_instantaneous_frequency_by_1_estimator() = 0;
  virtual roj_real_matrix* get_instantaneous_frequency_by_2_estimator() = 0;
  
  roj_real_matrix* get_chirp_rate(int=0);
 
  virtual roj_real_matrix* get_chirp_rate_by_k_estimator() = 0;
  virtual roj_real_matrix* get_chirp_rate_by_k1_estimator() = 0;
  virtual roj_real_matrix* get_chirp_rate_by_d_estimator() = 0;
  virtual roj_real_matrix* get_chirp_rate_by_f_estimator() = 0;
};

#endif

