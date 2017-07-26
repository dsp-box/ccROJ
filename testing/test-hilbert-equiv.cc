/* this is a part of ccROJ project: 
   example of loading wav file as signal object 
   and of time-frequency reassignment. The recorging 
   is nonstationary vibrato perfomed on flute. */
 
/* external headers */
#include <stdlib.h>
#include <stdio.h>

/* internal roj headers */
#include "../roj.hh"

int main(void){

  print_roj_info();

  /* signal load from a wav file */
  char* wav_name = "flute.wav";
  roj_complex_signal* in_signal = new roj_complex_signal(wav_name);

  /* Hilbert transform testing */
  roj_hilbert_equiv* hilbert_ptr = new roj_hilbert_equiv(in_signal);
  delete in_signal;
  in_signal = hilbert_ptr->get_equivalent();
  
  /* analyze only firs second */
  in_signal->cut(0.0, 1.0);
 
  /* get rate from signal */
  roj_signal_config conf_sig = in_signal->get_config();
  double rate = conf_sig.rate;

  /* array configuration is used in Fourier analyzer ************************************* */
  roj_array_config arr_conf;
  arr_conf.length = 1024*4;
  arr_conf.min = -1500;
  arr_conf.max = 1500;
  
  /* finite window definition ************* */
  roj_window_generator* win_gen = new roj_window_generator(rate);
  win_gen->set_chirp_rate(0.0);
  win_gen->set_length(150);
  win_gen->set_type(0);
 
  /* create TF Fourier analyzer */
  roj_fft_analyzer* tf_analyzer = new roj_fft_analyzer(arr_conf, win_gen); 
  delete win_gen;
    
  /* set signal */
  int hop = 10;
  tf_analyzer->set_signal(in_signal, hop);

  /* signal is copied, 
     so it can be removed after used */
  delete in_signal;
  
  /* get and save spectral energy */
  roj_real_matrix* s_energy = tf_analyzer->get_spectral_energy();
  s_energy->save("data-s-energy.txt"); 
  
  /* get and save instantaneous frequency */
  roj_real_matrix* i_freq = tf_analyzer->get_instantaneous_frequency();
  
  /* get and save spectral delay */
  roj_real_matrix* s_delay = tf_analyzer->get_spectral_delay();

  /* analyzer is more needed */
  delete tf_analyzer;

  /* zoom */
  roj_image_config img_conf = s_energy->get_config();
  img_conf.y.length = 400;
  img_conf.y.min = 1000;
  img_conf.y.max = 1100;
  img_conf.x.length = 900;
  
  roj_real_matrix* r_energy = new roj_real_matrix(img_conf);
  roj_time_frequency_reassign(s_delay, i_freq, s_energy, r_energy);
  r_energy->save("data-r-energy.txt");

  /* remove all imagines */
  delete s_energy, r_energy;
  delete i_freq, s_delay;

  return EXIT_SUCCESS;
}
