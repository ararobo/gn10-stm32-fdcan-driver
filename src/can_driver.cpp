#include "gn10_stm32_fdcan_driver/can_driver.hpp"

namespace gn10_can {
namespace drivers {

CANDriver::CANDriver(FDCAN_HandleTypeDef* hfdcan, uint32_t rx_fifo, bool receive_extended_id)
    : hfdcan_(hfdcan), rx_fifo_(rx_fifo)
{
    if (rx_fifo == FDCAN_RX_FIFO1) {
        filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
    } else {
        filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    }
    if (receive_extended_id) {
        filter.IdType = FDCAN_EXTENDED_ID;
    } else {
        filter.IdType = FDCAN_STANDARD_ID;
    }
    filter.FilterIndex = 0;
    filter.FilterType  = FDCAN_FILTER_MASK;
    filter.FilterID1   = 0x000;
    filter.FilterID2   = 0x000;
}

bool CANDriver::init()
{
    if (HAL_FDCAN_ConfigFilter(hfdcan_, &filter) != HAL_OK) {
        return false;
    }
    if (HAL_FDCAN_Start(hfdcan_) != HAL_OK) {
        return false;
    }
    if (rx_fifo_ == FDCAN_RX_FIFO1) {
        if (HAL_FDCAN_ActivateNotification(hfdcan_, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK) {
            return false;
        }
    } else {
        if (HAL_FDCAN_ActivateNotification(hfdcan_, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
            return false;
        }
    }

    return true;
}

bool CANDriver::send(const CANFrame& frame)
{
    FDCAN_TxHeaderTypeDef tx_header;
    // 拡張IDであるかどうかは引数のframeから判断する
    if (frame.is_extended) {
        tx_header.IdType = FDCAN_EXTENDED_ID;
    } else {
        tx_header.IdType = FDCAN_STANDARD_ID;
    }
    tx_header.Identifier          = frame.id;
    tx_header.TxFrameType         = FDCAN_DATA_FRAME;
    tx_header.DataLength          = (uint32_t)frame.dlc;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch       = FDCAN_BRS_OFF;
    tx_header.FDFormat            = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker       = 0;

    // 送信FIFOに空きがなければタイムアウトするまで待機
    uint32_t start_tick = HAL_GetTick();
    while (HAL_FDCAN_GetTxFifoFreeLevel(hfdcan_) == 0) {
        if (HAL_GetTick() - start_tick > tx_timeout_ms_) {
            return false;
        }
    }
    // 送信
    if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan_, &tx_header, frame.data.data()) != HAL_OK) {
        return false;
    }
    return true;
}

bool CANDriver::receive(CANFrame& out_frame)
{
    FDCAN_RxHeaderTypeDef rx_header;
    std::array<uint8_t, 8> rx_data{};
    if (HAL_FDCAN_GetRxMessage(hfdcan_, rx_fifo_, &rx_header, rx_data.data()) != HAL_OK) {
        return false;
    }
    out_frame.id          = rx_header.Identifier;
    out_frame.is_extended = (rx_header.IdType == FDCAN_EXTENDED_ID);
    out_frame.dlc         = (uint8_t)rx_header.DataLength;
    out_frame.data        = rx_data;
    return true;
}

}  // namespace drivers
}  // namespace gn10_can
