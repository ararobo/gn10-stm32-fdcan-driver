# gn10-stm32-fdcan-driver

STM32G4/H5/H7 FDCAN用のgn10-can::driversライブラリ

## 目次

1. [概要](#1-概要)
2. [ドキュメント](#2-ドキュメント)
3. [コントリビューション](#3-コントリビューション)
4. [ビルド・使い方](#4-ビルド使い方)
5. [システム構成](#5-システム構成)
6. [ライセンス](#6-ライセンス)

## 1. 概要

gn10-canはクロスプラットフォームライブラリであり、各デバイスのCANを自前で実装する必要がある。
このリポジトリではSTM32のHALライブラリのfdcan.hを用いてgn10-canが必要とするCANDriverクラスが実装される。

### 依存関係：
CMakeのSTM32プロジェクトのみ対応
- stm32cubemx
- gn10_can

## 2. ドキュメント

| ドキュメント | 説明 |
| :-: | :-: |
| [CONTRIBUTING.md](./CONTRIBUTING.md) | 開発フロー・コミット規約・コーディング規約 |
| [docs/coding-rules.md](./docs/coding-rules.md) | コーディング規約の詳細 |
| [docs/uml/](./docs/uml/) | UML図 |

## 3. コントリビューション

[CONTRIBUTING.md](./CONTRIBUTING.md) を参照してください。

## 4. ビルド・使い方

STM32のCMakeに以下の変更を加えてください。
単体でビルドすることはできません。

### ルートCMakeの変更箇所
まず、CMakeのサブディレクトリにライブラリ類を登録する
```cmake
# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
```
上を次のように変更
```cmake
# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(gn10-can)
add_subdirectory(gn10-stm32-fdcan-driver)
```

次にライブラリをリンクすることで利用できるようにする
```cmake
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
)
```
上を次のように変更
```cmake
# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    gn10_can
    gn10_stm32_fdcan_driver
    # Add user defined libraries
)
```

## 5. システム構成

```
stm32_project/
    cmake/                      :CubeMX生成物
    Core/                       :CubeMX生成物
    Drivers/                    :STM32Drivers
    gn10-can/                   :gn10-canライブラリ
    gn10-stm32-fdcan-driver/    :本リポジトリ
    stm32_project.ioc           :CubeMX
    CMakeLists.txt              :ルートCMake
```

## 6. ライセンス

本リポジトリは [MITライセンス](./LICENSE) のもとで公開されています。
