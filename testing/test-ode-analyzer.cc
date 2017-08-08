/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */
 
/* external headers */
#include <stdlib.h>
#include <stdio.h>

/* internal tfr headers */
#include "../roj.hh"

int main(void){

  print_roj_info();

  /* the first stage is empty signal construction */
  int length = 700;
  double start = 0.0;
  double rate = 1000.0;

  roj_signal_config sig_conf;
  sig_conf.length = length; 
  sig_conf.start = start; 
  sig_conf.rate = rate; 

  roj_complex_signal* signal_ptr = new roj_complex_signal(sig_conf);

  /* generator definition *************************************************** */

  /* generator configuration is used depended on which signal is generated */
  /* here LFM chirp definition */

  /* new unconfigured generator */
  roj_pulse_generator* pulse_gener_ptr = new roj_pulse_generator();

  /* configuration */
  pulse_gener_ptr->m_transition = 0.5;
  pulse_gener_ptr->m_period = 0.5;
  pulse_gener_ptr->m_delay = 0.35;
  pulse_gener_ptr->m_width = 0.3;

  /* first component */
  double crate1 = 1000.0;
  pulse_gener_ptr->m_bandwidth = crate1 * pulse_gener_ptr->m_width; /* 300.0; */ 
  pulse_gener_ptr->m_frequency = 200.0;
  pulse_gener_ptr->m_amplitude = 1.0;
  
  /* generator add a generated signal to the argument 
     the added signal type can be choosen by using 
     an apropriate generator method */
  pulse_gener_ptr->add_lfm_chirp(signal_ptr);

  /* random fsk signal */
  /* gener_ptr->add_random_fsk(signal_ptr); */

  /* generator configuration can be updated */
  /* second component */
  double crate2 = 500.0;
  pulse_gener_ptr->m_bandwidth = crate2 * pulse_gener_ptr->m_width; /* 150.0; */ 
  pulse_gener_ptr->m_frequency = 400.0;

  /* generator add a generated signal to the argument (2) */
  pulse_gener_ptr->add_lfm_chirp(signal_ptr);
  
  /* add awgn also by using the generator */
  roj_noise_generator* noise_gener_ptr = new roj_noise_generator();
  double snr = 60.0;
  
  double sig_energy = signal_ptr->calc_energy();
  double noise_energy = noise_gener_ptr->add_awgn_at_exact_snr(signal_ptr, snr);

  double true_snr = 10.0*log10(sig_energy/noise_energy); 
  call_info("true SNR is (dB): ", true_snr);

  /* array configuration is used in filter analyzer as well as in Fourier analyzer ************* */

  /* parameters for sampling of frequency axis */
  roj_array_config arr_conf;
  arr_conf.min = -10.0;
  arr_conf.max = 500.0;
  arr_conf.length = 1024;
  
  /* #define FFT_EXAMPLE /**/ /* <- comment this for FFT example */
#ifndef FFT_EXAMPLE

  /* first variant of analysis (ODE) */
  /* filter analyzer definition and filtering *************************************************** */
  
  /* causal filter definition */
  /* sampling rate should be the same as for the analyzed signal */
  roj_filter_generator *filter_gen = new roj_filter_generator(rate);

  /* time spread which corresponds to effective duration of impulse response */
  filter_gen->set_spread(0.01);

  /* ODE filter order */
  filter_gen->set_order(5);
  
  /* create TF filter analyzer */
  roj_ode_analyzer* analyzer_ptr = new roj_ode_analyzer(arr_conf, filter_gen); 

  /* after it, the filter generator can be removed */
  delete filter_gen;
  
  /* ******************************************************************************************** */

#else

  /* second variant of analysis (FFT) */
  /* window definition and Fourier transformation (STFT) */

  /* finite window definition */
  /* sampling rate should be the same as for the analyzed signal */
  roj_window_generator *win_gen = new roj_window_generator(rate);
  
  /* time length of the window in samples, the most important window propertie */
  win_gen->set_length(100);
  
  /* if crate is diffrent than 0, then chirplet transformation is performed */
  win_gen->set_chirp_rate(1000.0);
  
  /* At the moment, only 0 works which means Blackman-Harris window */
  win_gen->set_type(0);

  /* create TF Fourier analyzer */
  roj_fft_analyzer* analyzer_ptr = new roj_fft_analyzer(arr_conf, win_gen); /**/

  /* ******************************************************************************************** */

#endif

  /* signal introduction */
  analyzer_ptr->set_signal(signal_ptr, 2);

  /* signal is copied, 
     so they can be removed after used */
  delete signal_ptr;

  /* get and save spectral energy */
  roj_real_matrix* s_energy = analyzer_ptr->get_spectral_energy();
  s_energy->save("data-s-energy.txt"); 
  
  /* get and save instantaneous frequency */
  roj_real_matrix* i_freq = analyzer_ptr->get_instantaneous_frequency(0);
  /* i_freq->save("data-i-freq.txt"); */

  /* get and save spectral delay */
  roj_real_matrix* s_delay = analyzer_ptr->get_spectral_delay();
  /* s_delay->save("data-s-delay.txt"); */

  /* get and save chirp rate */
  roj_real_matrix* c_rate = analyzer_ptr->get_chirp_rate(2);
  c_rate->save("data-c-rate.txt"); 

  /* analyzer is more needed */
  delete analyzer_ptr;

  /* time-frequency reassignment ************************************* */

  /* we have two functions dedicated for energy reassignment.
     First (time-frequency) uses only spectral delay and instantaneous frequency.
     Second (frequency) uses spectral delay, instantaneous frequency, as well as chirp rate.
   */
  
  /* time frequency reassignment */
  roj_real_matrix* r_energy = roj_time_frequency_reassign(s_delay, i_freq, s_energy); /**/
  
  /* frequency reassignment - you can check it by switch on the next lines: */
  /* roj_real_matrix* r_energy = roj_frequency_reassign(i_freq, s_energy); /**/

  /* save energy */
  r_energy->save("data-r-energy.txt");

  /* reassignment as profile ******************************************* */
  /* this is an additional statistic analysis */

  /* chirp-rate profile (histogram) calculation */
  roj_array_config arr_prof_conf;
  arr_prof_conf.min = 0.0;
  arr_prof_conf.max = 1500.0;
  arr_prof_conf.length = 10000;

  roj_real_array* cr_profile_ptr = roj_calculate_profile(c_rate, s_energy, arr_prof_conf);
  cr_profile_ptr->save("profile.txt");

  /* vertical marginal distribution of chirp rate */
  roj_real_array* cr_marginal_ptr = c_rate->get_mean_over_y(s_energy);
  cr_marginal_ptr->save("marginal.txt");
  
  /* remove all imagines */
  delete s_delay, i_freq, c_rate;
  delete r_energy, s_energy;
  
  return EXIT_SUCCESS;
}
