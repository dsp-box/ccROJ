/* *************************************************** *
 * This file is a part of ccROJ project (version 0-45) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-analyzer.hh"

/* ************************************************************************************************************************* */
/**
 * @type: destructor
 * @brief: This is an anayzer deconstructor.
 */
roj_analyzer :: ~roj_analyzer(){
  
  if (m_input_signal!=NULL)
    delete m_input_signal;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is functiona allows to set a signal for analysis.
 *
 * @param [in] a_sample: A pointer to a signal for filtering.
 * @param [in] a_hop: A hopsize.
 */
void roj_analyzer :: set_signal (roj_complex_signal* a_sig, int a_hop){

  /* this constructor is used in other classes */
  
  /* check args */
  if(a_hop<1)
    call_warning("hop is smaller than 1");  
  m_hop = a_hop;

  if(a_sig == NULL)
    call_error("signal is null!");
  
  /* check input signal is assigned */
  if (m_input_signal != NULL)
    call_error("you can set only one signal!");

  roj_signal_config conf = a_sig->get_config();
  m_input_signal = new roj_complex_signal(conf);
  m_input_signal->copy(a_sig);
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is function allows to choose estimator of instantaneous frequency.
 *
 * @param [in] a_code: an estimator code.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_analyzer :: get_instantaneous_frequency (int a_code){
  
  switch(a_code){
  case IF_1_ESTIMATOR: return get_instantaneous_frequency_by_1_estimator();
  case IF_2_ESTIMATOR: return get_instantaneous_frequency_by_2_estimator();
  default:
    call_error("not known code");
  }
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This is functiona allows to choose estimator of chirp-rate.
 *
 * @param [in] a_code: an estimator code.
 *
 * @return: A pointer to obtained distribution.
 */
roj_real_matrix* roj_analyzer :: get_chirp_rate (int a_code){

  switch(a_code){
  case CR_K_ESTIMATOR: return get_chirp_rate_by_k_estimator();
  case CR_D_ESTIMATOR: return get_chirp_rate_by_d_estimator();
  case CR_F_ESTIMATOR: return get_chirp_rate_by_f_estimator();
  default:
    call_error("not known code");
  }
}


