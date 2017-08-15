/* *************************************************** *
 * This file is a part of ccROJ project (version 0-45) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

/* external headers */
#include <stdlib.h>
#include <stdio.h>

/* internal roj headers */
#include "../roj.hh"

int main(void){

  print_roj_info();

  /* generate signal */
  roj_signal_config sig_conf;
  sig_conf.start = 1.0;
  sig_conf.rate = 1000.0;
  sig_conf.length = 1000;
  double frequency = 20.0;
  roj_complex_signal* in_signal = new roj_complex_signal(sig_conf);
  for(int n=0; n<sig_conf.length; n++){
    double time = (double)n / sig_conf.rate;
    double arg = TWO_PI * frequency * time;
    in_signal->m_waveform[n] = cexp(1I * arg);
  }

  in_signal->remove_imag();
  in_signal->append_cos_head(0.1);
  in_signal->append_cos_tail(0.3);

  /* array configuration is used in Fourier analyzer */
  roj_array_config arr_conf;
  arr_conf.length = 4096*8;
  arr_conf.min = -399.0;
  arr_conf.max = 400.0;
  
  /* finite window definition */
  roj_window_generator* win_gen = new roj_window_generator(sig_conf.rate);
  win_gen->set_chirp_rate(0.0);
  win_gen->set_length(100);
  win_gen->set_type(0);
  
  /* create TF Fourier analyzer */
  roj_fft_analyzer* tf_analyzer = new roj_fft_analyzer(arr_conf, win_gen); /**/
    
  /* set analyzed signal */
  int hop = 1;
  tf_analyzer->set_signal(in_signal, hop);
  in_signal->save("in.txt");

  /* get stft and signal recovery */
  roj_stft_transform* s_stft = tf_analyzer->get_stft_transform();
  roj_real_matrix* s_energy = s_stft->get_spectral_energy ();
  /* s_energy->save("energy.txt"); */
  
  /* signal reconstruction */
  roj_complex_signal* out_signal = s_stft->get_signal();
  out_signal->save("out.txt");
  
  return EXIT_SUCCESS;
}
