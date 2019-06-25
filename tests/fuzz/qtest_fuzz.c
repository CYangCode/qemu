

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "qemu-common.h"
#include "exec/memory.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "qemu/main-loop.h"


#include "fuzz.h"
#include "qtest_fuzz.h"
#include "tests/libqtest.h"



static uint16_t normalize_io_port(uint16_t addr) {
	if(addr>=0x5655 && addr<=0x565b)
		return 0;
	if(addr>=0x510 && addr<=0x518)
		return 0;
	return addr;
}

int qtest_fuzz(const unsigned char *Data, size_t Size){
	const unsigned char *pos = Data;
	const unsigned char *End = Data + Size;

	qtest_cmd *cmd;
	
	while(pos < Data+Size)
	{
		// What is the command?
		cmd = &commands[(*pos)%(sizeof(commands)/sizeof(qtest_cmd))];
		pos++;
		
		if(strcmp(cmd->name, "clock_step") == 0){
			/* qtest_clock_step_next(s); */
		} // TODO: Add second clock_step
		/* else if(strcmp(cmd->name, "clock_set")) { */
		/* 	if(pos + sizeof(int64_t) < End) { */
		/* 		qtest_clock_set(s, *(int64_t*)(pos)); */
		/* 		pos += sizeof(int64_t); */
		/* 	} */
		/* } */
		else if(strcmp(cmd->name, "outb") == 0) {
			if(pos + sizeof(uint16_t) + sizeof(uint8_t) < End) {
				uint16_t addr = *(int16_t*)(pos);
				pos += sizeof(uint16_t);
				uint8_t val = *(uint16_t*)(pos);
				pos += sizeof(uint8_t);
				addr = normalize_io_port(addr);
				qtest_outb(s, addr, val);
			}
		}
		else if(strcmp(cmd->name, "outw") == 0) {
			if(pos + sizeof(uint16_t) + sizeof(uint16_t) < End) {
				uint16_t addr = *(int16_t*)(pos);
				pos += sizeof(uint16_t);
				uint16_t val = *(uint16_t*)(pos);
				pos += sizeof(uint16_t);
				addr = normalize_io_port(addr);
				qtest_outw(s, addr, val);
			}
		}
		else if(strcmp(cmd->name, "outl") == 0) {
			if(pos + sizeof(uint16_t) + sizeof(uint32_t) < End) {
				uint16_t addr = *(int16_t*)(pos);
				pos += sizeof(uint16_t);
				uint32_t val = *(uint32_t*)(pos);
				pos += sizeof(uint32_t);
				addr = normalize_io_port(addr);
				qtest_outl(s, addr, val);
			}
		}
		else if(strcmp(cmd->name, "inb") == 0) {
			if(pos + sizeof(uint16_t) < End) {
				uint16_t addr = *(int16_t*)(pos);
				pos += sizeof(uint16_t);
				addr = normalize_io_port(addr);
				qtest_inb(s, addr);
			}
		}
		else if(strcmp(cmd->name, "inw") == 0) {
			if(pos + sizeof(uint16_t) < End) {
				uint16_t addr = *(int16_t*)(pos);
				pos += sizeof(uint16_t);
				addr = normalize_io_port(addr);
				qtest_inw(s, addr);
			}
		}
		else if(strcmp(cmd->name, "inl") == 0) {
			if(pos + sizeof(uint16_t) < End) {
				uint16_t addr = *(int16_t*)(pos);
				pos += sizeof(uint16_t);
				addr = normalize_io_port(addr);
				qtest_inl(s, addr);
			}
		}
		else if(strcmp(cmd->name, "writeb") == 0) {
			if(pos + sizeof(uint32_t) + sizeof(uint8_t) < End) {
				uint32_t addr = *(int32_t*)(pos);
				pos += sizeof(uint32_t);
				uint8_t val = *(uint8_t*)(pos);
				pos += sizeof(uint8_t);
				qtest_writeb(s, addr, val);
			}
		}
		else if(strcmp(cmd->name, "writew") == 0) {
			if(pos + sizeof(uint32_t) + sizeof(uint16_t) < End) {
				uint32_t addr = *(int32_t*)(pos);
				pos += sizeof(uint32_t);
				uint16_t val = *(uint16_t*)(pos);
				pos += sizeof(uint16_t);
				qtest_writew(s, addr, val);
			}
		}
		else if(strcmp(cmd->name, "writel") == 0) {
			if(pos + sizeof(uint32_t) + sizeof(uint32_t) < End) {
				uint32_t addr = *(int32_t*)(pos);
				pos += sizeof(uint32_t);
				uint32_t val = *(uint32_t*)(pos);
				pos += sizeof(uint32_t);
				qtest_writel(s, addr, val);
			}
		}
		else if(strcmp(cmd->name, "readb") == 0) {
			if(pos + sizeof(uint32_t) < End) {
				uint32_t addr = *(int32_t*)(pos);
				pos += sizeof(uint32_t);
				qtest_readb(s, addr);
			}
		}
		else if(strcmp(cmd->name, "readw") == 0) {
			if(pos + sizeof(uint32_t) < End) {
				uint32_t addr = *(int32_t*)(pos);
				pos += sizeof(uint32_t);
				qtest_readw(s, addr);
			}
		}
		else if(strcmp(cmd->name, "readl") == 0) {
			if(pos + sizeof(uint32_t) < End) {
				uint32_t addr = *(int32_t*)(pos);
				pos += sizeof(uint32_t);
				qtest_readl(s, addr);
			}
		}
		// Now get the system up and running
		qtest_recv_line(s);
		qtest_clock_step_next(s);
		main_loop_wait(false);
	}
	reset();
	return 0;
}
