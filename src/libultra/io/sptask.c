#include <ultra64.h>
#include <global.h>

OSTask* _VirtualToPhysicalTask(OSTask* intp) {
    OSTask* tp = &tmp_task;
    bcopy((void*)intp, (void*)tp, sizeof(OSTask));
    if (tp->t.ucode) {
        tp->t.ucode = (u64*)osVirtualToPhysical(tp->t.ucode);
    }
    if (tp->t.ucode_data) {
        tp->t.ucode_data = (u64*)osVirtualToPhysical(tp->t.ucode_data);
    }
    if (tp->t.dram_stack) {
        tp->t.dram_stack = (u64*)osVirtualToPhysical(tp->t.dram_stack);
    }
    if (tp->t.output_buff) {
        tp->t.output_buff = (u64*)osVirtualToPhysical(tp->t.output_buff);
    }
    if (tp->t.output_buff_size) {
        tp->t.output_buff_size = (u64*)osVirtualToPhysical(tp->t.output_buff_size);
    }
    if (tp->t.data_ptr) {
        tp->t.data_ptr = (u64*)osVirtualToPhysical(tp->t.data_ptr);
    }
    if (tp->t.yield_data_ptr) {
        tp->t.yield_data_ptr = (u64*)osVirtualToPhysical(tp->t.yield_data_ptr);
    }

    return &tmp_task;
}

void osSpTaskLoad(OSTask* intp) {
    OSTask* tp;

    tp = _VirtualToPhysicalTask(intp);

    if ((tp->t.flags & 0x1) != 0) {
        tp->t.ucode_data = tp->t.yield_data_ptr;
        tp->t.ucode_data_size = tp->t.yield_data_size;

        if (tp->t.flags & OS_TASK_LOADABLE) {
            tp->t.ucode = HW_REG((uintptr_t)intp->t.yieldDataPtr + OS_YIELD_DATA_SIZE - 4, u32);
        }
    }

    while (__osSpSetPc((void*)SP_IMEM_START) == -1) {}

    while (__osSpRawStartDma(1, (void*)(SP_IMEM_START - sizeof(*tp)), tp, sizeof(OSTask)) == -1) {}

    while (__osSpDeviceBusy()) {}

    while (__osSpRawStartDma(1, (void*)SP_IMEM_START, tp->t.ucodeBoot, tp->t.ucodeBootSize) == -1) {}
}

void osSpTaskStartGo(OSTask* tp) {
    while (__osSpDeviceBusy()) {}

    __osSpSetStatus(SP_SET_INTR_BREAK | SP_CLR_SSTEP | SP_CLR_BROKE | SP_CLR_HALT);
}
