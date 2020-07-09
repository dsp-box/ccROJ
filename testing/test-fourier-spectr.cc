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

  print_roj_info();

  /* empty signal construction */
  roj_signal_config sig_conf;
  sig_conf.rate = 44100.0; 
  sig_conf.length = 3e5; 
  roj_complex_signal* signal_ptr = new roj_complex_signal(sig_conf);

  /* noise generation */
  roj_noise_generator* gener_ptr = new roj_noise_generator();
  double noise_energy = gener_ptr->add_pink_using_range(signal_ptr, 1.0);  
  call_info("added energy:", noise_energy/sig_conf.length);

  /* save to file */
  signal_ptr->save_wav("pink.wav");

  /* spectrum calculation and saving */
  roj_fourier_spectrum* spectrum_ptr = signal_ptr->get_spectrum();
  spectrum_ptr->save("pink.txt");
  
  /* cleanning */
  delete signal_ptr; 
  delete spectrum_ptr;
  return EXIT_SUCCESS;
}
