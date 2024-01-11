#ifndef __INVESTOR_H__
#define __INVESTOR_H__

#include "stock.h"

#define MAX_HOLDINGS  32

typedef struct investor_s {
  uint16_t id;
  uint32_t cache;
	uint32_t initial_cash;
	//int32_t gain;
  char name[64];
  stock_holding_t holdings[MAX_HOLDINGS];
} investor_t;

typedef struct investor_stock_holding_s {
	investor_t* investor;
	stock_holding_t *holding;
} investor_stock_holding_t;


void __svc(0) svc_zero( void *obj);
void __svc(1) svc_one( void *obj);
void __svc(2) svc_two( void *obj);
void __svc(3) svc_three( void *obj);

void investor_init (investor_t *investor, char* name, uint32_t cache);
void investor_display(investor_t *investor);
char investor_buy (investor_t *investor, stock_holding_t *holding);
char investor_sell (investor_t *investor, stock_holding_t *holding);
void investor_add_transaction(investor_t *investor);
uint32_t get_num_buy(void);
uint32_t get_num_sell(void);
uint32_t get_net_worth(investor_t *investor);
int32_t get_gain(investor_t *investor);

#endif
