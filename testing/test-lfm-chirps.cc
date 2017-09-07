/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
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

  /* signal generation */
  roj_signal_config sig_conf;
  sig_conf.start = -0.1;
  sig_conf.rate = 2000.0;
  sig_conf.length = 2400;

  roj_complex_signal in_sig = roj_complex_signal(sig_conf);

  roj_pulse_generator pulse_gen; 
  pulse_gen.m_period = 666.0;
  pulse_gen.m_transition=0.0;
  pulse_gen.m_width = 1.0;
  pulse_gen.m_delay = 0.5;

  pulse_gen.m_frequency = 500;
  pulse_gen.m_bandwidth = 1000;
  pulse_gen.add_lfm_chirp (&in_sig);

  in_sig.save_wav("lfm.wav");

  pulse_gen.m_frequency = 0;
  pulse_gen.m_bandwidth = 0;
  pulse_gen.add_lfm_chirp (&in_sig);

  pulse_gen.m_frequency = -500;
  pulse_gen.m_bandwidth = -1000;
  pulse_gen.add_lfm_chirp (&in_sig);

   
  /* array configuration is used in Fourier analyzer */
  roj_array_config arr_conf;
  arr_conf.length = 4*1024;

  arr_conf.max = 1000;
  arr_conf.min = -1000;
   
  /* finite window definition */
  roj_window_generator win_gen = roj_window_generator(sig_conf.rate);
  win_gen.set_chirp_rate(0);
  win_gen.set_length(100);
  win_gen.set_type(0);
   
  /* create TF Fourier analyzer */
  roj_fft_analyzer tf_analyzer = roj_fft_analyzer(arr_conf, &win_gen);
     
  /* set analyzed signal */
  tf_analyzer.set_signal(&in_sig, 1);

  /* get and save spectral energy */
  roj_real_matrix* s_energy = tf_analyzer.get_spectral_energy();
  s_energy->save("data-s-energy.txt");
  
  /* get and save chirp rate */
  roj_real_matrix* c_rate = tf_analyzer.get_chirp_rate(2);
  /*
    roj_real_matrix* c_rate = tf_analyzer.get_chirp_rate_by_k_estimator();
    roj_real_matrix* c_rate = tf_analyzer.get_chirp_rate_by_m_estimator();
  */
  c_rate->save("data-c-rate.txt"); 
     
  /* cleanning */
  delete s_energy;
  delete c_rate;

  return EXIT_SUCCESS;
}
