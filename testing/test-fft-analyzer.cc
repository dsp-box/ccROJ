/* *************************************************** *
 * This file is a part of ccROJ project (version 0-49) *
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

  print_roj_info ();

  /* signal load from a wav file */
  char* wav_name = "mail.wav";
  roj_complex_signal* in_signal = new roj_complex_signal(wav_name);
  double rate = in_signal->get_config().rate;
  
  /* array configuration is used in Fourier analyzer */
  roj_array_config arr_conf;
  arr_conf.min = -in_signal->get_config().rate / 10;
  arr_conf.max = in_signal->get_config().rate / 10;
  arr_conf.length = 4096;
  
  /* finite window definition */
  roj_window_generator* win_gen = new roj_window_generator(rate);
  win_gen->set_chirp_rate(0.0);
  win_gen->set_length(750);
  win_gen->set_type(0);
  
  /* create TF Fourier analyzer */
  roj_fft_analyzer* tf_analyzer = new roj_fft_analyzer(arr_conf, win_gen); /**/
  delete win_gen;
    
  /* set analyzed signal */
  tf_analyzer->set_signal(in_signal, 5);
  delete in_signal;

  /* get and save spectral energy */
  roj_real_matrix* s_energy = tf_analyzer->get_spectral_energy();
  s_energy->save("data-s-energy.txt");

  /* get and save instantaneous frequency */
  roj_real_matrix* i_freq = tf_analyzer->get_instantaneous_frequency();

  /* get and save spectral delay */
  roj_real_matrix* s_delay = tf_analyzer->get_spectral_delay();

  /* time frequency reassignment */
  roj_real_matrix* r_energy = roj_time_frequency_reassign(s_delay, i_freq, s_energy); 
  r_energy->save("data-r-energy.txt");

  /* get and save chirp rate */
  roj_real_matrix* c_rate = tf_analyzer->get_chirp_rate(2);
  c_rate->save("data-c-rate.txt"); 

  roj_stft_transform* c_stft = tf_analyzer->get_stft_transform();
  roj_complex_signal* rec_signal = c_stft->get_signal();
  rec_signal->save_wav("out.wav");
    
  /* cleanning */
  delete s_energy;
  delete r_energy;
  delete s_delay;
  delete i_freq;
  delete c_rate;

  delete rec_signal;
  delete c_stft;
  delete tf_analyzer;
  
  return EXIT_SUCCESS;
}
