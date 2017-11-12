/* *************************************************** *
 * This file is a part of ccROJ project (version 0-47) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-fft-analyzer.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of roj_fft_analyzer.
*
* @param [in] a_bank_conf: A bank configuration (frequency axis).
* @param [in] a_window_gen: A pointer to a window generator.
*/
roj_fft_analyzer :: roj_fft_analyzer (roj_array_config a_bank_conf, roj_window_generator* a_window_gen)
 : roj_xxt_analyzer(a_bank_conf, a_window_gen){
}

/* ************************************************************************************************************************* */
/**
* @type: private
* @brief: This routine roj_calculate short-time Fourier transform (STFT).
*
* @param [in] a_d_order: An order of window derivative with respect to time.
* @param [in] a_t_order: An order of power of timeramp which is multiplied by the window waveform.
*
* @return: A pointer to the resultant STFT.
*/
complex double ** roj_fft_analyzer :: transforming (int a_d_order, int a_t_order){

  /* allocate memory for stft */
  complex double** stft =  allocate_stft();
  
  /* tmp signal for fft - only length is used */
  roj_signal_config sig_conf;
  sig_conf.length = m_bank_config.length;
  sig_conf.rate = 1; /* not used here, however checked */
    
  roj_complex_signal* in_tmp = new roj_complex_signal(sig_conf);
  int start_index = (m_bank_config.length-m_window_gen->get_length()) / 2;

  int byte_size_win = m_window_gen->get_length() * sizeof(complex double);
  int byte_size_out = get_height() * sizeof(complex double);

  /* get window waveform */
  roj_complex_signal* window_waveform = m_window_gen->get_window(a_d_order, a_t_order);
  int initial = get_initial ();
  
  /* calculating spectra by fft */
  for(int n=0; n<get_width();n++){
    in_tmp->clear();
    
    int curr_index = n*m_hop;
    memcpy(&in_tmp->m_waveform[start_index], &m_input_signal->m_waveform[curr_index], byte_size_win);
    for(int m=0; m<m_window_gen->get_length(); m++)
      in_tmp->m_waveform[start_index+m] *= window_waveform->m_waveform[m];

    roj_fourier_spectrum* out_tmp = in_tmp->get_spectrum();
    memcpy(stft[n], &out_tmp->m_spectrum[initial], byte_size_out);
    delete out_tmp;

    print_progress(n+1, get_width(), "stft");
  }

  print_progress(0, 0, "stft");  
  delete window_waveform;
  delete in_tmp;
  return stft;
}
