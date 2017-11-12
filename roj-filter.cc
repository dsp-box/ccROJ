/* *************************************************** *
 * This file is a part of ccROJ project (version 0-47) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-filter.hh"

/* ************************************************************************************************************************* */
/**
 * @type: constructor
 * @brief: This is constructor of roj_filter.
 *
 * @param [in] a_order: filter order, length of internal shift buffer.
 * @param [in] a_outlen (default 1): length of internal delay buffer.
 */
roj_filter :: roj_filter(unsigned int a_order, unsigned int a_outlen){

  /* arg parse */
  if(a_order<1){
    call_warning("in roj_filter :: roj_filter");
    call_error("order < 1");
  }
  m_order = a_order;

  if(a_outlen<1){
    call_warning("in roj_filter :: roj_filter");
    call_error("outlen too small");
  }
  m_outlen = a_outlen;
  m_index = 0;
  
  /* internal buffers alloc */
  m_acoeff = new complex double [m_order + 1];
  m_buffer = new complex double [m_order];
  m_bcoeff = new complex double [m_order];
  m_output = new complex double [m_outlen];
  for(int n=0; n<m_outlen; n++)
    m_output[n] = 0.0;
  
  /* internal buffers init */
  int byte_size = m_order * sizeof(complex double);
  memset(m_buffer, 0x0, byte_size);  
  memset(m_bcoeff, 0x0, byte_size);
  byte_size = (m_order+1) * sizeof(complex double);
  memset(m_acoeff, 0x0, byte_size);
  m_bcoeff[0] = 1.0;
}

/**
 * @type: destructor
 * @brief: This is destructor.
 */
roj_filter :: ~roj_filter(){
  
  delete [] m_output;
  delete [] m_buffer;
  delete [] m_acoeff;
  delete [] m_bcoeff;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine processes a single sample.
 *
 * @param [in] a_sample: An input sample.
 *
 * @return: A sample of filtered output.
 */
complex double roj_filter :: process(complex double a_sample){

  m_index++;
  m_index %= m_outlen;

  m_output[m_index] = m_bcoeff[0] * a_sample;
  m_output[m_index] += m_buffer[0];
  
  for (int n=0; n<m_order-1; n++){
    m_buffer[n] = m_buffer[n+1];
    m_buffer[n] += m_bcoeff[n+1] * a_sample;
    m_buffer[n] -= m_acoeff[n+1] * m_output[m_index];
  }

  m_buffer[m_order-1] = -m_acoeff[m_order] * m_output[m_index];
  return m_output[m_index];
}

/**
 * @type: method
 * @brief: This routine gets last output sample.
 *
 * @param [in] a_shift (default 1): an index of returned sample from output buffer. Give 0 to get non-delayed sample. Give 1 to get maximal delayed sample. 
 *
 * @return: A sample of filtered output.
 */
complex double roj_filter :: get_output(unsigned int a_shift){

  return m_output[(m_index+a_shift) % m_outlen];
} 

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine processes a single sample.
 *
 * @param [in] a_sig: A pointer to input signal.
 *
 * @return: A pointer to output signal.
 */
roj_complex_signal* roj_filter :: process(roj_complex_signal* a_sig){

  /* args checking */
  if(a_sig == NULL){
    call_warning("in roj_filter :: process");
    call_error("sig is NULL");
  }

  roj_signal_config in_conf = a_sig->get_config();
  roj_complex_signal* out = new roj_complex_signal(in_conf);

  for(int n=0; n<in_conf.length; n++)
    out->m_waveform[n] = process(a_sig->m_waveform[n]);

  return out;
}




