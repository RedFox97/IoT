#include "stm8s_conf.h"
#include "iostm8s003f3fix.h"
#define rf	PC_IDR_IDR3

int decode, soChuKy, chuKyMau, chuKy;
unsigned long timer, timer1, thoiGianXungCao;
unsigned long value, rfData;
int chuKyThu[26];

void configTimer4() {
	/*
		Thanh ghi CR1
		ARPE = 1	Thiet lap timer 4 o che do nap lai
		CEN = 1		Cho phép Counter hoat dong
	*/
	TIM4_CR1 = 0x81;
	/*
		Tính tan so ngat
	  F = Fmaster / (2^PSCR * ARR)
		F = 20kHz, ngat 50us
	*/
	TIM4_PSCR = 4; 				// he so chia là 4
	TIM4_ARR = 50; 				// TOP counter là 20
	TIM4_IER = 0x01; 			// cho phép ngat
}

#pragma vector = TIM4_OVR_UIF_vector
__interrupt void ISR_TIM4(){
	timer++;
	switch(decode) {
		case 0: 
						if(timer > 180) {				// Ða so dieu khien se o che do cho truoc khi doc du lieu khoang 10ms
							decode++;
							timer = 0;
							chuKyMau = 0;
							soChuKy = 0;
							TIM4_CR1_CEN = 0;
						}
						break;
		case 3: 
						timer1++;
						if(timer > 1000) {				// Qua trinh 3, 4 kiem tra bit ket thuc qua trinh doc du lieu
							decode++;
							timer = 0;
							TIM4_CR1_CEN = 0;
						}
						break;
		case 4: 
						timer1++;
						if(timer > 1000) {
							decode=0;
							timer = 0;
							TIM4_CR1_CEN = 0;
						}
						else if(timer1 > 5000) {
							decode	=0;
							timer		=0;
							timer1	=0;
						}
						break;
	}
	TIM4_SR = 0; // Xóa het co
} 

#pragma vector = 7		// Tham khao https://blog.mark-stevens.co.uk/2012/09/interrupts-on-the-stm8s/
__interrupt void EXTI_PORTD_IRQHandler(void) {
	switch(decode) {
		case 0:
						if(rf == 0) {						// Dem thoi gian tin hieu o muc thap
							TIM4_CR1_CEN = 1;
						}
						else {													// Reset thoi gian khi tin hieu o muc cao
							TIM4_CR1_CEN = 0;
							timer = 0;
						}
						rfData = 0;
						break;
		case 1:
						if(rf == 1) {										// Bat duoc xung suon len
							TIM4_CR1_CEN = 1;							// Bat timer bat dau dem
							timer = 0;										// Reset bien dem
							value = 0;										// Reset bien value
							decode++;
						}
						break;
		case 2:
						if(soChuKy <= 24) {																													// Chi doc du 24 bit du lieu
							if(rf == 0) {									// Doc thoi gian xung o muc cao
								thoiGianXungCao = timer;
							}
							else {
								chuKy = timer;																															// Doc thoi gian 1 chu ky xung
								timer = 0;																																	// Reset bien dem
								soChuKy++;																																	// Tang so bit doc duoc them 1
								if(soChuKy == 1)																														// Luu chu ky doc lan dau lam cu ky mau de so sanh
									chuKyMau=chuKy;
								chuKyThu[soChuKy] = chuKy;																								// Luu chu ky vua doc duoc
								if(chuKyThu[soChuKy] >= chuKyMau-3 && chuKyThu[soChuKy] <= chuKyMau+3) {	// So sanh sai so giua chu ky vua doc duoc voi chu ky mau
									if(soChuKy == 4) {																											// Neu 3 chu ky dau giong nhau thi set chu ky mau bang gia tri trung binh cua 3 chu ky dau
										chuKyMau = chuKyThu[1]+chuKyThu[2]+chuKyThu[3];
											chuKyMau = chuKyMau/3;
									}
									value = value<<1;																												// Kiem tra bit vua doc la 0 hay 1
									if(thoiGianXungCao < chuKyMau/2)
										value +=1;
								}
								else {											// Neu khong xap xi voi chu ky mau, thi cho tin hieu sau doc lai
									decode = 0;
									value = 0;
									TIM4_CR1_CEN = 0;
								}
							}
						}
						if(soChuKy > 23) {								// Hoan thanh qua trinh doc, tra ket qua, tat timer
							TIM4_CR1_CEN = 0;
							soChuKy = 0;
							rfData = value;
							decode++;
						}
						break;
		case 3:
						if(rf == 0) {				// Doc thoi gian xung o muc thap
							TIM4_CR1_CEN = 1;
						}
						else {
							TIM4_CR1_CEN = 0;
							timer = 0;
						}
						break;
		case 4:
						if(rf == 1) {				// Doc thoi gian xung o muc cao
							TIM4_CR1_CEN = 1;
						} 
						else {
							TIM4_CR1_CEN = 0;
							timer = 0;
						}
						break;
	}
}