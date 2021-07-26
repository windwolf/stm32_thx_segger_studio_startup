#if defined(QSPI_LOADER_W25Q256)

#include "bootloader_qspi.h"
#include "stdint.h"
#include "stm32h7xx_hal.h"

extern QSPI_HandleTypeDef hqspi;

static HAL_StatusTypeDef bootloader_qspi_busy_wait(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_reset(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_qpi_exit(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_qpi_enter(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_4bytes_enter(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_write_enable(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_qe_set(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_dummy_circle_set(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_mm_qpi(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_mm(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_ddr_mm(QSPI_CommandTypeDef *cmd);

static HAL_StatusTypeDef bootloader_qspi_adp_set(QSPI_CommandTypeDef *cmd);

// void bootloader_qspi_startup()
// {
//     HAL_StatusTypeDef rst;
//     MX_QUADSPI_Init();

//     command_qspi_create(&qspi_cmd, &hqspi, 4);
//     w25qxx_create(&qspi_w25qxx, (Command *)&qspi_cmd, 0);

//     w25qxx_reset(&qspi_w25qxx);
//     qspi_w25qxx.status2Bits.QE = 1;
//     w25qxx_status_set(&qspi_w25qxx);

//     QSPI_CommandTypeDef mmCmd = {0};
//     bootloader_qspi_mm(&mmCmd);
// }

void bootloader_qspi_startup()
{
    QSPI_CommandTypeDef mmCmd = {0};
    HAL_StatusTypeDef rst;

    mmCmd.AddressMode = QSPI_ADDRESS_NONE;
    mmCmd.Address = 0;

    mmCmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;

    mmCmd.DummyCycles = 0;

    mmCmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    mmCmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    mmCmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    rst = bootloader_qspi_qpi_exit(&mmCmd);

    rst = bootloader_qspi_reset(&mmCmd);

    rst = bootloader_qspi_4bytes_enter(&mmCmd);

    rst = bootloader_qspi_write_enable(&mmCmd);

    rst = bootloader_qspi_qe_set(&mmCmd);

    //rst = bootloader_qspi_qpi_enter(&mmCmd);

    //rst = bootloader_qspi_dummy_circle_set(&mmCmd);

    rst = bootloader_qspi_mm(&mmCmd);
}

static HAL_StatusTypeDef bootloader_qspi_busy_wait(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0x05;

    cmd->DataMode = QSPI_DATA_1_LINE;
    cmd->NbData = 1;
    uint8_t data;

    do
    {
        rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
        rst = HAL_QSPI_Receive(&hqspi, &data, HAL_MAX_DELAY);

    } while (data);

    return rst;
};

static HAL_StatusTypeDef bootloader_qspi_reset(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0x66;

    cmd->DataMode = QSPI_DATA_NONE;
    rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);

    cmd->Instruction = 0x99;
    rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);

    return rst;
};

static HAL_StatusTypeDef bootloader_qspi_qpi_exit(QSPI_CommandTypeDef *cmd)
{
    cmd->InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd->Instruction = 0xFF;

    cmd->DataMode = QSPI_DATA_NONE;
    cmd->NbData = 0;

    return HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
};

static HAL_StatusTypeDef bootloader_qspi_qpi_enter(QSPI_CommandTypeDef *cmd)
{
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0x38;

    cmd->DataMode = QSPI_DATA_NONE;
    cmd->NbData = 0;

    return HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
};

static HAL_StatusTypeDef bootloader_qspi_4bytes_enter(QSPI_CommandTypeDef *cmd)
{
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0xB7;

    cmd->DataMode = QSPI_DATA_NONE;
    cmd->NbData = 0;

    return HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
};

static HAL_StatusTypeDef bootloader_qspi_write_enable(QSPI_CommandTypeDef *cmd)
{
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0x06;

    cmd->DataMode = QSPI_DATA_NONE;
    cmd->NbData = 0;

    return HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
};

static HAL_StatusTypeDef bootloader_qspi_qe_set(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0x31;

    cmd->DataMode = QSPI_DATA_1_LINE;
    cmd->NbData = 1;
    uint8_t data = 0x02;
    rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
    rst = HAL_QSPI_Transmit(&hqspi, &data, HAL_MAX_DELAY);
    return rst;
};

static HAL_StatusTypeDef bootloader_qspi_adp_set(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;
    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0x11;

    cmd->DataMode = QSPI_DATA_1_LINE;
    cmd->NbData = 1;
    uint8_t data = 0x02;
    rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
    rst = HAL_QSPI_Transmit(&hqspi, &data, HAL_MAX_DELAY);
    return rst;
};

static HAL_StatusTypeDef bootloader_qspi_dummy_circle_set(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;
    cmd->InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd->Instruction = 0xC0;

    cmd->DataMode = QSPI_DATA_4_LINES;
    cmd->NbData = 1;
    uint8_t data = 0x03 << 4;
    rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
    rst = HAL_QSPI_Transmit(&hqspi, &data, HAL_MAX_DELAY);
    return rst;
};

static HAL_StatusTypeDef bootloader_qspi_mm_qpi(QSPI_CommandTypeDef *cmd)
{
    QSPI_MemoryMappedTypeDef mmCfg = {0};

    cmd->InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd->Instruction = 0x0B;

    cmd->AddressMode = QSPI_ADDRESS_4_LINES;
    cmd->AddressSize = QSPI_ADDRESS_24_BITS;

    cmd->DummyCycles = 8;

    cmd->DataMode = QSPI_DATA_4_LINES;

    cmd->DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* 关闭溢出计数 */
    mmCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mmCfg.TimeOutPeriod = 0;

    return HAL_QSPI_MemoryMapped(&hqspi, cmd, &mmCfg);
};

static HAL_StatusTypeDef bootloader_qspi_mm(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;

    // test code
    // cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    // cmd->Instruction = 0xEB;

    // cmd->AddressMode = QSPI_ADDRESS_4_LINES;
    // cmd->AddressSize = QSPI_ADDRESS_24_BITS;
    // cmd->Address = 0x000000;
    // cmd->AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    // cmd->AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    // cmd->AlternateBytes = 0x00 << 4;

    // cmd->DummyCycles = 4;

    // cmd->DataMode = QSPI_DATA_4_LINES;
    // cmd->NbData = 4;
    // cmd->DdrMode = QSPI_DDR_MODE_DISABLE;
    // cmd->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    // cmd->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    // uint32_t data;
    // rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
    // rst = HAL_QSPI_Receive(&hqspi, &data, HAL_MAX_DELAY);

    QSPI_MemoryMappedTypeDef mmCfg = {0};

    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0xEC;

    cmd->AddressMode = QSPI_ADDRESS_4_LINES;
    cmd->AddressSize = QSPI_ADDRESS_32_BITS;

    cmd->AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    cmd->AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    cmd->AlternateBytes = 0x2 << 4;

    cmd->DummyCycles = 4;

    cmd->DataMode = QSPI_DATA_4_LINES;

    cmd->DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd->SIOOMode = QSPI_SIOO_INST_ONLY_FIRST_CMD;

    /* 关闭溢出计数 */
    mmCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mmCfg.TimeOutPeriod = 0;

    return HAL_QSPI_MemoryMapped(&hqspi, cmd, &mmCfg);
};

static HAL_StatusTypeDef bootloader_qspi_ddr_mm(QSPI_CommandTypeDef *cmd)
{
    HAL_StatusTypeDef rst;

    // test code
    // cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    // cmd->Instruction = 0xED;

    // cmd->AddressMode = QSPI_ADDRESS_4_LINES;
    // cmd->AddressSize = QSPI_ADDRESS_24_BITS;
    // cmd->Address = 0x000000;
    // cmd->AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    // cmd->AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    // cmd->AlternateBytes = 0x00 << 4;

    // cmd->DummyCycles = 7;

    // cmd->DataMode = QSPI_DATA_4_LINES;
    // cmd->NbData = 4;
    // cmd->DdrMode = QSPI_DDR_MODE_ENABLE;
    // cmd->DdrHoldHalfCycle = QSPI_DDR_HHC_HALF_CLK_DELAY;
    // cmd->SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    // uint32_t data;
    // rst = HAL_QSPI_Command(&hqspi, cmd, HAL_MAX_DELAY);
    // rst = HAL_QSPI_Receive(&hqspi, &data, HAL_MAX_DELAY);

    QSPI_MemoryMappedTypeDef mmCfg = {0};

    cmd->InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd->Instruction = 0xED;

    cmd->AddressMode = QSPI_ADDRESS_4_LINES;
    cmd->AddressSize = QSPI_ADDRESS_24_BITS;

    cmd->AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
    cmd->AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
    cmd->AlternateBytes = 0x2 << 4;

    cmd->DummyCycles = 7;

    cmd->DataMode = QSPI_DATA_4_LINES;

    cmd->DdrMode = QSPI_DDR_MODE_ENABLE;
    cmd->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd->SIOOMode = QSPI_SIOO_INST_ONLY_FIRST_CMD;

    /* 关闭溢出计数 */
    mmCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mmCfg.TimeOutPeriod = 0;

    return HAL_QSPI_MemoryMapped(&hqspi, cmd, &mmCfg);
};

#endif