/*
  this is a part of ccROJ project: 
  example of loading wav file as signal object 
  and of time-frequency reassignment using CCT method 
  as well as median filter
*/
 
/* external headers */
#include <stdlib.h>
#include <stdio.h>

/* internal roj headers */
#include "../roj.hh"

int main(void){

  print_roj_info();

  /* signal load from a wav file */
  char* wav_name = "mail.wav";
  roj_complex_signal* in_signal = new roj_complex_signal(wav_name);

  roj_signal_config conf_sig = in_signal->get_config();
  double rate = conf_sig.rate;

  /* array configuration is used in Fourier analyzer */
  roj_array_config arr_conf;
  arr_conf.length = 4096*2;
  arr_conf.max = 2000.0;
  arr_conf.min = 0.0;
  
  /* finite window definition */
  roj_window_generator* win_gen = new roj_window_generator(rate);
  win_gen->set_chirp_rate(0.0);
  win_gen->set_length(1400);
  win_gen->set_type(0);
 
  /* iteration 1 *************************************************** */

  /* create TF Fourier analyzer */
  roj_fft_analyzer* tf_analyzer = new roj_fft_analyzer(arr_conf, win_gen); /**/
  
  /* set signal */
  int hop = 40;
  tf_analyzer->set_signal(in_signal, hop);

  /* get and save spectral energy */
  roj_real_matrix* s_energy = tf_analyzer->get_spectral_energy();
  s_energy->save("data-s-energy-1.txt");

  /* get and save instantaneous frequency */
  roj_real_matrix* i_freq = tf_analyzer->get_instantaneous_frequency();

  /* get and save spectral delay */
  roj_real_matrix* s_delay = tf_analyzer->get_spectral_delay();

  /* time frequency reassignment */
  roj_real_matrix* r_energy = roj_time_frequency_reassign(s_delay, i_freq, s_energy); 
  r_energy->save("data-r-energy-1.txt");

  /* get and save chirp rate */
  roj_real_matrix* c_rate = tf_analyzer->get_chirp_rate(2);

  /* double median filtering */
  int h_order = 5;
  int v_order = 4;
  int hop_med = 2;
  int vop_med = 2;

  /* 1. in TF domain */
  roj_median_filter *median_filter = new roj_median_filter(h_order, v_order);
  roj_real_matrix* c_rate_med = median_filter->smart_filtering(c_rate, hop_med, vop_med);
  
  roj_real_array* cr_profile = c_rate_med->get_max_over_time(s_energy); /**/
  cr_profile->save("data-cr-profile-0.txt"); 

  /* 2. in CR domain */
  roj_real_array* cr_profile_med = median_filter->smart_filtering(cr_profile, hop_med);
  cr_profile_med->save("data-cr-profile-1.txt"); 

  
  /* iteration 2 *************************************************** */
  /* cct */

  win_gen->set_length(1600);
  roj_cct_analyzer* tf_analyzer2 = new roj_cct_analyzer(arr_conf, win_gen);

  tf_analyzer2->set_signal(in_signal, hop);
  tf_analyzer2->set_chirp_rate(cr_profile_med);

  /* get and save spectral energy */
  roj_real_matrix* s_energy2 = tf_analyzer2->get_spectral_energy();
  s_energy2->save("data-s-energy-2.txt");

  /* get and save instantaneous frequency */
  roj_real_matrix* i_freq2 = tf_analyzer2->get_instantaneous_frequency();

  /* get and save spectral delay */
  roj_real_matrix* s_delay2 = tf_analyzer2->get_spectral_delay();

  /* time frequency reassignment */
  roj_real_matrix* r_energy2 = roj_time_frequency_reassign(s_delay2, i_freq2, s_energy2); 
  r_energy2->save("data-r-energy-2.txt");

  /* get and save chirp rate */
  roj_real_matrix* c_rate2 = tf_analyzer2->get_chirp_rate(2);

  /* roj_real_array* cr_profile2 = c_rate2->get_average_in_time(s_energy2); /**/
  roj_real_array* cr_profile2 = c_rate2->get_max_over_time(s_energy2); /**/

  /* median filtering */
  roj_real_array* cr_profile2_med = median_filter->smart_filtering(cr_profile2, hop_med);
  cr_profile2_med->save("data-cr-profile-2.txt"); 


  /* iteration 3 *************************************************** */
  /* cct 2 */

  win_gen->set_length(1800);
  roj_cct_analyzer* tf_analyzer3 = new roj_cct_analyzer(arr_conf, win_gen);
  delete win_gen;
  
  tf_analyzer3->set_signal(in_signal, hop);
  tf_analyzer3->set_chirp_rate(cr_profile2_med);

  /* get and save spectral energy */
  roj_real_matrix* s_energy3 = tf_analyzer3->get_spectral_energy();
  s_energy3->save("data-s-energy-3.txt");

  /* get and save instantaneous frequency */
  roj_real_matrix* i_freq3 = tf_analyzer3->get_instantaneous_frequency();

  /* get and save spectral delay */
  roj_real_matrix* s_delay3 = tf_analyzer3->get_spectral_delay();

  /* time frequency reassignment */
  roj_real_matrix* r_energy3 = roj_time_frequency_reassign(s_delay3, i_freq3, s_energy3); 
  r_energy3->save("data-r-energy-3.txt");

  /* ending *************************************************** */

  return EXIT_SUCCESS;
}
