/* *************************************************** *
 * This file is a part of ccROJ project (version 0-46) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-filter-gener.hh"

/* ************************************************************************************************************************* */
/**
* @type: constructor
* @brief: This is a constructor of an universal roj_filter_generator.
*
* @param [in] a_rate: Sampling rate.
*/
roj_filter_generator :: roj_filter_generator (double a_rate){

  set_rate (a_rate);

  /* defaults */
  m_type = ROJ_ODE_FILTER;
  m_frequency = 0.0;
  m_spread = 1.0;
  m_outlen = 1;
  m_order = 4;
}

/**
* @type: destructor
* @brief: The destructor is default.
*/

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This routine allows to set sampling rate.
*
* @param [in] a_rate: Sampling rate.
*/
void roj_filter_generator :: set_rate (double a_rate){
  
  if(a_rate<=0)
    call_error("a_rate <= 0");
  m_rate = a_rate;
}

/**
* @type: method
* @brief: This routine returns filter sampling rate.
*
* @return: Sampling rate.
*/
double roj_filter_generator :: get_rate (){
  
 return  m_rate;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: Order setter, filter length.
*
* @param [in] a_order: Filter order.
*/
void roj_filter_generator :: set_order(unsigned int a_order){

  if(a_order<=0)
    call_error("a_order <= 0");
  m_order = a_order;
}

/**
* @type: method
* @brief: Order getter, filter length.
*
* @return: Filter order.
*/
unsigned int roj_filter_generator :: get_order(){
  
  return m_order;
}

/**
* @type: method
* @brief: Outlen setter, output delay buffer length.
*
* @param [in] a_outlen: A length of delay filter.
*/
void roj_filter_generator :: set_delay(unsigned int a_outlen){

  if(a_outlen<=0)
    call_error("a_outlen <= 0");
  m_outlen = a_outlen;
}

/**
* @type: method
* @brief: Order getter, filter length.
*
* @return: Filter outlen.
*/
unsigned int roj_filter_generator :: get_delay(){
  
  return m_outlen;
}

/**
* @type: method
* @brief: Type setter. Checking is during generation.
*
* @param [in] a_code: A code of filter type.
*/
void roj_filter_generator :: set_type(int a_code){
  
  m_type = a_code;
}

/**
* @type: method
* @brief: Type getter.
*
* @return: A code of filter type.
*/
int roj_filter_generator :: get_type(){
  
  return m_type;
}

/**
* @type: method
* @brief: Frequency setter.
*
* @param [in] a_frequency: A center frequency.
*/
void roj_filter_generator :: set_frequency(double a_frequency){
  
  m_frequency = a_frequency;
}

/**
* @type: method
* @brief: Frequency getter.
*
* @return: A frequency.
*/
double roj_filter_generator :: get_frequency(){

  return m_frequency;
}

/**
* @type: method
* @brief: Spread setter.
*
* @param [in] a_spread: A time spread.
*/
void roj_filter_generator :: set_spread(double a_spread){
  
  if(a_spread<=0)
    call_error("a_spread <= 0");
  m_spread = a_spread;
}

/**
* @type: method
* @brief: Spread getter.
*
* @return: A parametr.
*/
double roj_filter_generator :: get_spread(){

  return m_spread;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This method generates a filter object.
*
* @return: A pointer to generated filter.
*/
roj_filter* roj_filter_generator :: get_filter (){

  roj_filter* filter;
  
  switch(m_type){
  case ROJ_ODE_FILTER:
    filter = get_ode_filter();
    break;
    
  default:
    call_error("unknown filter type");
  }

  return filter;
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: This method generates an ODE filter object.
*
* @return: A pointer to generated filter.
*/
roj_filter* roj_filter_generator :: get_ode_filter(){

  roj_filter* filter = new roj_filter(m_order, m_outlen);
  complex double pole = I*TWO_PI * m_frequency -1.0/m_spread;
  complex double alpha = cexp(pole / m_rate);

  for(int i=0; i<=m_order; i++)
    filter->m_acoeff[i] = cpow(-alpha, i) *  calc_binominal (m_order, i);
  double n_spread = m_spread * m_rate;
  double b_factor = 1.0 / (pow(n_spread, m_order) * calc_factorial(m_order));  
  for(int i=0; i<m_order; i++)
    filter->m_bcoeff[i] = b_factor * calc_eulerian(m_order, m_order-i) * cpow(alpha, i);  

  return filter;
}

