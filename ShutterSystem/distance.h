
#ifndef DISTANCE_H_
#define DISTANCE_H_

#define BEGIN 0x1
#define END 0x0

void init_ports(void);
void init_timer(void);
void init_ext_int(void);
uint16_t calc_cm(uint16_t counter);

#endif /* DISTANCE_H_ */