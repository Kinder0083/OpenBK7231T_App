#ifdef WINDOWS

#include "selftest_local.h"


void Test_TuyaMCU_BatteryPowered_Style1() {
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("logfeature 12 1", 0);
	CMD_ExecuteCommand("loglevel 6", 0);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("startDriver tmSensor", 0);

	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 1 val 1", 0);
	CMD_ExecuteCommand("setChannelType 1 OpenClosed", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 3 val 3", 0);
	CMD_ExecuteCommand("setChannelType 3 Custom", 0);

	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Pressed: 1. Send query product information
	(simulated delays are disabled)
	Sent: 55 AA 00 01 00 00 00
	Sent: Ver=0, Cmd=QueryInfo, Len=0, CHECKSUM OK
	*/
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 01 00 00 00");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Received: 55 AA 00 01 00 24 7B 22 70 22 3A 22 33 73 6C 70 69 72 6C 6E 6C 71 70 30 62 71 6F 31 22 2C 22 76 22 3A 22 31 2E 30 2E 30 22 7D C6
	Received: Ver=0, Cmd=QueryInfo, Len=36, JSON={"p":"3slpirlnlqp0bqo1","v":"1.0.0"}, CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 01 00 24 7B 22 70 22 3A 22 33 73 6C 70 69 72 6C 6E 6C 71 70 30 62 71 6F 31 22 2C 22 76 22 3A 22 31 2E 30 2E 30 22 7D C6", 0);
	Sim_RunSeconds(2.0f, false);
	/*
	Pressed: 2. Send MCU conf (WiFiState 0x03)
	(simulated delays are disabled)
	Sent: 55 AA 00 02 00 01 03 05
	Sent: Ver=0, Cmd=QueryStatus, Len=1, CHECKSUM OK
	*/
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 03 05");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Received: 55 AA 00 02 00 00 01
	Received: Ver=0, Cmd=QueryStatus, Len=0, CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(2.0f, false);
	Sim_RunSeconds(2.0f, false);

	/*
	Pressed: 3. Send MCU conf (WiFiState 0x04)
	(simulated delays are disabled)
	Sent: 55 AA 00 02 00 01 04 06
	Sent: Ver=0, Cmd=QueryStatus, Len=1, CHECKSUM OK
	*/
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 04 06");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	/*
	Received: 55 AA 00 02 00 00 01
	Received: Ver=0, Cmd=QueryStatus, Len=0, CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	Sim_RunSeconds(2.0f, false);
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Received: 55 AA 00 05 00 05 03 04 00 01 02 13
	dpID 3 is battery state
	Received: Ver=0, Cmd=ReportRealtimeStatus, Len=5, [dpID=3, type=4, len=1, val=2], CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 05 00 05 03 04 00 01 02 13", 0);
	/*
	Received: 55 AA 00 05 00 05 01 01 00 01 00 0C
	dpID 1 is door state
	Received: Ver=0, Cmd=ReportRealtimeStatus, Len=5, [dpID=1, type=1, len=1, val=0], CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 05 00 05 01 01 00 01 00 0C", 0);
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	Sim_RunSeconds(3.0f, false);
	// dpID 1 is door state
	SELFTEST_ASSERT_CHANNEL(1, 0);
	// dpID 3 is battery state
	SELFTEST_ASSERT_CHANNEL(3, 2);
	Sim_RunSeconds(3.0f, false);
	/*
	Pressed: 3B. Send data accept confirmation 0x05 (will put device back to sleep)
	Waiting: 1,00
	Pressed: 3B. Send data accept confirmation 0x05 (will put device back to sleep)
	Waiting: 1,00
	*/
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 05 00 01 00 05");
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 05 00 01 00 05");

	// cause error
	//SELFTEST_ASSERT_CHANNEL(15, 666);
}
void Test_TuyaMCU_BatteryPowered_Style2() {
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("startDriver tmSensor", 0);

	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 1 val 1", 0);
	CMD_ExecuteCommand("setChannelType 1 OpenClosed", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 3 val 3", 0);
	CMD_ExecuteCommand("setChannelType 3 Custom", 0);

	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Pressed: 1. Send query product information
	(simulated delays are disabled)
	Sent: 55 AA 00 01 00 00 00
	Sent: Ver=0, Cmd=QueryInfo, Len=0, CHECKSUM OK
	*/
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 01 00 00 00");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Received: 55 AA 00 01 00 24 7B 22 70 22 3A 22 65 37 64 6E 79 38 7A 76 6D 69 79 68 71 65 72 77 22 2C 22 76 22 3A 22 31 2E 30 2E 30 22 7D 24
	Received: Ver=0, Cmd=QueryInfo, Len=36, JSON={"p":"e7dny8zvmiyhqerw","v":"1.0.0"}, CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 01 00 24 7B 22 70 22 3A 22 65 37 64 6E 79 38 7A 76 6D 69 79 68 71 65 72 77 22 2C 22 76 22 3A 22 31 2E 30 2E 30 22 7D 24", 0);
	Sim_RunSeconds(2.0f, false);
	/*
	Pressed: 2. Send MCU conf (WiFiState 0x03)
	(simulated delays are disabled)
	Sent: 55 AA 00 02 00 01 03 05
	Sent: Ver=0, Cmd=QueryStatus, Len=1, CHECKSUM OK
	*/
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 03 05");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Received: 55 AA 00 02 00 00 01
	Received: Ver=0, Cmd=QueryStatus, Len=0, CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(2.0f, false);

	/*
	Pressed: 3. Send MCU conf (WiFiState 0x04)
	(simulated delays are disabled)
	Sent: 55 AA 00 02 00 01 04 06
	Sent: Ver=0, Cmd=QueryStatus, Len=1, CHECKSUM OK
	*/
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 04 06");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	/*
	Received: 55 AA 00 02 00 00 01
	Received: Ver=0, Cmd=QueryStatus, Len=0, CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(2.0f, false);

	Sim_RunSeconds(2.0f, false);
	/*
	Received: 55 AA 00 08 00 0C 00 02 02 02 02 02 02 01 01 00 01 01 23
	dpID 1 is door state
	Received: Ver=0, Cmd=QueryRecordStorage, Len=12, [dpID=1, type=1, len=1, val=1], CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 08 00 0C 00 02 02 02 02 02 02 01 01 00 01 01 23", 0);
	/*
	Received: 55 AA 00 08 00 0C 00 01 01 01 01 01 01 03 04 00 01 02 23
	dpID 3 is battery state
	Received: Ver=0, Cmd=QueryRecordStorage, Len=12, [dpID=3, type=4, len=1, val=2], CHECKSUM OK
	*/
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 08 00 0C 00 01 01 01 01 01 01 03 04 00 01 02 23", 0);
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	Sim_RunSeconds(3.0f, false);
	// dpID 1 is door state
	SELFTEST_ASSERT_CHANNEL(1, 1);
	// dpID 3 is battery state
	SELFTEST_ASSERT_CHANNEL(3, 2);
	Sim_RunSeconds(3.0f, false);
	/*
	Pressed: 3B. Send data accept confirmation 0x05 (will put device back to sleep)
	Waiting: 1,00
	Pressed: 3B. Send data accept confirmation 0x05 (will put device back to sleep)
	Waiting: 1,00
	*/
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 08 00 01 00 08");
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 08 00 01 00 08");

	// cause error
	//SELFTEST_ASSERT_CHANNEL(15, 666);
}
/*
//
// setup dpCache - temperature interval
//
// Show textfield for that
setChannelType 5 TextField
// setup display name
setChannelLabel 5 Temperature Interval
// Make value persistant (stored between reboots), 
// start value -1 means "remember last"
SetStartValue 5 -1
// set default value if not set
alias set_default_5 setChannel 5 1
if $CH5==0 then set_default_5
// link dpID 17 to channel 5, the type is val, extra '1' means that its dpCache variable
linkTuyaMCUOutputToChannel 17 val 5 1

setChannelType 6 TextField
setChannelLabel 6 Humidity Interval
SetStartValue 6 -1
alias set_default_6 setChannel 6 1
if $CH6==0 then set_default_6
linkTuyaMCUOutputToChannel 18 val 6 1

*/
void Test_TuyaMCU_BatteryPowered_DPcacheFeature() {
	/*
		Received by WiFi module:
		55 AA	00	10		00 01	00	10
		HEADER	VER=00	ObtainDPcache		LEN		CHK
		Sent by WiFi module:
		55 AA	00	10		00 12	010211020004000000011202000400000001	55
		HEADER	VER=00	ObtainDPcache		LEN	dpId=17 Val V=1,dpId=18 Val V=1	CHK
	*/
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);

	SELFTEST_ASSERT_HAS_UART_EMPTY();
	// packet is the following:
	// 01 02 
	// result 1 (OK), numVars 2
	// 11 02 0004 00000001  		
	// dpId = 17 Len = 0004 Val V = 1
	// 12 02 0004 00000001
	// dpId = 18 Len = 0004 Val V = 1	
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 18 val 2 1", 0);
	CMD_ExecuteCommand("setChannel 2 1", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 17 val 1 1", 0);
	CMD_ExecuteCommand("setChannel 1 1", 0);

	SIM_ClearUART();
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	TuyaMCU_V0_SendDPCacheReply();

	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55AA0010001201021102000400000001120200040000000155");
}
void Test_TuyaMCU_BatteryPowered_DPcacheFeature3() {
	/*
	55 AA	00	10		00 07	01010904000101	27
	HEADER	VER=00	ObtainDPcache		LEN	dpId=9 Enum V=1	CHK
	*/
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);

	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// just for fun, use channel 33, it doesn't matter, dpID is still 9
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 9 enum 33 1", 0);
	CMD_ExecuteCommand("setChannel 33 1", 0);

	SIM_ClearUART();
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	TuyaMCU_V0_SendDPCacheReply();

	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA	00	10		00 07	01010904000101	27");
}
void Test_TuyaMCU_BatteryPowered_DPcacheFeature4() {
	/*
	55 AA	00	10		00 07	01010904000100	26
	HEADER	VER=00	ObtainDPcache		LEN	dpId=9 Enum V=0	CHK
	*/
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);

	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// just for fun, use channel 33, it doesn't matter, dpID is still 9
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 9 enum 33 1", 0);
	CMD_ExecuteCommand("setChannel 33 0", 0);

	SIM_ClearUART();
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	TuyaMCU_V0_SendDPCacheReply();

	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55AA001000070101090400010026");
}
void Test_TuyaMCU_BatteryPowered_DPcacheFeature2() {
	/*
	Received by WiFi module:
	55 AA	00	10		00 01	00	10
	HEADER	VER=00	ObtainDPcache		LEN		CHK

	Sent by WiFi module:
	55 AA	00	10		00 02	0100	12
	HEADER	VER=00	ObtainDPcache		LEN		CHK
	*/
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);

	SELFTEST_ASSERT_HAS_UART_EMPTY();

	SIM_ClearUART();
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	TuyaMCU_V0_SendDPCacheReply();

	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA	00	10		00 02	0100	12");
}
void Test_TuyaMCU_BatteryPowered_QuerySignalStrength() {
	// reset whole device
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("startDriver tmSensor", 0);

	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	/*
	Pressed: 1. Send query product information
	(simulated delays are disabled)
	Sent: 55 AA 00 01 00 00 00
	Sent: Ver=0, Cmd=QueryInfo, Len=0, CHECKSUM OK
	*/
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 01 00 00 00");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
	
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 0B 00 00 0A", 0);
	Sim_RunSeconds(0.1f, false);
	
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 0B 00 02 01 50 5D");
	// nothing is sent by OBK at that point
	SELFTEST_ASSERT_HAS_UART_EMPTY();
}
// Low-power TuyaMCU (Tuya "serial port protocol" for battery devices) uses
// command 0x09 ("send module command") for DP writes; 0x06 means "get local time" there.
void Test_TuyaMCU_SetDpCommand() {
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 106 val 4", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 109 bool 6", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 107 enum 5", 0);
	SIM_ClearUART();

	// default is unchanged: 0x06
	CMD_ExecuteCommand("setChannel 4 30", 0);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 06 00 08 6A 02 00 04 00 00 00 1E 9B");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	CMD_ExecuteCommand("tuyaMcu_setDpCommand 0x09", 0);
	CMD_ExecuteCommand("setChannel 4 5", 0);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 08 6A 02 00 04 00 00 00 05 85");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	CMD_ExecuteCommand("setChannel 6 1", 0);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 05 6D 01 00 01 01 7D");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	CMD_ExecuteCommand("setChannel 5 2", 0);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 05 6B 04 00 01 02 7F");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// decimal argument works too, and switches back
	// (not "setChannel 6 0": outside tmSensor mode a write equal to the mapping's
	// initial prevValue 0 is skipped by design)
	CMD_ExecuteCommand("tuyaMcu_setDpCommand 6", 0);
	CMD_ExecuteCommand("setChannel 4 7", 0);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 06 00 08 6A 02 00 04 00 00 00 07 84");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// driver restart resets to default 0x06
	CMD_ExecuteCommand("tuyaMcu_setDpCommand 0x09", 0);
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);
	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 109 bool 6", 0);
	SIM_ClearUART();
	CMD_ExecuteCommand("setChannel 6 1", 0);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 06 00 05 6D 01 00 01 01 7A");
	SELFTEST_ASSERT_HAS_UART_EMPTY();
}
// Writes requested while the low-power handshake is not finished (e.g. retained
// MQTT values arriving right after connect) must wait, keep only the latest value
// per dpID, and go out once the MCU accepts states.
void Test_TuyaMCU_BatteryPowered_PendingWrites() {
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);

	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("startDriver tmSensor", 0);
	CMD_ExecuteCommand("tuyaMcu_setDpCommand 0x09", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 2 val 1", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 6 enum 2", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 102 bool 3", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 106 val 4", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 107 enum 5", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 109 bool 6", 0);
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 01 00 00 00");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// product info of the solar PIR siren {"p":"1xupv3vnoeqpslsb","v":"1.0.0"}
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 01 00 24 7B 22 70 22 3A 22 31 78 75 70 76 33 76 6E 6F 65 71 70 73 6C 73 62 22 2C 22 76 22 3A 22 31 2E 30 2E 30 22 7D 23", 0);
	Sim_RunSeconds(2.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 03 05");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// retained MQTT settings arrive now - MCU is not ready yet
	CMD_ExecuteCommand("setChannel 4 5", 0);
	CMD_ExecuteCommand("setChannel 4 30", 0);
	CMD_ExecuteCommand("setChannel 6 1", 0);
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(2.0f, false);
	Sim_RunSeconds(2.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 04 06");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// second ack: MCU now accepts states, pending writes go out, latest value only
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(5.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 08 6A 02 00 04 00 00 00 1E 9E");
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 05 6D 01 00 01 01 7D");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// MCU echoes the written value: no resend, only the usual 0x05 ack
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 05 00 08 6A 02 00 04 00 00 00 1E 9A", 0);
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_CHANNEL(4, 30);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 05 00 01 00 05");
	Sim_RunSeconds(5.0f, false);
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// MCU reports a value changed locally on the device: channel follows, nothing is sent back
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 05 00 05 6B 04 00 01 01 7A", 0);
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_CHANNEL(5, 1);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 05 00 01 00 05");
	Sim_RunSeconds(5.0f, false);
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	// after the handshake, writes go out directly
	CMD_ExecuteCommand("setChannel 5 2", 0);
	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 05 6B 04 00 01 02 7F");
	SELFTEST_ASSERT_HAS_UART_EMPTY();
}
// Clearing a retained MQTT value publishes an empty payload on <topic>/<ch>/set:
// it must not set the channel to 0 (alarm duration 0 s!).
void Test_MQTT_EmptyChannelSetIgnored() {
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);
	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("tuyaMcu_setDpCommand 0x09", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 106 val 4", 0);
	CMD_ExecuteCommand("setChannel 4 30", 0);
	SIM_ClearUART();

	SIM_SendFakeMQTTRawChannelSet(4, "");
	SELFTEST_ASSERT_CHANNEL(4, 30);
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	SIM_SendFakeMQTTRawChannelSet(4, "  ");
	SELFTEST_ASSERT_CHANNEL(4, 30);
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	SIM_SendFakeMQTTRawChannelSet(4, "20");
	SELFTEST_ASSERT_CHANNEL(4, 20);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 08 6A 02 00 04 00 00 00 14 94");
	SELFTEST_ASSERT_HAS_UART_EMPTY();
}
// On a low-power device channels start at 0 on every wake and some dpIDs are
// rarely reported, so a retained "0" (e.g. sound on, mode 0) must still be sent.
void Test_TuyaMCU_BatteryPowered_RetainedZeroIsSent() {
	SIM_ClearOBK(0);
	SIM_UART_InitReceiveRingBuffer(1024);
	CMD_ExecuteCommand("startDriver TuyaMCU", 0);
	CMD_ExecuteCommand("startDriver tmSensor", 0);
	CMD_ExecuteCommand("tuyaMcu_setDpCommand 0x09", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 109 bool 6", 0);
	CMD_ExecuteCommand("linkTuyaMCUOutputToChannel 107 enum 5", 0);

	Sim_RunSeconds(3.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 01 00 00 00");
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 01 00 24 7B 22 70 22 3A 22 31 78 75 70 76 33 76 6E 6F 65 71 70 73 6C 73 62 22 2C 22 76 22 3A 22 31 2E 30 2E 30 22 7D 23", 0);
	Sim_RunSeconds(2.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 03 05");

	// channels 5 and 6 are 0 already; retained values "0" arrive right after MQTT connect
	SELFTEST_ASSERT_CHANNEL(6, 0);
	SIM_SendFakeMQTTRawChannelSet(6, "0");
	SIM_SendFakeMQTTRawChannelSet(5, "0");
	SELFTEST_ASSERT_HAS_UART_EMPTY();

	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(4.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 02 00 01 04 06");
	CMD_ExecuteCommand("uartFakeHex 55 AA 00 02 00 00 01", 0);
	Sim_RunSeconds(5.0f, false);
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 05 6D 01 00 01 00 7C");
	SELFTEST_ASSERT_HAS_SENT_UART_STRING("55 AA 00 09 00 05 6B 04 00 01 00 7D");
	SELFTEST_ASSERT_HAS_UART_EMPTY();
}
void Test_TuyaMCU_BatteryPowered() {
	Test_TuyaMCU_BatteryPowered_Style2();
	Test_TuyaMCU_BatteryPowered_Style1();
	Test_TuyaMCU_BatteryPowered_Style2();
	Test_TuyaMCU_BatteryPowered_Style1();
	Test_TuyaMCU_BatteryPowered_DPcacheFeature();
	Test_TuyaMCU_BatteryPowered_DPcacheFeature2();
	Test_TuyaMCU_BatteryPowered_DPcacheFeature3();
	Test_TuyaMCU_BatteryPowered_DPcacheFeature4();

	Test_TuyaMCU_BatteryPowered_QuerySignalStrength();

	// run last: they advance the simulated clock, which shifts the retry phase of the tests above
	Test_TuyaMCU_SetDpCommand();
	Test_TuyaMCU_BatteryPowered_PendingWrites();
	Test_MQTT_EmptyChannelSetIgnored();
	Test_TuyaMCU_BatteryPowered_RetainedZeroIsSent();
}

#endif

