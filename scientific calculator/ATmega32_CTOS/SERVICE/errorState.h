/*
 * errorState.h
 *
 *  Created on: Jul 15, 2023
 *      Author: Raghad Islam
 */

#ifndef ERRORSTATE_H_
#define ERRORSTATE_H_


typedef enum{
	ES_NOK,
	ES_OK,
	ES_NULL_POINTER,
	ES_OUT_OF_RANGE,
	ES_WRONG_TYPE,
	ES_PIN_NOT_CONNECTED
}ES_t;

#endif /* ERRORSTATE_H_ */
