/**
 * @file fdcan_driver.hpp
 * @author Gento Aiba (aiba-gento)
 * @brief STM32 FDCANのドライバ具体化クラスのヘッダファイル
 * @version 0.9.0
 * @date 2026-09-03
 *
 * @copyright Copyright (c) 2026
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "gn10_can/drivers/fdcan_driver_interface.hpp"
#include "main.h"

namespace gn10_can {
namespace drivers {

class FDCANDriver : public IFDCANDriver
{
public:
    /**
     * @brief STM32のCAN設定
     *
     * @param hfdcan FDCANのハンドラ
     * @param rx_fifo FDCAN_RX_FIFO0 / FDCAN_RX_FIFO1を選択（FIFO0の方がデフォルト）
     * @param receive_extended_id Extended idのパケットを受信するか（送信設定は含まない）
     */
    FDCANDriver(
        FDCAN_HandleTypeDef* hfdcan,
        uint32_t rx_fifo         = FDCAN_RX_FIFO0,
        bool receive_extended_id = false
    );

    /**
     * @brief 送信時にFIFOが空くまで待機する際のタイムアウトを指定
     *
     * @param tx_timeout_ms
     */
    void set_tx_timeout(uint32_t tx_timeout_ms);

    /**
     * @brief CANを開始する
     *
     * @return true 開始成功
     * @return false 開始失敗
     */
    bool init();

    /**
     * @brief パケットを送信
     *
     * @param frame 送信するフレーム
     * @return true 送信成功
     * @return false 送信失敗
     *
     * @note 送信時に拡張IDを使うかどうかはCANFrameのis_extendedに依存
     */
    bool send(const FDCANFrame& frame) override;

    /**
     * @brief パケットを受信
     *
     * @param out_frame 受信したフレーム
     * @return true 受信成功
     * @return false 受信失敗（受信するものがない場合を含む）
     */
    bool receive(FDCANFrame& out_frame) override;

private:
    FDCAN_HandleTypeDef* hfdcan_;
    FDCAN_FilterTypeDef filter;
    uint32_t rx_fifo_;
    uint32_t tx_timeout_ms_ = 0;
};
}  // namespace drivers
}  // namespace gn10_can
