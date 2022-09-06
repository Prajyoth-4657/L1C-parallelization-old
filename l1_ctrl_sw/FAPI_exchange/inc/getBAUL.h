
#ifndef _GET_BA_UL_H_
#define _GET_BA_UL_H_

#define FLAG_FRZ 0
#define FLAG_CRC 1
#define FLAG_INF 2
#define FLAG_PCS 3

typedef short	      cnt_t; // 0-1024
typedef short         idx_t;
typedef unsigned char flg_t;
typedef unsigned int  bat_t;
typedef unsigned char grw_t;

void getIMAP(cnt_t E, idx_t A, idx_t L, idx_t Npc, idx_t Npc_wm, flg_t imap[1024], idx_t para[2]);

char getBAOUT(flg_t imap[1024], idx_t para[2], bat_t BA_o[64]);

#endif
