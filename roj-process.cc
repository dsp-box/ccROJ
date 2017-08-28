/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
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

  if(a_sig_1==NULL or a_sig_2==NULL){
    call_warning("in roj_convolve_signals");
    call_error("arg is NULL");
  }

  roj_signal_config config_1 = a_sig_1->get_config();
  roj_signal_config config_2 = a_sig_2->get_config();

  if(config_1.rate != config_2.rate){
    call_warning("in roj_convolve_signals");
    call_error("rates are different");
  }

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

  if(a_sig_1==NULL or a_sig_2==NULL){
    call_warning("in roj_correlate_signals");
    call_error("arg is NULL");
  }

  roj_complex_signal* tmp_signal = new roj_complex_signal(a_sig_2);
  tmp_signal->conjugate();
  tmp_signal->reverse();

  roj_complex_signal* out_signal = roj_convolve_signals(a_sig_1, tmp_signal);
    
  delete tmp_signal;
  return out_signal;
}

/* ************************************************************************************************************************* */
/**
 * @type: function
 * @brief: This routine removes running constant value (local DC offset).
 *
 * @param [in] a_signal: a pointer to input signal.
 * @param [in] a_order_radius: radius, order is equal to 1 + 2 radius.
 *
 * @return: a pointer to output signal
 */
roj_complex_signal* roj_remove_const (roj_complex_signal* a_signal, int a_order_radius){

  if(a_signal==NULL){
    call_warning("in roj_remove_const");
    call_error("arg is NULL");
  }
  
  if(a_order_radius<1){
    call_warning("in roj_remove_const");
    call_error("order is <= 1");
  }

  roj_signal_config conf = a_signal->get_config();
  roj_complex_signal* out_signal = new roj_complex_signal(conf);

  for(int n=0; n<conf.length; n++){
    int count = 0;

    for(int r=-a_order_radius; r<=a_order_radius; r++){
      if (n+r>=conf.length) continue;
      if (n+r<0) continue; 

      count++;
      out_signal->m_waveform[n] += a_signal->m_waveform[n+r];
   }

    out_signal->m_waveform[n] /= count;
    out_signal->m_waveform[n] = a_signal->m_waveform[n] - out_signal->m_waveform[n];
    print_progress(n, conf.length, "dcrm");
  }
  print_progress(0, 0, "dcrm");
  
  return out_signal;
}

/* ************************************************************************************************************************* */
/**
 * @type: function
 * @brief: This routine calculates short-time cross-correlation. [HAS TO BE TESTED]
 *
 * @param [in] a_arr_1: a pointer to input signal.
 * @param [in] a_arr_2: a pointer to input signal.
 * @param [in] a_width: window width.
 * @param [in] a_hop: hopsize.
 *
 * @return: a pointer to output signal
 */
roj_real_array* roj_short_time_correlation (roj_real_array* a_arr_1, roj_real_array* a_arr_2, int a_width, int a_hop){
  
  if(a_arr_1==NULL or a_arr_2==NULL){
    call_warning("in roj_short_time_correlation");
    call_error("arg is NULL");
  }
  
  if(a_width<2){
    call_warning("in roj_short_time_correlation");
    call_error("width < 2");
  }

  if(a_hop<1){
    call_warning("in roj_short_time_correlation");
    call_error("hop < 1");
  }
  
  roj_array_config in_conf = a_arr_1->get_config();
  if (!a_arr_2->compare_config (in_conf)){
    call_warning("in roj_short_time_correlation");
    call_error("configs are diff");
  }

  double delta = a_arr_1->get_delta ();
  int new_length = 1 + (in_conf.length - a_width) / a_hop;

  roj_array_config out_conf;
  out_conf.length = new_length;
  out_conf.min = in_conf.min + 0.5*delta*(a_width-1);
  out_conf.max = out_conf.min + new_length * delta;

  for(int n=0; n<new_length; n++) {

    
    /* TO DO: Pearson correlation */
  }
}
