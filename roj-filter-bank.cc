/* *************************************************** *
 * This file is a part of ccROJ project (version 0-48) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-filter-bank.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is a constructor of roj_filter_bank.
 *
 * @param [in] a_bank_conf: A bank configuration as an array configuration.
 * @param [in] a_filter_gen: A pointer to a filter generator.
 */
roj_filter_bank :: roj_filter_bank (roj_array_config a_bank_conf, roj_filter_generator* a_filter_gen){

  /* check args */
  if(a_filter_gen==NULL){
    call_warning("in roj_filter_bank :: roj_filter_bank");
    call_error("arg is null");
  }

  /* check and set frequencies */
  if(a_bank_conf.min>=a_bank_conf.max and a_bank_conf.length>1){
    call_warning("in roj_filter_bank :: roj_filter_bank");
    call_error("init frequency is grater than final");
  }

  /* m_filter_array contains m_filter_number filters */
  if(a_bank_conf.length<1){
    call_warning("in roj_filter_bank :: roj_filter_bank");
    call_error("filter number is negative or one");
  }

  /* copy config data */
  m_config = a_bank_conf;

  /* copy filter generator and allocate memory */
  m_filter_gen = new roj_filter_generator(*a_filter_gen);   
  m_filter_array = new roj_filter* [m_config.length];

  double delta = (m_config.max - m_config.min) / (m_config.length-1);
  if (m_config.length == 1)
    delta = 0.0;
    
  for(int n=0; n<m_config.length; n++){

    double frequency = m_config.min + n * delta;
    m_filter_gen->set_frequency(frequency);
    m_filter_array[n] = m_filter_gen->get_filter();
  }
}

/**
 * @type: destructor
 * @brief: This is a bank deconstructor. This function release also memory.
 */
roj_filter_bank :: ~roj_filter_bank (){

  delete m_filter_gen;
  
  for(int n=0; n<m_config.length; n++)
    delete m_filter_array[n];
  delete [] m_filter_array;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns bank parameters as a structure.
 *
 * @return: The structure which contains the configuration.
 */
roj_array_config roj_filter_bank :: get_config (){
  
  return m_config;
}

/**
 * @type: method
 * @brief: This routine returns a handle to a filter as a pointer.
 *
 * @param [in] a_index (default 0): number of filter in the bank.
 *
 * @return: pointer to returned filter. The filter is not copied.
 */
roj_filter* roj_filter_bank :: get_filter (int a_index){

  if(a_index<0 or a_index>=m_config.length){
    call_warning("in roj_filter_bank :: get_filter");
    call_error("wrong index");
  }
  
  roj_filter* filter = m_filter_array[a_index];
  return filter;
}

/**
 * @type: method
 * @brief: This routine returns a currier frequency of a filter which has a given index.
 *
 * @param [in] a_index (default 0): number of filter in the bank.
 *
 * @return: Currier frequency.
 */
double roj_filter_bank :: get_frequency(int a_index){

  double delta = (m_config.max - m_config.min) / (m_config.length-1);
  if (m_config.length == 1)
    delta = 0.0;

  double frequency = m_config.min + a_index * delta;
  return frequency;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine performs filtering operation sample-by-sample.
 *
 * @param [in] a_sample: An input sample.
 */
void roj_filter_bank :: filtering (complex double a_sample){

  for(int n=0; n<m_config.length; n++)
    m_filter_array[n]->process(a_sample);    
}

/**
 * @type: method
 * @brief: This routine performs filtering operation (the whole signal).
 *
 * @param [in] a_sample: An signal for filtering.
 * @param [in] a_hop (default 1): A hopsize.
 *
 * @return: A pointer to a pointer to output filtered signals.
 */
roj_complex_signal** roj_filter_bank :: filtering (roj_complex_signal* a_sig, int a_hop){

  /* args checking */
  if(a_hop<1){
    call_warning("in roj_filter_bank :: filtering");
    call_error("hop<1");
  }

  if(a_sig == NULL){
    call_warning("in roj_filter_bank :: filtering");
    call_error("sig is NULL");
  }

  roj_signal_config in_conf = a_sig->get_config();
  if(in_conf.rate != m_filter_gen->get_rate()){
    call_warning("in roj_filter_bank :: filtering");    
    call_error("rates are not equal");
  }

  roj_signal_config out_conf;
  out_conf.rate = in_conf.rate / a_hop;
  out_conf.length = in_conf.length / a_hop;
  out_conf.start = in_conf.start + (double)(a_hop-1) / in_conf.rate;
  
  roj_complex_signal** output = new roj_complex_signal* [m_config.length];
  for(int k=0; k<m_config.length; k++){
    output[k] = new roj_complex_signal(out_conf);
    
    for(int n=0; n<in_conf.length; n++){

      m_filter_array[k]->process(a_sig->m_waveform[n]);
      
      if(n%a_hop == a_hop-1)
	output[k]->m_waveform[n/a_hop] = m_filter_array[k]->get_output();       
    }
    
    print_progress(k+1, m_config.length, "filtering");
  }

  print_progress(0, 0, "filtering");  
  return output;
}
