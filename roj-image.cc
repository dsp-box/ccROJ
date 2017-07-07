
#include "roj-image.hh"

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns imag parameters as a structure.
 *
 * @return: the structure which contains the configuration.
 */
roj_image_config roj_image :: get_config (){
  
  return m_config;
}

/**
 * @type: method
 * @brief: This routine checks configuration of the object.
 *
 * @return: True if object is correctly configured, false otherwise.
 */
bool roj_image :: check_config (){

  if(m_config.time.length<1){ 
    call_warning("width<1");
    return false;
  }
  if(m_config.frequency.length<1){ 
    call_warning("height<1");
    return false;
  }
  
  if(m_config.time.min >= m_config.time.max
     and m_config.time.length>1){ 
    call_warning("min t >= max t");
    return false;
  }
  if(m_config.frequency.min>=m_config.frequency.max
     and m_config.frequency.length>1){ 
    call_warning("min f >= max f");
    return false;
  }

  if (m_config.time.length==1){
    call_warning("time length is 1");
    m_config.time.max = m_config.time.min;
  }
  
  if (m_config.frequency.length==1){
    call_warning("frequency length is 1");
    m_config.frequency.max = m_config.frequency.min;
  }
  
  return true;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine compares the internal configuration to a given configuration.
 *
 * @param [in] a_conf: A pointer to the given configuration.
 *
 * @return: True if configurations are equal, false oterwise.
 */
bool roj_image :: compare_config (roj_image_config a_conf){

  if(m_config.time.length!=a_conf.time.length)
    return false;
  if(m_config.frequency.length!=a_conf.frequency.length)
    return false;
  
  if(m_config.time.min!=a_conf.time.min)
    return false;
  if(m_config.time.max!=a_conf.time.max)
    return false;
  
  if(m_config.frequency.min!=a_conf.frequency.min)
    return false;  
  if(m_config.frequency.max!=a_conf.frequency.max)
    return false;

  return true;
}

/**
 * @type: method
 * @brief: This routine compares the internal time axis configuration to a given configuration.
 *
 * @param [in] a_conf: A pointer to the given configuration.
 *
 * @return: True if configurations are equal, false oterwise.
 */
bool roj_image :: compare_time_config (roj_image_config a_conf){
  
  if(m_config.time.length!=a_conf.time.length)
    return false;
  if(m_config.time.min!=a_conf.time.min)
    return false;  
  if(m_config.time.max!=a_conf.time.max)
    return false;

  return true;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns time argument by an index.
 *
 * @param [in] a_index: A given index.
 *
 * @return: The returned time.
 */
double roj_image :: get_time_by_index (int a_index){
  
  if(m_config.time.length==1){
    call_warning("time length is 1");
    return m_config.time.min;
  }
  
  double hop = (double)(m_config.time.max - m_config.time.min) / (m_config.time.length - 1);  
  return m_config.time.min + a_index * hop;
}

/**
 * @type: method
 * @brief: This routine returns frequency argument by an index.
 *
 * @param [in] a_index: A given index.
 *
 * @return: The returned time.
 */
double roj_image :: get_frequency_by_index (int a_index){

  if(m_config.frequency.length==1){
    call_warning("frequency length is 1");
    return m_config.frequency.min;
  }

  double hop = (double)(m_config.frequency.max - m_config.frequency.min) / (m_config.frequency.length - 1);
  return m_config.frequency.min + a_index * hop;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns the nearest index for a given time argument.
 *
 * @param [in] a_time: A given time argument.
 *
 * @return: The nearest index.
 */
int roj_image :: get_index_by_time (double a_time){

  if(m_config.time.length==1){
    call_warning("time length is 1");
    return 0;
  }
  
  double hop = (double)(m_config.time.max - m_config.time.min) / (m_config.time.length - 1);
  double tmp = (a_time - m_config.time.min);
  return round(tmp/hop);
}

/**
 * @type: method
 * @brief: This routine returns the nearest index for a given frequency argument.
 *
 * @param [in] a_freq: A given frequency argument.
 *
 * @return: The nearest index.
 */
int roj_image :: get_index_by_frequency (double a_freq){

  if(m_config.frequency.length==1){
    call_warning("frequency length is 1");
    return 0;
  }

  double hop = (double)(m_config.frequency.max - m_config.frequency.min) / (m_config.frequency.length - 1);
  double tmp = (a_freq - m_config.frequency.min);
  return round(tmp/hop);
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: Checking time index.
*
* @param [in] a_index: A given index.
*
* @return: True if index is valid, false otherwise.
*/
bool roj_image :: check_in_time_index (int a_index){

  if(a_index>=m_config.time.length) return false;
  if(a_index<0) return false;
  return true;
}

/**
* @type: method
* @brief: Checking frequency index.
*
* @param [in] a_index: A given index.
*
* @return: True if index is valid, false otherwise.
*/
bool roj_image :: check_in_frequency_index (int a_index){

  if(a_index>=m_config.frequency.length) return false;
  if(a_index<0) return false;
  return true;
}
