/* *************************************************** *
 * This file is a part of ccROJ project (version 0-49) *
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
 * @brief: This routine calculates short-time cross-correlation (Pearson). [HAS TO BE TESTED]
 *
 * @param [in] a_arr_1: a pointer to input array.
 * @param [in] a_arr_2: a pointer to input array.
 * @param [in] a_width: window width.
 * @param [in] a_hop: hopsize.
 *
 * @return: a pointer to output array
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
  double delta = a_arr_1->get_delta ();

  roj_array_config in2_conf = a_arr_2->get_config();
  double in2_delta = a_arr_2->get_delta ();

  if (in_conf.min!=in2_conf.min){
    call_warning("in roj_short_time_correlation");
    call_error("mins are diff");
  }

  if (delta!=in2_delta){
    call_warning("in roj_short_time_correlation");
    call_error("deltas are diff");
  }
  
  int min_length = in_conf.length < in2_conf.length ? in_conf.length : in2_conf.length;
  int new_length = 1 + (min_length - a_width) / a_hop;

  if (new_length<2){
    call_warning("in roj_short_time_correlation");
    call_error("out len too short");
  }
  
  roj_array_config out_conf;
  out_conf.length = new_length;
  out_conf.min = in_conf.min + 0.5*delta*(a_width-1);
  out_conf.max = out_conf.min + new_length * delta;

  roj_real_array* out_array = new roj_real_array(out_conf);
  for(int n=0; n<new_length; n++) {

    double mean_1 = 0, mean_2;
    for(int w=0; w<a_width; w++) {      
      mean_1 += a_arr_1->m_data[a_hop*n+w];
      mean_2 += a_arr_2->m_data[a_hop*n+w];      
    }
    mean_1 /= a_width;
    mean_2 /= a_width;

    double cross_var = 0;
    double var_1 = 0, var_2 = 0;
    for(int w=0; w<a_width; w++) {      
      
      var_1 += pow(a_arr_1->m_data[a_hop*n+w] - mean_1, 2.0);
      var_2 += pow(a_arr_2->m_data[a_hop*n+w] - mean_2, 2.0);
      cross_var += (a_arr_1->m_data[a_hop*n+w] - mean_1) * (a_arr_2->m_data[a_hop*n+w] - mean_2);
    }
    
    out_array->m_data[n] = cross_var / (sqrt(var_1) * sqrt(var_2));
  }

  return out_array;
}
