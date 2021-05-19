#ifndef __APP_PROFILE_THREAD_H__
#define __APP_PROFILE_THREAD_H__


void aep_thread_init(void);


void decode_aep_data(char *data);

void report_gps(int8_t mode);

extern void send_data_asy_callback(uint8_t result, uint32_t id);

#endif
