/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-process.hh"

/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function returns the convolution of two signals.
*
* @param [in] a_sig_1: first signal.
* @param [in] a_sig_2: second signal.
*
* @return: a pointer to new signal.
*/
roj_complex_signal* roj_convolve_signals (roj_complex_signal* a_sig_1, roj_complex_signal* a_sig_2){

  if(a_sig_1==NULL or a_sig_2==NULL)
    call_error("arg is NULL");
  
  roj_signal_config config_1 = a_sig_1->get_config();
  roj_signal_config config_2 = a_sig_2->get_config();

  if(config_1.rate != config_2.rate)
    call_error("rates are different");

  roj_signal_config config_out;
  config_out.rate = config_1.rate;
  config_out.start = config_1.start;
  config_out.length = config_1.length + config_2.length - 1;
  roj_complex_signal* out_signal = new roj_complex_signal(config_out);
  
  for(int n=0; n<config_out.length; n++){

    complex double tmp = 0.0 + 1I * 0.0;      
    for(int k=0; k<config_1.length; k++){	
      if(n-k<0 or n-k>=config_2.length)
	continue;
      tmp += a_sig_1->m_waveform[k] * a_sig_2->m_waveform[n-k];    
    }
	 
    out_signal->m_waveform[n] = tmp;
    if(!(n%(1+config_out.length/20)))
      print_progress(n, config_out.length, "conv");
  }

  print_progress(0, 0, "conv");
  return out_signal;
}


/* ************************************************************************************************************************* */
/**
* @type: function
* @brief: This function returns the (cross) correlation of two signals.
*
* @param [in] a_sig_1: first signal.
* @param [in] a_sig_2: second signal.
*
* @return: a pointer to new signal.
*/
roj_complex_signal* roj_correlate_signals (roj_complex_signal* a_sig_1, roj_complex_signal* a_sig_2){

  if(a_sig_1==NULL or a_sig_2==NULL)
    call_error("arg is NULL");


  roj_complex_signal* tmp_signal = new roj_complex_signal(a_sig_2);
  tmp_signal->conjugate();
  tmp_signal->reverse();

  roj_complex_signal* out_signal = roj_convolve_signals(a_sig_1, tmp_signal);
    
  delete tmp_signal;
  return out_signal;
}
