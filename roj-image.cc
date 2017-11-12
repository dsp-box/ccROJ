/* *************************************************** *
 * This file is a part of ccROJ project (version 0-47) *
 * distributed under GNU General Public License v3.0.  *
 * Please visit the webpage: github.com/dsp-box/ccROJ  *
 * for more information.                               *
 *                       contact: Krzysztof Czarnecki  *
 *                email: czarnecki.krzysiek@gmail.com  *
 * *************************************************** */

#include "roj-image.hh"

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns a roj_imag configuration.
 *
 * @return: the structure which contains the configuration
 */
roj_image_config roj_image :: get_config (){
  
  return m_config;
}

/**
 * @type: method
 * @brief: This routine checks configuration of the object.
 *
 * @return: true if object is correctly configured, false otherwise
 */
bool roj_image :: check_config (){

  if(m_config.x.length<1){ 
    call_warning("width<1");
    return false;
  }
  if(m_config.y.length<1){ 
    call_warning("height<1");
    return false;
  }
  
  if(m_config.x.min >= m_config.x.max
     and m_config.x.length>1){ 
    call_warning("min t >= max t");
    call_info("min t: ", m_config.x.min);
    call_info("max t: ", m_config.x.max);
    return false;
  }
  if(m_config.y.min>=m_config.y.max
     and m_config.y.length>1){ 
    call_warning("min f >= max f");
    call_info("min f: ", m_config.y.min);
    call_info("max f: ", m_config.y.max);
    return false;
  }

  if (m_config.x.length==1){
    call_warning("time length is 1");
    m_config.x.max = m_config.x.min;
  }
  
  if (m_config.y.length==1){
    call_warning("frequency length is 1");
    m_config.y.max = m_config.y.min;
  }
  
  return true;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine compares the internal configuration to a given configuration.
 *
 * @param [in] a_conf: a configuration.
 *
 * @return: true if configurations are equal, false oterwise.
 */
bool roj_image :: compare_config (roj_image_config a_conf){

  if(m_config.x.length!=a_conf.x.length)
    return false;
  if(m_config.y.length!=a_conf.y.length)
    return false;
  
  if(m_config.x.min!=a_conf.x.min)
    return false;
  if(m_config.x.max!=a_conf.x.max)
    return false;
  
  if(m_config.y.min!=a_conf.y.min)
    return false;  
  if(m_config.y.max!=a_conf.y.max)
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
bool roj_image :: compare_x_config (roj_image_config a_conf){
  
  if(m_config.x.length!=a_conf.x.length)
    return false;
  if(m_config.x.min!=a_conf.x.min)
    return false;  
  if(m_config.x.max!=a_conf.x.max)
    return false;

  return true;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns x value corresponded to a given index.
 *
 * @param [in] a_index: given index
 *
 * @return: x value
 */
double roj_image :: get_x_by_index (int a_index){
  
  if(m_config.x.length==1){
    call_warning("time length is 1");
    return m_config.x.min;
  }
  
  double hop = (double)(m_config.x.max - m_config.x.min) / (m_config.x.length - 1);  
  return m_config.x.min + a_index * hop;
}

/**
 * @type: method
 * @brief: This routine returns y value corresponded to a given index.
 *
 * @param [in] a_index: given index
 *
 * @return: y value
 */
double roj_image :: get_y_by_index (int a_index){

  if(m_config.y.length==1){
    call_warning("frequency length is 1");
    return m_config.y.min;
  }

  double hop = (double)(m_config.y.max - m_config.y.min) / (m_config.y.length - 1);
  return m_config.y.min + a_index * hop;
}

/* ************************************************************************************************************************* */
/**
 * @type: method
 * @brief: This routine returns the nearest index for a given x argument.
 *
 * @param [in] a_time: given x argument
 *
 * @return: the nearest index
 */
int roj_image :: get_index_by_x (double a_time){

  if(m_config.x.length==1){
    call_warning("time length is 1");
    return 0;
  }
  
  double hop = (double)(m_config.x.max - m_config.x.min) / (m_config.x.length - 1);
  double tmp = (a_time - m_config.x.min);
  return round(tmp/hop);
}

/**
 * @type: method
 * @brief: This routine returns the nearest index for a given y argument.
 *
 * @param [in] a_freq: given y argument
 *
 * @return: the nearest index
 */
int roj_image :: get_index_by_y (double a_freq){

  if(m_config.y.length==1){
    call_warning("frequency length is 1");
    return 0;
  }

  double hop = (double)(m_config.y.max - m_config.y.min) / (m_config.y.length - 1);
  double tmp = (a_freq - m_config.y.min);
  return round(tmp/hop);
}

/* ************************************************************************************************************************* */
/**
* @type: method
* @brief: Checking x index.
*
* @param [in] a_index: given index
*
* @return: true if index is valid, false otherwise
*/
bool roj_image :: check_in_x_index (int a_index){

  if(a_index>=m_config.x.length) return false;
  if(a_index<0) return false;
  return true;
}

/**
* @type: method
* @brief: Checking y index.
*
* @param [in] a_index: given index
*
* @return: true if index is valid, false otherwise
*/
bool roj_image :: check_in_y_index (int a_index){

  if(a_index>=m_config.y.length) return false;
  if(a_index<0) return false;
  return true;
}
