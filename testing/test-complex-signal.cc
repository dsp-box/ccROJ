/* *************************************************** *
 * This file is a part of ccROJ project (version 0-44) *
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
  
  /* signal load from a wav file */
  roj_complex_signal* signal_from_wav = new roj_complex_signal("mail.wav");
  double energy1 = signal_from_wav->calc_energy();
  call_info("energy:", energy1);

  /* cut, reverse, and save */
  signal_from_wav->reverse();
  signal_from_wav->modulate (400);
  signal_from_wav->remove_imag();
  signal_from_wav->reverse();
  signal_from_wav->remove_const();
  signal_from_wav->cut(0.0, 0.7);
  signal_from_wav->append_zero_tail(1.5);
  signal_from_wav->append_zero_head(0.5);
  signal_from_wav->save_wav("mail2.wav");
  double energy2 = signal_from_wav->calc_energy();
  call_info("energy:", energy2);
  delete signal_from_wav;

  /* generate signal: LFM chirp */
  roj_signal_config sig_conf;
  sig_conf.start = 1.0;
  sig_conf.rate = 1000.0;
  sig_conf.length = 1000;
  double chirp_rate = 300.0;
  roj_complex_signal* chirp_signal = new roj_complex_signal(sig_conf);
  for(int n=0; n<sig_conf.length; n++){
    double time = (double)n / sig_conf.rate;
    double arg = M_PI*chirp_rate*pow(time, 2.0);
    chirp_signal->m_waveform[n] = cexp(1I * arg);
  }

  /* calc and save IF */
  roj_complex_signal* instantaneous_frequency = chirp_signal->get_instantaneous_complex_frequency(); 
  instantaneous_frequency->save("ifreq.txt");
  /* see in gnuplot: 
     plot 'ifreq.txt' u 1:3 wit lines */

  /* ending */
  delete chirp_signal;
  delete instantaneous_frequency;  
  return EXIT_SUCCESS;
}
