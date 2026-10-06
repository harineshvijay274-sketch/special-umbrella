/*
 * error.h
 *
 *  Created on: Jul 25, 2026
 *      Author: harineshvijay
 *
 *      contains typedef of the error_t
 */

#ifndef INC_ERROR_H_
#define INC_ERROR_H_


/*
 * Return types used of the function that required an error status return
 *
 * */

typedef int error_t


 /*
  *
  * it contain the type of error that occured
  * */


typedef enum {
  ERR_OK= 0,
  ERR_FAIL = -1,
  ERR_TIMEOUT = -2,
  ERR_INVALID_PARA = -3


}error_e;




#endif /* INC_ERROR_H_ */
