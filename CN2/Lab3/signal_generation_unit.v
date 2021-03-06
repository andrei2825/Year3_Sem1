`include "defines.vh"
module signal_generation_unit (
        input  wire  [`STAGE_COUNT-1:0] pipeline_stage,
        input  wire [`OPCODE_COUNT-1:0] opcode_type,
        input  wire  [`GROUP_COUNT-1:0] opcode_group,
        output wire [`SIGNAL_COUNT-1:0] signals
    );

    /* Control signals */

    /* Register interface logic */
	 /* TODO 4: STS */
	 /* TODO 7: register reads */
    assign signals
	 [`CONTROL_REG_RR_READ] = 
            (pipeline_stage == `STAGE_ID) &&
            (opcode_group[`GROUP_ALU_TWO_OP]);
    assign signals[`CONTROL_REG_RR_WRITE] = 0;
    assign signals[`CONTROL_REG_RD_READ] =
            (pipeline_stage == `STAGE_ID) &&
            (opcode_group[`GROUP_ALU]);
    assign signals[`CONTROL_REG_RD_WRITE] = 
				/* TODO 3: LDI */
				/* TODO 5,6,7: register writes */
            (pipeline_stage == `STAGE_WB) &&
            (opcode_group[`GROUP_ALU]);
	
    /* Memory interface logic */
	 /* TODO 5,6: LOADs */
    assign signals[`CONTROL_MEM_READ] =
           (pipeline_stage == `STAGE_MEM) &&
           0;
	 /* TODO 4: STS 
	 inspectati bus_interface_unit.v */
    assign signals[`CONTROL_MEM_WRITE] =
           (pipeline_stage == `STAGE_MEM) &&
           0;
endmodule