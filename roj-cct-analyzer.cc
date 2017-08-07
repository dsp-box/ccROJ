/* *************************************************** *
 * This file is a part of ccROJ project (version 0-43) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                                Krzysztof Czarnecki  *
 *                       czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-cct-analyzer.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is a constructor of roj_cct_analyzer.
 *
 * @param [in] a_bank_conf: A bank configuration (frequency axis).
 * @param [in] a_window_gen: A pointer to a window generator.
 */
roj_cct_analyzer :: roj_cct_analyzer (roj_array_config a_bank_conf, roj_window_generator* a_window_gen)

  /* run parent fft constructor */
  : roj_xxt_analyzer(a_bank_conf, a_window_gen){
  
  /* set null */
  m_chirprate = NULL;
}

/**
 * @type: destructor
 * @brief: This is an anayzer deconstructor. This function release also memory.
 */
roj_cct_analyzer :: ~roj_cct_analyzer (){

  if(m_chirprate != NULL)
    delete m_chirprate;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine allows to set an instantaneous chirp-rate.
 *
 * @param [in] a_crate: A pointer a distribution which stored the instantaneous chirp-rate.
 */
void roj_cct_analyzer :: set_chirp_rate (roj_real_array* a_crate){
  
  if(a_crate == NULL)
    call_error("arg is null");

  if(m_chirprate != NULL)
    call_error("only one chirprate can be set");

  roj_array_config conf = a_crate->get_config();
  m_chirprate = new roj_real_array(conf);
  int byte_size = conf.length * sizeof(double);
  memcpy(m_chirprate->m_data, a_crate->m_data, byte_size);
}

/* ************************************************************************************************************************* */
/**
 * @type: private
 * @brief: This routine roj_calculate short-time Fourier transform (STFT).
 *
 * @param [in] a_d_order: An order of of window derivative with respect to time.
 * @param [in] a_t_order: An order of power of timeramp which is multiplied by the window waveform.
 *
 * @return: A pointer to the resultant STFT.
 */
complex double ** roj_cct_analyzer :: transforming (int a_d_order, int a_t_order){

  /* check to chirprate is set */
  if(m_chirprate==NULL)
    call_error("chirp-rate is not set");
  
  /* allocate memory for stft */
  complex double** stft = allocate_stft();

  /* tmp signal for fft - only length is used */
  roj_signal_config tmp_conf;
  tmp_conf.length = m_bank_config.length;    
  roj_complex_signal* in_tmp = new roj_complex_signal(tmp_conf);
  int start_index = (m_bank_config.length-m_window_gen->get_length()) / 2;

  int byte_size_win = m_window_gen->get_length() * sizeof(complex double);
  int byte_size_out = get_height() * sizeof(complex double);

  double delta = m_window_gen->get_rate() / m_bank_config.length;
  int initial = (m_bank_config.min + m_window_gen->get_rate()/2) / delta;

  roj_array_config arr_conf = m_chirprate->get_config();
  roj_signal_config sig_conf = m_input_signal->get_config();
  
  /* calculating spectrua by fft */
  bool nline_flag1 = true;
  bool nline_flag2 = true; 
  for(int n=0; n<get_width();n++){
    int curr_index = n*m_hop;
    in_tmp->clear();

    /* get nearest index */    
    double time = sig_conf.start + ((double)curr_index + (double)m_window_gen->get_length()/2) / m_window_gen->get_rate();
    int cr_index = m_chirprate->get_index_by_arg(time);
    if (cr_index>=arr_conf.length){
      cr_index = arr_conf.length-1;
      if (nline_flag2 and cr_index>=arr_conf.length+1){
	fprintf(stderr,"\n");
	nline_flag2 = false;
	call_warning("extrapolation in chirprate");
      }
    }
    if (cr_index<0){
      cr_index =0;
      if(nline_flag1 and cr_index<-1){
	fprintf(stderr,"\n");
	nline_flag1 = false;
	call_warning("extrapolation in chirprate");
      }
    }
        
    double c_rate = m_chirprate->m_data[cr_index];
    m_window_gen->set_chirp_rate(c_rate);

    roj_complex_signal* window_waveform = m_window_gen->get_window(a_d_order, a_t_order); 
    memcpy(&in_tmp->m_waveform[start_index], &m_input_signal->m_waveform[curr_index], byte_size_win);

    for(int m=0; m<m_window_gen->get_length(); m++)
      in_tmp->m_waveform[start_index+m] *= window_waveform->m_waveform[m];

    roj_fourier_spectrum* out_tmp = in_tmp->get_spectrum();
    memcpy(stft[n], &out_tmp->m_spectrum[initial], byte_size_out);
    delete out_tmp;

    print_progress(n+1, get_width(), "stft");
    delete window_waveform;
  }

  print_progress(0, 0, "stft");  
  delete in_tmp;
  return stft;
}
